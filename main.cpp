#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 1024
#define BACKLOG 8
#define EPOLL_SIZE 16
#define PORT 9573

void errorHandling(const char* msg) {
    perror(msg);
    exit(1);
}

void setNonBlockingMode(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

int main() {
    int servSock, clntSock;
    struct sockaddr_in servAddr, clntAddr;
    socklen_t addrSize;
    char buf[BUF_SIZE];
    int len;

    struct epoll_event *epEvents;
    struct epoll_event event;
    int epfd, eventCount;

    servSock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&servAddr, 0, sizeof servAddr);
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(PORT);

    if (bind(servSock, (struct sockaddr*) &servAddr, sizeof servAddr) < 0) {
        errorHandling("bind()");
    }
    if (listen(servSock, BACKLOG) < 0) {
        errorHandling("listen()");
    }

    epfd = epoll_create(EPOLL_SIZE);
    epEvents = new struct epoll_event[EPOLL_SIZE];

    event.events = EPOLLIN;
    event.data.fd = servSock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, servSock, &event);

    std::cout << "[*] READY\n";

    while(true) {
        eventCount = epoll_wait(epfd, epEvents, EPOLL_SIZE, -1);
        if (eventCount < 0) {
            std::cerr << "epoll_wait()" << std::endl;
            break;
        }

        for (int i = 0; i < eventCount; i++) {
            if (epEvents[i].data.fd == servSock) {
                addrSize = sizeof clntAddr;
                clntSock = accept(servSock, (struct sockaddr*) &clntAddr, &addrSize);
                setNonBlockingMode(clntSock);
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = clntSock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clntSock, &event);
                std::cout << "[*] Client Connected: " << clntSock << "\n";
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

    close(servSock);
    close(epfd);
    delete epEvents;
    return 0;
}