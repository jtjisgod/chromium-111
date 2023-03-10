// Copyright 2012 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/json/json_writer.h"
#include "base/json/json_reader.h"

#include "base/containers/span.h"
#include "base/memory/ptr_util.h"
#include "base/strings/stringprintf.h"
#include "base/values.h"
#include "build/build_config.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {

TEST(JSONWriterTest, BasicTypes) {
  std::string output_js;

  // Test null.
  EXPECT_TRUE(JSONWriter::Write(Value(), &output_js));
  EXPECT_EQ("null", output_js);

  // Test empty dict.
  EXPECT_TRUE(JSONWriter::Write(Value(Value::Type::DICT), &output_js));
  EXPECT_EQ("{}", output_js);

  // Test empty list.
  EXPECT_TRUE(JSONWriter::Write(Value(Value::Type::LIST), &output_js));
  EXPECT_EQ("[]", output_js);

  // Test integer values.
  EXPECT_TRUE(JSONWriter::Write(Value(42), &output_js));
  EXPECT_EQ("42", output_js);

  // Test boolean values.
  EXPECT_TRUE(JSONWriter::Write(Value(true), &output_js));
  EXPECT_EQ("true", output_js);

  // Test Real values should always have a decimal or an 'e'.
  EXPECT_TRUE(JSONWriter::Write(Value(1.0), &output_js));
  EXPECT_EQ("1.0", output_js);

  // Test Real values in the range (-1, 1) must have leading zeros
  EXPECT_TRUE(JSONWriter::Write(Value(0.2), &output_js));
  EXPECT_EQ("0.2", output_js);

  // Test Real values in the range (-1, 1) must have leading zeros
  EXPECT_TRUE(JSONWriter::Write(Value(-0.8), &output_js));
  EXPECT_EQ("-0.8", output_js);

  // Test String values.
  EXPECT_TRUE(JSONWriter::Write(Value("foo"), &output_js));
  EXPECT_EQ("\"foo\"", output_js);
}

TEST(JSONWriterTest, NestedTypes) {
  std::string output_js;

  // Writer unittests like empty list/dict nesting,
  // list list nesting, etc.
  Value::Dict root_dict;
  Value::List list;
  Value::Dict inner_dict;
  inner_dict.Set("inner int", 10);
  list.Append(std::move(inner_dict));
  Value::Dict empty_dict;
  list.Append(std::move(empty_dict));
  list.Append(Value::List());
  list.Append(true);
  root_dict.Set("list", Value(std::move(list)));

  // The pretty-printer uses a different newline style on Windows than on
  // other platforms.
#if BUILDFLAG(IS_WIN)
#define JSON_NEWLINE "\r\n"
#else
#define JSON_NEWLINE "\n"
#endif

  // Test the pretty-printer.
  EXPECT_TRUE(JSONWriter::Write(root_dict, &output_js));
  EXPECT_EQ("{\"list\":[{\"inner int\":10},{},[],true]}", output_js);
  EXPECT_TRUE(JSONWriter::WriteWithOptions(
      root_dict, JSONWriter::OPTIONS_PRETTY_PRINT, &output_js));
  EXPECT_EQ("{" JSON_NEWLINE "   \"list\": [ {" JSON_NEWLINE
            "      \"inner int\": 10" JSON_NEWLINE "   }, {" JSON_NEWLINE
            "   }, [  ], true ]" JSON_NEWLINE "}" JSON_NEWLINE,
            output_js);

#undef JSON_NEWLINE
}

TEST(JSONWriterTest, KeysWithPeriods) {
  std::string output_js;

  Value::Dict period_dict;
  period_dict.Set("a.b", 3);
  period_dict.Set("c", 2);
  Value::Dict period_dict2;
  period_dict2.Set("g.h.i.j", 1);
  period_dict.Set("d.e.f", std::move(period_dict2));
  EXPECT_TRUE(JSONWriter::Write(period_dict, &output_js));
  EXPECT_EQ("{\"a.b\":3,\"c\":2,\"d.e.f\":{\"g.h.i.j\":1}}", output_js);

  Value::Dict period_dict3;
  period_dict3.SetByDottedPath("a.b", 2);
  period_dict3.Set("a.b", 1);
  EXPECT_TRUE(JSONWriter::Write(period_dict3, &output_js));
  EXPECT_EQ("{\"a\":{\"b\":2},\"a.b\":1}", output_js);
}

