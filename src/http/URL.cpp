#include "URL.h"

using std::string;
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

URL URL::Decode(const string&)
{
    // TODO
}

inline void URL::Escape(string&)
{
    // TODO
}

inline bool URL::IsValid(const string&)
{
    // TODO
}

inline string EncodeQuery(const Query&)
{
    // TODO
}

inline string EncodeParms(const Parametres&)
{
    // TODO
}

inline Query DecodeQuery(const string&)
{
    // TODO
}

inline Parametres DecodeParms(const string&)
{
    // TODO
}
