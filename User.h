#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>

using namespace std;

class User {
private:
    string username;
    string password;
    string role; // Cashier, Store Manager, System Administrator
    string fullName;

public:
    User();
    User(string uName, string pwd, string r, string fName);

    string getUsername();
    string getRole();
    string getFullName();

    void setPassword(string newPwd);
    void setRole(string newRole);
    void setFullName(string newName);

    bool verifyPassword(string candidatePwd);
    int getRoleLevel();
    bool hasPermission(int requiredLevel);

    string serialize();
    static User deserialize(string line);
};

#endif
