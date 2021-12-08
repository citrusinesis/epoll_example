//
// Created by jihoj on 12/8/2021.
//

#include <unistd.h>
#include <fcntl.h>
#include <cstdio>

#include "Socket.h"

Socket::Socket() : fd(0), addr(0) {}
Socket::Socket(int fd, sockaddr_in *addr) : fd(fd), addr(addr) {}
Socket::Socket(const Socket &socket) : fd(socket.fd), addr(socket.addr) {}

Socket::~Socket() {
    close(this->fd);
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