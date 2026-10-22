
#include "../include/Searching.h"

using namespace std;

int Searching::linearSearch(
    const vector<DependencyNode>& packages,
    const string& packageName
) {
    for (int i = 0; i < static_cast<int>(packages.size()); ++i) {
        if (packages[i].getPackageName() == packageName) {
            return i;
        }
    }

    return -1;
}

int Searching::binarySearch(
    const vector<DependencyNode>& packages,
    const string& packageName
) {
    int left = 0;
    int right = static_cast<int>(packages.size()) - 1;

    while (left <= right) {
        int middle = left + (right - left) / 2;
        const string& current = packages[middle].getPackageName();

        if (current == packageName) {
            return middle;
        }

        if (current < packageName) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
    }

    return -1;
}
