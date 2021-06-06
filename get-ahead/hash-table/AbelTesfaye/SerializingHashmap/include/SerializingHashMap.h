#ifndef SERIALIZINGHASHMAP_H
#define SERIALIZINGHASHMAP_H

#include <string>
#include <vector>
#include <functional>

/*
  A hash table implementation that can be used on any data type.
  
    Basic example:
        SerializingHashMap<std::string, int> myMap;
        myMap.insert("some string", 7);
        myMap.get("some string"); // will return 7

    Example on a custom data type:
        - Check the file: CustomDataStructureTest.cpp
*/
template <typename TKey, typename TValue>
class SerializingHashMap
{
    /*
        Allow access to private members for the following tests
    */
    FRIEND_TEST(StringToIntTest, insertsAndGetsWorksAsExpected);
    FRIEND_TEST(StringToIntTest, existsAndEraseWorkAsExpected);
    FRIEND_TEST(CustomDataStructureTest, worksAsExpected);

public:
    SerializingHashMap(
        const std::function<std::string(TKey)> keySerializerFunction = [](std::string str)
        { return str; },
        const int initSize = 1) : container(std::vector<std::unique_ptr<Entry>>(initSize)), keySerializerFunction(keySerializerFunction)
    {
    }

    /*
        Returns "true" if this insertion is a new insertion i.e if there was no
        entry previously created for `key` or if `key` was previously deleted.
    */
    bool insert(const TKey key, const TValue value)
    {
        if (itemCountInContainer == container.size())
        {
            resizeHashTableContainer(itemCountInContainer == 0 ? 1 : container.size() * 2);
        }

        return serializedKeyInsert(keySerializerFunction(key), value);
    }
    /*
        Returns the value in the hash table that is stored for `key`. 
        
        If `key` was not previously stored, then it will return a default constructor
        initialized object of "TValue".
    */
    TValue get(const TKey key) const
    {
        Entry *entry = serializedKeyGetEntry(keySerializerFunction(key));
        if (entry == nullptr)
        {
            return TValue();
        }
        return entry->value;
    }

    void erase(const TKey key)
    {
        int i = serializedKeyGetEntryIndex(keySerializerFunction(key));

        if (i == -1)
            return;

        Entry *el = container[i].get();

        if (el != nullptr)
        {
            el->isDeleted = true;
            itemCountInContainer--;

            int half = container.size() / 2;
            if (itemCountInContainer < half)
            {
                resizeHashTableContainer(half);
            }
        };
    }

    bool exists(const TKey key) const
    {
        return serializedKeyGetEntry(keySerializerFunction(key)) != nullptr;
    }

    int size() const
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
    std::vector<std::unique_ptr<Entry>> container;

    std::function<std::string(TKey)> keySerializerFunction;

    int hash(const std::string &key, const int containerSize) const
    {
        int idx = 0;

        for (int i = 0; i < key.size(); i++)
        {
            char byteAtOffset = key[i];
            idx = (idx + byteAtOffset) % containerSize;
        }

        return idx;
    }

    bool serializedKeyInsert(const std::string &key, const TValue value)
    {
        bool isNewInsertion = serializedKeyInsertInContainer(key, value, container);

        if (isNewInsertion)
            itemCountInContainer++;

        return isNewInsertion;
    }

    bool serializedKeyInsertInContainer(const std::string &key, const TValue value, std::vector<std::unique_ptr<Entry>> &containerToUse)
    {
        int idx = hash(key, containerToUse.size());

        /*
            The code below searches for the correct index to place the hash table entry.

            One of the following can happen:
            - Use the first index that points to "nullptr"
            - Use the first index that has a key which is equal to `key`.
                This can happen when the value of a pre-existing entry is getting updated. 
                Or it can happen when we want to update an entry that was deleted but a resize didn't occur.
            
            - If none of the above conditions are fullfilled, then append this entry to the container
        */
        for (int i = idx; i < containerToUse.size(); i++)
        {
            if (containerToUse[i] == nullptr)
            {
                containerToUse[i] = std::make_unique<Entry>(Entry{key, value});
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

        containerToUse.push_back(std::make_unique<Entry>(Entry{key, value}));

        return true;
    }
    /*
        Returns the index of the "Entry" for `key` in `container`. If "Entry" at `key`
        is deleted or never inserted in the first place, then -1 will be returned.
    */
    int serializedKeyGetEntryIndex(const std::string &key) const
    {
        int idx = hash(key, container.size());

        for (int i = idx; i < container.size(); i++)
        {
            Entry *el = container[i].get();

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
    Entry *serializedKeyGetEntry(const std::string &key) const
    {
        int idx = serializedKeyGetEntryIndex(key);
        if (idx == -1)
            return nullptr;

        return container[idx].get();
    }
    /*
        Resizes `container` to `newSize`.
        
        - If `newSize` is 0 then the hash table will be emptied.
        - If `newSize` is less than or equal to `itemCountInContainer` then the resize 
            will not be performed.
    */
    void resizeHashTableContainer(const int newSize)
    {
        if (newSize < 0)
            return;
        if (newSize != 0 && newSize <= itemCountInContainer)
            return;

        std::vector<std::unique_ptr<Entry>> newContainer(newSize);

        if (newSize == 0)
        {
            itemCountInContainer = 0;
        }
        else
        {
            for (std::unique_ptr<Entry> &el : container)
            {
                if (el == nullptr || el->isDeleted)
                    continue;

                std::string &key = el->key;
                TValue &value = el->value;

                serializedKeyInsertInContainer(key, value, newContainer);
            }
        }

        /*
            Destroy all entries that `container` owned and transfer ownership
            of entries from `newContainer` to `container`.
        */
        container.clear();
        for (std::unique_ptr<Entry> &el : newContainer)
        {
            container.push_back(std::move(el));
        }
    }
};

#endif // SERIALIZINGHASHMAP_H