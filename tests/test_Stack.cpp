#include <gtest/gtest.h>
#include "/home/anton/3laba_2kurs/src/Stack.cpp"


TEST(Stack_test, defaultConstructor)
{
    Stack stack;
    EXPECT_EQ(stack.is_empty(), 1);
}

TEST(Stack_test, isEmpty)
{
    Stack stack;
    EXPECT_EQ(stack.is_empty(), 1);
}

TEST(Stack_test, isEmptyError)
{
    Stack stack;
    stack.push("1");
    EXPECT_EQ(stack.is_empty(), 0);
}

TEST(Stack_test, push)
{
    Stack stack;
    stack.push("1");
    EXPECT_EQ(stack.is_empty(), 0);
}

TEST(Stack_test, pop)
{
    Stack stack;
    stack.push("1");
    stack.pop();
    EXPECT_EQ(stack.is_empty(), 1);
}

TEST(Stack_test, popError)
{
    Stack stack;
    EXPECT_THROW(stack.pop();, runtime_error);
    EXPECT_EQ(stack.is_empty(), 1);
}