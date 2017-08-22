#ifndef __Socket_h__
#define __Socket_h__

#include <string>
#include <exception>
#include <cassert>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

/** indicates sockets protocol 
 * info: http://www.diffen.com/difference/TCP_vs_UDP
*/
enum SocketType
{
    TCP,
    UDP,
    Raw,
    Any
};

/** Will it connect to other port, or bind to an empty one?
 *  info: https://stackoverflow.com/questions/27014955/socket-connect-vs-bind
 */
enum SocketUse
{
    toConnect,
    toBind,

    /** Warning! internal use only! using it externally throws exception
     *  indicates socket is a server and is serving at this time
     *  socket.use takes this when a connection is created
     */
    toServe 
};

using FileDescriptor = int;

/** allows communications between programs
 *  info: https://www.tutorialspoint.com/unix_sockets/what_is_socket.htm
 */
class Socket
{
public:
    Socket(std::string host, std::string service, SocketType type, SocketUse use);

    /** if host is localhost */
    Socket(std::string portNumber, SocketType type, SocketUse use);

    /** copy constructor */
    Socket(const Socket&);
    ~Socket();

    /** blocks execution untill there is a connection
     *  used only with `toBind` socket
     *  throws exception if socket is closed
     *  @maxNumberOfCon: maximum number of connections that can be listened to at same time
     */
    void Listen(unsigned int maxNumberOfCon);

    /** deques next connection to `toBind` only sockets
     *  if socket is a blocking socket, it blocks execution until there is a connection in the queue
     *  throws exception if socket is closed
     *  returns a `toServe` connected socket that can send/receive
     */
    Socket Accept();

    /** sends buffer to peer 
     *  must be non-`toBind` socket 
     *  throws exception if socket is closed
     */
    void Send(const void* buffer, const int size);

    /** same as the other Send, but changes `string toSend` into a buffer */
    void Send(std::string toSend);

    /** receives a `chunkSize` bites string from the peer in a `timeOut` seconds
     *  sokcet must be `toConnect` or `toServe`, and must be connected (not closed)
     *  copied & edited from 'http://www.binarytides.com/receive-full-data-with-recv-socket-function-in-c/'
     */
    std::string Receive(const unsigned int timeOut, const unsigned int chunkSize=512); // TODO: make timeOut double of melliseconds

    /** closes connection, stops socket from reading/writing, removes peer info
     *  can't close a closed socket
     */
    void Close();

    /** is socket not closed?
     *  check last time it tried to read from peer, if it is connected or not
     *  doesn't check in real time, socket could be closed by peer but marked as connected
     *  to refresh it try to receive more data
     */
    bool IsConnected() const;

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

#endif  /* __Socket_h__ */