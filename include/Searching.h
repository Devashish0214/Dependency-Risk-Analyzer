
#ifndef SEARCHING_H
#define SEARCHING_H

#include <vector>
#include <string>
#include "DependencyNode.h"

class Searching {
public:
    // Returns the index of the package, or -1 if not found.
    static int linearSearch(
        const std::vector<DependencyNode>& packages,
        const std::string& packageName
    );

    // Requires packages to be sorted by package name.
    static int binarySearch(
        const std::vector<DependencyNode>& packages,
        const std::string& packageName
    );
};

#endif
