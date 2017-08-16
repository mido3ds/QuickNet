#include "URL.h"

using std::string;
using std::stringstream;
using std::exception;
using std::smatch;

using namespace http;

const std::regex URL::urlRegex(
    "(?:(\\w*)?(?::\\/\\/)([^/:]*)?(?::(\\d*))?(\\/[^;?]*)?(?:;([^?]*))?(?:\\?(\\S*))?)|\\*|\\/"
);

URL::URL(
        const std::string& scheme, const std::string& host, 
        const std::string& port, const std::string& path, 
        const Parametres& parms, const Query& query
        URIType type
    )
    :scheme(scheme), host(host), port(port), path(path), parms(parms), query(query), type(type)
{}

inline URL URL::Asterisk()
{
    return URL("", "", "", "", {}, {}, ASTERISK);
}

string URL::Encode(URIType toType) const
{
    string encoded;

    if (toType == ABS_URI)
        encoded = scheme + "://" + host;
    
    // TODO: make sure no duplicates of '/' when appending host and path
    encoded += path;
    if (parms.size() != 0)
        encoded += EncodeParms(parms);
    if (query.size() != 0)
        encoded += EncodeQuery(query);

    return encoded;
}

URL URL::Decode(const string& toDecode)
{
    if (toDecode == "*")
        return URL::Asterisk();
 
    smatch results;

    if (!TryMatch(toDecode, results))
        throw exception(); // TODO

    string scheme = results[SCHEME_INDEX],
        host = results[HOST_INDEX],
        port = results[PORT_INDEX],
        path = results[PATH_INDEX];

    Parametres parms = DecodeParms(results[PARMS_INDEX]);
    Query query = DecodeQuery(results[QUERY_INDEX]);

    URIType type = GetType(toDecode);

    if (scheme == "") scheme = "http";
    if (port == "") port = "80";
    if (path == "") path = "/";

    return URL(scheme, host, port, path, parms, query, type);
}

inline void URL::Escape(string&)
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
    // TODO
    /*pseudocode

    */
}

inline string URL::EncodeParms(const Parametres& toEncode)
{
    // TODO
    /*pseudocode

    */
}

inline Query URL::DecodeQuery(const string& toDecode)
{
    // TODO
    /*pseudocode
        Query decoded;
        static std::regex queryRegExpr = "....";
        smatch results;

        if (!regexpr_match(queryRegExpr, results,toEncode)) 
            throw exception(); // TODO
        
        for (int i = 0; i < smatch.size(); i += 2)
            decoded[results[i]] = decoded[results[i+1]];

        return decoded;
    */
}

inline Parametres URL::DecodeParms(const string& toDecode)
{
    // TODO
    /*pseudocode
        Parametres decoded;
        static std::regex parmsRegExpr = "....";
        smatch results;

        if (!regexpr_match(regex, results,toEncode)) 
            throw exception(); // TODO
        
        for (int i = 0; i < smatch.size(); i += 2)
            decoded[results[i]] = decoded[results[i+1]];

        return decoded;
    */
}

inline bool URL::TryMatch(const std::string& toMatch, std::smatch& results)
{
    return regex_match(toMatch, results, urlRegex);
}