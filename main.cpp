#include <iostream>
#include <string>
#include <fstream>

#include "User.h"
#include "AuthManager.h"
#include "AuditLog.h"
#include "Item.h"
#include "LinkedList.h"
#include "Queue.h"
#include "Stack.h"
#include "PriorityQueue.h"
#include "Tree.h"
#include "HashTable.h"
#include "Graph.h"
#include "Sorting.h"

using namespace std;

class InventorySystemApp {
private:
    AuditLog auditLog;
    AuthManager authManager;
    LinkedList inventoryList;
    Queue orderQueue;
    Stack actionStack;
    PriorityQueue restockHeap;
    Tree categoryBst;
    HashTable itemHashTable;
    Graph warehouseGraph;

    void rebuildFastIndex() {
        categoryBst.clear();
        itemHashTable.clear();
        Item items[100];
        int count = inventoryList.toArray(items, 100);
        for (int i = 0; i < count; ++i) {
            categoryBst.insert(items[i]);
            itemHashTable.insert(items[i]);
        }
    }

public:
    InventorySystemApp()
        : auditLog("data/audit_logs.txt"),
          authManager("data/users.txt", &auditLog) {
        inventoryList.loadFromFile("data/inventory.txt");
        orderQueue.loadFromFile("data/orders.txt");
        warehouseGraph.loadFromFile("data/locations.txt");
        rebuildFastIndex();
    }

    void saveAllData() {
        inventoryList.saveToFile("data/inventory.txt");
        orderQueue.saveToFile("data/orders.txt");
        warehouseGraph.saveToFile("data/locations.txt");
    }

    // Authentication
    bool loginUser(string username, string password) {
        return authManager.login(username, password);
    }

    void logoutUser() {
        authManager.logout();
    }

    bool registerUser(string username, string password, string role, string fullName) {
        return authManager.addUser(username, password, role, fullName);
    }

    string getCurrentFullName() {
        return authManager.getCurrentUser().getFullName();
    }

    string getCurrentRole() {
        return authManager.getCurrentRole();
    }

    int getCurrentRoleLevel() {
        return authManager.getCurrentRoleLevel();
    }

    // 1. Inventory View & Search
    void viewInventory() {
        inventoryList.displayAll();
    }

    void searchItem(string query) {
        // Fast O(1) Hash Table lookup first if exact ID
        Item* exact = itemHashTable.search(query);
        if (exact != nullptr) {
            cout << "\n[EXACT MATCH FOUND (Hash Table)]\n";
            cout << "ID:       " << exact->getItemId() << "\n";
            cout << "Name:     " << exact->getName() << "\n";
            cout << "Category: " << exact->getCategory() << "\n";
            cout << "Stock:    " << exact->getQuantity() << "\n";
            cout << "Price:    $" << exact->getUnitPrice() << "\n";
            cout << "Location: " << exact->getLocation() << "\n";
            return;
        }

        // Otherwise linear search by name or category
        Item items[100];
        int count = inventoryList.toArray(items, 100);
        Sorting::linearSearch(items, count, query);
    }

    // 2. Orders Processing
    bool placeOrder(string customerName, string itemId, int qty) {
        Item* it = inventoryList.searchById(itemId);
        if (it == nullptr) {
            cout << "[ERROR] Item " << itemId << " does not exist.\n";
            return false;
        }
        if (it->getQuantity() < qty) {
            cout << "[ERROR] Insufficient stock. Only " << it->getQuantity() << " available.\n";
            return false;
        }

        int nextIdNum = orderQueue.getMaxOrderIdNumber() + 1;
        string ordId = "ORD-" + to_string(nextIdNum);
        Order ord(ordId, customerName, itemId, qty, it->getUnitPrice(), "2026-10-12", "Pending");

        orderQueue.enqueue(ord);
        saveAllData();
        auditLog.record(authManager.getCurrentUsername(), authManager.getCurrentRole(),
                         "ORDER_PLACED", "Order " + ordId + " for " + itemId);
        cout << "[SUCCESS] Order " << ordId << " placed.\n";
        return true;
    }

