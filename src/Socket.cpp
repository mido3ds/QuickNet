#include "Socket.hpp"

using std::string;
using std::exception;

////////////////////////////////////////////////////////////////////////////////

Socket::Socket(string host, string service, Protocol protocol, SocketUse use)
    :protocol(protocol), use(use)
{
    addrinfo *res = GetAddrInfo(host, service);
    fd = GetAndConnectFD(res);
    freeaddrinfo(res);

    isConnected = true;
}

Socket::Socket(string portNumber, Protocol protocol, SocketUse use)
    :Socket("", portNumber, protocol, use)
{}

// private constructor
Socket::Socket(FileDescriptor fd, Protocol protocol, SocketUse use)
    :fd(fd), protocol(protocol), use(use)
{}

Socket::~Socket()
{
    if (fd != -1)
        Close();
}

////////////////////////////////////////////////////////////////////////////////

void Socket::Listen(int maxNumberOfCon)
{
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

    return Socket(new_fd, protocol, toServe);
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
}

bool Socket::IsConnected()
{
    return isConnected;
}

////////////////////////////////////////////////////////////////////////////////

addrinfo* Socket::GetAddrInfo(string host, string service) const
{
    addrinfo hints, *res = nullptr;

    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_flags = this->use == toBind ? AI_PASSIVE : 0;
    hints.ai_family = PF_UNSPEC;
    switch (this->protocol)
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
            break;
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

    return sfd;
}