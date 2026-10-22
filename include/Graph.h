
#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class Graph {
private:
    unordered_map<string, vector<string>> adjacencyList;

    void DFSHelper(
        const string& packageName,
        unordered_set<string>& visited
    );

public:
    void addPackage(string packageName);
    void addDependency(string packageName, string dependency);
    bool packageExists(string packageName) const;
    void displayGraph() const;
    void BFS(string startPackage);
    void DFS(string startPackage);
};

#endif
