/*
    Tests a "string" to "int" implementation of "SerializingHashMap".
*/
#include <gtest/gtest.h>

#include <string>

#include "../include/SerializingHashMap.h"

class StringToIntTest : public ::testing::Test {};

TEST_F(StringToIntTest, insertGetAndSizeWorkAsExpected) {
  SerializingHashMap<std::string, int> stringToInt;

  ASSERT_EQ(stringToInt.size(), 0);
  ASSERT_TRUE(stringToInt.insert("hi0", 2));
  ASSERT_EQ(stringToInt.get("hi0"), 2);
  ASSERT_EQ(stringToInt.size(), 1);

  ASSERT_EQ(stringToInt.insert("hi0", 6), false);
  ASSERT_EQ(stringToInt.insert("hi0", 7), false);

  ASSERT_EQ(stringToInt.get("hi0"), 7);
  ASSERT_EQ(stringToInt.size(), 1);

  ASSERT_TRUE(stringToInt.insert("hi1", 6));
  ASSERT_TRUE(stringToInt.insert("hi2", 7));
  ASSERT_TRUE(stringToInt.insert("hi3", 8));
  ASSERT_TRUE(stringToInt.insert("hi4", 9));
  ASSERT_EQ(stringToInt.size(), 5);
  ASSERT_EQ(stringToInt.get("hi1"), 6);
  ASSERT_EQ(stringToInt.get("hi2"), 7);
  ASSERT_EQ(stringToInt.get("hi3"), 8);
  ASSERT_EQ(stringToInt.get("hi4"), 9);
}

TEST_F(StringToIntTest, existsAndEraseWorkAsExpected) {
  SerializingHashMap<std::string, int> stringToInt;

  stringToInt.insert("hi0", 7);
  stringToInt.insert("hi1", 6);
  stringToInt.insert("hi2", 7);
  stringToInt.insert("hi3", 8);
  stringToInt.insert("hi4", 9);

  ASSERT_FALSE(stringToInt.exists("hi5"));
  ASSERT_FALSE(stringToInt.exists("hi10"));
  ASSERT_EQ(stringToInt.size(), 5);

  stringToInt.erase("hi0");
  ASSERT_EQ(stringToInt.size(), 4);

  stringToInt.erase("hi2");
  ASSERT_EQ(stringToInt.size(), 3);

  ASSERT_FALSE(stringToInt.exists("hi0"));
  ASSERT_TRUE(stringToInt.exists("hi1"));
  ASSERT_FALSE(stringToInt.exists("hi2"));

  stringToInt.erase("hi3");
  stringToInt.erase("hi4");
  ASSERT_EQ(stringToInt.size(), 1);

  stringToInt.erase("hi1");
}

TEST_F(StringToIntTest, getZeroForUndefinedValues) {
  SerializingHashMap<std::string, int> stringToInt;
  stringToInt.insert("THIS IS A DEFINED KEY", 6);

  for (int i = 0; i < 10; i++) {
    std::string key = "THIS IS **NOT** A DEFINED KEY: " + std::to_string(i);

    if (key == "THIS IS A DEFINED KEY") {
      ASSERT_EQ(stringToInt.get(key), 6);
      continue;
    }

    ASSERT_EQ(stringToInt.get(key), 0);
  }
}

TEST_F(StringToIntTest, initSizeWorksAsExpected) {
  SerializingHashMap<std::string, int> stringToInt0, stringToInt10(10),
      stringToInt1000(1000);

  ASSERT_EQ(stringToInt0.container.size(), 0);
  ASSERT_EQ(stringToInt10.container.size(), 10);
  ASSERT_EQ(stringToInt1000.container.size(), 1000);
}

TEST_F(StringToIntTest, resizeWorksAsExpected) {
  SerializingHashMap<std::string, int> stringToInt;

  ASSERT_EQ(stringToInt.container.size(), 0);
  stringToInt.insert("hi0", 123);
  ASSERT_EQ(stringToInt.container.size(), 1);
  stringToInt.insert("hi1", 123);
  ASSERT_EQ(stringToInt.container.size(), 2);
  stringToInt.insert("hi2", 123);
  ASSERT_EQ(stringToInt.container.size(), 4);

  stringToInt.erase("hi2");
  ASSERT_EQ(stringToInt.container.size(), 2);
  stringToInt.erase("hi1");
  ASSERT_EQ(stringToInt.container.size(), 1);
  stringToInt.erase("hi0");
  ASSERT_EQ(stringToInt.container.size(), 0);
}

TEST_F(StringToIntTest, resizeWithInitSizeWorksAsExpected) {
  SerializingHashMap<std::string, int> stringToInt8(8);

  ASSERT_EQ(stringToInt8.container.size(), 8);
  stringToInt8.insert("hi0", 123);
  ASSERT_EQ(stringToInt8.container.size(), 8);
  stringToInt8.insert("hi1", 123);
  stringToInt8.insert("hi2", 123);
  stringToInt8.insert("hi3", 123);
  stringToInt8.insert("hi4", 123);
  ASSERT_EQ(stringToInt8.container.size(), 8);
  stringToInt8.insert("hi5", 123);
  stringToInt8.insert("hi6", 123);
  stringToInt8.insert("hi7", 123);
  ASSERT_EQ(stringToInt8.container.size(), 8);
  stringToInt8.insert("hi8", 123);
  ASSERT_EQ(stringToInt8.container.size(), 16);

  stringToInt8.erase("hi8");
  ASSERT_EQ(stringToInt8.container.size(), 8);
  stringToInt8.erase("hi7");
  stringToInt8.erase("hi6");
  stringToInt8.erase("hi5");
  stringToInt8.erase("hi4");
  stringToInt8.erase("hi3");
  ASSERT_EQ(stringToInt8.container.size(), 4);
  stringToInt8.erase("hi2");
  ASSERT_EQ(stringToInt8.container.size(), 2);
  stringToInt8.erase("hi1");
  ASSERT_EQ(stringToInt8.container.size(), 1);
  stringToInt8.erase("hi0");
  ASSERT_EQ(stringToInt8.container.size(), 0);
}

TEST_F(StringToIntTest, customHashFunctionWorksAsExpected) {
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

  SerializingHashMap<std::string, int> stringToInt(
      0, DefaultSerializingHashMapFunctions::keySerializerFunction<std::string>,
      myHashFunction);

  ASSERT_TRUE(stringToInt.insert("hi0", 123));
  ASSERT_EQ(stringToInt
                .container[myHashFunction("hi0", stringToInt.container.size())]
                ->key,
            "hi0");

  ASSERT_TRUE(stringToInt.insert("hi1", 123));
  ASSERT_EQ(stringToInt
                .container[myHashFunction("hi1", stringToInt.container.size())]
                ->key,
            "hi1");
}