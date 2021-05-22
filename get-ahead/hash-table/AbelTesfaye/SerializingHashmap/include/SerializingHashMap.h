/*
  A hash table implementation that can be used on any data type.
  
    Basic example:
        SerializingHashMap<std::string, int> myMap;
        myMap.insert("some string", 7);
        myMap.get("some string"); // will return 7

    Example on a custom data type:
        - Check the file: CustomDataStructureTest.cpp
*/

#ifndef SERIALIZINGHASHMAP_H
#define SERIALIZINGHASHMAP_H

#include <string>
#include <vector>
#include <functional>

template <typename TKey, typename TValue>
class SerializingHashMap
{
public:
    /*
        `keySerializerFunction`: A function that converts "TKey" to a "string".
        `initSize`: Initial size of `container`.
    */
    SerializingHashMap(
        std::function<std::string(TKey)> keySerializerFunction = [](std::string str) {
            return str;
        },
        int initSize = 1) : container(std::vector<std::shared_ptr<Entry>>(initSize)), keySerializerFunction(keySerializerFunction)
    {
    }

    /*
        Creates or updates the hash table entry for `key`.

        `key`: Hash table entry key
        `value`: Hash table value at entry `key`

        Returns "true" if this insertion is a new insertion i.e if there was no
        entry previously created for `key` or if `key` was previously deleted.
    */
    bool insert(TKey key, TValue value)
    {
        std::string serializedKey = keySerializerFunction(key);

        if (itemCountInContainer == container.size())
        {
            _resizeContainer(itemCountInContainer == 0 ? 1 : container.size() * 2);
        }

        bool isNewInsertion = _insert(serializedKey, value, container);

        if (isNewInsertion)
            itemCountInContainer++;

        return isNewInsertion;
    }
    /*
        Returns the value in the hash table that is stored for `key`. 
        
        If `key` was not previously stored, then it will return a default constructor
        initialized object of "TValue".
    */
    TValue get(TKey key)
    {
        std::string serializedKey = keySerializerFunction(key);
        std::shared_ptr<Entry> entry = _get(serializedKey);
        if (entry == nullptr)
        {
            return TValue();
        }
        return entry->value;
    }
    /*
        Removes the hash table entry of `key`
    */
    void erase(TKey key)
    {
        std::string serializedKey = keySerializerFunction(key);
        int i = _getIndex(serializedKey);

        if (i == -1)
            return;

        std::shared_ptr<Entry> el = container[i];

        if (el != nullptr)
        {
            el->isDeleted = true;
            itemCountInContainer--;

            int half = container.size() / 2;
            if (itemCountInContainer < half)
            {
                _resizeContainer(half);
            }
        };
    }
    /*
        Returns "true" if a hash table entry for `key` exists.
    */
    bool exists(TKey key)
    {
        std::string serializedKey = keySerializerFunction(key);
        return _get(serializedKey) != nullptr;
    }
    /*
        Returns the number of valid entries in the hash table.
    */
    int size()
    {
        return itemCountInContainer;
    }

private:
    /*
        Hash table entry used in `container`.
    */
    struct Entry
    {
        std::string key;
        TValue value;
        bool isDeleted = false;
    };

    int itemCountInContainer = 0;
    std::vector<std::shared_ptr<Entry>> container;

    /*
        Function to convert "TKey" to "string".
    */
    std::function<std::string(TKey)> keySerializerFunction;

    /*
        Returns an index where `key` should be stored in `containerToUse`.
    */
    int _hash(std::string &key, std::vector<std::shared_ptr<Entry>> &containerToUse)
    {
        int idx = 0;

        for (int i = 0; i < key.size(); i++)
        {
            char byteAtOffset = key[i];
            idx = (idx + byteAtOffset) % containerToUse.size();
        }

        return idx;
    }
    /*
        Inserts or updates the `value` for `key` in `containerToUse`
    */
    bool _insert(std::string &key, TValue value, std::vector<std::shared_ptr<Entry>> &containerToUse)
    {
        int idx = _hash(key, containerToUse);

        for (int i = idx; i < containerToUse.size(); i++)
        {
            if (containerToUse[i] == nullptr)
            {
                containerToUse[i] = std::make_shared<Entry>(Entry{key, value});
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

        /*
            The following line will be executed if all the rows including and below
            idx are occupied by other keys.
        */
        containerToUse.push_back(std::make_shared<Entry>(Entry{key, value}));

        return true;
    }
    /*
        Returns the index of the "Entry" for `key` in `container`. If "Entry" at `key`
        is deleted or never inserted in the first place, then -1 will be returned.
    */
    int _getIndex(std::string &key)
    {
        int idx = _hash(key, container);

        for (int i = idx; i < container.size(); i++)
        {
            std::shared_ptr<Entry> el = container[i];

            if (el == nullptr)
                return -1;
            if (el->key == key && !el->isDeleted)
                return i;
        }

        return -1;
    }
    /*
        Returns the "Entry" for `key`. If "Entry" at `key`
        is deleted or never inserted in the first place, then nullptr will be returned.
    */
    std::shared_ptr<Entry> _get(std::string &key)
    {
        int idx = _getIndex(key);
        if (idx == -1)
            return nullptr;

        return container[idx];
    }
    /*
        Resizes `container` to `newSize`.
        
        - If `newSize` is 0 then the hash table will be emptied.
        - If `newSize` is less than or equal to `itemCountInContainer` then the resize 
            will not be performed.
    */
    void _resizeContainer(int newSize)
    {
        if(newSize < 0) return;
        if(newSize != 0 && newSize <= itemCountInContainer) return;

        std::vector<std::shared_ptr<Entry>> newContainer(newSize);

        if (newSize == 0)
        {
            itemCountInContainer = 0;
        }
        else
        {
            for (std::shared_ptr<Entry> el : container)
            {
                if (el == nullptr || el->isDeleted)
                    continue;

                std::string &key = el->key;
                TValue &value = el->value;

                _insert(key, value, newContainer);
            }
        }

        container = newContainer;
    }
};

#endif // SERIALIZINGHASHMAP_H