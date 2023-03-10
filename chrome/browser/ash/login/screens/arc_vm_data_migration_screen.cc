// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ash/login/screens/arc_vm_data_migration_screen.h"

#include "ash/components/arc/arc_prefs.h"
#include "ash/components/arc/arc_util.h"
#include "ash/components/arc/session/arc_vm_data_migration_status.h"
#include "ash/public/cpp/session/scoped_screen_lock_blocker.h"
#include "ash/session/session_controller_impl.h"
#include "ash/shell.h"
#include "base/logging.h"
#include "base/notreached.h"
#include "chrome/browser/lifetime/application_lifetime.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chromeos/ash/components/dbus/spaced/spaced_client.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/device_service.h"
#include "services/device/public/mojom/wake_lock_provider.mojom.h"

namespace ash {

namespace {

constexpr char kPathToCheckFreeDiskSpace[] = "/home/chronos/user";
constexpr int64_t kMinimumFreeDiskSpaceForMigration = 1LL << 30;  // 1 GB.

constexpr char kUserActionSkip[] = "skip";
constexpr char kUserActionUpdate[] = "update";

}  // namespace

ArcVmDataMigrationScreen::ArcVmDataMigrationScreen(
    base::WeakPtr<ArcVmDataMigrationScreenView> view)
    : BaseScreen(ArcVmDataMigrationScreenView::kScreenId,
                 OobeScreenPriority::DEFAULT),
      view_(std::move(view)) {
  DCHECK(view_);
}

ArcVmDataMigrationScreen::~ArcVmDataMigrationScreen() = default;

void ArcVmDataMigrationScreen::ShowImpl() {
  if (!view_) {
    return;
  }

  // The migration screen is shown after a session restart with an ARC-enabled
  // login user, and thus the primary profile is available at this point.
  profile_ = ProfileManager::GetPrimaryUserProfile();
  DCHECK(profile_);

  GetWakeLock()->RequestWakeLock();

  DCHECK(Shell::Get());
  DCHECK(Shell::Get()->session_controller());
  scoped_screen_lock_blocker_ =
      Shell::Get()->session_controller()->GetScopedScreenLockBlocker();

  view_->Show();
  // TODO(b/258278176): Stop stale ARCVM and Upstart jobs while loading.
  SetUpInitialView();
}

void ArcVmDataMigrationScreen::HideImpl() {
  GetWakeLock()->CancelWakeLock();
  if (scoped_screen_lock_blocker_) {
    scoped_screen_lock_blocker_.reset();
  }
}

void ArcVmDataMigrationScreen::OnUserAction(const base::Value::List& args) {
  const std::string& action_id = args[0].GetString();
  VLOG(1) << "User action: action_id=" << action_id;
  if (action_id == kUserActionSkip) {
    HandleSkip();
  } else if (action_id == kUserActionUpdate) {
    HandleUpdate();
  } else {
    BaseScreen::OnUserAction(args);
  }
}

void ArcVmDataMigrationScreen::SetUpInitialView() {
  // TODO(b/258278176): Check battery state.
  arc::ArcVmDataMigrationStatus data_migration_status =
      arc::GetArcVmDataMigrationStatus(profile_->GetPrefs());
  switch (data_migration_status) {
    case arc::ArcVmDataMigrationStatus::kConfirmed:
      // Set the status back to kNotified to prepare for cases where the
      // migration is skipped or the device is shut down before the migration is
      // started.
      arc::SetArcVmDataMigrationStatus(
          profile_->GetPrefs(), arc::ArcVmDataMigrationStatus::kNotified);
      DCHECK(ash::SpacedClient::Get());
      ash::SpacedClient::Get()->GetFreeDiskSpace(
          kPathToCheckFreeDiskSpace,
          base::BindOnce(&ArcVmDataMigrationScreen::OnGetFreeDiskSpace,
                         weak_ptr_factory_.GetWeakPtr()));
      break;
    case arc::ArcVmDataMigrationStatus::kStarted:
      // TODO(b/258278176): Show the resume screen.
      UpdateUIState(ArcVmDataMigrationScreenView::UIState::kWelcome);
      break;
    default:
      NOTREACHED();
      break;
  }
}

void ArcVmDataMigrationScreen::OnGetFreeDiskSpace(
    absl::optional<int64_t> reply) {
  if (!reply.has_value() || reply.value() < 0) {
    LOG(ERROR) << "Failed to get free disk space from spaced";
    HandleFatalError();
    return;
  }

  if (!view_) {
    return;
  }

  const int64_t free_disk_space = reply.value();
  VLOG(1) << "Free disk space is " << free_disk_space;
  if (free_disk_space < kMinimumFreeDiskSpaceForMigration) {
    view_->SetRequiredFreeDiskSpace(kMinimumFreeDiskSpaceForMigration);
  }

  UpdateUIState(ArcVmDataMigrationScreenView::UIState::kWelcome);
}

void ArcVmDataMigrationScreen::UpdateUIState(
    ArcVmDataMigrationScreenView::UIState state) {
  if (view_) {
    view_->SetUIState(state);
  }
}

void ArcVmDataMigrationScreen::HandleSkip() {
  chrome::AttemptRelaunch();
}

void ArcVmDataMigrationScreen::HandleUpdate() {
  arc::SetArcVmDataMigrationStatus(profile_->GetPrefs(),
                                   arc::ArcVmDataMigrationStatus::kStarted);
  // TODO(b/258278176): Trigger the migration.
  NOTIMPLEMENTED();
}

void ArcVmDataMigrationScreen::HandleFatalError() {
  // TODO(b/258278176): Show a fatal error screen and report the reason.
  chrome::AttemptRelaunch();
}

device::mojom::WakeLock* ArcVmDataMigrationScreen::GetWakeLock() {
  // |wake_lock_| is lazy bound and reused, even after a connection error.
  if (wake_lock_) {
    return wake_lock_.get();
  }

  mojo::PendingReceiver<device::mojom::WakeLock> receiver =
      wake_lock_.BindNewPipeAndPassReceiver();

  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  mojo::Remote<device::mojom::WakeLockProvider> wake_lock_provider;
  content::GetDeviceService().BindWakeLockProvider(
      wake_lock_provider.BindNewPipeAndPassReceiver());
  wake_lock_provider->GetWakeLockWithoutContext(
      device::mojom::WakeLockType::kPreventAppSuspension,
      device::mojom::WakeLockReason::kOther,
      "ARCVM /data migration is in progress...", std::move(receiver));
  return wake_lock_.get();
}

}  // namespace ash
