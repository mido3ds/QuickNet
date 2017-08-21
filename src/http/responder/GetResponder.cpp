#include "GetResponder.h"

using namespace http;
using namespace responder;

Response GetResponder::GetResponse(const Request& req, const RoutesMap& map) noexcept
{
    InputPackage in;
    in["method"] = "Get";
    
}