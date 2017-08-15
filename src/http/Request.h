#ifndef __http_Request_h__
#define __http_Request_h__

#include <regex>
#include <string>
#include <unordered_map>
#include <exception>
#include <cstring>

#include "Methods.h"
#include "Fields.h"
#include "URI.h"

namespace http
{

class Request
{
public:
    Request(
        const Method& method, const URI& uri
        const std::string& version,
        const std::string& message, const Fields& fields
    );
    static Request Parse(const std::string& toParse);

    std::string ConstructString() const;
    static bool IsValid(const std::string& toParse);

    const Method method;
    const URI uri;
    const std::string version,
                      message;
    const Fields fields;

private:
    static const std::regex bodyRegEx;
    enum RegExIndices {ALL, METHOD_INDEX, URI_INDEX, VER_INDEX, FIELD_INDEX, MSG_INDEX};

    static inline bool TryMatch(const std::string& toParse, std::smatch& results, std::string& failureReason);
};

}

#endif  /* __http_Request_h__ */