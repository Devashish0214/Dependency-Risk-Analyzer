
#include <iostream>
#include <cassert>
#include <stdexcept>

#include "../include/MaxHeap.h"
#include "../include/DependencyNode.h"

using namespace std;

// Create a package with a specified risk score.
DependencyNode createPackage(string name, double risk) {
    DependencyNode node(name, "1.0.0");
    node.setRiskScore(risk);
    node.setTrustScore(100 - risk);
    return node;
}

int main() {
    cout << "Testing Max Heap...\n";

    MaxHeap heap;

    // Empty heap checks.
    assert(heap.empty());
    assert(heap.size() == 0);

    // Insert packages with different risk scores.
    heap.insert(createPackage("React", 30));
    heap.insert(createPackage("Axios", 80));
    heap.insert(createPackage("Express", 50));

    assert(!heap.empty());
    assert(heap.size() == 3);

    // Highest risk should be at the top.
    assert(heap.top().getPackageName() == "Axios");
    assert(heap.top().getRiskScore() == 80);

    // Extract maximum element.
    DependencyNode first = heap.extractMax();
    assert(first.getPackageName() == "Axios");
    assert(first.getRiskScore() == 80);

    assert(heap.size() == 2);

    // Next maximum should be Express.
    DependencyNode second = heap.extractMax();
    assert(second.getPackageName() == "Express");
    assert(second.getRiskScore() == 50);

    // Last element should be React.
    DependencyNode third = heap.extractMax();
    assert(third.getPackageName() == "React");
    assert(third.getRiskScore() == 30);

    assert(heap.empty());
    assert(heap.size() == 0);

    // Extracting from an empty heap should throw an exception.
    bool exceptionCaught = false;

    try {
        heap.extractMax();
    } catch (const runtime_error&) {
        exceptionCaught = true;
    }

    assert(exceptionCaught);

    // Reading top from an empty heap should also throw.
    exceptionCaught = false;

    try {
        heap.top();
    } catch (const runtime_error&) {
        exceptionCaught = true;
    }

    assert(exceptionCaught);

    cout << "All Max Heap tests passed!\n";

    return 0;
}
