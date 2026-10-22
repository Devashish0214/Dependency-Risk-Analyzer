
#include <iostream>
#include <cassert>
#include <vector>

#include "include/RiskEngine.h"
#include "include/DependencyNode.h"

using namespace std;

int main() {
    cout << "Testing Risk Engine...\n";

    // Normal cases
    assert(RiskEngine::calculateRiskScore(0, 0) == 0);
    assert(RiskEngine::calculateRiskScore(1, 2) == 35);
    assert(RiskEngine::calculateRiskScore(2, 3) == 65);

    // Maximum risk
    assert(RiskEngine::calculateRiskScore(4, 5) == 100);
    assert(RiskEngine::calculateRiskScore(10, 0) == 100);

    // Negative inputs are treated as zero
    assert(RiskEngine::calculateRiskScore(-1, 0) == 0);
    assert(RiskEngine::calculateRiskScore(0, -2) == 0);

    // Trust score boundaries
    assert(RiskEngine::calculateTrustScore(0) == 100);
    assert(RiskEngine::calculateTrustScore(35) == 65);
    assert(RiskEngine::calculateTrustScore(100) == 0);

    // Trust score is clamped for out-of-range inputs
    assert(RiskEngine::calculateTrustScore(-10) == 100);
    assert(RiskEngine::calculateTrustScore(150) == 0);

    // Empty collection
    vector<DependencyNode> packages;
    RiskEngine::evaluateAll(packages);
    assert(packages.empty());

    cout << "All Risk Engine tests passed!\n";
    return 0;
}
