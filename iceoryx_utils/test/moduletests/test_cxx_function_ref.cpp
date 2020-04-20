// Copyright (c) 2020 by Robert Bosch GmbH. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "iceoryx_utils/cxx/function_ref.hpp"
#include "test.hpp"

using namespace ::testing;
using namespace iox::cxx;

int freeFunction()
{
    return 42 + 42;
}

class Functor
{
  public:
    int operator()()
    {
        return 73;
    }
};

class function_refTest : public Test
{
  public:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
    uint8_t m_iter{0};
};

using function_refDeathTest = function_refTest;

TEST_F(function_refTest, CreateEmpty)
{
    function_ref<void()> sut;
    EXPECT_FALSE(sut);
}

TEST_F(function_refTest, CreateEmptyAndAssign)
{
    auto lambda = [] {};
    function_ref<void()> sut;
    sut = lambda;
    EXPECT_TRUE(sut);
}

TEST_F(function_refTest, CreateAndSwap)
{
    auto lambda1 = []() -> int { return 42; };
    auto lambda2 = []() -> int { return 73; };
    function_ref<int()> sut1(lambda1);
    function_ref<int()> sut2(lambda2);
    EXPECT_THAT(sut1(), Eq(42));
    EXPECT_THAT(sut2(), Eq(73));
    sut1.swap(sut2);
    EXPECT_THAT(sut1(), Eq(73));
    EXPECT_THAT(sut2(), Eq(42));
}

TEST_F(function_refTest, CreateWithNullptr)
{
    function_ref<void()> sut(nullptr);
    EXPECT_FALSE(sut);
}

TEST_F(function_refDeathTest, UBCreateWithTempLambda)
{
    function_ref<void()> sut([&] { m_iter++; });
    /// @todo
    // EXPECT_EXIT(sut(), ::testing::KilledBySignal(SIGSEGV), "Segmentation fault");
}

TEST_F(function_refTest, CreateWithCapturingLambdaVoidVoid)
{
    auto lambda = [&] { m_iter++; };
    function_ref<void(void)> sut(lambda);
    sut();
    EXPECT_THAT(m_iter, Eq(1));
}

TEST_F(function_refTest, CreateWithLambdaIntVoid)
{
    auto lambda = [](void) -> int { return 42; };
    function_ref<int(void)> sut(lambda);
    EXPECT_THAT(sut(), Eq(42));
}

TEST_F(function_refTest, CreateWithLambdaIntInt)
{
    auto lambda = [](int var) -> int { return ++var; };
    function_ref<int(int)> sut(lambda);
    EXPECT_THAT(sut(m_iter), Eq(1));
}

TEST_F(function_refTest, CreateWithFreeFunction)
{
    function_ref<int()> sut(freeFunction);
    EXPECT_THAT(sut(), Eq(84));
}

TEST_F(function_refTest, CreateWithFunctor)
{
    Functor foo;
    function_ref<int()> sut(foo);
    EXPECT_THAT(sut(), Eq(73));
}
