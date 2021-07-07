#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

const int MAXLINE = 1024;

using namespace std;

int main(int argc, char **argv) {
    int sockfd, n;
    char recvline[MAXLINE+1];
    struct sockaddr_in servaddr;
    if (argc != 2){
        cout << "usage: a.out < IPaddress" << endl;
        exit(0);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        cout << "socket error" << endl;
        exit(0);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
        cout << "inet_ptons error" << endl;
        exit(0);
    }
    if (connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        cout << "connect error" << endl;
        exit(0);
    }
    while ((n = read(sockfd, recvline, MAXLINE)) > 0){
        recvline[n] = 0;
        if (fputs(recvline, stdout) == EOF){
            cout << "fputs error" << endl;
            exit(0);
        }
    }
    if (n < 0){
        cout << "read error" << endl;
        exit(0);
    }
    exit(0);
}