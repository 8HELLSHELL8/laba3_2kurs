#include <gtest/gtest.h>
#include "/home/anton/3laba_2kurs/src/Queue.cpp"

TEST(Queue_test, defaultConstructor)
{
    QUEUE q;
    EXPECT_EQ(q.is_empty(), 1);
}

TEST(Queue_test, isEmpty)
{
    QUEUE q;
    EXPECT_EQ(q.is_empty(), 1);
}

TEST(Queue_test, push)
{
    QUEUE q;
    q.push("1");
    EXPECT_EQ(q.is_empty(), 0);
}

TEST(Queue_test, pop)
{
    QUEUE q;
    q.push("1");
    q.pop();
    EXPECT_EQ(q.is_empty(), 1);
}

TEST(Queue_test, popError)
{
    QUEUE q;
    EXPECT_THROW(q.pop(), runtime_error);
    EXPECT_EQ(q.is_empty(), 1);
}