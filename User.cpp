#include "User.h"

using namespace std;

User::User() {
    username = "";
    password = "";
    role = "Cashier";
    fullName = "";
}

User::User(string uName, string pwd, string r, string fName) {
    username = uName;
    password = pwd;
    role = r;
    fullName = fName;
}

string User::getUsername() { return username; }
string User::getRole() { return role; }
string User::getFullName() { return fullName; }

void User::setPassword(string newPwd) { password = newPwd; }
void User::setRole(string newRole) { role = newRole; }
void User::setFullName(string newName) { fullName = newName; }

bool User::verifyPassword(string candidatePwd) {
    return password == candidatePwd;
}

int User::getRoleLevel() {
    if (role == "System Administrator") return 3;
    if (role == "Store Manager") return 2;
    return 1; // Cashier
}

bool User::hasPermission(int requiredLevel) {
    return getRoleLevel() >= requiredLevel;
}

string User::serialize() {
    return username + "," + password + "," + role + "," + fullName;
}

User User::deserialize(string line) {
    // Simple parsing with find and substr
    size_t p1 = line.find(',');
    if (p1 == string::npos) return User();
    size_t p2 = line.find(',', p1 + 1);
    if (p2 == string::npos) return User();
    size_t p3 = line.find(',', p2 + 1);
    if (p3 == string::npos) return User();

    string uName = line.substr(0, p1);
    string pwd = line.substr(p1 + 1, p2 - p1 - 1);
    string r = line.substr(p2 + 1, p3 - p2 - 1);
    string fName = line.substr(p3 + 1);

    return User(uName, pwd, r, fName);
}
