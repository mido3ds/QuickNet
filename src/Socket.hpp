#ifndef __Socket_hpp__
#define __Socket_hpp__

#include <string>
#include <exception>

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

enum Protocol
{
    TCP,
    UDP,
    Raw,
    Any
};

enum SocketUse
{
    toConnect,
    toBind,
    toServe
};

using FileDescriptor = int;

class Socket
{
public:
    Socket(std::string host, std::string service, Protocol protocol=TCP, SocketUse use=toConnect);
    Socket(std::string portNumber, Protocol protocol=TCP, SocketUse use=toConnect);
    ~Socket();

    void Listen(int maxNumberOfCon);
    Socket Accept();

    void Send(std::string toSend);
    std::string Receive();

    void Close();
    bool IsConnected();

private:
    FileDescriptor fd = -1;
    bool isConnected = false;
    const Protocol protocol;
    const SocketUse use;

    Socket(FileDescriptor fd, Protocol protocol, SocketUse use);
    addrinfo *GetAddrInfo(std::string host, std::string service) const;
    FileDescriptor GetAndConnectFD(addrinfo* res) const;
};

#endif  /* __Socket_hpp__ */