#include "URL.h"

using std::string;
using std::exception;

using namespace http;

URL::URL(const string& path, const Parametres& parms)
    :path(path), parms(parms)
{
    if (path.size() == 0)
        throw exception(); // TODO
}

string URL::Encode() const
{
    // TODO
}

URL URL::Decode(const string&)
{
    // TODO
}