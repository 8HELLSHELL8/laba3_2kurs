#include <gtest/gtest.h>
#include "/home/anton/3laba_2kurs/src/LinkedList.cpp"

TEST(LinkedList_test, constructor)
{
    LinkedList list;
    EXPECT_EQ(list.size(), 0);
}

TEST(LinkedList_test, isEmpty)
{
    LinkedList list;
    EXPECT_EQ(list.size(), 0);
    EXPECT_EQ(list.is_empty(), 1);
}

TEST(LinkedList_test, search)
{
    LinkedList list;
    EXPECT_THROW(list.search("999"), runtime_error);
    EXPECT_EQ(list.size(), 0);

    list.addhead("100");
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.search("100"), 1);
    EXPECT_THROW(list.search("999"), runtime_error);
}

TEST(LinkedList_test, get)
{
    LinkedList list;
    EXPECT_THROW(list.get(0), out_of_range);
    list.addtail("CHECK");
    list.addtail("for");
    list.addtail("WORK");
    EXPECT_EQ(list.get(1), "for");
}

TEST(LinkedList_test, addHead)
{
    LinkedList list;
    EXPECT_EQ(list.size(), 0);

    list.addhead("check");
    EXPECT_EQ(list.get(0), "check");

    EXPECT_EQ(list.size(), 1);

    list.addhead("WORK");
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0), "WORK");
}

TEST(LinkedList_test, addTail)
{
    LinkedList list;
    EXPECT_EQ(list.size(), 0);

    list.addtail("check");
    EXPECT_EQ(list.search("check"), 1);
    EXPECT_EQ(list.get(0), "check");
    EXPECT_EQ(list.size(), 1);

    list.addtail("WORK");
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(1), "WORK");
}

TEST(LinkedList_test, delHead)
{
    LinkedList list;
    EXPECT_THROW(list.delhead(), runtime_error);
    list.addtail("1");
    list.addtail("2");
    list.addtail("3");
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(2), "3");

    list.delhead();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0), "2");

    list.delhead();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0), "3");

    list.delhead();
    EXPECT_EQ(list.size(), 0);
    EXPECT_THROW(list.get(0), out_of_range);
}

TEST(LinkedList_test, delTail)
{
    LinkedList list;
    EXPECT_THROW(list.delhead(), runtime_error);
    list.addtail("1");
    list.addtail("2");
    list.addtail("3");
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(2), "3");

    list.deltail();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(1), "2");

    list.deltail();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0), "1");

    list.deltail();
    EXPECT_EQ(list.size(), 0);
    EXPECT_THROW(list.get(0), out_of_range);
}

TEST(LinkedList_test, remove)
{
    LinkedList list;
    EXPECT_THROW(list.remove("999"), runtime_error);
    list.addtail("1");
    list.addtail("2");
    list.addtail("3");
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(2), "3");

    list.remove("3");
    EXPECT_THROW(list.search("3"), runtime_error);
    EXPECT_EQ(list.size(), 2);
}