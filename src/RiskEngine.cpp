
#include "../include/RiskEngine.h"

#include <algorithm>
#include <iostream>

using namespace std;

double RiskEngine::calculateRiskScore(
    int vulnerabilityCount,
    int dependencyDepth
) {
    // Illustrative scoring model, not a real vulnerability scan.
    vulnerabilityCount = max(0, vulnerabilityCount);
    dependencyDepth = max(0, dependencyDepth);

    double risk = vulnerabilityCount * 25.0
                + dependencyDepth * 5.0;

    return min(100.0, risk);
}

double RiskEngine::calculateTrustScore(double riskScore) {
    riskScore = max(0.0, min(100.0, riskScore));
    return 100.0 - riskScore;
}

void RiskEngine::evaluate(DependencyNode& node) {
    double risk = calculateRiskScore(
        node.getVulnerabilityCount(),
        node.getDependencyDepth()
    );

    node.setRiskScore(risk);
    node.setTrustScore(calculateTrustScore(risk));
}

void RiskEngine::evaluateAll(vector<DependencyNode>& nodes) {
    for (DependencyNode& node : nodes) {
        evaluate(node);
    }
}
