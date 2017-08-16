#include "URL.h"

using std::string;
using std::stringstream;
using std::exception;

using namespace http;

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
    // TODO
    /*pseudocode
        
    */
}

inline void URL::Escape(string&)
{
    // TODO
    /*pseudocode
        
    */
}

bool URL::IsValid(const string& in)
{
    if (in == "*") return true;

    // return regexpr_match(URLRegex, in);
}

inline string EncodeQuery(const Query& toEncode)
{
    // TODO
    /*pseudocode
        
    */
}

inline string EncodeParms(const Parametres& toEncode)
{
    // TODO
    /*pseudocode
        
    */
}

inline Query DecodeQuery(const string& toDecode)
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

inline Parametres DecodeParms(const string& toDecode)
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
