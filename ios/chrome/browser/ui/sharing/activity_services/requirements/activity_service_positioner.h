// Copyright 2017 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_CHROME_BROWSER_UI_SHARING_ACTIVITY_SERVICES_REQUIREMENTS_ACTIVITY_SERVICE_POSITIONER_H_
#define IOS_CHROME_BROWSER_UI_SHARING_ACTIVITY_SERVICES_REQUIREMENTS_ACTIVITY_SERVICE_POSITIONER_H_

#import <UIKit/UIKit.h>

// ActivityServicePositioner contains methods that are used to position the
// activity services menu on the screen.
@protocol ActivityServicePositioner <NSObject>

// Returns the view where the UIActivityViewController
// should be presented. This property is ignored if a barButtonItem is set.
- (UIView*)sourceView;

// Returns the bounds where the UIActivityViewController's popover should be
// presented. This property is ignored if a barButtonItem is set.
- (CGRect)sourceRect;

@optional

// Returns the bar button item where the UIActivityViewController should be
// presented from. If a non null value is returned, `sourceView` and
// `sourceRect` are not used.
- (UIBarButtonItem*)barButtonItem;

@end

#endif  // IOS_CHROME_BROWSER_UI_SHARING_ACTIVITY_SERVICES_REQUIREMENTS_ACTIVITY_SERVICE_POSITIONER_H_
