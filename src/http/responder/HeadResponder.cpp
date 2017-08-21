#include "HeadResponder.h"

using namespace http;
using namespace responder;

Response HeadResponder::GetResponse(const Request& req, const RoutesMap& map) noexcept
{
    // not implemented // TODO: migrate to "Method Not Allowed" response
    return Response("501", {}); 
}