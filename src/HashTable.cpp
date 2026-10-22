
#include "../include/HashTable.h"

#include <iostream>

void HashTable::insert(
    const std::string& packageName,
    const std::string& version) {
    table[packageName] = version;
}

bool HashTable::search(
    const std::string& packageName) const {
    return table.find(packageName) != table.end();
}

std::string HashTable::getVersion(
    const std::string& packageName) const {
    auto it = table.find(packageName);

    if (it != table.end()) {
        return it->second;
    }

    return "";
}

bool HashTable::remove(
    const std::string& packageName) {
    return table.erase(packageName) > 0;
}

int HashTable::size() const {
    return static_cast<int>(table.size());
}

void HashTable::display() const {
    std::cout << "\n--- Package Hash Table ---\n";

    for (const auto& entry : table) {
        std::cout << entry.first
                  << " | Version: "
                  << entry.second << '\n';
    }

    std::cout << "Total packages: " << table.size()
              << '\n';
}
