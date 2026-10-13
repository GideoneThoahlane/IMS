#include "PriorityQueue.h"

using namespace std;

RestockRequest::RestockRequest() {
    requestId = "";
    itemId = "";
    itemName = "";
    requestedQty = 0;
    urgencyTier = "Normal";
    priorityScore = 200;
    requestedBy = "";
    timestamp = "";
}

RestockRequest::RestockRequest(string rId, string iId, string iName, int qty, string tier, string user, string ts) {
    requestId = rId;
    itemId = iId;
    itemName = iName;
    requestedQty = qty;
    urgencyTier = tier;
    requestedBy = user;
    timestamp = ts;

    int bonus = (qty > 50) ? 50 : qty;
    if (tier == "Emergency") {
        priorityScore = 400 + bonus;
    } else if (tier == "High") {
        priorityScore = 300 + bonus;
    } else if (tier == "Normal") {
        priorityScore = 200 + bonus;
    } else {
        priorityScore = 100 + bonus;
    }
}

string RestockRequest::getRequestId() { return requestId; }
string RestockRequest::getItemId() { return itemId; }
string RestockRequest::getItemName() { return itemName; }
int RestockRequest::getRequestedQty() { return requestedQty; }
string RestockRequest::getUrgencyTier() { return urgencyTier; }
int RestockRequest::getPriorityScore() { return priorityScore; }
string RestockRequest::getRequestedBy() { return requestedBy; }
string RestockRequest::getTimestamp() { return timestamp; }

void RestockRequest::display() {
    cout << requestId << "\t["
         << urgencyTier << " - Score: " << priorityScore << "]\t"
         << itemId << "\t"
         << (itemName.length() > 20 ? itemName.substr(0, 17) + "..." : itemName) << "\tQty: "
         << requestedQty << "\tBy: "
         << requestedBy << "\n";
}

PriorityQueue::PriorityQueue() {
    count = 0;
}

int PriorityQueue::parent(int index) { return (index - 1) / 2; }
int PriorityQueue::leftChild(int index) { return (2 * index) + 1; }
int PriorityQueue::rightChild(int index) { return (2 * index) + 2; }

void PriorityQueue::swapItems(int i, int j) {
    RestockRequest temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void PriorityQueue::heapifyUp(int index) {
    while (index > 0 && heap[parent(index)].getPriorityScore() < heap[index].getPriorityScore()) {
        swapItems(index, parent(index));
        index = parent(index);
    }
}

void PriorityQueue::heapifyDown(int index) {
    int maxIndex = index;
    int left = leftChild(index);
    int right = rightChild(index);

    if (left < count && heap[left].getPriorityScore() > heap[maxIndex].getPriorityScore()) {
        maxIndex = left;
    }
    if (right < count && heap[right].getPriorityScore() > heap[maxIndex].getPriorityScore()) {
        maxIndex = right;
    }

    if (index != maxIndex) {
        swapItems(index, maxIndex);
        heapifyDown(maxIndex);
    }
}

void PriorityQueue::insert(RestockRequest request) {
    if (count >= 100) {
        cout << "[ERROR] Priority queue is full.\n";
        return;
    }
    heap[count] = request;
    count++;
    heapifyUp(count - 1);
}

bool PriorityQueue::extractMax(RestockRequest& outRequest) {
    if (isEmpty()) return false;
    outRequest = heap[0];
    heap[0] = heap[count - 1];
    count--;
    if (count > 0) {
        heapifyDown(0);
    }
    return true;
}

bool PriorityQueue::peek(RestockRequest& outRequest) {
    if (isEmpty()) return false;
    outRequest = heap[0];
    return true;
}

bool PriorityQueue::isEmpty() { return count == 0; }
int PriorityQueue::size() { return count; }
void PriorityQueue::clear() { count = 0; }

void PriorityQueue::displayAll() {
    cout << "\n----------------------------------------------------------------------------------------------------\n";
    cout << "Request ID\tTier & Score\t\tItem\tName\t\t\tQty\tRequested By\n";
    cout << "----------------------------------------------------------------------------------------------------\n";
    if (isEmpty()) {
        cout << "Priority restock heap is empty.\n";
    } else {
        for (int i = 0; i < count; ++i) {
            heap[i].display();
        }
    }
    cout << "----------------------------------------------------------------------------------------------------\n";
    cout << "Total items in Restock Max-Heap: " << count << "\n";
}
