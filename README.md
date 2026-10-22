\# Dependency Risk Analyzer



A Data Structures and Algorithms project that analyzes software dependency relationships and checks package versions for known security vulnerabilities using the OSV (Open Source Vulnerabilities) API.



\## Features



\- \*\*Dependency Graph:\*\* Represents relationships between software packages using a directed graph.

\- \*\*BFS and DFS:\*\* Traverses the dependency graph using Breadth-First Search and Depth-First Search.

\- \*\*Risk Analysis:\*\* Calculates heuristic dependency risk and trust scores.

\- \*\*OSV Integration:\*\* Queries known vulnerability information for specified package versions.

\- \*\*Data Structures:\*\* Demonstrates graphs, queues, stacks, hash tables, heaps, sorting, and searching.

\- \*\*Web Dashboard:\*\* Provides a frontend connected to a Python Flask backend and the C++ analyzer.

\- \*\*Project Deliverables:\*\* Includes architecture documentation, a presentation, and a project report.



\## Technology Stack



\- C++17

\- Python and Flask

\- HTML, CSS, and JavaScript

\- OSV API

\- Git and GitHub



\## Project Architecture



1\. The frontend accepts input and displays analysis results.

2\. The Flask backend validates input and runs the C++ analyzer.

3\. The C++ engine processes dependency relationships using data structures and algorithms.

4\. The OSV client queries vulnerability information for specified package versions.

5\. The results are returned for reporting and visualization.



\## Input Files



\### `data/dependencies.txt`



Contains directed dependency relationships, one per line.



Example:



```text

Project React

Project Express

React Axios

React Lodash

Express Lodash

Axios OpenSSL

```



Each line represents an edge from the first package to the second.



\### `data/osv\_packages.txt`



Contains the ecosystem, package name, and version to query.



Example:



```text

npm lodash 4.17.20

npm axios 0.21.0

npm express 4.17.1

```



The graph input and vulnerability-query input are separate because they represent different information.



\## Limitations



\- The sample dependency graph is manually provided and is not automatically generated from a package lockfile.

\- Risk scores are heuristic estimates, not certified security ratings.

\- No matching OSV record does not guarantee that a package is secure.

\- Results depend on the supplied package versions and available vulnerability information.



\## Project Deliverables



See `DependencyRiskAnalyzer\_Deliverables/` for the architecture diagram, presentation, and project report.



\## Disclaimer



This is an educational project intended to demonstrate data structures, dependency analysis, and vulnerability lookup. It does not guarantee software security.



