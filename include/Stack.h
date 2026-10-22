
#ifndef STACK_H
#define STACK_H

#include <string>
#include <vector>

class Stack {
private:
    std::vector<std::string> items;

public:
    void push(const std::string& packageName);

    bool pop(std::string& packageName);

    bool peek(std::string& packageName) const;

    bool isEmpty() const;

    int size() const;

    void display() const;
};

#endif
