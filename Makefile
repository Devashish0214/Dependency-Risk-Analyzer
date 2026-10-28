CXX = C:/msys64/ucrt64/bin/g++.exe
CXXFLAGS = -std=c++17 -Wall -Wextra -I include

TARGET = analyzer.exe

SOURCES = main.cpp \
          src/Graph.cpp \
          src/DependencyNode.cpp \
          src/HashTable.cpp \
          src/MaxHeap.cpp \
          src/OsvClient.cpp \
          src/Queue.cpp \
          src/RiskEngine.cpp \
          src/Searching.cpp \
          src/Sorting.cpp \
          src/Stack.cpp

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	del /Q $(TARGET) 2>NUL
