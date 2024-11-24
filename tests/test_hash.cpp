#include <gtest/gtest.h>
#include "/home/anton/3laba_2kurs/src/hash.cpp"

TEST(Hashtable_test, addAndGet)
{
    Hashtable ht(10);

    ht.add("key1", "value1");
    ht.add("key2", "value2");

    EXPECT_EQ(ht.get("key1"), "value1");
    EXPECT_EQ(ht.get("key2"), "value2");
    
    EXPECT_THROW(ht.get("nonexistent"), runtime_error);
}

TEST(Hashtable_test, updateValue)
{
    Hashtable ht(10);

    ht.add("key1", "value1");
    ht.add("key1", "new_value");

    EXPECT_EQ(ht.get("key1"), "new_value");
}

TEST(Hashtable_test, remove)
{
    Hashtable ht(10);

    ht.add("key1", "value1");
    ht.add("key2", "value2");

    ht.remove("key1");

    EXPECT_THROW(ht.get("key1"), runtime_error);

    EXPECT_EQ(ht.get("key2"), "value2");
}

TEST(Hashtable_test, collision)
{
    Hashtable ht(2); 

    ht.add("key1", "value1");
    ht.add("key2", "value2");

    EXPECT_EQ(ht.get("key1"), "value1");
    EXPECT_EQ(ht.get("key2"), "value2");
}