#include "GetResponder.h"

using namespace http;
using namespace responder;

Response GetResponder::GetResponse(const Request& req, const RoutesMap& routes) noexcept
{
    // try to find it in routes
    if (routes.find(req.url.path) == routes.end())
        return Response("404", {});

    const RouteListener& listener = routes.at(req.url.path);
    InputPackage in = req.url.query;
    string msg;

    // TODO: check reqeust query matches with givenFunctins parametres

    in["method"] = "Get";

    if (req.url.parms.size() != 0)
        for (auto& pair: req.url.parms)
            in.insert(pair);

    try
    {
        msg = listener(in);
    }
    catch (...)
    {
        return Response("400", {}); // send bad request
    }

    return Response("200"/*Ok*/, msg, {}); // TODO: send minimum needed fields
}