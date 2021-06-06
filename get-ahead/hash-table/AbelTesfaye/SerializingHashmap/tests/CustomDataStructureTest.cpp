/*
    Tests a "TestData" to "TestData" implementation of "SerializingHashMap".
*/
#include <string>
#include <functional>
#include <gtest/gtest.h>
#include "../include/SerializingHashMap.h"

class CustomDataStructureTest : public ::testing::Test
{
protected:
    struct TestData
    {
        char charData;
        int integerData1, integerData2;
        float floatData;
        double doubleData;
        std::string stringData;

        TestData(char ac = 'a', int ai1 = 456, int ai2 = 654, float af = .456f, double ad = .654f, std::string as = "abc") : charData(ac), integerData1(ai1), integerData2(ai2), floatData(af), doubleData(ad), stringData(as)
        {
        }
        /* 
            Checks equality of the "string" representation of both structs.
        */
        bool operator==(TestData rhs)
        {
            return this->toString() == rhs.toString();
        }
        /*
            Converts "TestData" to a "string".
        */
        std::string toString()
        {
            /* 
                Make sure `uniqueSeparator` is a sequence of characters that will
                never occur in `stringData`.
            */
            std::string uniqueSeparator = "|#^(}';";
            std::string str = "";

            str += charData + uniqueSeparator;
            str += std::to_string(integerData1) + uniqueSeparator;
            str += std::to_string(integerData2) + uniqueSeparator;
            str += std::to_string(floatData) + uniqueSeparator;
            str += std::to_string(doubleData) + uniqueSeparator;
            str += stringData;

            return str;
        }
    };
    /*
        Function that is used to "string"ify a "TestData" object.
    */
    std::function<std::string(TestData)> keySerializerFunc = [=](TestData key)
    {
        return key.toString();
    };
};

TEST_F(CustomDataStructureTest, worksAsExpected)
{

    TestData obj1{'a', 1, 2, 0.123, 0.321};
    TestData obj2{'b', 2, 1, 0.321, 0.123};

    SerializingHashMap<TestData, TestData> testDataToTestData1(keySerializerFunc);

    ASSERT_TRUE(testDataToTestData1.get(obj1) == TestData()) << "Default construction of custom data type is invalid";
    ASSERT_FALSE(testDataToTestData1.exists(obj2));
    ASSERT_TRUE(testDataToTestData1.insert(obj1, obj2));
    ASSERT_TRUE(testDataToTestData1.get(obj1) == obj2);
    ASSERT_EQ(testDataToTestData1.size(), 1);

    testDataToTestData1.erase(obj2);

    ASSERT_EQ(testDataToTestData1.size(), 1);

    testDataToTestData1.erase(obj1);
    ASSERT_FALSE(testDataToTestData1.exists(obj1));
    ASSERT_EQ(testDataToTestData1.size(), 0);
}
