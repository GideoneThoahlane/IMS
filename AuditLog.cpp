#include "AuditLog.h"
#include <vector>

using namespace std;

LogEntry::LogEntry() {
    username = "";
    role = "";
    action = "";
    details = "";
}

LogEntry::LogEntry(string u, string r, string a, string d) {
    username = u;
    role = r;
    action = a;
    details = d;
}

string LogEntry::getUsername() { return username; }
string LogEntry::getRole() { return role; }
string LogEntry::getAction() { return action; }
string LogEntry::getDetails() { return details; }

string LogEntry::toString() {
    return "[" + role + ": " + username + "] " + action + " - " + details;
}

string LogEntry::serialize() {
    return username + "|" + role + "|" + action + "|" + details;
}

LogEntry LogEntry::deserialize(string line) {
    size_t p1 = line.find('|');
    if (p1 == string::npos) return LogEntry();
    size_t p2 = line.find('|', p1 + 1);
    if (p2 == string::npos) return LogEntry();
    size_t p3 = line.find('|', p2 + 1);
    if (p3 == string::npos) return LogEntry();

    string firstField = line.substr(0, p1);
    bool hasLegacyTimestamp =
        firstField.length() >= 10 &&
        firstField[4] == '-' &&
        firstField[7] == '-';
    if (hasLegacyTimestamp) {
        size_t p4 = line.find('|', p3 + 1);
        if (p4 == string::npos) return LogEntry();
        return LogEntry(
            line.substr(p1 + 1, p2 - p1 - 1),
            line.substr(p2 + 1, p3 - p2 - 1),
            line.substr(p3 + 1, p4 - p3 - 1),
            line.substr(p4 + 1));
    }

    return LogEntry(
        firstField,
        line.substr(p1 + 1, p2 - p1 - 1),
        line.substr(p2 + 1, p3 - p2 - 1),
        line.substr(p3 + 1));
}

AuditLog::AuditLog() {
    filePath = "data/audit_logs.txt";
}

AuditLog::AuditLog(string path) {
    filePath = path;
}

void AuditLog::record(string username, string role, string action, string details) {
    LogEntry entry(username, role, action, details);

    ofstream outFile(filePath.c_str(), ios::app);
    if (outFile.is_open()) {
        outFile << entry.serialize() << "\n";
        outFile.close();
    }
}

void AuditLog::displayLogs() {
    cout << "\n=======================================================\n";
    cout << "                 IMMUTABLE AUDIT LOG TRAIL             \n";
    cout << "=======================================================\n";

    ifstream inFile(filePath.c_str());
    if (!inFile.is_open()) {
        cout << "No audit log file found.\n";
        return;
    }

    vector<string> lines;
    string line;
    while (getline(inFile, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    inFile.close();

    if (lines.empty()) {
        cout << "No audit log records found.\n";
        return;
    }

    cout << "Showing all " << lines.size() << " entries:\n";
    for (size_t i = 0; i < lines.size(); ++i) {
        LogEntry entry = LogEntry::deserialize(lines[i]);
        cout << entry.toString() << "\n";
    }
    cout << "=======================================================\n";
}
