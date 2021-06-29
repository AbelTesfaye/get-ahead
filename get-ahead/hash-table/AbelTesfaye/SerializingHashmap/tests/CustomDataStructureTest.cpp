/*
    Tests a "TestData" to "TestData" implementation of "SerializingHashMap".
*/
#include <gtest/gtest.h>

#include <functional>
#include <string>

#include "../include/SerializingHashMap.h"

class CustomDataStructureTest : public ::testing::Test {
 protected:
  struct TestData {
    char charData;
    int integerData1, integerData2;
    float floatData;
    double doubleData;
    std::string stringData;

    TestData(char ac = 'a', int ai1 = 456, int ai2 = 654, float af = .456f,
             double ad = .654f, std::string as = "abc")
        : charData(ac),
          integerData1(ai1),
          integerData2(ai2),
          floatData(af),
          doubleData(ad),
          stringData(as) {}
    /*
        Checks equality of the "string" representation of both structs.
    */
    const bool operator==(const TestData rhs) const {
      return this->toString() == rhs.toString();
    }
    /*
        Converts "TestData" to a "string".
    */
    const std::string toString() const {
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
  std::function<std::string(TestData)> keySerializerFunc = [=](TestData key) {
    return key.toString();
  };
};

TEST_F(CustomDataStructureTest, insertGetAndSizeWorkAsExpected) {
  SerializingHashMap<TestData, TestData> testDataToTestData(0,
                                                            keySerializerFunc);

  TestData obj1{'a', 1, 2, 0.123, 0.321};
  TestData obj2{'b', 2, 1, 0.321, 0.123};
  ASSERT_EQ(testDataToTestData.size(), 0);
  ASSERT_TRUE(testDataToTestData.insert(obj1, obj2));
  ASSERT_EQ(testDataToTestData.size(), 1);
  ASSERT_EQ(testDataToTestData.get(obj1), obj2);
}

TEST_F(CustomDataStructureTest, existsAndEraseWorkAsExpected) {
  SerializingHashMap<TestData, TestData> testDataToTestData(0,
                                                            keySerializerFunc);

  TestData obj1{'a', 1, 2, 0.123, 0.321};
  TestData obj2{'b', 2, 1, 0.321, 0.123};
  ASSERT_FALSE(testDataToTestData.exists(obj1));
  ASSERT_FALSE(testDataToTestData.exists(obj2));
  ASSERT_TRUE(testDataToTestData.insert(obj1, obj2));
  ASSERT_TRUE(testDataToTestData.exists(obj1));
  testDataToTestData.erase(obj1);
  ASSERT_FALSE(testDataToTestData.exists(obj1));
}

TEST_F(CustomDataStructureTest, getDefaultConstructedObjectForUndefinedKey) {
  SerializingHashMap<TestData, TestData> testDataToTestData(0,
                                                            keySerializerFunc);

  TestData obj1{'a', 1, 2, 0.123, 0.321};
  ASSERT_TRUE(testDataToTestData.get(obj1) == TestData())
      << "Default construction of custom data type is invalid";
}

TEST_F(CustomDataStructureTest, initCapacityWorksAsExpected) {
  SerializingHashMap<TestData, TestData> testDataToTestData0(0,
                                                             keySerializerFunc),
      testDataToTestData10(10, keySerializerFunc),
      testDataToTestData1000(1000, keySerializerFunc);

  ASSERT_EQ(testDataToTestData0.container.size(), 0);
  ASSERT_EQ(testDataToTestData10.container.size(), 10);
  ASSERT_EQ(testDataToTestData1000.container.size(), 1000);
}

TEST_F(CustomDataStructureTest, resizeZeroCapacityContainer) {
  SerializingHashMap<TestData, TestData> testDataToTestData(0,
                                                            keySerializerFunc);
  TestData obj1{'a', 1, 2, 0.123, 0.321};
  TestData obj2{'b', 2, 1, 0.321, 0.123};
  TestData obj3{'c', 3, 1, 0.132, 0.1};
  TestData obj4{'d', 4, 2, 0.312, 0.2};
  ASSERT_EQ(testDataToTestData.container.size(), 0);
  testDataToTestData.insert(obj1, obj2);
  ASSERT_EQ(testDataToTestData.container.size(), 1);
  testDataToTestData.insert(obj2, obj3);
  ASSERT_EQ(testDataToTestData.container.size(), 2);
  testDataToTestData.insert(obj3, obj4);
  ASSERT_EQ(testDataToTestData.container.size(), 4);

  testDataToTestData.erase(obj3);
  ASSERT_EQ(testDataToTestData.container.size(), 2);
  testDataToTestData.erase(obj2);
  ASSERT_EQ(testDataToTestData.container.size(), 1);
  testDataToTestData.erase(obj1);
  ASSERT_EQ(testDataToTestData.container.size(), 0);
}

TEST_F(CustomDataStructureTest, resizeWithInitCapacityWorksAsExpected) {
  SerializingHashMap<TestData, TestData> testDataToTestData8(8,
                                                             keySerializerFunc);

  TestData obj1{'a'}, obj2{'b'}, obj3{'c'}, obj4{'d'}, obj5{'!'}, obj6{')'},
      obj7{'&'}, obj8{';'}, obj9{'/'}, obj10{'Q'};

  ASSERT_EQ(testDataToTestData8.container.size(), 8);
  testDataToTestData8.insert(obj1, obj2);
  ASSERT_EQ(testDataToTestData8.container.size(), 8);
  testDataToTestData8.insert(obj2, obj3);
  testDataToTestData8.insert(obj3, obj4);
  testDataToTestData8.insert(obj4, obj5);
  testDataToTestData8.insert(obj5, obj6);
  ASSERT_EQ(testDataToTestData8.container.size(), 8);
  testDataToTestData8.insert(obj6, obj7);
  testDataToTestData8.insert(obj7, obj8);
  testDataToTestData8.insert(obj8, obj9);
  ASSERT_EQ(testDataToTestData8.container.size(), 8);
  testDataToTestData8.insert(obj9, obj10);
  ASSERT_EQ(testDataToTestData8.container.size(), 16);

  testDataToTestData8.erase(obj9);
  ASSERT_EQ(testDataToTestData8.container.size(), 8);
  testDataToTestData8.erase(obj8);
  testDataToTestData8.erase(obj7);
  testDataToTestData8.erase(obj6);
  testDataToTestData8.erase(obj5);
  testDataToTestData8.erase(obj4);
  ASSERT_EQ(testDataToTestData8.container.size(), 4);
  testDataToTestData8.erase(obj3);
  ASSERT_EQ(testDataToTestData8.container.size(), 2);
  testDataToTestData8.erase(obj2);
  ASSERT_EQ(testDataToTestData8.container.size(), 1);
  testDataToTestData8.erase(obj1);
  ASSERT_EQ(testDataToTestData8.container.size(), 0);
}

TEST_F(CustomDataStructureTest, customHashFunctionWorksAsExpected) {
  const std::function<int(const std::string &, const int)> myHashFunction =
      [](const std::string &key, const int containerSize) {
        if (containerSize == 0) return -1;

        int idx = 0;

        for (int i = 0; i < key.size(); i++) {
          char byteAtOffset = key[i];
          idx = (idx + byteAtOffset) % containerSize;
        }

        return idx;
      };

  SerializingHashMap<TestData, TestData> testDataToTestData(
      0, keySerializerFunc, myHashFunction);

  TestData obj1{'a', 1, 2, 0.123, 0.321};
  TestData obj2{'b', 2, 1, 0.321, 0.123};
  TestData obj3{'c'};

  ASSERT_TRUE(testDataToTestData.insert(obj1, obj2));
  ASSERT_EQ(testDataToTestData
                .container[myHashFunction(obj1.toString(),
                                          testDataToTestData.container.size())]
                ->key,
            obj1.toString());

  ASSERT_TRUE(testDataToTestData.insert(obj2, obj3));
  ASSERT_EQ(testDataToTestData
                .container[myHashFunction(obj2.toString(),
                                          testDataToTestData.container.size())]
                ->key,
            obj2.toString());
}