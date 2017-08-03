#include "Socket.hpp"

using std::string;
using std::exception;

////////////////////////////////////////////////////////////////////////////////

Socket::Socket(string host, string service, SocketType type, SocketUse use)
    :type(type), use(use)
{
    addrinfo *res = GetAddrInfo(host, service);
    assert(res != nullptr);

    fd = GetAndConnectFD(res);
    assert(fd != -1);

    freeaddrinfo(res);
    isConnected = true;
}

Socket::Socket(string portNumber, SocketType type, SocketUse use)
    :Socket("", portNumber, type, use)
{}

// private constructor
Socket::Socket(FileDescriptor fd, SocketType type, SocketUse use)
    :fd(fd), type(type), use(use)
{}

Socket::~Socket()
{
    if (fd != -1)
        Close();
}

////////////////////////////////////////////////////////////////////////////////

void Socket::Listen(int maxNumberOfCon)
{
    assert(maxNumberOfCon > 0);

    if (use != toBind)
        throw exception(); // TODO

    int stats = listen(fd, maxNumberOfCon);
    if (stats != 0)
        throw exception(); // TODO
}

Socket Socket::Accept()
{
    if (use != toBind)
        throw exception();  // TODO

    FileDescriptor new_fd;
    new_fd = accept(fd, NULL, NULL);

    if (new_fd < 0)
        throw exception();  // TODO

    return Socket(new_fd, type, toServe);
}

void Socket::Send(string toSend)
{
    // TODO
}

string Socket::Receive()
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void Socket::Close()
{
    close(fd);
    fd = -1;
    isConnected = false;
}

bool Socket::IsConnected()
{
    if (isConnected) assert(fd != -1);

    return isConnected;
}

////////////////////////////////////////////////////////////////////////////////

addrinfo* Socket::GetAddrInfo(string host, string service) const
{
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
    addrinfo *p = nullptr;
    int sfd;

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
    freeaddrinfo(res);

    if (p == nullptr)
        throw exception(); // TODO

    assert(sfd > 0);
    return sfd;
}