
#include "../include/Sorting.h"

using namespace std;

void Sorting::mergeSortByRisk(vector<DependencyNode>& packages) {
    if (packages.size() > 1) {
        sortByRisk(packages, 0, static_cast<int>(packages.size()) - 1);
    }
}

void Sorting::mergeSortByName(vector<DependencyNode>& packages) {
    if (packages.size() > 1) {
        sortByName(packages, 0, static_cast<int>(packages.size()) - 1);
    }
}

void Sorting::sortByRisk(
    vector<DependencyNode>& packages, int left, int right
) {
    if (left >= right) return;

    int middle = left + (right - left) / 2;
    sortByRisk(packages, left, middle);
    sortByRisk(packages, middle + 1, right);
    mergeByRisk(packages, left, middle, right);
}

void Sorting::mergeByRisk(
    vector<DependencyNode>& packages, int left, int middle, int right
) {
    vector<DependencyNode> temp;
    int i = left, j = middle + 1;

    while (i <= middle && j <= right) {
        if (packages[i].getRiskScore() >= packages[j].getRiskScore()) {
            temp.push_back(packages[i++]);
        } else {
            temp.push_back(packages[j++]);
        }
    }

    while (i <= middle) temp.push_back(packages[i++]);
    while (j <= right) temp.push_back(packages[j++]);

    for (int k = 0; k < static_cast<int>(temp.size()); ++k) {
        packages[left + k] = temp[k];
    }
}

void Sorting::sortByName(
    vector<DependencyNode>& packages, int left, int right
) {
    if (left >= right) return;

    int middle = left + (right - left) / 2;
    sortByName(packages, left, middle);
    sortByName(packages, middle + 1, right);
    mergeByName(packages, left, middle, right);
}

void Sorting::mergeByName(
    vector<DependencyNode>& packages, int left, int middle, int right
) {
    vector<DependencyNode> temp;
    int i = left, j = middle + 1;

    while (i <= middle && j <= right) {
        if (packages[i].getPackageName() <= packages[j].getPackageName()) {
            temp.push_back(packages[i++]);
        } else {
            temp.push_back(packages[j++]);
        }
    }

    while (i <= middle) temp.push_back(packages[i++]);
    while (j <= right) temp.push_back(packages[j++]);

    for (int k = 0; k < static_cast<int>(temp.size()); ++k) {
        packages[left + k] = temp[k];
    }
}
