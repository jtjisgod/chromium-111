// Copyright 2015 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

chrome.test.assertTrue(
    !chrome.enterprise || !chrome.enterprise.deviceAttributes);
chrome.test.notifyPass();
