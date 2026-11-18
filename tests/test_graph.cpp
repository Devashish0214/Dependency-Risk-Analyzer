
#include <iostream>
#include <cassert>

#include "../include/Graph.h"

using namespace std;

int main() {
    cout << "Testing Graph...\n";

    Graph graph;

    // Add packages.
    graph.addPackage("Project");
    graph.addPackage("React");
    graph.addPackage("Express");

    // Add directed dependencies.
    graph.addDependency("Project", "React");
    graph.addDependency("Project", "Express");

    // Check that packages exist.
    assert(graph.packageExists("Project"));
    assert(graph.packageExists("React"));
    assert(graph.packageExists("Express"));

    // Check a package that was never added.
    assert(!graph.packageExists("Unknown"));

    // Adding the same package again should not remove it.
    graph.addPackage("Project");
    assert(graph.packageExists("Project"));

    // Dependencies should also create missing packages.
    graph.addDependency("React", "Axios");
    assert(graph.packageExists("Axios"));

    cout << "All Graph tests passed!\n";
    return 0;
}
