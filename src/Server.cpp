#include "Server.h"

using std::string;
using std::exception;

Server::Server(const Socket& clientSocket)
    :clientSocket(clientSocket)
{}

Server::~Server()
{}

/////////////////////////////////////////////////////////////////

void Server::Serve()
{
    // TODO
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