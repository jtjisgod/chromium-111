// Copyright 2018 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "ios/chrome/browser/ui/toolbar/adaptive_toolbar_view_controller.h"

#import <UIKit/UIGestureRecognizerSubclass.h>

#import "base/ios/ios_util.h"
#import "base/run_loop.h"
#import "base/test/ios/wait_util.h"
#import "base/test/task_environment.h"
#import "ios/chrome/browser/ui/commands/omnibox_commands.h"
#import "ios/chrome/browser/ui/commands/popup_menu_commands.h"
#import "ios/chrome/browser/ui/icons/symbols.h"
#import "ios/chrome/browser/ui/popup_menu/public/popup_menu_long_press_delegate.h"
#import "ios/chrome/browser/ui/toolbar/buttons/toolbar_button.h"
#import "ios/chrome/browser/ui/toolbar/buttons/toolbar_button_factory.h"
#import "ios/chrome/browser/ui/toolbar/primary_toolbar_view_controller.h"
#import "ios/chrome/browser/ui/toolbar/public/toolbar_constants.h"
#import "testing/gtest/include/gtest/gtest.h"
#import "testing/platform_test.h"
#import "third_party/ocmock/OCMock/OCMock.h"
#import "third_party/ocmock/gtest_support.h"
#import "ui/base/device_form_factor.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

namespace {

UIView* GetTabGridToolbarButton(UIView* parentView) {
  if (parentView.accessibilityIdentifier == kToolbarStackButtonIdentifier)
    return parentView;

  for (UIView* subview in parentView.subviews) {
    UIView* buttonSubview = GetTabGridToolbarButton(subview);
    if (buttonSubview)
      return buttonSubview;
  }
  return nil;
}

class AdaptiveToolbarViewControllerTest : public PlatformTest {
 protected:
  AdaptiveToolbarViewControllerTest()
      : TaskEnvironment_(base::test::TaskEnvironment::MainThreadType::UI) {}

  base::test::TaskEnvironment TaskEnvironment_;
};

TEST_F(AdaptiveToolbarViewControllerTest, DetectForceTouch) {
  if (UseSymbols()) {
    // With SF Symbols, another popup menu is displayed.
    return;
  }
  if (ui::GetDeviceFormFactor() == ui::DEVICE_FORM_FACTOR_TABLET) {
    // IPad doesn't have force touch.
    return;
  }

  id omniboxCommandsHandler = OCMProtocolMock(@protocol(OmniboxCommands));
  id popupMenuCommandsHandler = OCMProtocolMock(@protocol(PopupMenuCommands));
  id longPressDelegate = OCMProtocolMock(@protocol(PopupMenuLongPressDelegate));
  ToolbarButtonFactory* factory =
      [[ToolbarButtonFactory alloc] initWithStyle:NORMAL];

  AdaptiveToolbarViewController* toolbar =
      [[PrimaryToolbarViewController alloc] init];
  toolbar.buttonFactory = factory;
  toolbar.longPressDelegate = longPressDelegate;
  toolbar.omniboxCommandsHandler = omniboxCommandsHandler;
  toolbar.popupMenuCommandsHandler = popupMenuCommandsHandler;

  UIView* buttonView = GetTabGridToolbarButton(toolbar.view);

  ASSERT_NE(buttonView, nil);
  ASSERT_GE(buttonView.gestureRecognizers.count, 1UL);

  UIGestureRecognizer* gestureRecognizer = buttonView.gestureRecognizers[0];

  id event = OCMClassMock([UIEvent class]);

  CGFloat maximumForce = 1;
  CGFloat currentForce = 0.7;
  id touch = OCMClassMock([UITouch class]);
  OCMStub([touch maximumPossibleForce]).andReturn(maximumForce);
  OCMStub([touch force]).andReturn(currentForce);
  [gestureRecognizer touchesBegan:[NSSet setWithObject:touch] withEvent:event];
  [gestureRecognizer touchesMoved:[NSSet setWithObject:touch] withEvent:event];

  // Check that the popupMenuCommandsHandler is called when the force touch is
  // detected.
  OCMExpect([popupMenuCommandsHandler showTabGridButtonPopup]);

  currentForce = 0.9;

  touch = OCMClassMock([UITouch class]);
  OCMStub([touch maximumPossibleForce]).andReturn(maximumForce);
  OCMStub([touch force]).andReturn(currentForce);
  [gestureRecognizer touchesMoved:[NSSet setWithObject:touch] withEvent:event];

  base::test::ios::SpinRunLoopWithMinDelay(base::Seconds(0.05));

  EXPECT_OCMOCK_VERIFY(popupMenuCommandsHandler);

  // Check that the longPressDelegate is notified when the gesture recognizer
  // changes, even with lower force.
  [[[longPressDelegate expect] ignoringNonObjectArgs]
      longPressFocusPointChangedTo:CGPointZero];

  touch = OCMClassMock([UITouch class]);
  OCMStub([touch maximumPossibleForce]).andReturn(maximumForce);
  OCMStub([touch force]).andReturn(currentForce);
  [gestureRecognizer touchesMoved:[NSSet setWithObject:touch] withEvent:event];

  base::test::ios::SpinRunLoopWithMinDelay(base::Seconds(0.05));

  EXPECT_OCMOCK_VERIFY(longPressDelegate);

  // Change the state to Ended here, as the long press gesture recognizer isn't
  // working on unit test (the state is cancelled).
  gestureRecognizer.state = UIGestureRecognizerStateEnded;

  base::test::ios::SpinRunLoopWithMinDelay(base::Seconds(0.05));

  EXPECT_OCMOCK_VERIFY(longPressDelegate);
}

}  // namespace
