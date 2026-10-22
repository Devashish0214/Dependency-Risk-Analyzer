
#include "../include/MaxHeap.h"
#include <iostream>
#include <stdexcept>
#include <utility>

using namespace std;

bool MaxHeap::empty() const {
    return heap.empty();
}

int MaxHeap::size() const {
    return static_cast<int>(heap.size());
}

void MaxHeap::insert(const DependencyNode& node) {
    heap.push_back(node);
    heapifyUp(size() - 1);
}

DependencyNode MaxHeap::top() const {
    if (empty()) {
        throw runtime_error("Cannot read the top of an empty max heap.");
    }

    return heap.front();
}

DependencyNode MaxHeap::extractMax() {
    if (empty()) {
        throw runtime_error("Cannot extract from an empty max heap.");
    }

    DependencyNode result = heap.front();

    if (heap.size() == 1) {
        heap.pop_back();
        return result;
    }

    heap.front() = heap.back();
    heap.pop_back();
    heapifyDown(0);

    return result;
}

void MaxHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;

        if (heap[parent].getRiskScore() >=
            heap[index].getRiskScore()) {
            break;
        }

        swap(heap[parent], heap[index]);
        index = parent;
    }
}

void MaxHeap::heapifyDown(int index) {
    int count = size();

    while (true) {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < count &&
            heap[left].getRiskScore() >
            heap[largest].getRiskScore()) {
            largest = left;
        }

        if (right < count &&
            heap[right].getRiskScore() >
            heap[largest].getRiskScore()) {
            largest = right;
        }

        if (largest == index) {
            break;
        }

        swap(heap[index], heap[largest]);
        index = largest;
    }
}

void MaxHeap::display() const {
    if (empty()) {
        cout << "Max heap is empty.\n";
        return;
    }

    cout << "\n--- Packages in Max Heap (highest risk first) ---\n";

    // Display a sorted order without modifying the original heap.
    MaxHeap copy = *this;

    while (!copy.empty()) {
        DependencyNode node = copy.extractMax();

        cout << node.getPackageName()
             << " | Risk Score: "
             << node.getRiskScore()
             << " | Trust Score: "
             << node.getTrustScore()
             << '\n';
    }
}
