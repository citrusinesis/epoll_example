//
// Created by jihoj on 12/9/2021.
//

#include <unistd.h>
#include <array>
#include <iostream>

#include "EpollManager.h"
#include "Socket.h"

EpollManager::EpollManager() = default;
EpollManager::EpollManager(int size) {
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
void EpollManager::wait() {

}