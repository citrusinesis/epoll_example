//
// Created by jihoj on 12/8/2021.
//

#ifndef EPOLL_EXAMPLE_SOCKET_H
#define EPOLL_EXAMPLE_SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>

class Socket {
private:
    int fd{};
    struct sockaddr_in addr{};

    void init(int port);
    void init(const char* ip, int port);

public:
    Socket();
    Socket(int fd, sockaddr_in addr);
    Socket(Socket const &socket);
    ~Socket();

    const int getFD() {
        return this->fd;
    }
    const struct sockaddr_in& getSockAddrIn() {
        return this->addr;
    }
    struct sockaddr* getSockAddr() {
        return (struct sockaddr*) &addr;
    }
    void setFD(int fd) {
        this->fd = fd;
    }
    void setAddr(struct sockaddr_in addr) {
        this->addr = addr;
    }

    void setNonBlockingMode();
    void releaseNonBlockingMode();

    int start(const char* ip, int port);
    int start(int port, int backlog);
};

#endif //EPOLL_EXAMPLE_SOCKET_H
