#ifndef __URI_h__
#define __URI_h__

#include <string>
#include <sstream>
#include <regex>
#include <unordered_map>
#include <exception>
#include <cassert>

namespace http
{

// info: https://www.tutorialspoint.com/http/http_requests.htm
enum URIType {ASTERISK, ABS_PATH, ABS_URI};

class URL
{
    using Parametres = std::unordered_map<std::string, std::string>;
    using Query = Parametres;

public:
    URL(
        const std::string& scheme, const std::string& host, 
        const std::string& port, const std::string& path, 
        const Parametres& parms, const Query& query
    );

    std::string Encode(URIType toType=ABS_PATH) const;
    static URL Decode(const std::string&);
    static inline void Escape(std::string&);
    static inline void RemoveEscape(std::string&);
    static bool IsValid(const std::string&);

    const std::string scheme, host, port, path;
    const Parametres parms;
    const Query query;

private:
    const static std::regex urlRegex;
    const static std::regex queryOrParmsRegExpr;
    enum RegExIndices {SCHEME_INDEX, HOST_INDEX, PORT_INDEX, PATH_INDEX, PARMS_INDEX, QUERY_INDEX};

    static inline std::string EncodeQuery(const Query&);
    static inline std::string EncodeParms(const Parametres&);
    static inline Query DecodeQuery(const std::string&);
    static inline Parametres DecodeParms(const std::string&);
    static inline bool TryMatch(const std::string& toMatch, std::smatch& results);
};

}


#endif  /* __URI_h__ */