    bool processNextOrder() {
        Order ord;
        if (!orderQueue.dequeue(ord)) {
            cout << "[INFO] No pending orders in queue.\n";
            return false;
        }

        Item* it = inventoryList.searchById(ord.getItemId());
        if (it != nullptr && it->getQuantity() >= ord.getQuantity()) {
            it->setQuantity(it->getQuantity() - ord.getQuantity());
            ord.setStatus("Fulfilled");
            rebuildFastIndex();
            saveAllData();

            auditLog.record(authManager.getCurrentUsername(), authManager.getCurrentRole(),
                             "ORDER_PROCESSED", "Fulfilled " + ord.getOrderId());
            cout << "[SUCCESS] Fulfilled order " << ord.getOrderId() << ": "
                 << ord.getQuantity() << " x " << ord.getItemId()
                 << " for " << ord.getCustomerName() << "\n";
            return true;
        } else {
            cout << "[WARNING] Insufficient stock for order " << ord.getOrderId() << ".\n";
            return false;
        }
    }

    void viewOrders() {
        orderQueue.displayAll();
    }

    // 3. Item Management (Manager & Admin)
    bool addItem(Item item) {
        if (!authManager.checkPermission(2, "ADD_ITEM")) return false;

        if (inventoryList.searchById(item.getItemId()) != nullptr) {
            cout << "[ERROR] Item ID already exists: " << item.getItemId() << "\n";
            return false;
        }

        inventoryList.insertTail(item);
        rebuildFastIndex();
        saveAllData();

        Action act("ADD_ITEM", Item(), item, "Added: " + item.getName(), "2026-10-12");
        actionStack.push(act);
        auditLog.record(authManager.getCurrentUsername(), authManager.getCurrentRole(),
                         "ITEM_CREATE", "Added: " + item.getItemId());
        cout << "[SUCCESS] Item " << item.getItemId() << " added.\n";
        return true;
    }

    bool updateItemStock(string itemId, int newQty) {
        if (!authManager.checkPermission(2, "UPDATE_STOCK")) return false;

        Item* existing = inventoryList.searchById(itemId);
        if (existing == nullptr) {
            cout << "[ERROR] Item not found: " << itemId << "\n";
            return false;
        }

        Item prev = *existing;
        inventoryList.updateQuantity(itemId, newQty);
        rebuildFastIndex();
        saveAllData();

        Item next = *existing;
        Action act("UPDATE_QTY", prev, next, "Stock updated for " + itemId, "2026-10-12");
        actionStack.push(act);
        auditLog.record(authManager.getCurrentUsername(), authManager.getCurrentRole(),
                         "ITEM_QTY_UPDATE", "Updated " + itemId + " stock to " + to_string(newQty));
        cout << "[SUCCESS] Stock updated for " << itemId << " to " << newQty << ".\n";
        return true;
    }

    bool updateItemPrice(string itemId, double newPrice) {
        if (!authManager.checkPermission(2, "UPDATE_PRICE")) return false;

        Item* existing = inventoryList.searchById(itemId);
        if (existing == nullptr) {
            cout << "[ERROR] Item not found: " << itemId << "\n";
            return false;
        }

        Item prev = *existing;
        inventoryList.updatePrice(itemId, newPrice);
        rebuildFastIndex();
        saveAllData();

        Item next = *existing;
        Action act("UPDATE_PRICE", prev, next, "Price updated for " + itemId, "2026-10-12");
        actionStack.push(act);
        auditLog.record(authManager.getCurrentUsername(), authManager.getCurrentRole(),
                         "ITEM_PRICE_UPDATE", "Updated " + itemId + " price to $" + to_string(newPrice));
        cout << "[SUCCESS] Price updated for " << itemId << " to $" << newPrice << ".\n";
        return true;
    }

    bool deleteItem(string itemId) {
        if (!authManager.checkPermission(2, "DELETE_ITEM")) return false;

        Item* existing = inventoryList.searchById(itemId);
        if (existing == nullptr) {
            cout << "[ERROR] Item not found: " << itemId << "\n";
            return false;
        }

        Item prev = *existing;
        inventoryList.deleteById(itemId);
        rebuildFastIndex();
        saveAllData();

        Action act("DELETE_ITEM", prev, Item(), "Deleted: " + prev.getName(), "2026-10-12");
        actionStack.push(act);
        auditLog.record(authManager.getCurrentUsername(), authManager.getCurrentRole(),
                         "ITEM_DELETE", "Deleted: " + itemId);
        cout << "[SUCCESS] Item " << itemId << " deleted.\n";
        return true;
    }

