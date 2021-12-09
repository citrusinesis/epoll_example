//
// Created by jihoj on 12/9/2021.
//

#include <unistd.h>
#include <array>
#include <sys/epoll.h>
#include <iostream>

#include "EpollManager.h"
#include "Socket.h"
#include "Routine.h"

#define BUF_SIZE 1024

EpollManager::EpollManager() = default;
EpollManager::EpollManager(int size) {
    this->size = size;
    this->epfd = epoll_create(size);
    this->epEvents = new struct epoll_event[size];
}
EpollManager::~EpollManager() {
    close(epfd);
    delete epEvents;
}

int EpollManager::enrollSocket(Socket socket) {
    event.events = EPOLLIN;
    event.data.fd = socket.getFD();
    epoll_ctl(epfd, EPOLL_CTL_ADD, socket.getFD(), &event);
}
void EpollManager::wait(Socket servSock, Routine routine) {
    int addrSize
    Socket clntSock;
    while(true) {
        int cnt = epoll_wait(epfd, epEvents, size, -1);
        if (cnt < 0) {
            std::cerr << "epoll_wait()" << std::endl;
            break;
        }

        for (int i = 0; i < cnt; i++) {
            if (epEvents[i].data.fd == servSock.getFD()) {
                addrSize = sizeof clntSock.getSockAddrIn();
                clntSock.setFD(accept(servSock.getFD(),
                                      clntSock.getSockAddr(),
                                      reinterpret_cast<socklen_t *>(&addrSize)));
                clntSock.setNonBlockingMode();
                this->enrollSocket(clntSock);
                std::cout << "[*] Client Connected: " << clntSock.getFD() << "\n";
            } else {
                routine();
            }
        }
    }
}