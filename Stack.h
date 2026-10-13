#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <string>
#include "Item.h"

using namespace std;

class Action {
private:
    string actionType; // ADD_ITEM, DELETE_ITEM, UPDATE_PRICE, UPDATE_QTY
    Item previousItem;
    Item newItem;
    string description;
    string timestamp;

public:
    Action();
    Action(string type, Item prev, Item current, string desc, string ts);

    string getActionType();
    Item getPreviousItem();
    Item getNewItem();
    string getDescription();
    string getTimestamp();

    void display();
};

class StackNode {
public:
    Action data;
    StackNode* next;

    StackNode(Action actionVal);
};

class Stack {
private:
    StackNode* topNode;
    int count;

public:
    Stack();
    ~Stack();

    void push(Action action);
    bool pop(Action& outAction);
    bool peek(Action& outAction);

    bool isEmpty();
    int size();
    void clear();

    void displayAll();
};

#endif
