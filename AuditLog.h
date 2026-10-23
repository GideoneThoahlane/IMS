#ifndef AUDITLOG_H
#define AUDITLOG_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class LogEntry {
private:
    string username;
    string role;
    string action;
    string details;

public:
    LogEntry();
    LogEntry(string u, string r, string a, string d);

    string getUsername();
    string getRole();
    string getAction();
    string getDetails();

    string toString();
    string serialize();
    static LogEntry deserialize(string line);
};

class AuditLog {
private:
    string filePath;

public:
    AuditLog();
    AuditLog(string path);

    void record(string username, string role, string action, string details);
    void displayLogs();
};

#endif
