
#include <iostream>
#include <cassert>
#include <vector>

#include "include/RiskEngine.h"
#include "include/DependencyNode.h"

using namespace std;

int main() {
    cout << "===== RISK ENGINE TESTS =====\n\n";

    // Test 1: Zero vulnerabilities and zero depth
    assert(RiskEngine::calculateRiskScore(0, 0) == 0.0);
    assert(RiskEngine::calculateTrustScore(0.0) == 100.0);
    cout << "[PASS] Zero risk and maximum trust\n";

    // Test 2: One vulnerability, depth two
    assert(RiskEngine::calculateRiskScore(1, 2) == 35.0);
    assert(RiskEngine::calculateTrustScore(35.0) == 65.0);
    cout << "[PASS] Risk 35, trust 65\n";

    // Test 3: Two vulnerabilities, depth three
    assert(RiskEngine::calculateRiskScore(2, 3) == 65.0);
    assert(RiskEngine::calculateTrustScore(65.0) == 35.0);
    cout << "[PASS] Risk 65, trust 35\n";

    // Test 4: Risk score must not exceed 100
    assert(RiskEngine::calculateRiskScore(4, 5) == 100.0);
    assert(RiskEngine::calculateRiskScore(10, 0) == 100.0);
    cout << "[PASS] Risk score capped at 100\n";

    // Test 5: Negative inputs are treated as zero
    assert(RiskEngine::calculateRiskScore(-2, 0) == 0.0);
    assert(RiskEngine::calculateRiskScore(0, -3) == 0.0);
    assert(RiskEngine::calculateRiskScore(-2, -3) == 0.0);
    cout << "[PASS] Negative inputs handled safely\n";

    // Test 6: Trust score is bounded between 0 and 100
    assert(RiskEngine::calculateTrustScore(-10.0) == 100.0);
    assert(RiskEngine::calculateTrustScore(120.0) == 0.0);
    cout << "[PASS] Trust score remains within bounds\n";

    // Test 7: Evaluate an individual package
    DependencyNode package("TestPackage", "1.0");
    package.setVulnerabilityCount(1);
    package.setDependencyDepth(2);

    RiskEngine::evaluate(package);

    assert(package.getRiskScore() == 35.0);
    assert(package.getTrustScore() == 65.0);
    cout << "[PASS] Individual package evaluation\n";

    // Test 8: Evaluate multiple packages
    vector<DependencyNode> packages;

    DependencyNode first("PackageA", "1.0");
    first.setVulnerabilityCount(0);
    first.setDependencyDepth(0);

    DependencyNode second("PackageB", "2.0");
    second.setVulnerabilityCount(2);
    second.setDependencyDepth(3);

    packages.push_back(first);
    packages.push_back(second);

    RiskEngine::evaluateAll(packages);

    assert(packages[0].getRiskScore() == 0.0);
    assert(packages[0].getTrustScore() == 100.0);
    assert(packages[1].getRiskScore() == 65.0);
    assert(packages[1].getTrustScore() == 35.0);

    cout << "[PASS] Multiple package evaluation\n";

    cout << "\nAll 8 risk engine test groups passed!\n";
    return 0;
}
