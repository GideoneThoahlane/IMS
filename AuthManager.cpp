#include "AuthManager.h"

using namespace std;

AuthManager::AuthManager() {
    usersFilePath = "data/users.txt";
    userCount = 0;
    loggedIn = false;
    auditLog = nullptr;
    loadUsers();
}

AuthManager::AuthManager(string path, AuditLog* logPtr) {
    usersFilePath = path;
    userCount = 0;
    loggedIn = false;
    auditLog = logPtr;
    loadUsers();
}

void AuthManager::loadUsers() {
    userCount = 0;
    ifstream inFile(usersFilePath.c_str());
    if (!inFile.is_open()) {
        users[0] = User("admin", "admin123", "System Administrator", "Administrator");
        users[1] = User("manager", "manager123", "Store Manager", "Alice Manager");
        users[2] = User("cashier", "cashier123", "Cashier", "Bob Cashier");
        userCount = 3;
        saveUsers();
        return;
    }

    string line;
    while (getline(inFile, line) && userCount < 50) {
        if (!line.empty()) {
            users[userCount] = User::deserialize(line);
            userCount++;
        }
    }
    inFile.close();
}

void AuthManager::saveUsers() {
    ofstream outFile(usersFilePath.c_str(), ios::trunc);
    if (!outFile.is_open()) return;
    for (int i = 0; i < userCount; ++i) {
        outFile << users[i].serialize() << "\n";
    }
    outFile.close();
}

bool AuthManager::login(string username, string password) {
    for (int i = 0; i < userCount; ++i) {
        if (users[i].getUsername() == username) {
            if (users[i].verifyPassword(password)) {
                currentUser = users[i];
                loggedIn = true;
                if (auditLog != nullptr) {
                    auditLog->record(currentUser.getUsername(), currentUser.getRole(), "AUTH_LOGIN", "Login success");
                }
                return true;
            } else {
                if (auditLog != nullptr) {
                    auditLog->record(username, "Unknown", "AUTH_FAIL", "Wrong password");
                }
                return false;
            }
        }
    }
    if (auditLog != nullptr) {
        auditLog->record(username, "Unknown", "AUTH_FAIL", "User not found");
    }
    return false;
}

void AuthManager::logout() {
    if (loggedIn) {
        if (auditLog != nullptr) {
            auditLog->record(currentUser.getUsername(), currentUser.getRole(), "AUTH_LOGOUT", "User logged out");
        }
        currentUser = User();
        loggedIn = false;
    }
}

bool AuthManager::isLoggedIn() { return loggedIn; }
User AuthManager::getCurrentUser() { return currentUser; }
string AuthManager::getCurrentUsername() { return currentUser.getUsername(); }
string AuthManager::getCurrentRole() { return currentUser.getRole(); }
int AuthManager::getCurrentRoleLevel() { return currentUser.getRoleLevel(); }

bool AuthManager::checkPermission(int requiredLevel, string actionName) {
    if (!loggedIn) {
        cout << "[ACCESS DENIED] Please login first.\n";
        return false;
    }
    if (!currentUser.hasPermission(requiredLevel)) {
        string roleReq = (requiredLevel == 3 ? "System Administrator" : (requiredLevel == 2 ? "Store Manager" : "Cashier"));
        cout << "[ACCESS DENIED] Your role '" << currentUser.getRole()
             << "' is not allowed to perform '" << actionName << "'. Required: " << roleReq << "\n";
        if (auditLog != nullptr) {
            auditLog->record(currentUser.getUsername(), currentUser.getRole(), "AUTH_DENIED", actionName);
        }
        return false;
    }
    return true;
}

bool AuthManager::addUser(string username, string password, string role, string fullName) {
    for (int i = 0; i < userCount; ++i) {
        if (users[i].getUsername() == username) {
            cout << "[ERROR] Username already taken: " << username << "\n";
            return false;
        }
    }
    if (userCount >= 50) {
        cout << "[ERROR] Maximum user limit reached.\n";
        return false;
    }
    users[userCount] = User(username, password, role, fullName);
    userCount++;
    saveUsers();
    if (auditLog != nullptr) {
        auditLog->record(currentUser.getUsername(), currentUser.getRole(), "USER_REGISTER", "Created: " + username);
    }
    cout << "[SUCCESS] Account registered for " << username << " (" << role << ").\n";
    return true;
}

void AuthManager::listAllUsers() {
    if (!checkPermission(3, "LIST_USERS")) return;
    cout << "\n---------------------------------------------------------------\n";
    cout << "Username        Role                    Full Name\n";
    cout << "---------------------------------------------------------------\n";
    for (int i = 0; i < userCount; ++i) {
        cout << users[i].getUsername() << "\t\t"
             << users[i].getRole() << "\t\t"
             << users[i].getFullName() << "\n";
    }
    cout << "---------------------------------------------------------------\n";
}

int AuthManager::getUserCount() {
    return userCount;
}
