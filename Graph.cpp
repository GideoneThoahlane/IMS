#include "Graph.h"

using namespace std;

WarehouseLocation::WarehouseLocation() {
    name = "";
    zone = "Storage";
    description = "";
}

WarehouseLocation::WarehouseLocation(string n, string z, string d) {
    name = n;
    zone = z;
    description = d;
}

Graph::Graph() {
    clear();
}

void Graph::clear() {
    vertexCount = 0;
    for (int i = 0; i < MAX_VERTICES; ++i) {
        for (int j = 0; j < MAX_VERTICES; ++j) {
            adjMatrix[i][j] = 0;
        }
    }
}

int Graph::getVertexIndex(string name) {
    for (int i = 0; i < vertexCount; ++i) {
        if (vertices[i].name == name) {
            return i;
        }
    }
    return -1;
}

int Graph::addVertex(string name, string zone, string desc) {
    int idx = getVertexIndex(name);
    if (idx != -1) return idx;

    if (vertexCount < MAX_VERTICES) {
        vertices[vertexCount] = WarehouseLocation(name, zone, desc);
        vertexCount++;
        return vertexCount - 1;
    }
    return -1;
}

void Graph::addEdge(string from, string to, int weight) {
    int u = addVertex(from);
    int v = addVertex(to);
    if (u != -1 && v != -1) {
        adjMatrix[u][v] = weight;
        adjMatrix[v][u] = weight;
    }
}

void Graph::bfs(string startName) {
    int startIdx = getVertexIndex(startName);
    if (startIdx == -1) {
        cout << "[GRAPH] Location '" << startName << "' not found.\n";
        return;
    }

    bool visited[MAX_VERTICES] = { false };
    int q[MAX_VERTICES];
    int front = 0;
    int rear = 0;

    visited[startIdx] = true;
    q[rear++] = startIdx;

    cout << "\n[WAREHOUSE BFS] Traversal starting from '" << startName << "':\n";
    bool first = true;
    while (front < rear) {
        int u = q[front++];
        if (!first) cout << " -> ";
        cout << vertices[u].name;
        first = false;

        for (int v = 0; v < vertexCount; ++v) {
            if (adjMatrix[u][v] > 0 && !visited[v]) {
                visited[v] = true;
                q[rear++] = v;
            }
        }
    }
    cout << "\n";
}

void Graph::dfsUtil(int u, bool visited[]) {
    visited[u] = true;
    cout << vertices[u].name;

    for (int v = 0; v < vertexCount; ++v) {
        if (adjMatrix[u][v] > 0 && !visited[v]) {
            cout << " -> ";
            dfsUtil(v, visited);
        }
    }
}

void Graph::dfs(string startName) {
    int startIdx = getVertexIndex(startName);
    if (startIdx == -1) {
        cout << "[GRAPH] Location '" << startName << "' not found.\n";
        return;
    }

    bool visited[MAX_VERTICES] = { false };
    cout << "\n[WAREHOUSE DFS] Traversal starting from '" << startName << "':\n";
    dfsUtil(startIdx, visited);
    cout << "\n";
}

void Graph::findShortestPath(string startName, string endName) {
    int startIdx = getVertexIndex(startName);
    int endIdx = getVertexIndex(endName);

    if (startIdx == -1 || endIdx == -1) {
        cout << "[DIJKSTRA] Locations not found.\n";
        return;
    }

    const int INF = 9999999;
    int dist[MAX_VERTICES];
    int parent[MAX_VERTICES];
    bool visited[MAX_VERTICES];

    for (int i = 0; i < vertexCount; ++i) {
        dist[i] = INF;
        parent[i] = -1;
        visited[i] = false;
    }

    dist[startIdx] = 0;

    for (int count = 0; count < vertexCount; ++count) {
        int u = -1;
        int minD = INF;
        for (int i = 0; i < vertexCount; ++i) {
            if (!visited[i] && dist[i] < minD) {
                minD = dist[i];
                u = i;
            }
        }

        if (u == -1 || u == endIdx) break;
        visited[u] = true;

        for (int v = 0; v < vertexCount; ++v) {
            if (adjMatrix[u][v] > 0 && !visited[v]) {
                if (dist[u] + adjMatrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + adjMatrix[u][v];
                    parent[v] = u;
                }
            }
        }
    }

    if (dist[endIdx] == INF) {
        cout << "[DIJKSTRA] No route found between " << startName << " and " << endName << ".\n";
        return;
    }

    // Reconstruct path
    int path[MAX_VERTICES];
    int pathLen = 0;
    int curr = endIdx;
    while (curr != -1) {
        path[pathLen++] = curr;
        curr = parent[curr];
    }

    cout << "\n[DIJKSTRA SHORTEST PATH] From '" << startName << "' to '" << endName << "':\n";
    cout << "Distance: " << dist[endIdx] << " meters\nRoute: ";
    for (int i = pathLen - 1; i >= 0; --i) {
        cout << vertices[path[i]].name;
        if (i > 0) cout << " -> ";
    }
    cout << "\n";
}

