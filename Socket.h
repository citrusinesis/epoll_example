//
// Created by jihoj on 12/8/2021.
//

#ifndef EPOLL_EXAMPLE_SOCKET_H
#define EPOLL_EXAMPLE_SOCKET_H

#include <sys/socket.h>

class Socket {
private:
    int fd;
    struct sockaddr_in *addr;

public:
    Socket();
    Socket(int fd, sockaddr_in *addr);
    Socket(Socket const &socket);
    ~Socket();

    int getFD() {
        return fd;
    }
    struct sockaddr_in* getSockAddrIn() {
        return addr;
    }
    struct sockaddr* getSockAddr() {
        return (struct sockaddr*)addr;
    }

    void setNonBlockingMode();
    void releaseNonBlockingMode();
};


#endif //EPOLL_EXAMPLE_SOCKET_H
