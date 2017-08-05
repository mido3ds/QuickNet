#include "Response.h"

#include "Response.h"

using namespace http;
using std::smatch;
using std::cmatch;
using std::regex_match;
using std::string;
using std::exception;

const std::regex Response::bodyRegEx(
    "HTTP\\/(\\d.\\d) +([1-5]\\d\\d) +(.*) *\\r\\n((?:(?:\\S*): +(?:.*) *\\r\\n)*)? *(?:\\n\\r\\n([\\s\\S\\n\\t]*)\\r\\n)?\\r\\n", 
    std::regex_constants::optimize
); 

Response::Response(
        const string& version, const string& statusCode,
        const string& reasonPhrase, const string& message, const Fields& fields
    )
    :version(version), statusCode(statusCode), reasonPhrase(reasonPhrase), message(message), fields(fields)
{
    if (version.size() == 0 || statusCode.size() == 0 || reasonPhrase.size() == 0)
        throw exception(); // TODO
}

Response Response::Parse(const string& toParse)
{
    smatch results;
    string failure_description;

    if (!TryMatch(toParse, results, failure_description)) 
        throw exception(); // TODO

    auto &version = results[VER_INDEX],
        &statusCode = results[STATUS_INDEX],                   
        &reasonPhrase = results[REASON_INDEX],
        &message = results[MSG_INDEX];
    Fields fs = ParseFields(results[FIELD_INDEX]);

    return Response(version, statusCode, reasonPhrase, message, fs);
}

string Response::ConstructString()
{
    const char *CRLF = "\r\n", 
                *SP = " ";

    string newString = "HTTP/" + version + SP + statusCode + reasonPhrase + CRLF;
    for (auto& pair:fields)
        newString += pair.first + ": " + pair.second + CRLF;
    if (message != "")
        newString += '\n' + message + CRLF;
    newString += CRLF;

    return newString;
}

bool Response::IsValid(const string& toParse)
{
    smatch temp_results;
    string temp_str;
    return TryMatch(toParse, temp_results, temp_str);
}

inline bool Response::TryMatch(const string& toParse, smatch& matchResults, string& failureReason)
{
    if (!regex_match(toParse, matchResults, bodyRegEx))
    {
        failureReason = "Doesn't match regexpr";
        return false;
    }

    const string& version = matchResults[VER_INDEX];
    if (version != "1.1" && version != "1.0")
    {
        failureReason = "Not supported version of http";
        return false;
    }

    return true;
}