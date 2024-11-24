#include <gtest/gtest.h>
#include "/home/anton/3laba_2kurs/src/ForwardList.cpp"

TEST(ForwardList_test, defaultConstructor)
{
    ForwardList list;
    EXPECT_EQ(list.size(), 0);
}

TEST(ForwardList_test, isEmpty)
{
    ForwardList list;
    EXPECT_EQ(list.is_empty(), 1);
    list.addtail("element");
    EXPECT_EQ(list.is_empty(), 0);
}

TEST(ForwardList_test, size)
{
    ForwardList list;
    EXPECT_EQ(list.size(), 0);
    list.addtail("kek");
    EXPECT_EQ(list.size(), 1);
}

TEST(ForwardList_test, get)
{
    ForwardList list;
    list.addtail("check");
    list.addtail("words");
    list.addtail("inList");
    EXPECT_EQ(list.get(0), "check");
    EXPECT_EQ(list.get(1), "words");
    EXPECT_EQ(list.get(2), "inList");
    EXPECT_EQ(list.size(), 3);
    EXPECT_THROW(list.get(-1), out_of_range);
}

TEST(ForwardList_test, addTail)
{
    ForwardList list;
    list.addtail("check");
    EXPECT_EQ(list.size(), 1);
    list.addtail("newword");
    EXPECT_EQ(list.get(0), "check");
    EXPECT_EQ(list.get(1), "newword");
    EXPECT_EQ(list.size(), 2);
}

TEST(ForwardList_test, addHead)
{
    ForwardList list;
    list.addhead("check");
    EXPECT_EQ(list.size(), 1);
    list.addhead("newword");
    EXPECT_EQ(list.get(0), "newword");
    EXPECT_EQ(list.get(1), "check");
    EXPECT_EQ(list.size(), 2);
}

TEST(ForwardList_test, delHead)
{
    ForwardList list;
    EXPECT_THROW(list.delhead(), runtime_error);
    list.addhead("check");
    list.addhead("deletion");
    EXPECT_EQ(list.size(), 2);
    list.delhead();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0), "check");
    list.delhead();
    EXPECT_EQ(list.size(), 0);
}

TEST(ForwardList_test, delTail)
{
    ForwardList list;
    EXPECT_THROW(list.deltail(), runtime_error);
    list.addhead("check");
    list.addhead("deletion");
    EXPECT_EQ(list.size(), 2);
    list.deltail();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0), "deletion");
    list.deltail();
    EXPECT_EQ(list.size(), 0);
}

TEST(ForwardList_test, search)
{
    ForwardList list;
    EXPECT_THROW(list.search("1000"), runtime_error);
    list.addhead("1");
    list.addhead("2");
    list.addhead("3");
    EXPECT_EQ(list.search("2"), 1);
    EXPECT_EQ(list.search("999"), 0);
}

TEST(ForwardList_test, remove)
{
    ForwardList list;
    list.addhead("1");
    list.addhead("2");
    list.addhead("3");
    EXPECT_EQ(list.search("2"), 1);
    list.remove("2");
    EXPECT_EQ(list.search("2"), 0);
    EXPECT_EQ(list.size(), 2);
}