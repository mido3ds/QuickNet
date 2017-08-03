#ifndef __Socket_hpp__
#define __Socket_hpp__

#include <string>
#include <exception>
#include <cassert>

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

enum SocketType
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
    Socket(std::string host, std::string service, SocketType type=TCP, SocketUse use=toConnect);
    Socket(std::string portNumber, SocketType type=TCP, SocketUse use=toConnect);
    ~Socket();

    void Listen(int maxNumberOfCon);
    Socket Accept();

    void Send(std::string toSend);
    std::string Receive(const int timeOut, const int chunkSize=512);

    void Close();
    bool IsConnected(); const

    const SocketType type;
    const SocketUse use;

private:
    FileDescriptor fd = -1;
    bool isConnected = false;
    sockaddr_storage* peer = nullptr;
    socklen_t peerLen;

    Socket(FileDescriptor fd, SocketType type, SocketUse use);
    addrinfo* GetAddrInfo(std::string host, std::string service) const;
    FileDescriptor GetAndConnectFD(addrinfo* res) const;
    void GetPeer(sockaddr_storage* &other, socklen_t &len) const;
};

#endif  /* __Socket_hpp__ */