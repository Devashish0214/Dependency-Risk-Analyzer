
@echo off
setlocal

echo ==========================================
echo   DEPENDENCY RISK ANALYZER - TEST SUITE
echo ==========================================

set "CXX=C:\msys64\ucrt64\bin\g++.exe"

echo.
echo [1/5] Compiling Graph Test...
"%CXX%" -std=c++17 -Wall -Wextra tests\test_graph.cpp src\Graph.cpp -o test_graph.exe
if errorlevel 1 goto failed

echo [2/5] Compiling BFS Test...
"%CXX%" -std=c++17 -Wall -Wextra tests\test_bfs.cpp src\Graph.cpp -o test_bfs.exe
if errorlevel 1 goto failed

echo [3/5] Compiling DFS Test...
"%CXX%" -std=c++17 -Wall -Wextra tests\test_dfs.cpp src\Graph.cpp -o test_dfs.exe
if errorlevel 1 goto failed

echo [4/5] Compiling Max Heap Test...
"%CXX%" -std=c++17 -Wall -Wextra tests\test_heap.cpp src\MaxHeap.cpp src\DependencyNode.cpp -o test_heap.exe
if errorlevel 1 goto failed

echo [5/5] Compiling Risk Engine Test...
"%CXX%" -std=c++17 -Wall -Wextra tests\test_risk.cpp src\RiskEngine.cpp src\DependencyNode.cpp -o test_risk.exe
if errorlevel 1 goto failed

echo.
echo ==========================================
echo   RUNNING ALL TESTS
echo ==========================================

test_graph.exe
if errorlevel 1 goto failed

test_bfs.exe
if errorlevel 1 goto failed

test_dfs.exe
if errorlevel 1 goto failed

test_heap.exe
if errorlevel 1 goto failed

test_risk.exe
if errorlevel 1 goto failed

echo.
echo ==========================================
echo   ALL 5 TEST PROGRAMS PASSED!
echo ==========================================
exit /b 0

:failed
echo.
echo TEST FAILED! Check the error above.
exit /b 1
