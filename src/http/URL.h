#ifndef __URI_h__
#define __URI_h__

#include <string>
#include <regex>
#include <unordered_map>
#include <exception>
#include <cassert>

namespace http
{

/** info: https://www.tutorialspoint.com/http/http_requests.htm */
enum URIType {ABS_PATH, ABS_URI};

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

    /** "application/x-www-form-urlencoded" serializing
     *  only apply it to queries/parms not the whole url or it would cause errors
     *  info: https://url.spec.whatwg.org/#urlencoded-serializing
     */
    static inline std::string Encode(const std::string&);
    static inline std::string Decode(const std::string&);

    const std::string scheme, host, port, path;
    const Parametres parms;
    const Query query;

private:
    const static std::regex urlRegex;
    const static std::regex queryOrParmsRegExpr;
    const static std::string hexValues[];
    enum RegExIndices {SCHEME_INDEX, HOST_INDEX, PORT_INDEX, PATH_INDEX, PARMS_INDEX, QUERY_INDEX};

    static inline std::string QueryToString(const Query&);
    static inline std::string ParmsToString(const Parametres&);
    static inline Query ParseQuery(const std::string&);
    static inline Parametres ParseParms(const std::string&);
    static inline bool TryMatching(const std::string& toMatch, std::smatch& results);

    /** hex beginns with % not 0x, for optimisation */
    static inline std::string CharToHex(const char& c);
    /** hex is a string of characters representing hexadecimal number without a prefix (0x or %) */
    static inline char HexToChar(const std::string& hex);
};

}


#endif  /* __URI_h__ */