# IMS Inventory Management System

A C++ console application for managing inventory, users, warehouse routes, and business operations in a retail or warehouse setting.

## How to compile the program

From the project folder, run:

```bash
g++ *.cpp -std=c++17 -o IMS.exe
```

This compiles all project source files into an executable named `IMS.exe`.

## How to run the program

After compiling, run:

```bash
./IMS.exe
```

On Windows PowerShell, you can also run:

```powershell
.\IMS.exe
```

When the program starts, you will be given the option to:
- Login
- Sign up
- Exit

The system then loads role-based dashboards for:
- Cashier
- Store Manager
- System Administrator

## Group members

- Motloheloa Mokhele - 202404281
- Thaane Abraham Moletsane - 202322687
- Thoahlane Mampheli - 202403757

## Major features

- User authentication and role-based access control
- Inventory management with add, update, delete, and search operations
- Customer order queue processing
- Undo functionality using a stack
- Restock requests managed with a priority queue
- Binary search tree and hash table support for item lookup
- Warehouse graph with shortest-path route planning
- Audit logging for important actions and access events
- File-based persistence for users, inventory, orders, locations, and audit trails

## Project structure

The main application starts in `main.cpp` and connects the system components together. The project uses separate header and source files for each major component, including:

- `User.h` / `User.cpp`
- `AuthManager.h` / `AuthManager.cpp`
- `AuditLog.h` / `AuditLog.cpp`
- `Item.h` / `Item.cpp`
- `LinkedList.h` / `LinkedList.cpp`
- `Tree.h` / `Tree.cpp`
- `HashTable.h` / `HashTable.cpp`
- `Queue.h` / `Queue.cpp`
- `Stack.h` / `Stack.cpp`
- `PriorityQueue.h` / `PriorityQueue.cpp`
- `Graph.h` / `Graph.cpp`
- `Sorting.h` / `Sorting.cpp`

## Notes

The system stores persistent data in the `data/` folder:

- `users.txt`
- `inventory.txt`
- `orders.txt`
- `locations.txt`
- `audit_logs.txt`
