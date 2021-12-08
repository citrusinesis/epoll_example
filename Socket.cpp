//
// Created by jihoj on 12/8/2021.
//

#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <cstring>
#include <arpa/inet.h>

#include "Socket.h"

Socket::Socket() = default;
Socket::Socket(int fd, sockaddr_in addr) : fd(fd), addr(addr) {}
Socket::Socket(const Socket &socket) : fd(socket.fd), addr(socket.addr) {}

Socket::~Socket() {
    close(this->fd);
}

void Socket::init(int port) {
    fd = socket(PF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
}

void Socket::init(const char *ip, int port) {
    fd = socket(PF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(inet_addr(ip));
    addr.sin_port = htons(port);
}

void Socket::setNonBlockingMode() {
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

void Socket::releaseNonBlockingMode() {
    int oldflag = fcntl(fd, F_GETFL, 0);
    if (oldflag == -1) {
        perror("fcntl()");
        return;
    }
    fcntl(fd, F_SETFL, oldflag & ~O_NONBLOCK);
}

int Socket::start(int backlog) {
    if (bind(fd, this->getSockAddr(), sizeof addr) < 0) {
        perror("bind()");
        return -1;
    }

    if (listen(fd, backlog) < 0) {
        perror("listen()");
        return -1;
    }
}

int Socket::start() {
    return connect(fd, this->getSockAddr(), sizeof addr);
}