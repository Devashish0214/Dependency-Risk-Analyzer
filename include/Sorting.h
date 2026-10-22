
#ifndef SORTING_H
#define SORTING_H

#include <vector>
#include "DependencyNode.h"

class Sorting {
public:
    static void mergeSortByRisk(std::vector<DependencyNode>& packages);
    static void mergeSortByName(std::vector<DependencyNode>& packages);

private:
    static void sortByRisk(
        std::vector<DependencyNode>& packages,
        int left,
        int right
    );

    static void sortByName(
        std::vector<DependencyNode>& packages,
        int left,
        int right
    );

    static void mergeByRisk(
        std::vector<DependencyNode>& packages,
        int left,
        int middle,
        int right
    );

    static void mergeByName(
        std::vector<DependencyNode>& packages,
        int left,
        int middle,
        int right
    );
};

#endif
