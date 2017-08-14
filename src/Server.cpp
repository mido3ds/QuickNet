#include "Server.h"

using std::string;
using std::exception;
using namespace http;

Server::Server(const Socket& clientSocket)
    :clientSocket(clientSocket)
{}

Server::~Server()
{}

/////////////////////////////////////////////////////////////////

void Server::Serve()
{
    Request req = ReceiveRequest();
    Response res = HandleRequest(req);
    SendResponse(res);
}

void Server::AssignRoute(const std::string& route, const RouteListener& listener)
{
    if (RouteIsAssigned(route))
        throw exception(); // TODO

    routes[route] = listener;
}

/////////////////////////////////////////////////////////////////

bool Server::RouteIsAssigned(const std::string& route)
{
    return routes.find(route) != routes.end();
}

/////////////////////////////////////////////////////////////////

Request Server::ReceiveRequest()
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

Response Server::HandleRequest(const Request& req) const
{
    // TODO
}

void Server::SendResponse(const Response& res)
{
    clientSocket.Send(res.ConstructString());
    
    // TODO: handle exceptions
}