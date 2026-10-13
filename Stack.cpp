#include "Stack.h"

using namespace std;

Action::Action() {
    actionType = "";
    description = "";
    timestamp = "";
}

Action::Action(string type, Item prev, Item current, string desc, string ts) {
    actionType = type;
    previousItem = prev;
    newItem = current;
    description = desc;
    timestamp = ts;
}

string Action::getActionType() { return actionType; }
Item Action::getPreviousItem() { return previousItem; }
Item Action::getNewItem() { return newItem; }
string Action::getDescription() { return description; }
string Action::getTimestamp() { return timestamp; }

void Action::display() {
    cout << "[" << timestamp << "] " << actionType << ": " << description << "\n";
}

StackNode::StackNode(Action actionVal) {
    data = actionVal;
    next = nullptr;
}

Stack::Stack() {
    topNode = nullptr;
    count = 0;
}

Stack::~Stack() {
    clear();
}

void Stack::push(Action action) {
    StackNode* newNode = new StackNode(action);
    newNode->next = topNode;
    topNode = newNode;
    count++;
}

bool Stack::pop(Action& outAction) {
    if (isEmpty()) return false;
    StackNode* temp = topNode;
    outAction = temp->data;
    topNode = topNode->next;
    delete temp;
    count--;
    return true;
}

bool Stack::peek(Action& outAction) {
    if (isEmpty()) return false;
    outAction = topNode->data;
    return true;
}

bool Stack::isEmpty() { return topNode == nullptr; }
int Stack::size() { return count; }

void Stack::clear() {
    Action dummy;
    while (pop(dummy)) {}
}

void Stack::displayAll() {
    cout << "\n---------------------------------------------------------------\n";
    cout << "             LIFO ACTION STACK (UNDO HISTORY)                  \n";
    cout << "---------------------------------------------------------------\n";
    if (isEmpty()) {
        cout << "Undo history is empty.\n";
    } else {
        StackNode* curr = topNode;
        int idx = 1;
        while (curr != nullptr) {
            cout << "#" << idx++ << " ";
            curr->data.display();
            curr = curr->next;
        }
    }
    cout << "---------------------------------------------------------------\n";
    cout << "Total undo actions: " << count << "\n";
}
