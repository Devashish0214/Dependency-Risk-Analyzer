#ifndef DEPENDENCY_NODE_H
#define DEPENDENCY_NODE_H

#include <string>

using namespace std;

class DependencyNode {
private:
    string packageName;
    string version;

    double riskScore;
    double trustScore;

    int vulnerabilityCount;
    int dependencyDepth;

public:
    DependencyNode();

    DependencyNode(string name, string ver);

    string getPackageName() const;
    string getVersion() const;

    double getRiskScore() const;
    double getTrustScore() const;

    int getVulnerabilityCount() const;
    int getDependencyDepth() const;

    void setRiskScore(double score);
    void setTrustScore(double score);

    void setVulnerabilityCount(int count);
    void setDependencyDepth(int depth);

    void display() const;
};

#endif