    // 4. Priority Restocking
    bool requestRestock(string itemId, int qty, string urgency) {
        if (!authManager.checkPermission(2, "REQUEST_RESTOCK")) return false;

        Item* it = inventoryList.searchById(itemId);
        string itemName = (it != nullptr ? it->getName() : "Unknown");

        static int rCount = 500;
        string reqId = "REQ-" + to_string(++rCount);
        RestockRequest req(reqId, itemId, itemName, qty, urgency, authManager.getCurrentUsername(), "2026-10-12");
        restockHeap.insert(req);

        auditLog.record(authManager.getCurrentUsername(), authManager.getCurrentRole(),
                         "RESTOCK_REQUEST", reqId + " (" + urgency + ")");
        cout << "[SUCCESS] Restock request " << reqId << " submitted.\n";
        return true;
    }

    bool fulfillRestock() {
        if (!authManager.checkPermission(2, "FULFILL_RESTOCK")) return false;

        RestockRequest req;
        if (!restockHeap.extractMax(req)) {
            cout << "[INFO] No restock requests pending.\n";
            return false;
        }

        Item* it = inventoryList.searchById(req.getItemId());
        if (it != nullptr) {
            it->setQuantity(it->getQuantity() + req.getRequestedQty());
            rebuildFastIndex();
            saveAllData();
        }

        auditLog.record(authManager.getCurrentUsername(), authManager.getCurrentRole(),
                         "RESTOCK_FULFILLED", req.getRequestId());
        cout << "[SUCCESS] Restocked: " << req.getItemName() << " (+" << req.getRequestedQty() << " units)\n";
        return true;
    }

    void viewRestockQueue() {
        restockHeap.displayAll();
    }

    // 5. Warehouse Map & Shortest Path (Graph)
    void viewWarehouseMap() {
        warehouseGraph.displayGraph();
    }

    void findWarehouseRoute(string start, string end) {
        warehouseGraph.findShortestPath(start, end);
    }

    // 6. Tree & Sorting Tools
    void viewBstTraversals() {
        categoryBst.displayInorder();
    }

    void sortInventory(int criterion) {
        Item items[100];
        int count = inventoryList.toArray(items, 100);
        Sorting::quickSort(items, 0, count - 1, criterion);
        cout << "\n--- Sorted Items ---\n";
        for (int i = 0; i < count; ++i) {
            items[i].display();
        }
    }

    // 7. Admin Features
    bool undoLastAction() {
        if (!authManager.checkPermission(3, "UNDO_ACTION")) return false;

        Action act;
        if (!actionStack.pop(act)) {
            cout << "[INFO] Nothing to undo.\n";
            return false;
        }

        string type = act.getActionType();
        if (type == "ADD_ITEM") {
            inventoryList.deleteById(act.getNewItem().getItemId());
        } else if (type == "DELETE_ITEM") {
            inventoryList.insertTail(act.getPreviousItem());
        } else if (type == "UPDATE_PRICE") {
            inventoryList.updatePrice(act.getPreviousItem().getItemId(), act.getPreviousItem().getUnitPrice());
        } else if (type == "UPDATE_QTY") {
            inventoryList.updateQuantity(act.getPreviousItem().getItemId(), act.getPreviousItem().getQuantity());
        }

        rebuildFastIndex();
        saveAllData();
        auditLog.record(authManager.getCurrentUsername(), authManager.getCurrentRole(),
                         "UNDO_ACTION", act.getDescription());
        cout << "[SUCCESS] Undone: " << act.getDescription() << "\n";
        return true;
    }

    void viewAuditTrail() {
        if (!authManager.checkPermission(3, "VIEW_AUDIT_LOG")) return;
        auditLog.displayLogs();
    }

    void listUsers() {
        if (!authManager.checkPermission(3, "LIST_USERS")) return;
        authManager.listAllUsers();
    }
};

