//
// Created by jihoj on 12/9/2021.
//

#ifndef EPOLL_EXAMPLE_EPOLLMANAGER_H
#define EPOLL_EXAMPLE_EPOLLMANAGER_H

#include <sys/epoll.h>

#include "Socket.h"
#include "Routine.h"

class EpollManager {
private:
    int epfd{}, size{};
    struct epoll_event event{};
    struct epoll_event *epEvents{};

public:
    EpollManager();
    EpollManager(int size);
    ~EpollManager();

    int enrollSocket(Socket socket);
    void wait(Socket servSock, Routine routine);
};


#endif //EPOLL_EXAMPLE_EPOLLMANAGER_H
