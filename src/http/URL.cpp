#include "URL.h"

using std::string;
using std::stringstream;
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

string URL::Encode(URIType toType) const
{
    string encoded;

    if (toType == ABS_URI)
        encoded = scheme + "://" + host;
    
    encoded += path;
    if (parms.size() != 0)
        encoded += EncodeParms(parms);
    if (query.size() != 0)
        encoded += EncodeQuery(query);

    return encoded;
}

URL URL::Decode(const string& toDecode)
{
    smatch results;

    if (!TryMatch(toDecode, results))
        throw exception(); // TODO

    string scheme = results[SCHEME_INDEX],
        host = results[HOST_INDEX],
        port = results[PORT_INDEX],
        path = results[PATH_INDEX];

    Parametres parms = DecodeParms(results[PARMS_INDEX]);
    Query query = DecodeQuery(results[QUERY_INDEX]);

    if (scheme == "") scheme = "http";
    if (port == "") port = "80";
    if (path == "") path = "/";

    return URL(scheme, host, port, path, parms, query);
}

inline void URL::Escape(string& toEsc)
{
    // TODO
    /*pseudocode

    */
}

inline void URL::RemoveEscape(string& toEsc)
{
    // TODO
    /*pseudocode

    */
}

bool URL::IsValid(const string& in)
{
    smatch temp;
    return TryMatch(in, temp);
}

inline string URL::EncodeQuery(const Query& toEncode)
{
    if (toEncode.size() == 0) return "";

    string toReturn;
    for (auto& query: toEncode)
        toReturn += "&" + query.first + "=" + query.second;
    
    toReturn[0] = '?';

    return toReturn;
}

inline string URL::EncodeParms(const Parametres& toEncode)
{
    if (toEncode.size() == 0) return "";

    string toReturn;
    for (auto& query: toEncode)
        toReturn += ";" + query.first + "=" + query.second;

    return toReturn;
}

inline URL::Query URL::DecodeQuery(const string& toDecode)
{
    if (toDecode == "") return {};
    assert(toDecode[0] == '?');

    Query decoded;
    cmatch results;

    // copy from position 1 .. to avoid ? at beginning
    char* cstr = new char[toDecode.size()];
    strcpy(cstr, toDecode.c_str() + 1);

    for (cstr = strtok (cstr, "&"); cstr != nullptr; cstr = strtok (cstr, "&"))
    {
        if (!regex_match(cstr, results, queryOrParmsRegExpr))
            throw exception(); // TODO

        for (int i = 0; i < results.size(); i += 2)
            decoded[results[i]] = decoded[results[i+1]];
    }

    return decoded;
}

inline URL::Parametres URL::DecodeParms(const string& toDecode)
{
    if (toDecode == "") return {};
    assert(toDecode[0] == ';');

    Parametres decoded;
    cmatch results;

    // copy from position 1 .. to avoid ; at beginning
    char* cstr = new char[toDecode.size()];
    strcpy(cstr, toDecode.c_str() + 1);

    for (cstr = strtok (cstr, "&"); cstr != nullptr; cstr = strtok (cstr, "&"))
    {
        if (!regex_match(cstr, results, queryOrParmsRegExpr))
            throw exception(); // TODO

        for (int i = 0; i < results.size(); i += 2)
            decoded[results[i]] = decoded[results[i+1]];
    }

    return decoded;
}

inline bool URL::TryMatch(const string& toMatch, smatch& results)
{
    return regex_match(toMatch, results, urlRegex);
}