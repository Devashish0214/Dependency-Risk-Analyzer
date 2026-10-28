# Dependency Risk Analyzer

A C++ Data Structures and Algorithms (DSA) project that analyzes software dependencies, checks package versions against the Open Source Vulnerabilities (OSV) database, and presents dependency risk information through a web dashboard.

## Overview

Modern applications use external libraries and packages. These dependencies can contain known security vulnerabilities or introduce risks through their dependency relationships.

The Dependency Risk Analyzer combines graph traversal, hashing, heaps, sorting, searching, and vulnerability data from the OSV API to help users inspect their software dependencies.

**Important:** Risk scores are generated using a simple educational heuristic. They are not official security ratings or a substitute for a professional security audit.

## Features

- **Dependency graph:** Represents relationships between software components.
- **Breadth-First Search (BFS):** Traverses dependencies level by level.
- **Depth-First Search (DFS):** Explores dependency paths using recursive traversal.
- **Dependency tree:** Helps illustrate relationships between dependencies.
- **Hash table:** Supports efficient lookup and storage of dependency information.
- **Max heap:** Ranks dependencies by their calculated risk scores.
- **Sorting and searching:** Organizes and locates dependency records.
- **Stack and queue:** Demonstrates additional fundamental data structures.
- **OSV API integration:** Queries known vulnerability information for specified package versions.
- **Risk and trust scores:** Calculates illustrative scores from vulnerability counts and dependency depth.
- **Web dashboard:** Provides an interface for uploading inputs and reviewing analysis results.
- **Input validation:** Rejects malformed or empty input files where validation applies.

## Technology Stack

| Component | Technology |
|---|---|
| Core analyzer | C++17 |
| Data structures | Graph, hash table, max heap, stack, queue |
| Vulnerability information | OSV API |
| Backend | Python and Flask |
| Frontend | HTML, CSS, JavaScript |
| Compiler | GCC / MinGW-compatible compiler |
| Platform | Windows |

## Project Structure

```text
DependencyRiskAnalyzer/
├── backend/
│   └── app.py
├── data/
│   ├── dependencies.txt
│   └── osv_packages.txt
├── frontend/
│   ├── index.html
│   ├── style.css
│   └── app.js
├── include/
│   ├── Graph.h
│   ├── DependencyNode.h
│   ├── HashTable.h
│   ├── MaxHeap.h
│   ├── OsvClient.h
│   ├── Queue.h
│   ├── RiskEngine.h
│   ├── Searching.h
│   ├── Sorting.h
│   └── Stack.h
├── src/
│   ├── Graph.cpp
│   ├── DependencyNode.cpp
│   ├── HashTable.cpp
│   ├── MaxHeap.cpp
│   ├── OsvClient.cpp
│   ├── Queue.cpp
│   ├── RiskEngine.cpp
│   ├── Searching.cpp
│   ├── Sorting.cpp
│   └── Stack.cpp
├── tests/
│   ├── sample_inputs/
│   └── ...
├── main.cpp
├── analyzer.exe
├── requirements.txt
└── README.md
```

The exact files present in a checkout may differ. The compiled executable is ignored by Git in this repository, so a fresh clone may need to compile it before running the dashboard.

## Input Files

### 1. Dependency relationships

File: `data/dependencies.txt`

Each line contains two names: a parent component followed by its dependency.

Example:

```text
Project React
Project Express
React Axios
React Lodash
Express Lodash
Axios OpenSSL
```

This input defines the graph used for dependency traversal.

### 2. Package versions for vulnerability checks

File: `data/osv_packages.txt`

Each line contains three fields:

```text
ecosystem package version
```

Example:

```text
npm lodash 4.17.20
npm axios 0.21.0
npm express 4.17.1
```

The package-version file is separate from the graph file because the graph describes dependency relationships, while the package-version file identifies specific releases to query against OSV.

## Risk Calculation

The current educational heuristic is:

```text
Risk Score = min(100, vulnerabilities * 25 + dependency depth * 5)
Trust Score = 100 - Risk Score
```

The scores are capped between 0 and 100 by the current formula.

These values are illustrative. The formula does not account for all factors used in real-world security assessments, such as exploitability, severity, reachability, or whether a vulnerable code path is used.

## Requirements

- Windows
- Python installed
- Flask, installed using `requirements.txt`
- A C++17-compatible GCC compiler if rebuilding the analyzer
- `curl.exe` available on PATH for the current OSV integration
- Internet connectivity for OSV vulnerability queries

The prebuilt `analyzer.exe` is Windows-specific. Other operating systems may require changes to the backend and API invocation code.

## Run the Web Dashboard

Open Command Prompt (CMD) in the project directory.

### 1. Install Python dependencies

If you already have the project's virtual environment, run:

```cmd
.venv\Scripts\python.exe -m pip install -r requirements.txt
```

For a fresh checkout without a virtual environment, create one first:

```cmd
python -m venv .venv
.venv\Scripts\python.exe -m pip install -r requirements.txt
```

### 2. Start the backend

```cmd
.venv\Scripts\python.exe backend\app.py
```

Keep this Command Prompt window open while using the dashboard.

### 3. Open the dashboard

Visit:

http://127.0.0.1:5000

Upload the dependency relationship file and package-version file, then run the analysis.

## Compile the C++ Analyzer

Install a C++17-compatible GCC compiler and ensure `g++` is available
from Command Prompt.

From the project root, build the analyzer using:

```cmd
mingw32-make -B
## Testing

The repository includes C++ test sources and sample input files. The `tests/sample_inputs` directory contains examples for testing empty and malformed inputs.

The web backend can also be tested by uploading valid and invalid files through the dashboard or by sending requests to its analysis endpoint.

## Limitations

- OSV results depend on the package ecosystem, package name, version, database coverage, and API availability.
- A successful query with no matching records does not prove that a package is secure.
- Vulnerability counts are not necessarily equivalent to the number of unique vulnerabilities affecting the entire application.
- The risk and trust scores are heuristic educational indicators, not certified security measurements.
- The current backend and prebuilt executable target Windows.

## Future Improvements

- Add severity-aware scoring using vulnerability metadata.
- Improve dependency version resolution and transitive dependency discovery.
- Add automated unit and integration tests.
- Provide exportable reports and historical comparisons.
- Add configurable risk policies and improved error reporting.

## Disclaimer

This project is intended for educational and research purposes. It helps demonstrate data structures and dependency analysis; it does not replace professional security scanning or an independent security review.
