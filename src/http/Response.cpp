#include "Response.h"

#include "Response.h"

using namespace http;
using std::smatch;
using std::cmatch;
using std::regex_match;
using std::string;
using std::exception;

const std::regex Response::bodyRegEx(
    "(GET|HEAD|POST|PUT|DELETE|CONNECT|OPTIONS|TRACE) +(\\S+) +HTTP\\/(\\d.\\d) *\\r\\n((?:(?:\\S*): +(?:.*) *\\r\\n)*)? *(?:\\n\\r\\n([\\S\\n\\t]*)\\r\\n)?\\r\\n", // TODO
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

    // TODO
    return true;
}