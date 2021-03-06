#ifndef __http_Request_h__
#define __http_Request_h__

#include <regex>
#include <string>
#include <unordered_map>
#include <exception>
#include <cstring>

#include "Methods.h"
#include "Fields.h"
#include "URL.h"

namespace http
{

class Request
{
public:
    Request(
        const Method& method, const URL& url,
        const std::string& version,
        const std::string& message, const Fields& fields
    );
    static Request Parse(const std::string& toParse);

    std::string ConstructString() const;
    static bool IsValid(const std::string& toParse);

    Method method;
    URL url;
    std::string version,
                message;
    Fields fields;

private:
    static const std::regex bodyRegEx;
    enum RegExIndices {ALL, METHOD_INDEX, URI_INDEX, VER_INDEX, FIELD_INDEX, MSG_INDEX};

    static inline bool TryMatching(const std::string& toParse, std::smatch& results, std::string& failureReason);
};

}

#endif  /* __http_Request_h__ */