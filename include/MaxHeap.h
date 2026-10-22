
#ifndef MAX_HEAP_H
#define MAX_HEAP_H

#include <vector>
#include "../include/DependencyNode.h"

class MaxHeap {
private:
    std::vector<DependencyNode> heap;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    bool empty() const;
    int size() const;

    void insert(const DependencyNode& node);
    DependencyNode top() const;
    DependencyNode extractMax();

    void display() const;
};

#endif
