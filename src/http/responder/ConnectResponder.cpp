#include "ConnectResponder.h"

using namespace http;
using namespace responder;

Response ConnectResponder::GetResponse(const Request& req, const RoutesMap& map) noexcept
{
    // not implemented // TODO: migrate to "Method Not Allowed" response
    return Response("501", {}); 
}