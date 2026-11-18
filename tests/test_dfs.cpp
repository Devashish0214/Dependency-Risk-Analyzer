
#include <iostream>
#include <cassert>
#include <sstream>
#include <string>

#include "../include/Graph.h"

using namespace std;

int main() {
    cout << "Testing DFS...\n";

    Graph graph;

    // Construct a directed graph:
    // A -> B, C
    // B -> D
    // C -> D
    graph.addDependency("A", "B");
    graph.addDependency("A", "C");
    graph.addDependency("B", "D");
    graph.addDependency("C", "D");

    // Add a cycle.
    graph.addDependency("D", "A");

    // Capture printed traversal output.
    ostringstream captured;
    streambuf* original = cout.rdbuf(captured.rdbuf());

    graph.DFS("A");

    cout.rdbuf(original);

    string output = captured.str();

    // Expected DFS order: A, B, D, C.
    size_t a = output.find("A", output.find(":"));
    size_t b = output.find("B", a + 1);
    size_t d = output.find("D", b + 1);
    size_t c = output.find("C", d + 1);

    assert(a != string::npos);
    assert(b != string::npos);
    assert(d != string::npos);
    assert(c != string::npos);

    assert(a < b);
    assert(b < d);
    assert(d < c);

    cout << "DFS traversal order and cycle handling passed!\n";
    cout << "All DFS tests passed!\n";

    return 0;
}
