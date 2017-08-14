#ifndef __Server_h__
#define __Server_h__

#include <string>
#include <unordered_map>
#include <functional>
#include <exception>
#include "Socket.h"

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
    const Socket clientSocket;

    bool RouteIsAssigned(const std::string& route);
};

#endif  /* __Server_h__ */