#ifndef __URI_h__
#define __URI_h__

#include <string>
#include <regex>
#include <unordered_map>
#include <exception>

namespace http
{

enum URIType {ASTERISK, ABS_PATH, ABS_URI};

class URL
{
    using Parametres = std::unordered_map<std::string, std::string>;

public:
    URL(const std::string& path, const Parametres& parms);

    std::string Encode() const; // TODO: make options for full/short url encoding
    static URL Decode(const std::string&);
    static void Escape(std::string&);
    static bool IsValid(const std::string&);

    const std::string path;
    const Parametres parms;
};

}


#endif  /* __URI_h__ */