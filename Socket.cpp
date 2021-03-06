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
Socket::Socket(int port) {
    fd = socket(PF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
}
Socket::Socket(const char *ip, int port) {
    fd = socket(PF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(inet_addr(ip));
    addr.sin_port = htons(port);
}
Socket::~Socket() {
    close(this->fd);
}

void Socket::setNonBlockingMode() {
    int flag = fcntl(fd, F_GETFL, 0);
    if (flag == -1) {
        perror("fcntl()");
        return;
    }
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