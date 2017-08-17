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

const static string hexValues[] = {
    "%0", "%1",   "%2",  "%3",  "%4",  "%5",  "%6",  "%7",  "%8",  "%9",  "%A",  "%B",  "%C", 
    "%D",  "%E",  "%F",  "%10", "%11", "%12", "%13", "%14", "%15", "%16", "%17", "%18", "%19", 
    "%1A", "%1B", "%1C", "%1D", "%1E", "%1F", "%20", "%21", "%22", "%23", "%24", "%25", "%26", 
    "%27", "%28", "%29", "%2A", "%2B", "%2C", "%2D", "%2E", "%2F", "%30", "%31", "%32", "%33", 
    "%34", "%35", "%36", "%37", "%38", "%39", "%3A", "%3B", "%3C", "%3D", "%3E", "%3F", "%40", 
    "%41", "%42", "%43", "%44", "%45", "%46", "%47", "%48", "%49", "%4A", "%4B", "%4C", "%4D", 
    "%4E", "%4F", "%50", "%51", "%52", "%53", "%54", "%55", "%56", "%57", "%58", "%59", "%5A", 
    "%5B", "%5C", "%5D", "%5E", "%5F", "%60", "%61", "%62", "%63", "%64", "%65", "%66", "%67", 
    "%68", "%69", "%6A", "%6B", "%6C", "%6D", "%6E", "%6F", "%70", "%71", "%72", "%73", "%74", 
    "%75", "%76", "%77", "%78", "%79", "%7A", "%7B", "%7C", "%7D", "%7E", "%7F", "%80", "%81", 
    "%82", "%83", "%84", "%85", "%86", "%87", "%88", "%89", "%8A", "%8B", "%8C", "%8D", "%8E", 
    "%8F", "%90", "%91", "%92", "%93", "%94", "%95", "%96", "%97", "%98", "%99", "%9A", "%9B", 
    "%9C", "%9D", "%9E", "%9F", "%A0", "%A1", "%A2", "%A3", "%A4", "%A5", "%A6", "%A7", "%A8", 
    "%A9", "%AA", "%AB", "%AC", "%AD", "%AE", "%AF", "%B0", "%B1", "%B2", "%B3", "%B4", "%B5", 
    "%B6", "%B7", "%B8", "%B9", "%BA", "%BB", "%BC", "%BD", "%BE", "%BF", "%C0", "%C1", "%C2", 
    "%C3", "%C4", "%C5", "%C6", "%C7", "%C8", "%C9", "%CA", "%CB", "%CC", "%CD", "%CE", "%CF", 
    "%D0", "%D1", "%D2", "%D3", "%D4", "%D5", "%D6", "%D7", "%D8", "%D9", "%DA", "%DB", "%DC", 
    "%DD", "%DE", "%DF", "%E0", "%E1", "%E2", "%E3", "%E4", "%E5", "%E6", "%E7", "%E8", "%E9", 
    "%EA", "%EB", "%EC", "%ED", "%EE", "%EF", "%F0", "%F1", "%F2", "%F3", "%F4", "%F5", "%F6", 
    "%F7", "%F8", "%F9", "%FA", "%FB", "%FC", "%FD", "%FE", "%FF"
};

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
        encoded += Encode(ParmsToString(parms));
    if (query.size() != 0)
        encoded += Encode(QueryToString(query));

    return encoded;
}

URL URL::Parse(const string& toDecode)
{
    smatch results;
    Parametres parms;
    Query query;

    if (!TryMatching(toDecode, results))
        throw exception(); // TODO

    string scheme = results[SCHEME_INDEX],
        host = results[HOST_INDEX],
        port = results[PORT_INDEX],
        path = results[PATH_INDEX];

    if (results[PARMS_INDEX] != "")
        parms = ParseParms(Decode(results[PARMS_INDEX]));
    
    if (results[QUERY_INDEX] != "")
        query = ParseQuery(Decode(results[QUERY_INDEX]));

    if (scheme == "") scheme = "http";
    if (port == "") port = "80";
    if (path == "") path = "/";

    return URL(scheme, host, port, path, parms, query);
}

inline string URL::Encode(const string& toEnc)
{
    string out(toEnc);

    for (int i = 0; i < out.size(); i++)
    {
        char& c = out[i];

        if (c == ' ')
            out.replace(i, 1, "+");
        else if (!(c == '*' || c == '-' || c == '.' || c == '_' || isalpha(c) || isdigit(c)))
            out.replace(i, 1, CharToHex(c));
    }

    return out;
}

inline string URL::Decode(const string& toDec)
{
    string out(toDec);

    for (int i = 0; i < out.size(); i++)
    {
        if (out[i] == '%')
        {
            try 
            {
                string hex = out.substr(i, i+3),
                    decodedChar(1, HexToChar(hex));

                out.replace(i, 3, decodedChar);
            }
            catch (const std::out_of_range& oor)
            {
                throw exception(); // TODO
            }
        }
        else if (out[i] == '+')
            out[i] = ' ';
    }

    return out;
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

inline char URL::HexToChar(const string& hex)
{
    // hex -> int -> char
    return static_cast<char>(
        std::stoi(hex, nullptr, 16)
    );
}

inline std::string URL::CharToHex(const char& c)
{
    return hexValues[static_cast<int>(c)];
}
