#include "LinkedList.h"
#include <iomanip>
using namespace std;

ListNode::ListNode(Item itemVal) {
    data = itemVal;
    next = nullptr;
}

LinkedList::LinkedList() {
    head = nullptr;
    count = 0;
}

LinkedList::~LinkedList() {
    clear();
}

void LinkedList::insertHead(Item item) {
    ListNode* newNode = new ListNode(item);
    newNode->next = head;
    head = newNode;
    count++;
}

void LinkedList::insertTail(Item item) {
    ListNode* newNode = new ListNode(item);
    if (head == nullptr) {
        head = newNode;
    } else {
        ListNode* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    count++;
}

bool LinkedList::deleteById(string itemId) {
    if (head == nullptr) return false;

    if (head->data.getItemId() == itemId) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
        count--;
        return true;
    }

    ListNode* curr = head;
    while (curr->next != nullptr && curr->next->data.getItemId() != itemId) {
        curr = curr->next;
    }

    if (curr->next != nullptr) {
        ListNode* toDelete = curr->next;
        curr->next = toDelete->next;
        delete toDelete;
        count--;
        return true;
    }

    return false;
}

Item* LinkedList::searchById(string itemId) {
    ListNode* curr = head;
    while (curr != nullptr) {
        if (curr->data.getItemId() == itemId) {
            return &(curr->data);
        }
        curr = curr->next;
    }
    return nullptr;
}

bool LinkedList::updateQuantity(string itemId, int newQty) {
    Item* it = searchById(itemId);
    if (it != nullptr) {
        it->setQuantity(newQty);
        return true;
    }
    return false;
}

bool LinkedList::updatePrice(string itemId, double newPrice) {
    Item* it = searchById(itemId);
    if (it != nullptr) {
        it->setUnitPrice(newPrice);
        return true;
    }
    return false;
}

//void LinkedList::displayAll() {
//    cout << "\n----------------------------------------------------------------------------------------------------\n";
//    cout << "Item ID\tName\t\t\tCategory\t\tStock\tPrice\tLocation\n";
//    cout << "----------------------------------------------------------------------------------------------------\n";
//    if (head == nullptr) {
//        cout << "Inventory list is empty.\n";
//    } else {
//        ListNode* curr = head;
//        while (curr != nullptr) {
//            curr->data.display();
//            curr = curr->next;
//        }
//    }
//    cout << "----------------------------------------------------------------------------------------------------\n";
//    cout << "Total items: " << count << "\n";
//}

void LinkedList::displayAll() {
    cout << "\n";
    cout << string(115, '-') << "\n";

    cout << left
         << setw(12) << "Item ID"
         << setw(28) << "Name"
         << setw(32) << "Category"
         << right << setw(8) << "Quantity"
         << setw(12) << "Price\t"
         << left << setw(20) << "Location" << "\n";

    cout << string(115, '-') << "\n";

    if (head == nullptr) {
        cout << "Inventory list is empty.\n";
    } else {
        ListNode* curr = head;

        while (curr != nullptr) {
            curr->data.display();
            curr = curr->next;
        }
    }

    cout << string(115, '-') << "\n";
    cout << "Total items: " << count << "\n";
}


void LinkedList::clear() {
    ListNode* curr = head;
    while (curr != nullptr) {
        ListNode* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
    head = nullptr;
    count = 0;
}

int LinkedList::size() { return count; }
bool LinkedList::isEmpty() { return count == 0; }

int LinkedList::toArray(Item arr[], int maxLimit) {
    int n = 0;
    ListNode* curr = head;
    while (curr != nullptr && n < maxLimit) {
        arr[n++] = curr->data;
        curr = curr->next;
    }
    return n;
}

void LinkedList::loadFromFile(string filePath) {
    clear();
    ifstream inFile(filePath.c_str());
    if (!inFile.is_open()) return;

    string line;
    while (getline(inFile, line)) {
        if (!line.empty()) {
            insertTail(Item::deserialize(line));
        }
    }
    inFile.close();
}

void LinkedList::saveToFile(string filePath) {
    ofstream outFile(filePath.c_str(), ios::trunc);
    if (!outFile.is_open()) return;

    ListNode* curr = head;
    while (curr != nullptr) {
        outFile << curr->data.serialize() << "\n";
        curr = curr->next;
    }
    outFile.close();
}
