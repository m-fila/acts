// This file is part of the ACTS project.
//
// Copyright (C) 2016 CERN for the benefit of the ACTS project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

// Project include(s).
#include "traccc/utils/algorithm.hpp"

// GTest include(s).
#include <gtest/gtest.h>

// System include(s).
#include <string>

class double_int : public traccc::algorithm<int(const int &)> {
 public:
  virtual int operator()(const int &i) const override { return 2 * i; }
};

class double_int_affine : public traccc::algorithm<int(int &&)> {
 public:
  virtual int operator()(int &&i) const override { return 2 * i; }
};

class double_string_affine
    : public traccc::algorithm<std::string(std::string &&)> {
 public:
  virtual std::string operator()(std::string &&i) const override {
    return i + i;
  }
};

class double_string_regular
    : public traccc::algorithm<std::string(const std::string &)> {
 public:
  virtual std::string operator()(const std::string &i) const override {
    return i + i;
  }
};

class add : public traccc::algorithm<int(const int &, const int &)> {
 public:
  virtual int operator()(const int &i, const int &j) const override {
    return i + j;
  }
};

TEST(algorithm, basic) {
  double_int i;

  ASSERT_EQ(i(1), 2);
  ASSERT_EQ(i(-1), -2);
  ASSERT_EQ(i(5), 10);
}

TEST(algorithm, basic_affine) {
  double_int_affine i;

  ASSERT_EQ(i(1), 2);
  ASSERT_EQ(i(-1), -2);
  ASSERT_EQ(i(5), 10);
}

TEST(algorithm, string_affine) {
  double_string_affine i;

  std::string s = "test";

  ASSERT_EQ(i("hello"), "hellohello");
  ASSERT_EQ(i("bye"), "byebye");
  ASSERT_EQ(i(std::move(s)), "testtest");
}

TEST(algorithm, string_regular) {
  double_string_regular i;

  std::string s = "test";

  ASSERT_EQ(i("hello"), "hellohello");
  ASSERT_EQ(i("bye"), "byebye");
  ASSERT_EQ(i(s), "testtest");
}

TEST(algorithm, partial_application_bind) {
  add i;
  std::function<int(const int &)> f = std::bind(i, std::placeholders::_1, 5);

  ASSERT_EQ(f(1), 6);
  ASSERT_EQ(f(-1), 4);
  ASSERT_EQ(f(5), 10);
}

TEST(algorithm, partial_application_lambda) {
  add i;
  std::function<int(const int &)> f = [i](const int &j) { return i(j, 5); };

  ASSERT_EQ(f(1), 6);
  ASSERT_EQ(f(-1), 4);
  ASSERT_EQ(f(5), 10);
}
