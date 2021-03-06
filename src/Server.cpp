#include "Server.h"

using std::string;
using std::exception;
using namespace http;
using namespace http::responder;

Server::Server(const Socket& clientSocket)
    :clientSocket(clientSocket)
{}

Server::~Server()
{}

RoutesMap Server::routes;

/////////////////////////////////////////////////////////////////

void Server::Serve()
{
    Request req = ReceiveRequest();
    Response res = HandleRequest(req);
    SendResponse(res);

    // TODO: add loging
}

void Server::AssignRoute(const std::string& route, const RouteListener& listener)
{
    if (RouteIsAssigned(route))
        throw exception(); // TODO

    routes[route] = listener;
}

/////////////////////////////////////////////////////////////////

inline bool Server::RouteIsAssigned(const std::string& route)
{
    return routes.find(route) != routes.end();
}

/////////////////////////////////////////////////////////////////

inline Request Server::ReceiveRequest()
{
    string str, temp;
    const int timeOut = 2;

    while (true)
    {
        temp = clientSocket.Receive(timeOut);

        if (temp == "")
            break;
        else 
            str += temp;

        if (Request::IsValid(str))
            return Request::Parse(str);
    }

    throw exception();  // TODO
}

inline Response Server::HandleRequest(const Request& req) const
{
    IResponder* respondCreator;

    respondCreator = IResponder::MakeResponder(req.method);
    Response res = respondCreator->GetResponse(req, routes);
    
    delete respondCreator;
    return res;
}

inline void Server::SendResponse(const Response& res)
{
    clientSocket.Send(res.ConstructString());
    
    // TODO: handle exceptions
}