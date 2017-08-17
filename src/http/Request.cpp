#include "Request.h"

using namespace http;
using std::smatch;
using std::cmatch;
using std::regex_match;
using std::string;
using std::exception;

const std::regex Request::bodyRegEx(
    "(GET|HEAD|POST|PUT|DELETE|CONNECT|OPTIONS|TRACE) +(\\S+) +HTTP\\/(\\d.\\d) *\\r\\n((?:(?:\\S*): +(?:.*) *\\r\\n)*)? *(?:\\n\\r\\n([\\s\\S\\n\\t]*)\\r\\n)?\\r\\n",
    std::regex_constants::optimize
); 

Request::Request(const Method& method, const URL& url, const string& version, const string& message, const Fields& fields)
    :method(method), url(url), version(version), message(message), fields(fields)
{
    if (version.size() == 0) 
        throw exception(); // TODO
}

Request Request::Parse(const string& toParse)
{
    smatch results;
    string failure_description;

    if (!TryMatching(toParse, results, failure_description)) 
        throw exception(); // TODO

    auto &method = results[METHOD_INDEX],
           &url = results[URI_INDEX],
           &ver = results[VER_INDEX],
           &message = results[MSG_INDEX];
    Fields fs = ParseFields(results[FIELD_INDEX]);

    return Request(StringToMethod(method), URL::Parse(url), ver, message, fs);
}

string Request::ConstructString() const
{
    const char *CRLF = "\r\n", 
                *SP = " ";

    string newString = method + SP + url.ConstructString() + SP + "HTTP/" + version + CRLF;
    for (auto& pair:fields)
        newString += pair.first + ": " + pair.second + CRLF;
    if (message != "")
        newString += '\n' + message + CRLF;
    newString += CRLF;

    return newString;
}

bool Request::IsValid(const string& toParse)
{
    smatch temp_results;
    string temp_str;
    return TryMatching(toParse, temp_results, temp_str);
}

inline bool Request::TryMatching(const string& toParse, smatch& matchResults, string& failureReason)
{
    if (!regex_match(toParse, matchResults, bodyRegEx))
    {
        failureReason = "Doesn't match regexpr";
        return false;
    }

    const string& url = matchResults[URI_INDEX];
    if (url.size() == 0)
    {
        failureReason = "No given URL";
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