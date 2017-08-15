#ifndef __URI_h__
#define __URI_h__

#include <string>
#include <regex>
#include <unordered_map>
#include <exception>

namespace http
{

class URL
{
    using Parametres = std::unordered_map<std::string, std::string>;

public:
    URL(const std::string& path, const Parametres& parms);

    std::string Encode() const;
    static URL Decode(const std::string&);

    const std::string path;
    const Parametres parms;
private:
};

}


#endif  /* __URI_h__ */