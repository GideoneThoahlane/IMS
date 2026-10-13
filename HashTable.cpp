#include "HashTable.h"

using namespace std;

HashNode::HashNode(Item itemVal) {
    data = itemVal;
    next = nullptr;
}

HashTable::HashTable() {
    count = 0;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = nullptr;
    }
}

HashTable::~HashTable() {
    clear();
}

void HashTable::clear() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashNode* curr = table[i];
        while (curr != nullptr) {
            HashNode* toDelete = curr;
            curr = curr->next;
            delete toDelete;
        }
        table[i] = nullptr;
    }
    count = 0;
}

int HashTable::hashFunction(string key) {
    unsigned long hash = 5381;
    for (size_t i = 0; i < key.length(); ++i) {
        hash = ((hash << 5) + hash) + (unsigned char)key[i];
    }
    return (int)(hash % TABLE_SIZE);
}

void HashTable::insert(Item item) {
    int bucket = hashFunction(item.getItemId());
    HashNode* curr = table[bucket];

    while (curr != nullptr) {
        if (curr->data.getItemId() == item.getItemId()) {
            curr->data = item;
            return;
        }
        curr = curr->next;
    }

    HashNode* newNode = new HashNode(item);
    newNode->next = table[bucket];
    table[bucket] = newNode;
    count++;
}

bool HashTable::remove(string itemId) {
    int bucket = hashFunction(itemId);
    HashNode* curr = table[bucket];
    HashNode* prev = nullptr;

    while (curr != nullptr) {
        if (curr->data.getItemId() == itemId) {
            if (prev == nullptr) {
                table[bucket] = curr->next;
            } else {
                prev->next = curr->next;
            }
            delete curr;
            count--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

Item* HashTable::search(string itemId) {
    int bucket = hashFunction(itemId);
    HashNode* curr = table[bucket];

    while (curr != nullptr) {
        if (curr->data.getItemId() == itemId) {
            return &(curr->data);
        }
        curr = curr->next;
    }
    return nullptr;
}

int HashTable::size() { return count; }

int HashTable::getCollisionCount() {
    int collisions = 0;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        int chainLen = 0;
        HashNode* curr = table[i];
        while (curr != nullptr) {
            chainLen++;
            curr = curr->next;
        }
        if (chainLen > 1) {
            collisions += (chainLen - 1);
        }
    }
    return collisions;
}

void HashTable::displayTable() {
    cout << "\n--------------------------------------------------------------------\n";
    cout << "          HASH TABLE BUCKETS & COLLISION CHAINS (O(1) LOOKUP)       \n";
    cout << "--------------------------------------------------------------------\n";
    cout << "Capacity: " << TABLE_SIZE << " buckets | Items: " << count
         << " | Collisions: " << getCollisionCount() << "\n";
    cout << "--------------------------------------------------------------------\n";
    for (int i = 0; i < TABLE_SIZE; ++i) {
        if (table[i] != nullptr) {
            cout << "Bucket [" << i << "]: ";
            HashNode* curr = table[i];
            while (curr != nullptr) {
                cout << "[" << curr->data.getItemId() << ": " << curr->data.getName() << "]";
                if (curr->next != nullptr) cout << " -> ";
                curr = curr->next;
            }
            cout << "\n";
        }
    }
    cout << "--------------------------------------------------------------------\n";
}
