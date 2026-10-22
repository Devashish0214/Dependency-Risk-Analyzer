
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "include/DependencyNode.h"
#include "include/Graph.h"
#include "include/HashTable.h"
#include "include/MaxHeap.h"
#include "include/OsvClient.h"
#include "include/Queue.h"
#include "include/RiskEngine.h"
#include "include/Searching.h"
#include "include/Sorting.h"
#include "include/Stack.h"

using namespace std;

using AdjacencyMap = unordered_map<string, vector<string>>;

string normalizeName(string name) {
    transform(name.begin(), name.end(), name.begin(),
        [](unsigned char c) {
            return static_cast<char>(tolower(c));
        });
    return name;
}

void loadDependencies(
    Graph& graph,
    AdjacencyMap& adjacency,
    const string& filename
) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "ERROR: Cannot open " << filename << '\n';
        return;
    }

    string rawLine;
    int lineNumber = 0;
    int validLines = 0;
    int invalidLines = 0;

    while (getline(file, rawLine)) {
        ++lineNumber;

        if (rawLine.find_first_not_of(" \t\r\n") == string::npos) {
            continue;
        }

        istringstream line(rawLine);
        string parent, dependency, extra;

        if (!(line >> parent >> dependency) || (line >> extra)) {
            cerr << "Warning: Invalid dependency line "
                 << lineNumber << ".\n";
            ++invalidLines;
            continue;
        }

        if (normalizeName(parent) == normalizeName(dependency)) {
            cerr << "Warning: Self-dependency on line "
                 << lineNumber << ".\n";
            ++invalidLines;
            continue;
        }

        graph.addPackage(parent);
        graph.addPackage(dependency);
        graph.addDependency(parent, dependency);

        string normalizedParent = normalizeName(parent);
        string normalizedDependency = normalizeName(dependency);

        vector<string>& children = adjacency[normalizedParent];

        if (find(children.begin(), children.end(),
                 normalizedDependency) == children.end()) {
            children.push_back(normalizedDependency);
        }

        ++validLines;
    }

    cout << "\nDependency file: " << filename << '\n'
         << "Valid lines: " << validLines << '\n'
         << "Invalid lines: " << invalidLines << '\n';
}

unordered_map<string, int> calculateDepths(
    const AdjacencyMap& adjacency,
    const string& start
) {
    unordered_map<string, int> depths;
    queue<string> pending;

    string root = normalizeName(start);
    depths[root] = 0;
    pending.push(root);

    while (!pending.empty()) {
        string current = pending.front();
        pending.pop();

        auto it = adjacency.find(current);
        if (it == adjacency.end()) {
            continue;
        }

        for (const string& child : it->second) {
            if (depths.find(child) == depths.end()) {
                depths[child] = depths[current] + 1;
                pending.push(child);
            }
        }
    }

    return depths;
}

void displayRiskReport(const vector<DependencyNode>& packages) {
    cout << "\n========== RISK REPORT ==========\n";

    cout << left
         << setw(16) << "Package"
         << setw(14) << "Version"
         << setw(20) << "Vulnerabilities"
         << setw(12) << "Risk"
         << setw(12) << "Trust" << '\n';

    cout << string(74, '-') << '\n';

    for (const DependencyNode& package : packages) {
        cout << left
             << setw(16) << package.getPackageName()
             << setw(14) << package.getVersion()
             << setw(20) << package.getVulnerabilityCount()
             << setw(12) << fixed << setprecision(1)
             << package.getRiskScore()
             << setw(12) << package.getTrustScore()
             << '\n';
    }

    cout << string(74, '-') << '\n';

    cout << "\nRisk formula: min(100, vulnerabilities * 25 + dependency depth * 5)\n";
    cout << "Trust formula: 100 - risk score\n";
    cout << "These are demonstration heuristics, not calibrated security ratings.\n";
}

void checkOsvPackages(
    vector<DependencyNode>& packages,
    const unordered_map<string, int>& depths,
    const string& filename
) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "ERROR: Cannot open OSV package list: "
             << filename << '\n';
        return;
    }

    cout << "\n========== OSV VULNERABILITY CHECK ==========\n";

    string ecosystem, packageName, version, extra;
    int checked = 0;
    int failed = 0;

    while (file >> ecosystem >> packageName >> version) {
        ++checked;

        if (file.peek() != '\n' && file.peek() != EOF) {
            // Consume any remaining characters on this line.
            string remainder;
            getline(file, remainder);
        }

        cout << "\nPackage: " << packageName
             << "\nEcosystem: " << ecosystem
             << "\nVersion: " << version << '\n';

        OsvResult result =
            OsvClient::query(ecosystem, packageName, version);

        if (!result.success) {
            ++failed;
            cout << "Query status: FAILED\n"
                 << "Reason: "
                 << (result.error.empty()
                         ? "No error details returned."
                         : result.error)
                 << "\nThis package will not receive a risk score "
                    "from this failed query.\n";
            continue;
        }

        cout << "Query status: SUCCESS\n"
             << "Matching vulnerability records: "
             << result.vulnerabilityIds.size() << '\n';

        for (const string& id : result.vulnerabilityIds) {
            cout << "  - " << id << '\n';
        }

        int depth = 0;
        auto depthIt = depths.find(normalizeName(packageName));

        if (depthIt != depths.end()) {
            depth = depthIt->second;
        }

        DependencyNode node(packageName, version);
        node.setVulnerabilityCount(
            static_cast<int>(result.vulnerabilityIds.size()));
        node.setDependencyDepth(depth);

        RiskEngine::evaluate(node);
        packages.push_back(node);
    }

    cout << "\nOSV packages processed: " << checked
         << "\nFailed queries: " << failed
         << "\nSuccessful queries: " << checked - failed << '\n';

    if (checked == 0) {
        cout << "No package entries were found in "
             << filename << ".\n";
    }
}

