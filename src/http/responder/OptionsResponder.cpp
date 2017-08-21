#include "OptionsResponder.h"

using namespace http;
using namespace responder;

Response OptionsResponder::GetResponse(const Request& req, const RoutesMap& routes) noexcept
{
    // not implemented // TODO: migrate to "Method Not Allowed" response
    return Response("501", {}); 
}