#include <bits/stdc++.h>
#include <gtest/gtest.h>
#include "../include/SerializingHashMap.h"

using namespace std;

class CustomDataStructureTest : public ::testing::Test
{
protected:
    struct MyStruct
    {
        char c;
        int a, b;
        float f;
        double d;
        bool operator==(MyStruct &rhs)
        {
            string s1 = primitiveByteBasedSerializer(this, sizeof(MyStruct));
            string s2 = primitiveByteBasedSerializer(&rhs, sizeof(MyStruct));

            return s1 == s2;
        }
    };

    static string primitiveByteBasedSerializer(void *keyPtr, int size)
    {
        string serialized = "";
        for (int i = 0; i < size; i++)
        {
            byte b = *((byte *)(keyPtr) + i);
            serialized += (char)b;
        }
        return serialized;
    };

    function<string(MyStruct)> keySerializerFunc = [this](MyStruct key) {
        return primitiveByteBasedSerializer(&key, sizeof(key));
    };

    SerializingHashMap<MyStruct, MyStruct> *myStructToMyStruct1 = NULL;

    CustomDataStructureTest()
    {
        myStructToMyStruct1 = new SerializingHashMap<MyStruct, MyStruct>(keySerializerFunc);
    }

    ~CustomDataStructureTest()
    {
        delete myStructToMyStruct1;
    }
};

TEST_F(CustomDataStructureTest, worksAsExpected)
{

    MyStruct obj1{'a', 1, 2, 0.123, 0.321};
    MyStruct obj2{'b', 2, 1, 0.321, 0.123};

    ASSERT_TRUE(myStructToMyStruct1->get(obj1) == *(new MyStruct{})) << "Default initialization of custom data type is invalid";
    ASSERT_FALSE(myStructToMyStruct1->exists(obj2));
    ASSERT_TRUE(myStructToMyStruct1->insert(obj1, obj2));
    ASSERT_TRUE(myStructToMyStruct1->get(obj1) == obj2);
    ASSERT_EQ(myStructToMyStruct1->size(), 1);

    myStructToMyStruct1->erase(obj2);

    ASSERT_EQ(myStructToMyStruct1->size(), 1);

    myStructToMyStruct1->erase(obj1);
    ASSERT_FALSE(myStructToMyStruct1->exists(obj1));
    ASSERT_EQ(myStructToMyStruct1->size(), 0);
}
