// Copyright 2014 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "scoped_refptr.h"

struct Foo {
  int dummy;
};

// An example of an unsafe conversion, since the reference is bound to a
// scoped_refptr with local storage. The tool should ignore this, since it
// should prefer letting a human manually resolve trickier cases like this.
Foo* TestFunction() {
  scoped_refptr<Foo> a;
  scoped_refptr<Foo>& b = a;
  return b;
}
