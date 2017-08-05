#ifndef __http_Request_hpp__
#define __http_Request_hpp__

#include <regex>
#include <string>
#include <unordered_map>
#include <exception>

#include <cstring>
#include <cassert>

#include "Methods.hpp"

namespace http
{

using Fields = std::unordered_map<std::string, std::string>;

class Request
{
public:
    Request(
        const Method& method, const std::string& uri,  
        const std::string& version, const std::string& message, 
        const Fields& fields
    );
    static Request Parse(const std::string& toParse);

    static bool IsValid(const std::string& toParse);

    const Method method;
    const std::string uri,
                      version,
                      message;
    const Fields fields;
private:
    static const std::regex bodyRegEx;
    static const std::regex fieldsRegEx;
    enum RegExIndices {ALL, METHOD_INDEX, URI_INDEX, VER_INDEX, FIELD_INDEX, MSG_INDEX};

    static inline Fields ParseFields(const std::string& str);
    static inline bool TryMatch(const std::string& toParse, std::smatch& results, std::string& failureReason);
};

}

#endif  /* __http_Request_hpp__ */