#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>
#include "Item.h"

using namespace std;

class HashNode {
public:
    Item data;
    HashNode* next;

    HashNode(Item itemVal);
};

class HashTable {
private:
    static const int TABLE_SIZE = 37;
    HashNode* table[TABLE_SIZE];
    int count;

    int hashFunction(string key);

public:
    HashTable();
    ~HashTable();

    void insert(Item item);
    bool remove(string itemId);
    Item* search(string itemId);

    int size();
    int getCollisionCount();
    void displayTable();
    void clear();
};

#endif
