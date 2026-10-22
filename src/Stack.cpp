
#include "../include/Stack.h"

#include <iostream>

void Stack::push(const std::string& packageName) {
    items.push_back(packageName);
}

bool Stack::pop(std::string& packageName) {
    if (items.empty()) {
        return false;
    }

    packageName = items.back();
    items.pop_back();

    return true;
}

bool Stack::peek(std::string& packageName) const {
    if (items.empty()) {
        return false;
    }

    packageName = items.back();
    return true;
}

bool Stack::isEmpty() const {
    return items.empty();
}

int Stack::size() const {
    return static_cast<int>(items.size());
}

void Stack::display() const {
    std::cout << "\n--- Stack (top to bottom) ---\n";

    for (auto it = items.rbegin();
         it != items.rend(); ++it) {
        std::cout << *it << '\n';
    }
}
