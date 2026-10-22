
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <unordered_map>

class HashTable {
private:
    std::unordered_map<std::string, std::string> table;

public:
    void insert(const std::string& packageName,
                const std::string& version);

    bool search(const std::string& packageName) const;

    std::string getVersion(
        const std::string& packageName) const;

    bool remove(const std::string& packageName);

    int size() const;

    void display() const;
};

#endif
