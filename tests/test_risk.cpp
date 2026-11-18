
#include <iostream>
#include <cassert>
#include <vector>

#include "../include/RiskEngine.h"
#include "../include/DependencyNode.h"

using namespace std;

int main() {
    cout << "Testing Risk Engine...\n";

    // Group 1: Normal risk calculations.
    assert(RiskEngine::calculateRiskScore(0, 0) == 0);
    assert(RiskEngine::calculateRiskScore(1, 2) == 35);
    assert(RiskEngine::calculateRiskScore(2, 3) == 65);

    // Group 2: Maximum risk.
    assert(RiskEngine::calculateRiskScore(4, 5) == 100);
    assert(RiskEngine::calculateRiskScore(10, 0) == 100);

    // Group 3: Negative inputs.
    assert(RiskEngine::calculateRiskScore(-1, 0) == 0);
    assert(RiskEngine::calculateRiskScore(0, -2) == 0);

    // Group 4: Trust score boundaries.
    assert(RiskEngine::calculateTrustScore(0) == 100);
    assert(RiskEngine::calculateTrustScore(35) == 65);
    assert(RiskEngine::calculateTrustScore(100) == 0);

    // Group 5: Trust score clamping.
    assert(RiskEngine::calculateTrustScore(-10) == 100);
    assert(RiskEngine::calculateTrustScore(150) == 0);

    // Group 6: Evaluate an individual package.
    DependencyNode package("Axios", "0.21.0");
    package.setVulnerabilityCount(2);
    package.setDependencyDepth(3);

    RiskEngine::evaluate(package);

    assert(package.getRiskScore() == 65);
    assert(package.getTrustScore() == 35);

    // Group 7: Evaluate multiple packages.
    vector<DependencyNode> packages;

    DependencyNode first("React", "18.0.0");
    first.setVulnerabilityCount(1);
    first.setDependencyDepth(2);

    DependencyNode second("Express", "4.17.1");
    second.setVulnerabilityCount(0);
    second.setDependencyDepth(1);

    packages.push_back(first);
    packages.push_back(second);

    RiskEngine::evaluateAll(packages);

    assert(packages.size() == 2);
    assert(packages[0].getRiskScore() == 35);
    assert(packages[0].getTrustScore() == 65);
    assert(packages[1].getRiskScore() == 5);
    assert(packages[1].getTrustScore() == 95);

    // Group 8: Empty collection.
    vector<DependencyNode> emptyPackages;
    RiskEngine::evaluateAll(emptyPackages);
    assert(emptyPackages.empty());

    cout << "All 8 Risk Engine test groups passed!\n";

    return 0;
}