void Graph::displayGraph() {
    cout << "\n---------------------------------------------------------------\n";
    cout << "         WAREHOUSE TOPOLOGY & AISLE CONNECTIVITY GRAPH         \n";
    cout << "---------------------------------------------------------------\n";
    for (int i = 0; i < vertexCount; ++i) {
        cout << "[" << i << "] " << vertices[i].name << " (" << vertices[i].zone << "): ";
        bool hasConn = false;
        for (int j = 0; j < vertexCount; ++j) {
            if (adjMatrix[i][j] > 0) {
                if (hasConn) cout << ", ";
                cout << "-> " << vertices[j].name << " (" << adjMatrix[i][j] << "m)";
                hasConn = true;
            }
        }
        if (!hasConn) cout << "(no paths)";
        cout << "\n";
    }
    cout << "---------------------------------------------------------------\n";
}

int Graph::getVertexCount() {
    return vertexCount;
}

void Graph::loadFromFile(string filePath) {
    clear();
    ifstream inFile(filePath.c_str());
	if (!inFile.is_open()) {
    addVertex("Entrance", "Receiving", "Main warehouse entrance");
    addVertex("Column-A", "Shelves A1-A4", "Laptops and computers");
    addVertex("Column-B", "Shelves B1-B4", "Keyboards and accessories");
    addVertex("Column-C", "Shelves C1-C4", "Routers and networking equipment");
    addVertex("Column-D", "Shelves D1-D4", "Projectors and monitors");
    addVertex("Column-E", "Shelves E1-E4", "Stationery and office supplies");
    addVertex("Column-F", "Shelves F1-F4", "Tools and maintenance equipment");
    addVertex("Dispatch", "Collection", "Items ready for collection");

    addEdge("Entrance", "Column-A", 10);
    addEdge("Entrance", "Column-B", 12);
    addEdge("Entrance", "Dispatch", 8);
    addEdge("Column-A", "Column-B", 6);
    addEdge("Column-A", "Column-C", 10);
    addEdge("Column-B", "Column-C", 5);
    addEdge("Column-B", "Column-D", 8);
    addEdge("Column-C", "Column-D", 6);
    addEdge("Column-C", "Column-E", 9);
    addEdge("Column-D", "Column-F", 10);
    addEdge("Column-E", "Column-F", 7);
    addEdge("Column-F", "Dispatch", 12);

    saveToFile(filePath);
    return;
}

    string line;
    bool inPaths = false;
    while (getline(inFile, line)) {
        if (line.empty() || line[0] == '#') continue;
        if (line == "PATHS:") {
            inPaths = true;
            continue;
        }
        if (line == "LOCATIONS:") {
            inPaths = false;
            continue;
        }

        if (!inPaths) {
            size_t p1 = line.find(',');
            if (p1 != string::npos) {
                size_t p2 = line.find(',', p1 + 1);
                string n = line.substr(0, p1);
                string z = (p2 != string::npos) ? line.substr(p1 + 1, p2 - p1 - 1) : line.substr(p1 + 1);
                string d = (p2 != string::npos) ? line.substr(p2 + 1) : "";
                addVertex(n, z, d);
            }
        } else {
            size_t p1 = line.find(',');
            size_t p2 = (p1 != string::npos) ? line.find(',', p1 + 1) : string::npos;
            if (p1 != string::npos && p2 != string::npos) {
                string from = line.substr(0, p1);
                string to = line.substr(p1 + 1, p2 - p1 - 1);
                string wStr = line.substr(p2 + 1);
                int w = 10;
                try {
                    if (!wStr.empty()) w = stoi(wStr);
                } catch (...) { w = 10; }
                addEdge(from, to, w);
            }
        }
    }
    inFile.close();
}

void Graph::saveToFile(string filePath) {
    ofstream outFile(filePath.c_str(), ios::trunc);
    if (!outFile.is_open()) return;

    outFile << "LOCATIONS:\n";
    for (int i = 0; i < vertexCount; ++i) {
        outFile << vertices[i].name << "," << vertices[i].zone << "," << vertices[i].description << "\n";
    }

    outFile << "PATHS:\n";
    for (int i = 0; i < vertexCount; ++i) {
        for (int j = i + 1; j < vertexCount; ++j) {
            if (adjMatrix[i][j] > 0) {
                outFile << vertices[i].name << "," << vertices[j].name << "," << adjMatrix[i][j] << "\n";
            }
        }
    }
    outFile.close();
}
