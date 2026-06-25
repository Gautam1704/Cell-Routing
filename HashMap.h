#ifndef HASHMAP_H
#define HASHMAP_H

#include <string>

class HashMap {
private:
    struct Node {
        std::string key;
        int value;         
        Node* next;
        Node(const std::string& k, int v) : key(k), value(v), next(nullptr) {}
    };

    Node** buckets;        
    int bucketCount;
    int itemCount;

    unsigned long hashKey(const std::string& key) const {
        unsigned long h = 5381;
        for (int i = 0; i < (int)key.size(); ++i) {
            h = ((h << 5) + h) + (unsigned char)key[i];   // h * 33 + c
        }
        return h % bucketCount;
    }

public:
    HashMap(int initialBuckets = 101)
        : bucketCount(initialBuckets), itemCount(0) {
        buckets = new Node*[bucketCount];
        for (int i = 0; i < bucketCount; ++i) buckets[i] = nullptr;
    }

    ~HashMap() {
        for (int i = 0; i < bucketCount; ++i) {
            Node* cur = buckets[i];
            while (cur) {
                Node* tmp = cur;
                cur = cur->next;
                delete tmp;
            }
        }
        delete[] buckets;
    }

    void put(const std::string& key, int value) {
        unsigned long idx = hashKey(key);
        Node* cur = buckets[idx];
        while (cur) {
            if (cur->key == key) { 
                cur->value = value;
                return;
            }
            cur = cur->next;
        }
        Node* node = new Node(key, value);
        node->next = buckets[idx];
        buckets[idx] = node;
        ++itemCount;
    }

    bool get(const std::string& key, int& outValue) const {
        unsigned long idx = hashKey(key);
        Node* cur = buckets[idx];
        while (cur) {
            if (cur->key == key) {
                outValue = cur->value;
                return true;
            }
            cur = cur->next;
        }
        return false;
    }

    int size() const { return itemCount; }
};

#endif
