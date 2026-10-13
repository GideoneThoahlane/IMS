#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <string>
#include <fstream>
#include "Item.h"

using namespace std;

class ListNode {
public:
    Item data;
    ListNode* next;

    ListNode(Item itemVal);
};

class LinkedList {
private:
    ListNode* head;
    int count;

public:
    LinkedList();
    ~LinkedList();

    void insertHead(Item item);
    void insertTail(Item item);
    bool deleteById(string itemId);
    Item* searchById(string itemId);

    bool updateQuantity(string itemId, int newQty);
    bool updatePrice(string itemId, double newPrice);

    void displayAll();
    void clear();

    int size();
    bool isEmpty();

    int toArray(Item arr[], int maxLimit);
    void loadFromFile(string filePath);
    void saveToFile(string filePath);
};

#endif
