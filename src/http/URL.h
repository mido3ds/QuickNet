#ifndef __URI_h__
#define __URI_h__

#include <string>
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

    static URL Parse(const std::string&);
    std::string ConstructString(URIType toType=ABS_PATH) const;
    static bool IsValid(const std::string&);

    static inline void Encode(std::string&);
    static inline void Decode(std::string&);

    const std::string scheme, host, port, path;
    const Parametres parms;
    const Query query;

private:
    const static std::regex urlRegex;
    const static std::regex queryOrParmsRegExpr;
    enum RegExIndices {SCHEME_INDEX, HOST_INDEX, PORT_INDEX, PATH_INDEX, PARMS_INDEX, QUERY_INDEX};

    static inline std::string QueryToString(const Query&);
    static inline std::string ParmsToString(const Parametres&);
    static inline Query ParseQuery(const std::string&);
    static inline Parametres ParseParms(const std::string&);
    static inline bool TryMatching(const std::string& toMatch, std::smatch& results);
};

}


#endif  /* __URI_h__ */