#ifndef __URI_h__
#define __URI_h__

#include <string>
#include <regex>
#include <unordered_map>
#include <exception>

namespace http
{

// info: https://www.tutorialspoint.com/http/http_requests.htm
enum URIType {ASTERISK, ABS_PATH, ABS_URI};

class URL
{
    using Parametres = std::unordered_map<std::string, std::string>;
    using Query = Parametres;

public:
    std::string Encode(URIType toType=ABS_PATH) const;

    static URL Decode(const std::string&);
    static inline void Escape(std::string&);
    static inline bool IsValid(const std::string&);

    const std::string scheme, host, port, path;
    const Parametres parms;
    const Query query;

private:
    const URIType type;
    
    static inline std::string EncodeQuery(const Query&);
    static inline std::string EncodeParms(const Parametres&);
    static inline Query DecodeQuery(const std::string&);
    static inline Parametres DecodeParms(const std::string&);
};

}


#endif  /* __URI_h__ */