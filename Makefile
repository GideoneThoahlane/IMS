CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

SRCS = main.cpp User.cpp AuthManager.cpp AuditLog.cpp Item.cpp LinkedList.cpp Queue.cpp Stack.cpp PriorityQueue.cpp Tree.cpp HashTable.cpp Graph.cpp Sorting.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = InventorySystem

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET) --test

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all test clean
