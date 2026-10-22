
#ifndef OSV_CLIENT_H
#define OSV_CLIENT_H

#include <string>
#include <vector>

struct OsvResult {
    bool success = false;
    std::vector<std::string> vulnerabilityIds;
    std::string error;
};

class OsvClient {
public:
    static OsvResult query(
        const std::string& ecosystem,
        const std::string& packageName,
        const std::string& version
    );

private:
    static bool isSafeToken(
        const std::string& value,
        bool allowSlash
    );
};

#endif

