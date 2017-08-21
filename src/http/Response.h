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
    Response(const string& statusCode, const string& message, const Fields& fields);
    Response(const string& statusCode, const Fields& fields);
    static Response Parse(const std::string& toParse);

    std::string ConstructString() const;
    static bool IsValid(const std::string& toParse);

    std::string version,
                statusCode,                   
                reasonPhrase,
                message;
    Fields fields;
private:
    static const std::regex bodyRegEx;
    enum RegExIndices {ALL, VER_INDEX, STATUS_INDEX, REASON_INDEX, FIELD_INDEX, MSG_INDEX};

    static inline bool TryMatching(const std::string& toParse, std::smatch& results, std::string& failureReason);
};

}

#endif  /* __http_Response_h__ */