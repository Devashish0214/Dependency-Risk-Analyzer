
#include "include/OsvClient.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main() {
    std::ifstream input("data/osv_packages.txt");

    if (!input) {
        std::cerr << "Error: Cannot open data/osv_packages.txt\n";
        return 1;
    }

    std::string line;
    int packageCount = 0;
    int successfulQueries = 0;

    std::cout << "====================================\n";
    std::cout << " DependencyRiskAnalyzer - OSV Check\n";
    std::cout << "====================================\n\n";

    while (std::getline(input, line)) {
        std::istringstream row(line);

        std::string ecosystem;
        std::string packageName;
        std::string version;
        std::string extra;

        if (!(row >> ecosystem)) {
            continue;
        }

        if (!(row >> packageName >> version) || (row >> extra)) {
            std::cout << "Skipping invalid input line: "
                      << line << "\n\n";
            continue;
        }

        ++packageCount;

        std::cout << "Package: " << packageName << "\n";
        std::cout << "Ecosystem: " << ecosystem << "\n";
        std::cout << "Version: " << version << "\n";

        OsvResult result =
            OsvClient::query(ecosystem, packageName, version);

        if (!result.success) {
            std::cout << "Query status: FAILED\n";
            std::cout << "Reason: " << result.error << "\n\n";
            continue;
        }

        ++successfulQueries;

        std::cout << "Query status: SUCCESS\n";
        std::cout << "Matching vulnerability records: "
                  << result.vulnerabilityIds.size() << "\n";

        if (result.vulnerabilityIds.empty()) {
            std::cout
                << "No matching records were returned by OSV. "
                << "This does not prove the package is safe.\n";
        } else {
            for (const std::string& id : result.vulnerabilityIds) {
                std::cout << "  - " << id << "\n";
            }
        }

        std::cout << "\n";
    }

    std::cout << "====================================\n";
    std::cout << "Packages processed: " << packageCount << "\n";
    std::cout << "Successful queries: " << successfulQueries << "\n";
    std::cout << "Failed queries: "
              << packageCount - successfulQueries << "\n";
    std::cout << "====================================\n";

    return successfulQueries == packageCount ? 0 : 1;
}
