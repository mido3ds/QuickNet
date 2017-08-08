#ifndef __QuickNetApp_h__
#define __QuickNetApp_h__

#include <string>
#include <unordered_map>
#include <functional>
#include <exception>
#include "Socket.h"

struct OptionsPackage
{
    std::string host;
    std::string port;
    bool debug;
};

class QuickNetApp
{
    using InputPackage = std::unordered_map<std::string, std::string>;
    using RouteListener = std::function<std::string(InputPackage)>;
    using RoutesMap = std::unordered_map<std::string, RouteListener>;

public:
    QuickNetApp(std::string host="localhost", std::string port="8080", bool withDebugging=false);
    QuickNetApp(OptionsPackage pack);
    ~QuickNetApp();

    void Run() noexcept;
    void AssignRoute(const std::string& route, const RouteListener& listener);

private:
    bool RouteIsAssigned(const std::string& route);
    inline void ConstructorHelper(OptionsPackage pack);

    RoutesMap routes;
    Socket* socket;
};

#endif  /* __QuickNetApp_h__ */