void displayHeapRanking(const vector<DependencyNode>& packages) {
    cout << "\n========== MAX-HEAP RISK PRIORITY ==========\n";

    if (packages.empty()) {
        cout << "No scored packages available.\n";
        return;
    }

    MaxHeap heap;

    for (const DependencyNode& package : packages) {
        heap.insert(package);
    }

    heap.display();
}

void testSortingAndSearching(vector<DependencyNode> packages) {
    cout << "\n========== SORTING AND SEARCHING ==========\n";

    if (packages.empty()) {
        cout << "No scored packages available for testing.\n";
        return;
    }

    vector<DependencyNode> byRisk = packages;
    Sorting::mergeSortByRisk(byRisk);

    cout << "\nPackages sorted by risk:\n";
    for (const DependencyNode& package : byRisk) {
        cout << package.getPackageName()
             << " - Risk: " << package.getRiskScore() << '\n';
    }

    vector<DependencyNode> byName = packages;
    Sorting::mergeSortByName(byName);

    cout << "\nPackages sorted by name:\n";
    for (const DependencyNode& package : byName) {
        cout << package.getPackageName() << '\n';
    }

    const string target = packages.front().getPackageName();

    int linearIndex = Searching::linearSearch(packages, target);
    int binaryIndex = Searching::binarySearch(byName, target);

    cout << "\nSearch target: " << target
         << "\nLinear search index: " << linearIndex
         << "\nBinary search index (name-sorted list): "
         << binaryIndex << '\n';
}

void testHashTable(const vector<DependencyNode>& packages) {
    cout << "\n========== HASH TABLE ==========\n";

    HashTable table;

    for (const DependencyNode& package : packages) {
        table.insert(package.getPackageName(), package.getVersion());
    }

    table.display();
    cout << "Stored packages: " << table.size() << '\n';

    if (!packages.empty()) {
        const string name = packages.front().getPackageName();

        cout << "Search for " << name << ": "
             << (table.search(name) ? "FOUND" : "NOT FOUND")
             << "\nVersion: " << table.getVersion(name) << '\n';
    }
}

void testStack(const vector<DependencyNode>& packages) {
    cout << "\n========== STACK (LIFO) ==========\n";

    Stack stack;

    for (const DependencyNode& package : packages) {
        stack.push(package.getPackageName());
    }

    stack.display();

    string item;
    if (stack.peek(item)) {
        cout << "Top item: " << item << '\n';
    }

    if (stack.pop(item)) {
        cout << "Popped item: " << item << '\n';
    }
}

void testQueue(const vector<DependencyNode>& packages) {
    cout << "\n========== QUEUE (FIFO) ==========\n";

    Queue queue(100);

    for (const DependencyNode& package : packages) {
        queue.enqueue(package.getPackageName());
    }

    cout << "Items enqueued: " << queue.size() << '\n';

    if (!queue.isEmpty()) {
        cout << "Front item: " << queue.peek() << '\n';
        cout << "Dequeued item: " << queue.dequeue() << '\n';
    }

    cout << "Items remaining: " << queue.size() << '\n';
}

int main() {
    cout << "============================================\n"
         << " SOFTWARE DEPENDENCY TRUST & RISK ANALYZER\n"
         << " C++ DATA STRUCTURES + OSV API INTEGRATION\n"
         << "============================================\n";

    Graph dependencyGraph;
    AdjacencyMap adjacency;

    loadDependencies(
        dependencyGraph,
        adjacency,
        "data/dependencies.txt"
    );

      dependencyGraph.displayGraph();

    if (dependencyGraph.packageExists("Project")) {
        cout << "\n========== BFS ==========\n";
        dependencyGraph.BFS("Project");

        cout << "\n========== DFS ==========\n";
        dependencyGraph.DFS("Project");
    } else {
        cout << "\nNo 'Project' root package was found; "
                "BFS and DFS were skipped.\n";
    }

    unordered_map<string, int> depths =
        calculateDepths(adjacency, "Project");

    vector<DependencyNode> packages;
    checkOsvPackages(
        packages,
        depths,
        "data/osv_packages.txt"
    );

    RiskEngine::evaluateAll(packages);
    displayRiskReport(packages);
    displayHeapRanking(packages);
    testSortingAndSearching(packages);
    testHashTable(packages);
    testStack(packages);
    testQueue(packages);

    cout << "\n============================================\n"
         << " ANALYSIS FINISHED\n"
         << "============================================\n";

    return 0;
}
