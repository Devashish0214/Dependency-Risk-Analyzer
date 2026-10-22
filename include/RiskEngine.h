
#ifndef RISK_ENGINE_H
#define RISK_ENGINE_H

#include <vector>
#include "DependencyNode.h"

class RiskEngine {
public:
    // Calculate a demonstration risk score from 0 to 100.
    static double calculateRiskScore(
        int vulnerabilityCount,
        int dependencyDepth
    );

    // Trust is the inverse of the risk score.
    static double calculateTrustScore(double riskScore);

    // Update the scores stored inside a dependency node.
    static void evaluate(DependencyNode& node);

    // Evaluate every package in the supplied collection.
    static void evaluateAll(std::vector<DependencyNode>& nodes);
};

#endif

