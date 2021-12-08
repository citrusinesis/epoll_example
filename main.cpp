#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#include "Socket.h"

#define BUF_SIZE 1024
#define BACKLOG 8
#define EPOLL_SIZE 16
#define PORT 9573

int main() {
    Socket servSock, clntSock;
    socklen_t addrSize;
    char buf[BUF_SIZE];
    int len;

    struct epoll_event *epEvents;
    struct epoll_event event;
    int epfd, eventCount;

    if (servSock.start(PORT, BACKLOG) == -1) exit(1);

    epfd = epoll_create(EPOLL_SIZE);
    epEvents = new struct epoll_event[EPOLL_SIZE];

    event.events = EPOLLIN;
    event.data.fd = servSock.getFD();
    epoll_ctl(epfd, EPOLL_CTL_ADD, servSock.getFD(), &event);

    std::cout << "[*] READY\n";

    while(true) {
        eventCount = epoll_wait(epfd, epEvents, EPOLL_SIZE, -1);
        if (eventCount < 0) {
            std::cerr << "epoll_wait()" << std::endl;
            break;
        }

        for (int i = 0; i < eventCount; i++) {
            if (epEvents[i].data.fd == servSock.getFD()) {
                addrSize = sizeof clntSock.getSockAddrIn();
                clntSock.setFD(accept(servSock.getFD(), clntSock.getSockAddr(), &addrSize));
                clntSock.setNonBlockingMode();
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = clntSock.getFD();
                epoll_ctl(epfd, EPOLL_CTL_ADD, clntSock.getFD(), &event);
                std::cout << "[*] Client Connected: " << clntSock.getFD() << "\n";
            } else {
                len = read(epEvents[i].data.fd, buf, BUF_SIZE);
                if (len == 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, epEvents[i].data.fd, NULL);
                    close(epEvents[i].data.fd);
                    std::cout << "[*] Client Disconnected: " << epEvents[i].data.fd << "\n";
                } else {
                    write(epEvents[i].data.fd, buf, len);
                }
            }
        }
    }

    close(epfd);
    delete epEvents;
    return 0;
}