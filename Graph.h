#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class WarehouseLocation {
public:
    string name;
    string zone;
    string description;

    WarehouseLocation();
    WarehouseLocation(string n, string z, string d);
};

class Graph {
private:
    static const int MAX_VERTICES = 20;
    WarehouseLocation vertices[MAX_VERTICES];
    int adjMatrix[MAX_VERTICES][MAX_VERTICES];
    int vertexCount;

    int getVertexIndex(string name);
    void dfsUtil(int u, bool visited[]);

public:
    Graph();

    int addVertex(string name, string zone = "Storage", string desc = "");
    void addEdge(string from, string to, int weight);

    void bfs(string startName);
    void dfs(string startName);
    void findShortestPath(string startName, string endName);

    void displayGraph();
    int getVertexCount();

    void clear();
    void loadFromFile(string filePath);
    void saveToFile(string filePath);
};

#endif
