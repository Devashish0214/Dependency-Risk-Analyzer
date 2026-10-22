
#include "../include/Graph.h"
#include <iostream>
#include <queue>
#include <unordered_set>

using namespace std;

// Add a package if it does not already exist.
void Graph::addPackage(string packageName) {
    if (adjacencyList.find(packageName) == adjacencyList.end()) {
        adjacencyList[packageName] = vector<string>();
    }
}

// Add a directed dependency: package -> dependency.
void Graph::addDependency(string packageName, string dependency) {
    addPackage(packageName);
    addPackage(dependency);

    adjacencyList[packageName].push_back(dependency);
}

// Check whether a package exists.
bool Graph::packageExists(string packageName) const {
    return adjacencyList.find(packageName) != adjacencyList.end();
}

// Display all packages and their dependencies.
void Graph::displayGraph() const {
    cout << "\n========== DEPENDENCY GRAPH ==========\n";

    for (const auto& entry : adjacencyList) {
        cout << entry.first << " -> ";

        if (entry.second.empty()) {
            cout << "No dependencies";
        } else {
            for (size_t i = 0; i < entry.second.size(); i++) {
                cout << entry.second[i];

                if (i + 1 < entry.second.size()) {
                    cout << ", ";
                }
            }
        }

        cout << endl;
    }

    cout << "======================================\n";
}

// Breadth-First Search using a queue.
void Graph::BFS(string startPackage) {
    if (!packageExists(startPackage)) {
        cout << "Package not found: " << startPackage << endl;
        return;
    }

    unordered_set<string> visited;
    queue<string> pending;

    visited.insert(startPackage);
    pending.push(startPackage);

    cout << "BFS Traversal: ";

    while (!pending.empty()) {
        string current = pending.front();
        pending.pop();

        cout << current << " ";

        for (const string& neighbor : adjacencyList[current]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                pending.push(neighbor);
            }
        }
    }

    cout << endl;
}

// Depth-First Search using recursion.
void Graph::DFS(string startPackage) {
    if (!packageExists(startPackage)) {
        cout << "Package not found: " << startPackage << endl;
        return;
    }

    unordered_set<string> visited;

    cout << "DFS Traversal: ";
    DFSHelper(startPackage, visited);
    cout << endl;
}

// Recursive helper for DFS.
void Graph::DFSHelper(
    const string& packageName,
    unordered_set<string>& visited
) {
    visited.insert(packageName);
    cout << packageName << " ";

    for (const string& neighbor : adjacencyList[packageName]) {
        if (visited.find(neighbor) == visited.end()) {
            DFSHelper(neighbor, visited);
        }
    }
}
