#ifndef __http_Fields_h__
#define __http_Fields_h__

#include <string>
#include <cstring>
#include <regex>
#include <exception>
#include <unordered_map>

namespace http
{

using std::cmatch;
using std::regex_match;
using std::string;
using std::exception;

using Fields = std::unordered_map<std::string, std::string>;

inline Fields ParseFields(const string& str)
{
    cmatch results;
    Fields fs;
    char* cstr = new char[str.size() + 1];
    strcpy(cstr, str.c_str());

    for (cstr = strtok (cstr, "\r\n"); cstr != nullptr; cstr = strtok (cstr, "\r\n"))
    {
        if (!regex_match(cstr, results, fieldsRegEx))
            throw exception(); // TODO

        fs[results[1]] = results[2];
    }

    return fs;
}

}

#endif  /* __http_Fields_h__ */