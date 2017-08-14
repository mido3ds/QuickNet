#ifndef __Server_h__
#define __Server_h__

#include <string>
#include <unordered_map>
#include <functional>
#include <exception>
#include "Socket.h"
#include "http/Request.h"
#include "http/Response.h"

class Server
{
    using InputPackage = std::unordered_map<std::string, std::string>;
    using RouteListener = std::function<std::string(InputPackage)>;
    using RoutesMap = std::unordered_map<std::string, RouteListener>;

public:
    Server(const Socket& clientSocket);
    ~Server();

    void Serve();
    void AssignRoute(const std::string& route, const RouteListener& listener);

private:
    RoutesMap routes;
    Socket clientSocket;

    bool RouteIsAssigned(const std::string& route);
    http::Request ReceiveRequest();
    http::Response HandleRequest(const http::Request&) const;
    void SendResponse(const http::Response&);
};

#endif  /* __Server_h__ */