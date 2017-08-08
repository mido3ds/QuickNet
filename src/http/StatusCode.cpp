#include "StatusCode.h"

using namespace http;
using std::unordered_map;
using std::string;
using std::exception;

const unordered_map<string, string> StatusCode::descriptions = {
    {"100", "Continue"},
    {"101", "Switching Protocols"},

    {"200", "Ok"},
    {"201", "Created"},
    {"202", "Accepted"},
    {"203", "Non-Authoritative Information"},
    {"204", "No Content"},
    {"205", "Reset Content"},
    {"206", "Partial Content"},

    {"300", "Multiple Choices"},
    {"301", "Moved Permanently"},
    {"302", "Found"},
    {"303", "See Other"},
    {"304", "Not Modified"},
    {"305", "Use Proxy"},
    // 306 is not used by http protocol
    {"307", "Temporary Redirect"},

    {"400", "Bad Request"},
    {"401", "Unauthorized"},
    {"402", "Payment Required"}, // not used by protocol yet
    {"403", "Forbidden"},
    {"404", "Not Found"},
    {"405", "Method Not Allowed"},
    {"406", "Not Acceptable"},
    {"407", "Proxy Authentication Required"},
    {"408", "Request Timeout"},
    {"409", "Conflict"},
    {"410", "Gone"},
    {"411", "Length Required"},
    {"412", "Precondition Failed"},
    {"413", "Request Entity Too Large"},
    {"414", "Request URI Too Long"},
    {"415", "Unsupported Media Type"},
    {"416", "Requested Range Not Satisfiable"},
    {"417", "Expectation Failed"},

    {"500", "Internal Server Error"},
    {"501", "Not Implemented"},
    {"502", "Bad Gateway"},
    {"503", "Service Unavailable"},
    {"504", "Gateway Timeout"},
    {"505", "HTTP Version Not Supported"},
};

const unordered_map<char, std::string> StatusCode::mainClasses = {
    {'1', "Informational"},
    {'2', "Success"},
    {'3', "Redirection"},
    {'4', "Client Error"},
    {'5', "Server Error"}
};

string StatusCode::GetDescription(string code)
{
    try 
    {
        return descriptions.at(code);
    }
    catch (const std::out_of_range& e)
    {
        throw exception(); // TODO
    }
}

string StatusCode::GetMainClass(string code)
{
    try 
    {
        return descriptions.at(code);
    }
    catch (const std::out_of_range& e)
    {
        throw exception(); // TODO
    }
}