TEST(JSONWriterTest, BinaryValues) {
  std::string output_js;

  // Binary values should return errors unless suppressed via the
  // OPTIONS_OMIT_BINARY_VALUES flag.
  const auto kBufferSpan =
      base::make_span(reinterpret_cast<const uint8_t*>("asdf"), 4);
  Value root(kBufferSpan);
  EXPECT_FALSE(JSONWriter::Write(root, &output_js));
  EXPECT_TRUE(JSONWriter::WriteWithOptions(
      root, JSONWriter::OPTIONS_OMIT_BINARY_VALUES, &output_js));
  EXPECT_TRUE(output_js.empty());

  Value::List binary_list;
  binary_list.Append(Value(kBufferSpan));
  binary_list.Append(5);
  binary_list.Append(Value(kBufferSpan));
  binary_list.Append(2);
  binary_list.Append(Value(kBufferSpan));
  EXPECT_FALSE(JSONWriter::Write(binary_list, &output_js));
  EXPECT_TRUE(JSONWriter::WriteWithOptions(
      binary_list, JSONWriter::OPTIONS_OMIT_BINARY_VALUES, &output_js));
  EXPECT_EQ("[5,2]", output_js);

  Value::Dict binary_dict;
  binary_dict.Set("a", Value(kBufferSpan));
  binary_dict.Set("b", 5);
  binary_dict.Set("c", Value(kBufferSpan));
  binary_dict.Set("d", 2);
  binary_dict.Set("e", Value(kBufferSpan));
  EXPECT_FALSE(JSONWriter::Write(binary_dict, &output_js));
  EXPECT_TRUE(JSONWriter::WriteWithOptions(
      binary_dict, JSONWriter::OPTIONS_OMIT_BINARY_VALUES, &output_js));
  EXPECT_EQ("{\"b\":5,\"d\":2}", output_js);
}

TEST(JSONWriterTest, DoublesAsInts) {
  std::string output_js;

  // Test allowing a double with no fractional part to be written as an integer.
  Value double_value(1e10);
  EXPECT_TRUE(JSONWriter::WriteWithOptions(
      double_value, JSONWriter::OPTIONS_OMIT_DOUBLE_TYPE_PRESERVATION,
      &output_js));
  EXPECT_EQ("10000000000", output_js);
}

TEST(JSONWriterTest, StackOverflow) {
  std::string output_js;

  Value::List deep_list;
  const size_t max_depth = 100000;

  for (size_t i = 0; i < max_depth; ++i) {
    Value::List new_top_list;
    new_top_list.Append(std::move(deep_list));
    deep_list = std::move(new_top_list);
  }

  Value deep_list_value(std::move(deep_list));
  EXPECT_FALSE(JSONWriter::Write(deep_list_value, &output_js));
  EXPECT_FALSE(JSONWriter::WriteWithOptions(
      deep_list_value, JSONWriter::OPTIONS_PRETTY_PRINT, &output_js));

  // We cannot just let deep_list tear down since it
  // would cause a stack overflow. Therefore, we tear
  // down the deep list manually.
  deep_list = std::move(deep_list_value).TakeList();
  while (!deep_list.empty()) {
    DCHECK_EQ(deep_list.size(), 1u);
    Value::List inner_list = std::move(deep_list[0]).TakeList();
    deep_list = std::move(inner_list);
  }
}

TEST(JSONWriterTest, TestMaxDepthWithValidNodes) {
  // Create JSON to the max depth - 1.  Nodes at that depth are still valid
  // for writing which matches the JSONParser logic.
  std::string nested_json;
  for (int i = 0; i < 199; ++i) {
    std::string node = "[";
    for (int j = 0; j < 5; j++) {
      node.append(StringPrintf("%d,", j));
    }
    nested_json.insert(0, node);
    nested_json.append("]");
  }

  // Ensure we can read and write the JSON
  auto json_val = JSONReader::ReadAndReturnValueWithError(
      nested_json, JSON_ALLOW_TRAILING_COMMAS);
  EXPECT_TRUE(json_val.has_value());
  const Value& value = *json_val;
  std::string serialized;
  EXPECT_TRUE(JSONWriter::Write(value, &serialized));
}

}  // namespace base
