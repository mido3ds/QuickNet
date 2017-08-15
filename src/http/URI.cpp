#include "URI.h"

using std::string;
using std::exception;

using namespace http;

URI::URI(const string& path, const Parametres& parms)
    :path(path), parms(parms)
{
    if (path.size() == 0)
        throw exception(); // TODO
}

string URI::Encode()
{
    // TODO
}

static URI URI::Decode(const string&)
{
    // TODO
}