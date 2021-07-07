#include <iostream>
#include <netinet/in.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctime>
#include <fcntl.h>
#include <arpa/inet.h>
#include <cerrno>
#include <unistd.h>

using namespace std;

const int MAXLINE = 1024;

int main() {
    int listenfd, connfd;
    struct sockaddr_in servaddr{};
    char buff[MAXLINE];
    time_t ticks;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0){
        cout << "socket error" << strerror(errno) << endl;
        exit(0);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int bindfd = bind(listenfd, (sockaddr*)&servaddr, sizeof(servaddr));
    if (bindfd < 0){
        cout << "bind error" << bindfd << endl;
        exit(0);
    }
    listen(listenfd, MAXLINE);
    struct sockaddr_in clientaddr;
    while (1){
        FILE *logs = fopen("log.txt", "a+");
        if (logs == NULL) {
            printf("open file error:\n");
        }
        connfd = accept(listenfd, (sockaddr*)NULL, NULL);
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        cout << strlen(buff) << endl;
        write(connfd, buff, strlen(buff));
        if (connfd != 0) {
            getpeername(connfd, (struct sockaddr*)&clientaddr, (socklen_t *)sizeof(clientaddr));
            sprintf(buff, "请求时间:%sIP地址:%s\n", ctime(&ticks), inet_ntoa(clientaddr.sin_addr));
            fputs(buff, logs);
        }
        close(connfd);
        fclose(logs);
    }
}
