#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <iostream>
#include <string>

using namespace std;

class RestockRequest {
private:
    string requestId;
    string itemId;
    string itemName;
    int requestedQty;
    string urgencyTier; // Emergency, High, Normal, Low
    int priorityScore;
    string requestedBy;
    string timestamp;

public:
    RestockRequest();
    RestockRequest(string rId, string iId, string iName, int qty, string tier, string user, string ts);

    string getRequestId();
    string getItemId();
    string getItemName();
    int getRequestedQty();
    string getUrgencyTier();
    int getPriorityScore();
    string getRequestedBy();
    string getTimestamp();

    void display();
};

class PriorityQueue {
private:
    RestockRequest heap[100];
    int count;

    int parent(int index);
    int leftChild(int index);
    int rightChild(int index);
    void swapItems(int i, int j);
    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    PriorityQueue();

    void insert(RestockRequest request);
    bool extractMax(RestockRequest& outRequest);
    bool peek(RestockRequest& outRequest);

    bool isEmpty();
    int size();
    void clear();

    void displayAll();
};

#endif
