# Dependency Risk Analyzer

A C++ Data Structures and Algorithms (DSA) project that analyzes software dependencies, checks package versions against the Open Source Vulnerabilities (OSV) database, and presents dependency risk information through a web dashboard.

## 1. Overview

Modern applications use external libraries and packages. These dependencies may contain known security vulnerabilities or introduce risks through their dependency relationships.

The Dependency Risk Analyzer combines graph traversal, hashing, heaps, sorting, searching, and vulnerability information from the OSV API to help users inspect software dependencies.

**Important:** Risk scores are generated using an educational heuristic. They are not official security ratings or a substitute for a professional security audit.

## 2. Features

- **Dependency graph:** Represents relationships between software components.
- **Breadth-First Search (BFS):** Traverses dependencies level by level.
- **Depth-First Search (DFS):** Explores dependency paths using recursive traversal.
- **Dependency tree:** Helps illustrate dependency relationships.
- **Hash table:** Supports efficient lookup and storage of dependency information.
- **Max heap:** Ranks dependencies by calculated risk scores.
- **Sorting and searching:** Organizes and locates dependency records.
- **Stack and queue:** Demonstrates fundamental data structures.
- **OSV API integration:** Queries known vulnerability information for specified package versions.
- **Risk and trust scores:** Calculates illustrative scores using vulnerability counts and dependency depth.
- **Web dashboard:** Provides an interface for uploading inputs and reviewing analysis results.
- **Input validation:** Handles empty and malformed input files where validation applies.
- **Automated tests:** Includes five C++ test programs for graph, BFS, DFS, max heap, and risk engine functionality.

## 3. Technology Stack

| Component | Technology |
|---|---|
| Core analyzer | C++17 |
| Compiler | MSYS2 UCRT64 GCC |
| Data structures | Graph, hash table, max heap, stack, queue |
| Vulnerability database | OSV API |
| Backend | Python and Flask |
| Frontend | HTML, CSS, JavaScript |
| Build system | Makefile / mingw32-make |
| Testing | C++ test programs and Windows batch script |
| Platform | Windows |

## 4. Project Structure

```text
DependencyRiskAnalyzer/
|
|-- backend/
|   |-- app.py
|
|-- data/
|   |-- dependencies.txt
|   |-- osv_packages.txt
|
|-- frontend/
|   |-- index.html
|   |-- style.css
|   |-- app.js
|
|-- include/
|   |-- Graph.h
|   |-- DependencyNode.h
|   |-- HashTable.h
|   |-- MaxHeap.h
|   |-- OsvClient.h
|   |-- Queue.h
|   |-- RiskEngine.h
|   |-- Searching.h
|   |-- Sorting.h
|   |-- Stack.h
|
|-- src/
|   |-- Graph.cpp
|   |-- DependencyNode.cpp
|   |-- HashTable.cpp
|   |-- MaxHeap.cpp
|   |-- OsvClient.cpp
|   |-- Queue.cpp
|   |-- RiskEngine.cpp
|   |-- Searching.cpp
|   |-- Sorting.cpp
|
|-- tests/
|   |-- sample_inputs/
|   |   |-- sample_dependencies.txt
|   |   |-- sample_osv_packages.txt
|   |   |-- sample_empty_dependencies.txt
|   |   |-- test_empty_dependencies.txt
|   |   |-- test_invalid_dependencies.txt
|   |   |-- test_invalid_packages.txt
|   |   |-- test_upload_dependencies.txt
|   |
|   |-- test_graph.cpp
|   |-- test_bfs.cpp
|   |-- test_dfs.cpp
|   |-- test_heap.cpp
|   |-- test_risk.cpp
|
|-- main.cpp
|-- Makefile
|-- run_tests.bat
|-- requirements.txt
|-- README.md
```

Compiled executables and Python virtual environments are excluded from Git. A fresh checkout must build the C++ analyzer before running the dashboard.

## 5. Input Files

The analyzer uses two separate input files because dependency relationships and package versions serve different purposes.

### 5.1 Dependency relationships

File: `data/dependencies.txt`

Each line contains a parent component followed by its dependency.

Example:

```text
Project React
Project Express
React Axios
React Lodash
Express Lodash
Axios OpenSSL
```

This file defines the directed dependency graph used for traversal and dependency analysis.

### 5.2 Package versions for vulnerability checks

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

This file identifies the exact package versions to query against the OSV API.

**Difference:** The dependency file describes which components depend on which other components. The OSV package file identifies the package versions that should be checked for known vulnerabilities.

## 6. Risk Calculation

