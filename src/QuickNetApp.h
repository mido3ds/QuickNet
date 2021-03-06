#ifndef __QuickNetApp_h__
#define __QuickNetApp_h__

#include <string>
#include <exception>
#include "Socket.h"
#include "Server.h"

#include <iostream>
#include <cstring>
#include <cerrno>
#include "utility/Date.h"

struct OptionsPackage
{
    std::string host;
    std::string port;
    bool debug;
};

class QuickNetApp
{
public:
    QuickNetApp(std::string host="localhost", std::string port="8080", bool withDebugging=false);
    QuickNetApp(OptionsPackage pack);
    ~QuickNetApp();

    void Run() noexcept;

private:
    inline void ConstructorHelper(OptionsPackage pack);

    Socket* socket;
    OptionsPackage options;
    int numbOfConnections = 20;
};

#endif  /* __QuickNetApp_h__ */