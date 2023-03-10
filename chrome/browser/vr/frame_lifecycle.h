// Copyright 2018 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_VR_FRAME_LIFECYCLE_H_
#define CHROME_BROWSER_VR_FRAME_LIFECYCLE_H_

namespace vr {

enum UpdatePhase {
  kDirty = 0,
  kUpdatedBindings,
  kUpdatedAnimations,
  kUpdatedComputedOpacity,
  kUpdatedSize,
  kUpdatedLayout,
  kUpdatedWorldSpaceTransform,
  kUpdatedTextures,
  kClean = kUpdatedTextures,
};

struct FrameLifecycle {
  static UpdatePhase phase();
  static void set_phase(UpdatePhase phase);
};

}  // namespace vr

#endif  // CHROME_BROWSER_VR_FRAME_LIFECYCLE_H_
