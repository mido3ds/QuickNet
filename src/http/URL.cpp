#include "URL.h"

using std::string;
using std::exception;
using std::smatch;
using std::cmatch;
using std::regex;
using std::regex_match;

using namespace http;

const regex URL::urlRegex(
    "(?:(\\w*)?(?::\\/\\/)([^/:]*)?(?::(\\d*))?(\\/[^;?]*)?(;[^?]*)?(\\?\\S*)?)|\\/", 
    std::regex_constants::optimize
);

const regex queryOrParmsRegExpr(
    "(?:([^=]*)(?:=(\\S*))?)", 
    std::regex_constants::optimize
);

URL::URL(
        const std::string& scheme, const std::string& host, 
        const std::string& port, const std::string& path, 
        const Parametres& parms, const Query& query
    )
    :scheme(scheme), host(host), port(port), path(path), parms(parms), query(query)
{}

string URL::ConstructString(URIType toType) const
{
    string encoded;

    if (toType == ABS_URI)
        encoded = scheme + "://" + host;
    
    encoded += path;
    if (parms.size() != 0)
        encoded += ParmsToString(parms);
    if (query.size() != 0)
        encoded += QueryToString(query);

    return encoded;
}

URL URL::Parse(const string& toDecode)
{
    smatch results;

    if (!TryMatching(toDecode, results))
        throw exception(); // TODO

    string scheme = results[SCHEME_INDEX],
        host = results[HOST_INDEX],
        port = results[PORT_INDEX],
        path = results[PATH_INDEX];

    Parametres parms = ParseParms(results[PARMS_INDEX]);
    Query query = ParseQuery(results[QUERY_INDEX]);

    if (scheme == "") scheme = "http";
    if (port == "") port = "80";
    if (path == "") path = "/";

    return URL(scheme, host, port, path, parms, query);
}

inline void URL::Encode(string& toEnc)
{
    // TODO
    /*pseudocode

    */
}

inline void URL::Decode(string& toDec)
{
    // TODO
    /*pseudocode

    */
}

bool URL::IsValid(const string& url)
{
    return regex_match(url, urlRegex);
}

inline string URL::QueryToString(const Query& query)
{
    if (query.size() == 0) return "";

    string toReturn;
    for (auto& query: query)
        toReturn += "&" + query.first + "=" + query.second;
    
    toReturn[0] = '?';

    return toReturn;
}

inline string URL::ParmsToString(const Parametres& parms)
{
    if (parms.size() == 0) return "";

    string toReturn;
    for (auto& query: parms)
        toReturn += ";" + query.first + "=" + query.second;

    return toReturn;
}

inline URL::Query URL::ParseQuery(const string& toParse)
{
    if (toParse == "") return {};
    assert(toParse[0] == '?');

    Query decoded;
    cmatch results;

    // copy from position 1 .. to avoid ? at beginning
    char* cstr = new char[toParse.size()];
    strcpy(cstr, toParse.c_str() + 1);

    for (cstr = strtok (cstr, "&"); cstr != nullptr; cstr = strtok (cstr, "&"))
    {
        if (!regex_match(cstr, results, queryOrParmsRegExpr))
            throw exception(); // TODO

        for (int i = 0; i < results.size(); i += 2)
            decoded[results[i]] = decoded[results[i+1]];
    }

    return decoded;
}

inline URL::Parametres URL::ParseParms(const string& toParse)
{
    if (toParse == "") return {};
    assert(toParse[0] == ';');

    Parametres decoded;
    cmatch results;

    // copy from position 1 .. to avoid ; at beginning
    char* cstr = new char[toParse.size()];
    strcpy(cstr, toParse.c_str() + 1);

    for (cstr = strtok (cstr, "&"); cstr != nullptr; cstr = strtok (cstr, "&"))
    {
        if (!regex_match(cstr, results, queryOrParmsRegExpr))
            throw exception(); // TODO

        for (int i = 0; i < results.size(); i += 2)
            decoded[results[i]] = decoded[results[i+1]];
    }

    return decoded;
}

inline bool URL::TryMatching(const string& toMatch, smatch& results)
{
    return regex_match(toMatch, results, urlRegex);
}