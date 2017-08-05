#ifndef __http_StatusCode_h__
#define __http_StatusCode_h__

#include <string>
#include <cstring>
#include <regex>
#include <exception>
#include <unordered_map>

namespace http
{

class StatusCode 
{
    static std::string CodeToDescription(std::string code);
    
};

}

#endif  /* __http_StatusCode_h__ */