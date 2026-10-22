
#include "../include/OsvClient.h"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <regex>
#include <set>
#include <sstream>
#include <string>

bool OsvClient::isSafeToken(
    const std::string& value,
    bool allowSlash
) {
    if (value.empty()) {
        return false;
    }

    for (unsigned char ch : value) {
        bool allowed =
            std::isalnum(ch) ||
            ch == '.' ||
            ch == '_' ||
            ch == '-' ||
            ch == '+';

        if (allowSlash && (ch == '/' || ch == '@')) {
            allowed = true;
        }

        if (!allowed) {
            return false;
        }
    }

    return true;
}

OsvResult OsvClient::query(
    const std::string& ecosystem,
    const std::string& packageName,
    const std::string& version
) {
    OsvResult result;

    // Validate inputs before placing them in the request.
    if (!isSafeToken(ecosystem, false) ||
        !isSafeToken(packageName, true) ||
        !isSafeToken(version, false)) {
        result.error = "Invalid package information.";
        return result;
    }

    // Create the JSON request file.
    std::ofstream requestFile(
        "data/osv_request.json",
        std::ios::binary
    );

    if (!requestFile) {
        result.error = "Cannot create the OSV request file.";
        return result;
    }

    requestFile
        << "{\"package\":{\"ecosystem\":\""
        << ecosystem
        << "\",\"name\":\""
        << packageName
        << "\"},\"version\":\""
        << version
        << "\"}";

    requestFile.close();

    if (!requestFile) {
        result.error = "Failed to write the OSV request.";
        return result;
    }

    // curl.exe performs the HTTPS request.
    // All command arguments and file paths are fixed.
    const char* command =
        "curl.exe --fail --silent --show-error "
        "--connect-timeout 10 --max-time 30 "
        "-H \"Content-Type: application/json\" "
        "--data-binary \"@data\\osv_request.json\" "
        "\"https://api.osv.dev/v1/query\" "
        "-o \"data\\osv_response.json\"";

    int exitCode = std::system(command);

    if (exitCode != 0) {
        result.error =
            "OSV request failed. Check internet access, "
            "curl.exe, and the API response.";
        return result;
    }

    // Read the server response.
    std::ifstream responseFile(
        "data/osv_response.json",
        std::ios::binary
    );

    if (!responseFile) {
        result.error = "Cannot read the OSV response file.";
        return result;
    }

    std::ostringstream responseStream;
    responseStream << responseFile.rdbuf();
    std::string response = responseStream.str();

    // Basic validation that a JSON object was returned.
    std::size_t first = response.find_first_not_of(" \t\r\n");
    std::size_t last = response.find_last_not_of(" \t\r\n");

    if (first == std::string::npos ||
        response[first] != '{' ||
        response[last] != '}') {
        result.error = "OSV returned an invalid response.";
        return result;
    }

    // Find the top-level "vulns" array.
    std::size_t keyPosition = response.find("\"vulns\"");

    if (keyPosition == std::string::npos) {
        // A successful OSV response with no vulns array
        // means no matching vulnerability records were returned.
        result.success = true;
        return result;
    }

    std::size_t colonPosition = response.find(':', keyPosition);
    if (colonPosition == std::string::npos) {
        result.error = "Malformed OSV response.";
        return result;
    }

    std::size_t arrayStart = response.find('[', colonPosition);
    if (arrayStart == std::string::npos) {
        result.error = "Malformed OSV vulnerability list.";
        return result;
    }

    // Locate the end of the array while respecting quoted strings.
    int depth = 0;
    bool inString = false;
    bool escaped = false;
    std::size_t arrayEnd = std::string::npos;

    for (std::size_t i = arrayStart; i < response.size(); ++i) {
        char ch = response[i];

        if (inString) {
            if (escaped) {
                escaped = false;
            } else if (ch == '\\') {
                escaped = true;
            } else if (ch == '"') {
                inString = false;
            }
            continue;
        }

        if (ch == '"') {
            inString = true;
        } else if (ch == '[') {
            ++depth;
        } else if (ch == ']') {
            --depth;

            if (depth == 0) {
                arrayEnd = i;
                break;
            }
        }
    }

    if (arrayEnd == std::string::npos) {
        result.error = "Incomplete OSV vulnerability list.";
        return result;
    }

    std::string vulnsArray =
        response.substr(arrayStart, arrayEnd - arrayStart + 1);

    // Extract advisory IDs from the vulnerability objects.
    // This lightweight parser is intended for the OSV response
    // format; it is not a general-purpose JSON parser.
const std::regex idPattern(
    R"regex("id"\s*:\s*"([^"]+)")regex"
);
    std::set<std::string> uniqueIds;

    for (std::sregex_iterator it(
             vulnsArray.begin(),
             vulnsArray.end(),
             idPattern),
         end;
         it != end;
         ++it) {
        uniqueIds.insert((*it)[1].str());
    }

    result.vulnerabilityIds.assign(
        uniqueIds.begin(),
        uniqueIds.end()
    );

    result.success = true;
    return result;
}
