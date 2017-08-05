#ifndef __http_Methods_h__
#define __http_Methods_h__

#include <string>
#include <exception>

namespace http
{

enum Method
{
    GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE
};

inline Method StringToMethod(const std::string& input)
{
    if (input == "GET") return GET;
    if (input == "HEAD") return HEAD;
    if (input == "POST") return POST;
    if (input == "PUT") return PUT;
    if (input == "DELETE") return DELETE;
    if (input == "CONNECT") return CONNECT;
    if (input == "OPTIONS") return OPTIONS;
    if (input == "TRACE") return TRACE;

    throw std::exception(); // TODO
}

inline std::string MethodToString(Method input)
{
    if (input == GET) return "GET";
    if (input == HEAD) return "HEAD";
    if (input == POST) return "POST";
    if (input == PUT) return "PUT";
    if (input == DELETE) return "DELETE";
    if (input == CONNECT) return "CONNECT";
    if (input == OPTIONS) return "OPTIONS";
    if (input == TRACE) return "TRACE";

    throw std::exception(); // TODO
}

inline bool IsValidMethod(const std::string& input)
{
    return (input == "GET") 
        || (input == "HEAD") 
        || (input == "POST") 
        || (input == "PUT") 
        || (input == "DELETE") 
        || (input == "CONNECT") 
        || (input == "OPTIONS") 
        || (input == "TRACE");
}
}

#endif  /* __http_Methods_h__ */