The current educational heuristic is:

```text
Risk Score = min(100, vulnerabilities * 25 + dependency depth * 5)

Trust Score = 100 - Risk Score
```

The scores are bounded between 0 and 100 by the current implementation.

These values are illustrative. The formula does not account for all factors used in real-world security assessments, such as vulnerability severity, exploitability, reachability, or whether a vulnerable code path is used.

## 7. Requirements

- Windows operating system
- Python installed
- Flask installed using `requirements.txt`
- MSYS2 UCRT64 GCC compiler for rebuilding the analyzer
- `mingw32-make` available in Command Prompt
- `curl.exe` available on PATH
- Internet connectivity for OSV vulnerability queries

**Compiler configuration:** The current Makefile and test runner use:

```text
C:\msys64\ucrt64\bin\g++.exe
```

If GCC is installed at a different location, update the `CXX` variable in the Makefile and the compiler path in `run_tests.bat`.

## 8. Build the C++ Analyzer

Open Command Prompt (CMD) in the project root directory.

Build the analyzer:

```cmd
mingw32-make -B
```

The `-B` option forces recompilation.

To clean the generated analyzer executable:

```cmd
mingw32-make clean
```

## 9. Run Automated Tests

The project includes five C++ test programs:

| Test | Purpose |
|---|---|
| `test_graph.cpp` | Graph operations and package handling |
| `test_bfs.cpp` | Breadth-First Search and cycle handling |
| `test_dfs.cpp` | Recursive Depth-First Search and cycle handling |
| `test_heap.cpp` | Max heap operations |
| `test_risk.cpp` | Risk calculation and score boundaries |

Run all five test programs from the project root:

```cmd
run_tests.bat
```

The batch file compiles and executes the test programs. If all tests pass, it displays:

```text
ALL 5 TEST PROGRAMS PASSED!
```

## 10. Run the Web Dashboard

Open CMD in the project root directory.

### Step 1: Install Python dependencies

If the virtual environment already exists:

```cmd
.venv\Scripts\python.exe -m pip install -r requirements.txt
```

For a fresh checkout without a virtual environment:

```cmd
python -m venv .venv
.venv\Scripts\python.exe -m pip install -r requirements.txt
```

### Step 2: Start the backend

```cmd
.venv\Scripts\python.exe backend\app.py
```

Keep this CMD window open while using the dashboard.

### Step 3: Open the dashboard

Visit:

http://127.0.0.1:5000

Upload the dependency relationship file and package-version file, then run the analysis.

## 11. Sample Inputs

The `tests/sample_inputs/` directory contains sample files for testing:

- `sample_dependencies.txt`: Example dependency relationships.
- `sample_osv_packages.txt`: Example package versions.
- `sample_empty_dependencies.txt`: Empty dependency input.
- `test_empty_dependencies.txt`: Empty-input validation case.
- `test_invalid_dependencies.txt`: Invalid dependency input.
- `test_invalid_packages.txt`: Invalid package-version input.
- `test_upload_dependencies.txt`: Sample upload input.

These files can be used to check normal and invalid input scenarios through the dashboard.

## 12. Limitations

Install a C++17-compatible GCC compiler and ensure `g++` is available from Command Prompt.

From the project root, run:

```cmd
mingw32-make -B
```

Alternatively, compile the analyzer directly using the command documented in the project build instructions.

## Testing
The repository includes C++ test sources and sample input files. The `tests/sample_inputs` directory contains examples for testing empty and malformed inputs.

The web backend can also be tested by uploading valid and invalid files through the dashboard or by sending requests to its analysis endpoint.

## Limitations

- OSV results depend on the package ecosystem, package name, version, database coverage, and API availability.
- A successful query with no matching records does not prove that a package is secure.
- Vulnerability counts are not necessarily equivalent to the number of unique vulnerabilities affecting the entire application.
- Risk and trust scores are heuristic educational indicators, not certified security measurements.
- The current backend and prebuilt executable target Windows.
- The compiler path in the Makefile and test runner may need adjustment on another computer.

## 13. Future Improvements

- Add severity-aware scoring using vulnerability metadata.
- Improve dependency version resolution and transitive dependency discovery.
- Expand automated unit and integration test coverage.
- Add backend upload validation tests.
- Provide exportable reports and historical comparisons.
- Add configurable risk policies and improved error reporting.

## 14. Disclaimer

This project is intended for educational and research purposes. It demonstrates data structures, dependency analysis, and vulnerability lookup using the OSV API. It does not replace professional security scanning or an independent security review.