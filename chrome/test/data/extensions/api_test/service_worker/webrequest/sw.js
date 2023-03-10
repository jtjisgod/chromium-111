// Copyright 2018 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

self.addEventListener('fetch', event => {
  if (event.request.url.indexOf('fallthrough') != -1) {
    return;
  }
  if (event.request.url.indexOf('respondWithFetch') != -1) {
    event.respondWith(fetch(event.request));
    return;
  }
});
