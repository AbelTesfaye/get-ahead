/*
    Tests a "string" to "int" implementation of "SerializingHashMap".
*/
#include <string>
#include <gtest/gtest.h>
#include "../include/SerializingHashMap.h"

class StringToIntTest : public ::testing::Test
{
protected:
    SerializingHashMap<std::string, int> stringToInt1, stringToInt2, stringToInt3;

    StringToIntTest()
    {
        stringToInt2.insert("hi0", 7);
        stringToInt2.insert("hi1", 6);
        stringToInt2.insert("hi2", 7);
        stringToInt2.insert("hi3", 8);
        stringToInt2.insert("hi4", 9);

        stringToInt3.insert("hi1", 6);
    }
};

TEST_F(StringToIntTest, insertsAndGetsWorksAsExpected)
{
    ASSERT_EQ(stringToInt1.size(), 0);
    ASSERT_TRUE(stringToInt1.insert("hi0", 2));
    ASSERT_EQ(stringToInt1.get("hi0"), 2);
    ASSERT_EQ(stringToInt1.size(), 1);

    ASSERT_EQ(stringToInt1.insert("hi0", 6), false);
    ASSERT_EQ(stringToInt1.insert("hi0", 7), false);

    ASSERT_EQ(stringToInt1.get("hi0"), 7);
    ASSERT_EQ(stringToInt1.size(), 1);

    ASSERT_TRUE(stringToInt1.insert("hi1", 6));
    ASSERT_TRUE(stringToInt1.insert("hi2", 7));
    ASSERT_TRUE(stringToInt1.insert("hi3", 8));
    ASSERT_TRUE(stringToInt1.insert("hi4", 9));
    ASSERT_EQ(stringToInt1.size(), 5);
    ASSERT_EQ(stringToInt1.get("hi1"), 6);
    ASSERT_EQ(stringToInt1.get("hi2"), 7);
    ASSERT_EQ(stringToInt1.get("hi3"), 8);
    ASSERT_EQ(stringToInt1.get("hi4"), 9);
}

TEST_F(StringToIntTest, existsAndEraseWorkAsExpected)
{

    ASSERT_FALSE(stringToInt2.exists("hi5"));
    ASSERT_FALSE(stringToInt2.exists("hi10"));
    ASSERT_EQ(stringToInt2.size(), 5);

    stringToInt2.erase("hi0");
    ASSERT_EQ(stringToInt2.size(), 4);

    stringToInt2.erase("hi2");
    ASSERT_EQ(stringToInt2.size(), 3);

    ASSERT_FALSE(stringToInt2.exists("hi0"));
    ASSERT_TRUE(stringToInt2.exists("hi1"));
    ASSERT_FALSE(stringToInt2.exists("hi2"));

    stringToInt2.erase("hi3");
    stringToInt2.erase("hi4");
    ASSERT_EQ(stringToInt2.size(), 1);
}

TEST_F(StringToIntTest, getIsZeroForUndefinedValues)
{

    for (int i = 0; i < 10; i++)
    {
        std::string k = "hi" + std::to_string(i);

        if (k == "hi1")
        {
            ASSERT_EQ(stringToInt3.get(k), 6);
            continue;
        }

        ASSERT_EQ(stringToInt3.get(k), 0);
    }
}