#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Order {
private:
    string orderId;
    string customerName;
    string itemId;
    int quantity;
    double unitPrice;
    double totalAmount;
    string timestamp;
    string status;

public:
    Order();
    Order(string oId, string cName, string iId, int qty, double price, string ts, string stat = "Pending");

    string getOrderId();
    string getCustomerName();
    string getItemId();
    int getQuantity();
    double getUnitPrice();
    double getTotalAmount();
    string getTimestamp();
    string getStatus();

    void setStatus(string stat);
    void display();
    string serialize();
    static Order deserialize(string line);
};

class QueueNode {
public:
    Order data;
    QueueNode* next;

    QueueNode(Order orderVal);
};

class Queue {
private:
    QueueNode* frontNode;
    QueueNode* rearNode;
    int count;

public:
    Queue();
    ~Queue();

    void enqueue(Order order);
    bool dequeue(Order& outOrder);
    bool peek(Order& outOrder);

    bool isEmpty();
    int size();
    void clear();

    void displayAll();
    void loadFromFile(string filePath);
    void saveToFile(string filePath);
};

#endif
