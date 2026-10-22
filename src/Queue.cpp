#include "../include/Queue.h"
#include <iostream>

using namespace std;

Queue::Queue(int size) {

    capacity = size;

    data = new string[capacity];

    front = 0;
    rear = -1;
    currentSize = 0;
}

Queue::~Queue() {

    delete[] data;
}

bool Queue::isEmpty() const {

    return currentSize == 0;
}

bool Queue::isFull() const {

    return currentSize == capacity;
}

void Queue::enqueue(string value) {

    if (isFull()) {

        cout << "Queue Overflow!" << endl;

        return;
    }

    rear = (rear + 1) % capacity;

    data[rear] = value;

    currentSize++;
}

string Queue::dequeue() {

    if (isEmpty()) {

        cout << "Queue Underflow!" << endl;

        return "";
    }

    string value = data[front];

    front = (front + 1) % capacity;

    currentSize--;

    return value;
}

string Queue::peek() const {

    if (isEmpty()) {
        return "";
    }

    return data[front];
}

int Queue::size() const {

    return currentSize;
}