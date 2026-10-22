#ifndef QUEUE_H
#define QUEUE_H

#include <string>

using namespace std;

class Queue {
private:

    // Dynamic array used to store queue elements
    string* data;

    int capacity;
    int front;
    int rear;
    int currentSize;

public:

    Queue(int size = 100);

    ~Queue();

    bool isEmpty() const;

    bool isFull() const;

    void enqueue(string value);

    string dequeue();

    string peek() const;

    int size() const;
};

#endif