// Simplified, clean role-based dashboards without redundancy
void handleCashierDashboard(InventorySystemApp& app) {
    int choice = -1;
    while (choice != 0) {
        cout << "\n======================================\n";
        cout << " CASHIER: " << app.getCurrentFullName() << "\n";
        cout << "======================================\n";
        cout << " 1. View Inventory\n";
        cout << " 2. Search Item\n";
        cout << " 3. Place Customer Order\n";
        cout << " 4. Process Next Order\n";
        cout << " 5. View Pending Orders\n";
        cout << " 0. Logout\n";
        cout << "--------------------------------------\n";
        cout << "Select option: ";

        if (!(cin >> choice)) break;

        if (choice == 1) {
            app.viewInventory();
        } else if (choice == 2) {
            string q;
            cout << "Enter Item ID or keyword: ";
            cin >> q;
            app.searchItem(q);
        } else if (choice == 3) {
            string cName, id;
            int qty;
            cin.ignore();
            cout << "Customer Name: ";
            getline(cin, cName);
            cout << "Item ID: ";
            cin >> id;
            cout << "Quantity: ";
            cin >> qty;
            app.placeOrder(cName, id, qty);
        } else if (choice == 4) {
            app.processNextOrder();
        } else if (choice == 5) {
            app.viewOrders();
        } else if (choice == 0) {
            app.logoutUser();
            cout << "Logged out successfully.\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

void handleManagerDashboard(InventorySystemApp& app) {
    int choice = -1;
    while (choice != 0) {
        cout << "\n======================================\n";
        cout << " MANAGER: " << app.getCurrentFullName() << "\n";
        cout << "======================================\n";
        cout << " 1. View Inventory\n";
        cout << " 2. Search Item\n";
        cout << " 3. Add Item\n";
        cout << " 4. Update Quantity or Price\n";
        cout << " 5. Delete Item\n";
        cout << " 6. Customer Orders (Place / Fulfill)\n";
        cout << " 7. Priority Restock (Request / Fulfill)\n";
        cout << " 8. Warehouse Map & Shortest Route\n";
        cout << " 0. Logout\n";
        cout << "--------------------------------------\n";
        cout << "Select option: ";

        if (!(cin >> choice)) break;

        if (choice == 1) {
            app.viewInventory();
        } else if (choice == 2) {
            string q;
            cout << "Enter Item ID or keyword: ";
            cin >> q;
            app.searchItem(q);
        } else if (choice == 3) {
            string id, name, cat, loc;
            int qty;
            double price;
            cout << "Item ID: ";
            cin >> id;
            cin.ignore();
            cout << "Name: ";
            getline(cin, name);
            cout << "Category: ";
            getline(cin, cat);
            cout << "Quantity: ";
            cin >> qty;
            cout << "Price ($): ";
            cin >> price;
            cin.ignore();
            cout << "Location (e.g. Aisle-A): ";
            getline(cin, loc);
            app.addItem(Item(id, name, cat, qty, price, loc));
        } else if (choice == 4) {
            string id;
            int opt;
            cout << "Item ID: ";
            cin >> id;
            cout << "Update [1: Quantity, 2: Price]: ";
            cin >> opt;
            if (opt == 1) {
                int qty;
                cout << "New Quantity: ";
                cin >> qty;
                app.updateItemStock(id, qty);
            } else {
                double p;
                cout << "New Price ($): ";
                cin >> p;
                app.updateItemPrice(id, p);
            }
        } else if (choice == 5) {
            string id;
            cout << "Item ID to delete: ";
            cin >> id;
            app.deleteItem(id);
        } else if (choice == 6) {
            int sub;
            cout << "[1: Process Next Order, 2: View Order Queue]: ";
            cin >> sub;
            if (sub == 1) app.processNextOrder();
            else app.viewOrders();
        } else if (choice == 7) {
            int sub;
            cout << "[1: Submit Restock Request, 2: Fulfill Next Restock, 3: View Restock Queue]: ";
            cin >> sub;
            if (sub == 1) {
                string id, tier;
                int qty;
                cout << "Item ID: ";
                cin >> id;
                cout << "Quantity: ";
                cin >> qty;
                cout << "Tier [Emergency / High / Normal / Low]: ";
                cin >> tier;
                app.requestRestock(id, qty, tier);
            } else if (sub == 2) {
                app.fulfillRestock();
            } else {
                app.viewRestockQueue();
            }
        } else if (choice == 8) {
            string from, to;
            cout << "Start Location: ";
            cin >> from;
            cout << "Destination Location: ";
            cin >> to;
            app.findWarehouseRoute(from, to);
        } else if (choice == 0) {
            app.logoutUser();
            cout << "Logged out successfully.\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

void handleAdminDashboard(InventorySystemApp& app) {
    int choice = -1;
    while (choice != 0) {
        cout << "\n======================================\n";
        cout << " ADMINISTRATOR: " << app.getCurrentFullName() << "\n";
        cout << "======================================\n";
        cout << " 1. View Inventory\n";
        cout << " 2. Add / Delete / Update Item\n";
        cout << " 3. Orders Management\n";
        cout << " 4. Priority Restocking\n";
        cout << " 5. Undo Last Action\n";
        cout << " 6. View Audit Log Trail\n";
        cout << " 7. List All Registered Users\n";
        cout << " 8. Warehouse Routes & BST View\n";
        cout << " 0. Logout\n";
        cout << "--------------------------------------\n";
        cout << "Select option: ";

        if (!(cin >> choice)) break;

        if (choice == 1) {
            app.viewInventory();
        } else if (choice == 2) {
            int sub;
            cout << "[1: Add Item, 2: Update Stock, 3: Delete Item]: ";
            cin >> sub;
            if (sub == 1) {
                string id, name, cat, loc;
                int qty;
                double price;
                cout << "Item ID: ";
                cin >> id;
                cin.ignore();
                cout << "Name: ";
                getline(cin, name);
                cout << "Category: ";
                getline(cin, cat);
                cout << "Quantity: ";
                cin >> qty;
                cout << "Price ($): ";
                cin >> price;
                cin.ignore();
                cout << "Location: ";
                getline(cin, loc);
                app.addItem(Item(id, name, cat, qty, price, loc));
            } else if (sub == 2) {
                string id;
                int q;
                cout << "Item ID: ";
                cin >> id;
                cout << "New Stock: ";
                cin >> q;
                app.updateItemStock(id, q);
            } else if (sub == 3) {
                string id;
                cout << "Item ID to delete: ";
                cin >> id;
                app.deleteItem(id);
            }
        } else if (choice == 3) {
            int sub;
            cout << "[1: Process Next Order, 2: View Orders]: ";
            cin >> sub;
            if (sub == 1) app.processNextOrder();
            else app.viewOrders();
        } else if (choice == 4) {
            int sub;
            cout << "[1: Fulfill Restock, 2: View Restock Queue]: ";
            cin >> sub;
            if (sub == 1) app.fulfillRestock();
            else app.viewRestockQueue();
        } else if (choice == 5) {
            app.undoLastAction();
        } else if (choice == 6) {
            app.viewAuditTrail();
        } else if (choice == 7) {
            app.listUsers();
        } else if (choice == 8) {
            int sub;
            cout << "[1: Warehouse Routes, 2: BST Traversal]: ";
            cin >> sub;
            if (sub == 1) app.viewWarehouseMap();
            else app.viewBstTraversals();
        } else if (choice == 0) {
            app.logoutUser();
            cout << "Logged out successfully.\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

int main() {
    InventorySystemApp app;

    int choice = -1;
    while (choice != 0) {
        cout << "\n======================================\n";
        cout << "     INVENTORY MANAGEMENT SYSTEM      \n";
        cout << "======================================\n";
        cout << " 1. Login\n";
        cout << " 2. Sign Up\n";
        cout << " 0. Exit\n";
        cout << "--------------------------------------\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) break;

        if (choice == 1) {
            string username, password;
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;

            if (app.loginUser(username, password)) {
                cout << "\n[SUCCESS] Welcome, " << app.getCurrentFullName() << "!\n";
                int level = app.getCurrentRoleLevel();
                if (level == 3) {
                    handleAdminDashboard(app);
                } else if (level == 2) {
                    handleManagerDashboard(app);
                } else {
                    handleCashierDashboard(app);
                }
            } else {
                cout << "[ERROR] Invalid username or password.\n";
            }
        } else if (choice == 2) {
            string username, password, fullName, role;
            int roleChoice;
            cout << "Choose Username: ";
            cin >> username;
            cout << "Choose Password: ";
            cin >> password;
            cin.ignore();
            cout << "Full Name: ";
            getline(cin, fullName);
            cout << "Role [1: Cashier, 2: Store Manager, 3: System Administrator]: ";
            cin >> roleChoice;

            if (roleChoice == 3) role = "System Administrator";
            else if (roleChoice == 2) role = "Store Manager";
            else role = "Cashier";

            app.registerUser(username, password, role, fullName);
        } else if (choice == 0) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice. Please enter 1, 2, or 0.\n";
        }
    }

    return 0;
}
