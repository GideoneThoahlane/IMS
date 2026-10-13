#include "Queue.h"

using namespace std;

Order::Order() {
    orderId = "";
    customerName = "";
    itemId = "";
    quantity = 0;
    unitPrice = 0.0;
    totalAmount = 0.0;
    timestamp = "";
    status = "Pending";
}

Order::Order(string oId, string cName, string iId, int qty, double price, string ts, string stat) {
    orderId = oId;
    customerName = cName;
    itemId = iId;
    quantity = qty;
    unitPrice = price;
    totalAmount = qty * price;
    timestamp = ts;
    status = stat;
}

string Order::getOrderId() { return orderId; }
string Order::getCustomerName() { return customerName; }
string Order::getItemId() { return itemId; }
int Order::getQuantity() { return quantity; }
double Order::getUnitPrice() { return unitPrice; }
double Order::getTotalAmount() { return totalAmount; }
string Order::getTimestamp() { return timestamp; }
string Order::getStatus() { return status; }
void Order::setStatus(string stat) { status = stat; }

void Order::display() {
    cout << orderId << "\t"
         << (customerName.length() > 18 ? customerName.substr(0, 15) + "..." : customerName) << "\t"
         << itemId << "\tQty: "
         << quantity << "\tTotal: $"
         << totalAmount << "\t["
         << status << "]\n";
}

string Order::serialize() {
    return orderId + "," + customerName + "," + itemId + "," + to_string(quantity) + ","
           + to_string(unitPrice) + "," + status + "," + timestamp;
}

Order Order::deserialize(string line) {
    size_t p1 = line.find(',');
    if (p1 == string::npos) return Order();
    size_t p2 = line.find(',', p1 + 1);
    if (p2 == string::npos) return Order();
    size_t p3 = line.find(',', p2 + 1);
    if (p3 == string::npos) return Order();
    size_t p4 = line.find(',', p3 + 1);
    if (p4 == string::npos) return Order();
    size_t p5 = line.find(',', p4 + 1);
    if (p5 == string::npos) return Order();
    size_t p6 = line.find(',', p5 + 1);
    if (p6 == string::npos) return Order();

    string oId = line.substr(0, p1);
    string cName = line.substr(p1 + 1, p2 - p1 - 1);
    string iId = line.substr(p2 + 1, p3 - p2 - 1);
    string qtyStr = line.substr(p3 + 1, p4 - p3 - 1);
    string priceStr = line.substr(p4 + 1, p5 - p4 - 1);
    string stat = line.substr(p5 + 1, p6 - p5 - 1);
    string ts = line.substr(p6 + 1);

    int qty = 0;
    double price = 0.0;
    try {
        if (!qtyStr.empty()) qty = stoi(qtyStr);
    } catch (...) { qty = 0; }
    try {
        if (!priceStr.empty()) price = stod(priceStr);
    } catch (...) { price = 0.0; }

    return Order(oId, cName, iId, qty, price, ts, stat);
}

QueueNode::QueueNode(Order orderVal) {
    data = orderVal;
    next = nullptr;
}

Queue::Queue() {
    frontNode = nullptr;
    rearNode = nullptr;
    count = 0;
}

Queue::~Queue() {
    clear();
}

void Queue::enqueue(Order order) {
    QueueNode* newNode = new QueueNode(order);
    if (rearNode == nullptr) {
        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
    count++;
}

bool Queue::dequeue(Order& outOrder) {
    if (isEmpty()) return false;
    QueueNode* temp = frontNode;
    outOrder = temp->data;
    frontNode = frontNode->next;
    if (frontNode == nullptr) rearNode = nullptr;
    delete temp;
    count--;
    return true;
}

bool Queue::peek(Order& outOrder) {
    if (isEmpty()) return false;
    outOrder = frontNode->data;
    return true;
}

bool Queue::isEmpty() { return frontNode == nullptr; }
int Queue::size() { return count; }

void Queue::clear() {
    Order dummy;
    while (dequeue(dummy)) {}
}

void Queue::displayAll() {
    cout << "\n----------------------------------------------------------------------------------------------------\n";
    cout << "Order ID\tCustomer\t\tItem\tQuantity\tTotal\tStatus\n";
    cout << "----------------------------------------------------------------------------------------------------\n";
    if (isEmpty()) {
        cout << "Order queue is empty.\n";
    } else {
        QueueNode* curr = frontNode;
        while (curr != nullptr) {
            curr->data.display();
            curr = curr->next;
        }
    }
    cout << "----------------------------------------------------------------------------------------------------\n";
    cout << "Total pending orders in FIFO Queue: " << count << "\n";
}

void Queue::loadFromFile(string filePath) {
    clear();
    ifstream inFile(filePath.c_str());
    if (!inFile.is_open()) return;

    string line;
    while (getline(inFile, line)) {
        if (!line.empty()) {
            enqueue(Order::deserialize(line));
        }
    }
    inFile.close();
}

void Queue::saveToFile(string filePath) {
    ofstream outFile(filePath.c_str(), ios::trunc);
    if (!outFile.is_open()) return;

    QueueNode* curr = frontNode;
    while (curr != nullptr) {
        outFile << curr->data.serialize() << "\n";
        curr = curr->next;
    }
    outFile.close();
}
