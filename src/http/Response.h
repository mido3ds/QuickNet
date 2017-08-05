#ifndef __http_Response_h__
#define __http_Response_h__

#include <regex>
#include <string>
#include <unordered_map>
#include <exception>
#include <cstring>

#include "Fields.h"

namespace http
{

class Response
{
public:
    Response(
        const string& version, const string& statusCode,
        const string& reasonPhrase, const string& message, const Fields& fields
    );
    static Response Parse(const std::string& toParse);

    std::string ConstructString();
    static bool IsValid(const std::string& toParse);

    const std::string version,
                      statusCode,                   
                      reasonPhrase,
                      message;
    const Fields fields;
private:
    static const std::regex bodyRegEx;
    enum RegExIndices {ALL, VER_INDEX, STATUS_INDEX, REASON_INDEX, FIELD_INDEX, MSG_INDEX};

    static inline bool TryMatch(const std::string& toParse, std::smatch& results, std::string& failureReason);
};

}

#endif  /* __http_Response_h__ */