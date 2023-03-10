# Copyright 2016 The Chromium Authors
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import contextlib


class CodeWriter(object):
  """Helper class for code indentation."""
  def __init__(self):
    self.indent = 0
    self.value = []
    self.errors = []

  def GetErrors(self):
    return self.errors

  @contextlib.contextmanager
  def AddIndent(self, indent=2):
    self.indent += indent
    yield 0
    self.indent -= indent

  def IncreaseIndent(self, indent=2):
    self.indent += indent

  def DecreaseIndent(self, indent=2):
    self.indent -= indent

  def Output(self, fmt, **kwargs):
    s = fmt.format(**kwargs)
    s = s.rstrip('\n')
    lines = s.split('\n')
    lines = map(lambda s: (' ' * self.indent + s).rstrip(), lines)
    self.value.extend(lines)

  def AddError(self, fmt, **kwargs):
    self.errors.append(fmt.format(**kwargs))

  def GetValue(self):
    return '\n'.join(self.value) + '\n'

  def WriteCStyleHeader(self):
    self.Output("// GENERATED FILE")
    self.Output("// This file generated by DomDistillerJs protoc plugin.")
