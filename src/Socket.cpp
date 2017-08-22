#include "Socket.h"

using std::string;
using std::exception;

////////////////////////////////////////////////////////////////////////////////

Socket::Socket(string host, string service, SocketType type, SocketUse use)
    :type(type), use(use)
{
    addrinfo* res = nullptr;

    // user shouldn't provide this option
    if (use == toServe)
        throw exception(); // TODO

    // address
    res = GetAddrInfo(host, service);
    assert(res != nullptr);
    freeaddrinfo(res);

    // file descriptor 
    fd = GetAndConnectFD(res);
    assert(fd != -1);
    isConnected = true;

    // peer 
    if (use == toConnect) 
        GetPeer(peer, peerLen);
    assert(peer != nullptr);
}

Socket::Socket(string portNumber, SocketType type, SocketUse use)
    :Socket("", portNumber, type, use)
{}

// private constructor
Socket::Socket(FileDescriptor fd, SocketType type, SocketUse use)
    :fd(fd), type(type), use(use)
{
    GetPeer(peer, peerLen);
}

Socket::Socket(const Socket& s)
    :fd(s.fd), isConnected(s.isConnected), peer(s.peer), peerLen(s.peerLen), type(s.type), use(s.use)
{}

Socket::~Socket()
{}

////////////////////////////////////////////////////////////////////////////////

void Socket::Listen(unsigned int maxNumberOfCon)
{
    if (use != toBind)
        throw exception();  // TODO
    if (!isConnected)
        throw exception();  // TODO
    assert(fd != -1);

    int stats = listen(fd, maxNumberOfCon);
    if (stats != 0)
        throw exception(); // TODO
}

Socket Socket::Accept()
{
    if (use != toBind)
        throw exception();  // TODO
    if (!isConnected)
        throw exception();  // TODO
    assert(fd != -1);

    FileDescriptor new_fd;
    new_fd = accept(fd, NULL, NULL);

    if (new_fd < 0)
        throw exception();  // TODO

    return Socket(new_fd, type, toServe);
}

void Socket::Send(const void* toSend, const int size)
{
    int bytesSent = 0,
        flags = 0, n;
    char* buffer = (char *) toSend;

    if (use == toBind)
        throw exception();  // TODO
    assert(peer != nullptr)
    if (!isConnected) 
        throw exception(); // TODO
    assert(fd != -1);

    while (bytesSent < size)
    {
        n = sendto(fd, buffer+bytesSent, size-bytesSent, flags, (sockaddr*)peer, peerLen);

        if (n == -1)
            throw exception(); // TODO

        bytesSent += n;
    }

    assert(bytesSent == size);
}

void Socket::Send(string toSend)
{
    Send((void*) toSend.c_str(), toSend.size() + 1);
}

string Socket::Receive(const unsigned int timeOut, const unsigned int chunkSize)
{
    assert(timeOut != 0);
    assert(chunkSize != 0);

    int sizeRecvd = 0, 
        totalSize = 0, 
        flags = MSG_DONTWAIT;
    char chunk[chunkSize];
    double timeDiff;

    timeval begin, now;
    gettimeofday(&begin , NULL);
        
    while (true)
    {
        gettimeofday(&now , NULL);
        // time elapsed in seconds
        timeDiff = (now.tv_sec - begin.tv_sec) + 1e-6 * (now.tv_usec - begin.tv_usec);
        
        // if you got some data, then break after timeout
        if (totalSize > 0 && timeDiff > timeOut)
            break;
            
        // if you got no data at all, wait a little longer, 1.5 the timeout
        else if (timeDiff > timeOut*1.5)
            break;
        
        memset(chunk, 0, chunkSize); 

        // received successfully
        if ((sizeRecvd = recvfrom(fd, chunk, chunkSize-1, flags, (sockaddr*)peer, &peerLen)) > 0)
            totalSize += sizeRecvd;
        // connection closed [only TCP]
        else if (sizeRecvd == 0 && type == TCP)
        {
            Close();
            break;
        }
        // received nothing, wait [0.1 seconds] before trying again
        else
            usleep(100000);
    }

    // cut the chunk
    assert(totalSize+1 <= chunkSize);
    chunk[totalSize+1] = '\0';
    
    return string(chunk);
}

////////////////////////////////////////////////////////////////////////////////

void Socket::Close()
{
    if (!isConnected)   
        throw exception();  // TODO

    close(fd);
    fd = -1;
    peer = nullptr;
    isConnected = false;
}

bool Socket::IsConnected() const 
{
    if (isConnected) assert(fd != -1);
    
    return isConnected;
}

////////////////////////////////////////////////////////////////////////////////

addrinfo* Socket::GetAddrInfo(string host, string service) const
{
    assert(use != toServe);

    addrinfo hints, *res = nullptr;

    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_flags = (use == toBind) ? AI_PASSIVE : 0;
    hints.ai_family = PF_UNSPEC;
    switch (type)
    {
        case TCP:
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;
            break;
        case UDP:
            hints.ai_socktype = SOCK_DGRAM;
            hints.ai_protocol = IPPROTO_UDP;
            break;
        case Raw:
            hints.ai_socktype = SOCK_RAW;
            hints.ai_protocol = 0;
            break;
        case Any:
            hints.ai_socktype = 0;
            hints.ai_protocol = 0;
            break;
        default:
            throw exception(); // TODO
    }

    const char *host_c_string = host.c_str(),
            *serv_c_string = service.c_str();

    if (host == "" && service == "")
        throw exception(); // TODO
    if (host == "")
        host_c_string = nullptr;
    if (service == "")
        serv_c_string = nullptr;

    int stats = getaddrinfo(host_c_string, serv_c_string, &hints, &res);
    if (stats != 0)
        throw exception(); // TODO

    assert(res != nullptr);
    return res;
}

FileDescriptor Socket::GetAndConnectFD(addrinfo* res) const
{
    assert(use != toServe);
    
    addrinfo *p = nullptr;
    int sfd;

    // try to get a socket
    for (p = res; p != nullptr; p = p->ai_next)
    {
        if ((sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
            continue;

        if (use == toBind)
        {
            if (bind(sfd, res->ai_addr, res->ai_addrlen) >= 0)
                break;
        }
        else
        {
            if (connect(sfd, res->ai_addr, res->ai_addrlen) >= 0)
                break;
        }

        close(sfd);
    }

    // cant get a socket 
    // TODO: detect wheter exception is from socket() or connect() or bind()
    if (p == nullptr)
        throw exception(); // TODO

    assert(sfd > 0);
    return sfd;
}

void Socket::GetPeer(sockaddr_storage* &other, socklen_t &len) const
{
    assert(use != toBind);
    assert(fd != -1);

    other = nullptr;
    len = sizeof (sockaddr_storage);

    if (getpeername(fd, (sockaddr*)other, &len) != 0) 
        throw exception(); // TODO

    assert(other != nullptr);
}

////////////////////////////////////////////////////////////////////////////////
