#ifndef __http_StatusCode_h__
#define __http_StatusCode_h__

#include <string>
#include <exception>
#include <unordered_map>

namespace http
{

class StatusCode 
{
public:
    static std::string GetDescription(std::string code);
    static std::string GetMainClass(std::string code);

private:
    static const std::unordered_map<std::string, std::string> descriptions;
    static const std::unordered_map<char, std::string> mainClasses;
};
}

#endif  /* __http_StatusCode_h__ */