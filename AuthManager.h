#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <iostream>
#include <string>
#include <fstream>
#include "User.h"
#include "AuditLog.h"

using namespace std;

class AuthManager {
private:
    string usersFilePath;
    User users[50];
    int userCount;
    User currentUser;
    bool loggedIn;
    AuditLog* auditLog;

    void loadUsers();
    void saveUsers();

public:
    AuthManager();
    AuthManager(string path, AuditLog* logPtr);

    bool login(string username, string password);
    void logout();

    bool isLoggedIn();
    User getCurrentUser();
    string getCurrentUsername();
    string getCurrentRole();
    int getCurrentRoleLevel();

    bool checkPermission(int requiredLevel, string actionName = "");

    bool addUser(string username, string password, string role, string fullName);
    void listAllUsers();
    int getUserCount();
};

#endif
