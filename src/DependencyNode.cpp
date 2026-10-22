#include "../include/DependencyNode.h"
#include <iostream>

using namespace std;

DependencyNode::DependencyNode() {
    packageName = "";
    version = "unknown";

    riskScore = 0;
    trustScore = 100;

    vulnerabilityCount = 0;
    dependencyDepth = 0;
}

DependencyNode::DependencyNode(string name, string ver) {
    packageName = name;
    version = ver;

    riskScore = 0;
    trustScore = 100;

    vulnerabilityCount = 0;
    dependencyDepth = 0;
}

string DependencyNode::getPackageName() const {
    return packageName;
}

string DependencyNode::getVersion() const {
    return version;
}

double DependencyNode::getRiskScore() const {
    return riskScore;
}

double DependencyNode::getTrustScore() const {
    return trustScore;
}

int DependencyNode::getVulnerabilityCount() const {
    return vulnerabilityCount;
}

int DependencyNode::getDependencyDepth() const {
    return dependencyDepth;
}

void DependencyNode::setRiskScore(double score) {
    riskScore = score;
}

void DependencyNode::setTrustScore(double score) {
    trustScore = score;
}

void DependencyNode::setVulnerabilityCount(int count) {
    vulnerabilityCount = count;
}

void DependencyNode::setDependencyDepth(int depth) {
    dependencyDepth = depth;
}

void DependencyNode::display() const {

    cout << "Package Name       : " << packageName << endl;
    cout << "Version            : " << version << endl;
    cout << "Risk Score         : " << riskScore << endl;
    cout << "Trust Score        : " << trustScore << endl;
    cout << "Vulnerabilities    : " << vulnerabilityCount << endl;
    cout << "Dependency Depth   : " << dependencyDepth << endl;
}