
#include <iostream>
#include <cassert>
#include <sstream>
#include <string>

#include "../include/Graph.h"

using namespace std;

int main() {
    cout << "Testing BFS...\n";

    Graph graph;

    // Construct a directed graph:
    // A -> B, C
    // B -> D
    // C -> D
    graph.addDependency("A", "B");
    graph.addDependency("A", "C");
    graph.addDependency("B", "D");
    graph.addDependency("C", "D");

    // Add a cycle to verify visited-node handling.
    graph.addDependency("D", "A");

    // Capture printed traversal output.
    ostringstream captured;
    streambuf* original = cout.rdbuf(captured.rdbuf());

    graph.BFS("A");

    cout.rdbuf(original);

    string output = captured.str();

    // Expected BFS order: A, B, C, D.
    size_t a = output.find("A", output.find(":"));
    size_t b = output.find("B", a + 1);
    size_t c = output.find("C", b + 1);
    size_t d = output.find("D", c + 1);

    assert(a != string::npos);
    assert(b != string::npos);
    assert(c != string::npos);
    assert(d != string::npos);

    assert(a < b);
    assert(b < c);
    assert(c < d);

    cout << "BFS traversal order and cycle handling passed!\n";
    cout << "All BFS tests passed!\n";

    return 0;
}
