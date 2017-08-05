#include "Request.hpp"

using namespace http;
using std::smatch;
using std::cmatch;
using std::regex_match;
using std::string;
using std::exception;

const std::regex Request::bodyRegEx(
    "(GET|HEAD|POST|PUT|DELETE|CONNECT|OPTIONS|TRACE) +(\\S+) +HTTP\\/(\\d.\\d) *\\r\\n((?:(?:\\S*): +(?:.*) *\\r\\n)*)? *(?:\\n\\r\\n([\\S\\n\\t]*)\\r\\n)?\\r\\n",
    std::regex_constants::optimize
); 
const std::regex Request::fieldsRegEx(
    "(\\S*): +(.*)",
    std::regex_constants::optimize
);

Request::Request(const Method& method, const string& uri, const string& version, const string& message, const Fields& fields)
    :method(method), uri(uri), version(version), message(message), fields(fields)
{
    if (uri.size() == 0 || version.size() == 0) 
        throw exception(); // TODO
}

Request Request::Parse(const string& toParse)
{
    smatch results;
    string failure_description;

    if (!TryMatch(toParse, results, failure_description)) 
        throw exception(); // TODO

    auto &method = results[METHOD_INDEX],
           &uri = results[URI_INDEX],
           &ver = results[VER_INDEX],
           &message = results[MSG_INDEX];
    Fields fs = ParseFields(results[FIELD_INDEX]);

    return Request(StringToMethod(method), uri, ver, message, fs);
}

bool Request::IsValid(const string& toParse)
{
    smatch temp_results;
    string temp_str;
    return TryMatch(toParse, temp_results, temp_str);
}

inline bool Request::TryMatch(const string& toParse, smatch& matchResults, string& failureReason)
{
    if (!regex_match(toParse, matchResults, bodyRegEx))
    {
        failureReason = "Doesn't match regexpr";
        return false;
    }

    const string& method = matchResults[1];
    if (!IsValidMethod(method)) 
    {
        failureReason = "Method is not supported";
        return false;
    }

    const string& uri = matchResults[2];
    if (uri.size() == 0)
    {
        failureReason = "No given URI";
        return false;
    }

    const string& version = matchResults[3];
    if (version != "1.1" && version != "1.0")
    {
        failureReason = "Not supported version of http";
        return false;
    }

    return true;
}

inline Fields Request::ParseFields(const string& str)
{
    cmatch results;
    Fields fs;
    char* cstr = new char[str.size() + 1];
    strcpy(cstr, str.c_str());

    for (cstr = strtok (cstr, "\r\n"); cstr != nullptr; cstr = strtok (cstr, "\r\n"))
    {
        if (!regex_match(cstr, results, fieldsRegEx))
            throw exception(); // TODO

        fs[results[1]] = results[2];
    }

    return fs;
}