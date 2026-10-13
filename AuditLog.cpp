#include "AuditLog.h"

using namespace std;

LogEntry::LogEntry() {
    timestamp = "";
    username = "";
    role = "";
    action = "";
    details = "";
}

LogEntry::LogEntry(string ts, string u, string r, string a, string d) {
    timestamp = ts;
    username = u;
    role = r;
    action = a;
    details = d;
}

string LogEntry::getTimestamp() { return timestamp; }
string LogEntry::getUsername() { return username; }
string LogEntry::getRole() { return role; }
string LogEntry::getAction() { return action; }
string LogEntry::getDetails() { return details; }

string LogEntry::toString() {
    return "[" + timestamp + "] [" + role + ": " + username + "] " + action + " - " + details;
}

string LogEntry::serialize() {
    return timestamp + "|" + username + "|" + role + "|" + action + "|" + details;
}

LogEntry LogEntry::deserialize(string line) {
    size_t p1 = line.find('|');
    if (p1 == string::npos) return LogEntry();
    size_t p2 = line.find('|', p1 + 1);
    if (p2 == string::npos) return LogEntry();
    size_t p3 = line.find('|', p2 + 1);
    if (p3 == string::npos) return LogEntry();
    size_t p4 = line.find('|', p3 + 1);
    if (p4 == string::npos) return LogEntry();

    string ts = line.substr(0, p1);
    string u = line.substr(p1 + 1, p2 - p1 - 1);
    string r = line.substr(p2 + 1, p3 - p2 - 1);
    string a = line.substr(p3 + 1, p4 - p3 - 1);
    string d = line.substr(p4 + 1);

    return LogEntry(ts, u, r, a, d);
}

AuditLog::AuditLog() {
    filePath = "data/audit_logs.txt";
}

AuditLog::AuditLog(string path) {
    filePath = path;
}

void AuditLog::record(string username, string role, string action, string details) {
    // Simple timestamp format without ctime
    string ts = "2026-10-12";
    LogEntry entry(ts, username, role, action, details);

    ofstream outFile(filePath.c_str(), ios::app);
    if (outFile.is_open()) {
        outFile << entry.serialize() << "\n";
        outFile.close();
    }
}

void AuditLog::displayLogs(int maxCount) {
    cout << "\n=======================================================\n";
    cout << "                 IMMUTABLE AUDIT LOG TRAIL             \n";
    cout << "=======================================================\n";

    ifstream inFile(filePath.c_str());
    if (!inFile.is_open()) {
        cout << "No audit log file found.\n";
        return;
    }

    // Read lines into simple array of strings
    string lines[200];
    int count = 0;
    string line;
    while (getline(inFile, line) && count < 200) {
        if (!line.empty()) {
            lines[count] = line;
            count++;
        }
    }
    inFile.close();

    if (count == 0) {
        cout << "No audit log records found.\n";
        return;
    }

    int start = (count > maxCount) ? (count - maxCount) : 0;
    cout << "Showing " << (count - start) << " of " << count << " entries:\n";
    for (int i = start; i < count; ++i) {
        LogEntry entry = LogEntry::deserialize(lines[i]);
        cout << entry.toString() << "\n";
    }
    cout << "=======================================================\n";
}

void AuditLog::clearLogs() {
    ofstream outFile(filePath.c_str(), ios::trunc);
    if (outFile.is_open()) outFile.close();
}
