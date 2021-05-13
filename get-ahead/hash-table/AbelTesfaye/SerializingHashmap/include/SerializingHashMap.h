#ifndef SERIALIZINGHASHMAP_H
#define SERIALIZINGHASHMAP_H

#include <bits/stdc++.h>

using namespace std;

template <typename TKey, typename TValue>
class SerializingHashMap
{
    struct SerializingHashMapEntry
    {
        string key;
        TValue value;
        bool isDeleted = false;
    };

    int itemCountInContainer = 0;
    vector<SerializingHashMapEntry *> container;
    function<string(TKey)> keySerializerFunction;

    int _hash(string &key, vector<SerializingHashMapEntry *> &containerToUse)
    {
        int idx = 0;

        for (int i = 0; i < key.size(); i++)
        {
            char byteAtOffset = key[i];
            idx = (idx + byteAtOffset) % containerToUse.size();
        }

        return idx;
    }
    bool _insert(string &key, TValue value, vector<SerializingHashMap::SerializingHashMapEntry *> &containerToUse)
    {
        int idx = _hash(key, containerToUse);

        for (int i = idx; i < containerToUse.size(); i++)
        {
            if (containerToUse[i] == NULL)
            {
                containerToUse[i] = new SerializingHashMap::SerializingHashMapEntry{key, value};
                return true;
            }

            if (containerToUse[i]->key == key)
            {
                containerToUse[i]->value = value;

                if (containerToUse[i]->isDeleted)
                {
                    containerToUse[i]->isDeleted = false;
                    return true;
                }

                return false;
            }
        }

        /**
         *
         * The following lines will be executed if all the rows
         * including and below idx are occupied by other keys
         *  
         */
        containerToUse.push_back(new SerializingHashMap::SerializingHashMapEntry{key, value});

        return true;
    }
    int _getIndex(string &key)
    {
        int idx = _hash(key, container);

        for (int i = idx; i < container.size(); i++)
        {
            SerializingHashMap::SerializingHashMapEntry *el = container[i];

            if (el == NULL)
                return -1;
            if (el->key == key && !el->isDeleted)
                return i;
        }

        return -1;
    }
    SerializingHashMapEntry *_get(string &key)
    {
        int idx = _getIndex(key);
        if (idx == -1)
            return NULL;

        return container[idx];
    }

public:
    SerializingHashMap(
        function<string(TKey)> keySerializerFunction = [](string str) {
            return str;
        },
        int initSize = 1) : container(vector<SerializingHashMap::SerializingHashMapEntry *>(initSize)), keySerializerFunction(keySerializerFunction)
    {
    }

    bool insert(TKey key, TValue value)
    {
        string serializedKey = keySerializerFunction(key);

        if (itemCountInContainer == container.size())
        {
            resizeContainer(itemCountInContainer == 0 ? 1 : container.size() * 2);
        }

        bool isNewInsertion = _insert(serializedKey, value, container);

        if (isNewInsertion)
            itemCountInContainer++;

        return isNewInsertion;
    }
    void resizeContainer(int newSize)
    {
        vector<SerializingHashMap::SerializingHashMapEntry *> newContainer(newSize);

        if (newSize == 0)
        {
            itemCountInContainer = 0;
        }
        else
        {
            for (SerializingHashMap::SerializingHashMapEntry *el : container)
            {
                if (el == NULL || el->isDeleted)
                    continue;

                string &key = el->key;
                TValue &value = el->value;

                _insert(key, value, newContainer);
            }
        }

        container = newContainer;
    }
    TValue get(TKey key)
    {
        string serializedKey = keySerializerFunction(key);
        SerializingHashMap::SerializingHashMapEntry *entry = _get(serializedKey);
        if (entry == NULL)
        {
            return *(new TValue());
        }
        return entry->value;
    }
    void erase(TKey key)
    {
        string serializedKey = keySerializerFunction(key);
        int i = _getIndex(serializedKey);

        if (i == -1)
            return;

        SerializingHashMap::SerializingHashMapEntry *el = container[i];

        if (el != NULL)
        {
            el->isDeleted = true;
            itemCountInContainer--;

            int half = container.size() / 2;
            if (itemCountInContainer < half)
            {
                resizeContainer(half);
            }
        };
    }
    bool exists(TKey key)
    {
        string serializedKey = keySerializerFunction(key);
        return _get(serializedKey) != NULL;
    }
    int size()
    {
        return itemCountInContainer;
    }
};

#endif // SERIALIZINGHASHMAP_H