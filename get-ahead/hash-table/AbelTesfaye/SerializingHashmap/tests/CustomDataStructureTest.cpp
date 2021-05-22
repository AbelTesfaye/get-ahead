/*
    Tests a "MyStruct" to "MyStruct" implementation of "SerializingHashMap".
*/
#include <string>
#include <functional>
#include <gtest/gtest.h>
#include "../include/SerializingHashMap.h"

class CustomDataStructureTest : public ::testing::Test
{
protected:
    /*
        The packed attribute prevents the compiler from padding extra bytes
        between struct members. For more info: [https://riptutorial.com/gcc/example/31724/attribute-packed]
    */
    struct __attribute__((__packed__)) MyStruct
    {
        char c;
        int a, b;
        float f;
        double d;
        MyStruct(char ac = 'a', int aa = 456, int ab = 654, float af = .456f, double ad = .654f) : c(ac), a(aa), b(ab), f(af), d(ad)
        {
        }
        /* 
            Checks equality of the byte representation of both structs.
        */
        bool operator==(MyStruct rhs)
        {
            std::string s1 = primitiveByteBasedSerializer(this, sizeof(MyStruct));
            std::string s2 = primitiveByteBasedSerializer(&rhs, sizeof(MyStruct));
            return s1 == s2;
        }
    };

    /*
        Converts any data type to its byte representation.
    */
    static std::string primitiveByteBasedSerializer(void *keyPtr, int size)
    {
        std::string serialized = "";
        for (int i = 0; i < size; i++)
        {
            char b = *((char *)(keyPtr) + i);
            serialized += b;
        }
        return serialized;
    };

    /*
        Function that is used to "string"ify a "MyStruct" object.
    */
    std::function<std::string(MyStruct)> keySerializerFunc = [=](MyStruct key) {
        return primitiveByteBasedSerializer(&key, sizeof(key));
    };
};

TEST_F(CustomDataStructureTest, worksAsExpected)
{

    MyStruct obj1{'a', 1, 2, 0.123, 0.321};
    MyStruct obj2{'b', 2, 1, 0.321, 0.123};

    SerializingHashMap<MyStruct, MyStruct> myStructToMyStruct1(keySerializerFunc);

    ASSERT_TRUE(myStructToMyStruct1.get(obj1) == MyStruct()) << "Default construction of custom data type is invalid";
    ASSERT_FALSE(myStructToMyStruct1.exists(obj2));
    ASSERT_TRUE(myStructToMyStruct1.insert(obj1, obj2));
    ASSERT_TRUE(myStructToMyStruct1.get(obj1) == obj2);
    ASSERT_EQ(myStructToMyStruct1.size(), 1);

    myStructToMyStruct1.erase(obj2);

    ASSERT_EQ(myStructToMyStruct1.size(), 1);

    myStructToMyStruct1.erase(obj1);
    ASSERT_FALSE(myStructToMyStruct1.exists(obj1));
    ASSERT_EQ(myStructToMyStruct1.size(), 0);
}
