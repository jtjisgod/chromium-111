// Copyright 2021 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_WEB_PUBLIC_PERMISSIONS_PERMISSIONS_H_
#define IOS_WEB_PUBLIC_PERMISSIONS_PERMISSIONS_H_

#import <Foundation/Foundation.h>

namespace web {

// Callback that processes user's permission for a web state to asks the user
// whether it is allowed to access certain permissions on the device.
using WebStatePermissionDecisionHandler = void (^)(BOOL allow);

// Enum specifying different data or device hardwares that the app/site needs
// access permissions to.
typedef NS_ENUM(NSUInteger, Permission) {
  PermissionCamera = 0,
  PermissionMicrophone,
};

// Enum specifying whether a subject of permission is accessible and allowed.
typedef NS_ENUM(NSUInteger, PermissionState) {
  // The device has not granted the site access to the requested permission.
  // When this is the state of a given permission, the app is unable to change
  // it to ALLOWED or BLOCKED.
  PermissionStateNotAccessible = 0,
  // The site has access to the requested permission, but the user has disabled
  // or blocked the subject of permission so it cannot be used.
  PermissionStateBlocked,
  // The site has access to the requested permission and is able to use it when
  // needed.
  PermissionStateAllowed,
};

}  // namespace web

#endif  // IOS_WEB_PUBLIC_PERMISSIONS_PERMISSIONS_H_
