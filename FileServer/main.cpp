#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>
#include <fstream>

using namespace std;

int serverfd;
int fd;

void init(){
    serverfd = socket(PF_INET, SOCK_STREAM, 0);
    if (serverfd == -1){
        perror("创建socket失败");
        exit(-1);
    }
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(2333);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(serverfd, (struct sockaddr*)&addr, sizeof(addr)) == -1){
        perror("绑定失败");
        exit(-1);
    }
    if (listen(serverfd, 100) == -1){
        perror("设置监听失败");
        exit(-1);
    }
}

void split(const char* info, char* user, char* passwd, char dil){
    int i = 0;
    int j;
    for (; ; ++i) {
        if (info[i] == dil){
            for (j = i+1; j < strlen(info); ++j) {
                if (info[j] == '\n') break;
                passwd[j-i-1] = info[j];
            }
            break;
        } else {
            user[i] = info[i];
        }
    }
    user[i] = '\0';
    passwd[j-i-1] = '\0';
}

bool userRegister(){
    char buff[1024] = {0};
    recv(fd, buff, 1024, 0);
    fstream in("/root/info.txt", ios::in);
    if (!in.is_open()){
        cout << "open file faillllll" << endl;
    }
    string info;
    char user[100], passwd[100], user1[100];
    split(buff, user1, passwd, ':');
    while (getline(in, info)){
        split(info.c_str(), user, passwd, ':');
        if (strcmp(user1,user) == 0){
            send(fd, "用户已存在", sizeof("用户已存在"), 0);
            in.close();
            return false;
        }
    }
    in.close();
    fstream out;
    out.open("/root/info.txt", ios::out | ios::app);
    if (!out.is_open()){
        cout << "OPEN FILE FAILE" << endl;
    }
    out << buff;
    out.flush();
    out.close();
    send(fd, "注册成功", sizeof("注册成功"), 0);
    cout << user1 << "注册成功" << endl;
    return true;
}

bool userLogin(){
    char buff[1024] = {0};
    recv(fd, buff, 1024, 0);
    ifstream in("/root/info.txt");
    string info;
    char user[100], passwd[100], user1[100], passwd1[100];
    split(buff, user1, passwd1, ':');
    while (getline(in, info)){
        split(info.c_str(), user, passwd, ':');
        if (strcmp(user, user1) == 0 && strcmp(passwd, passwd1) == 0){
            send(fd, "登录成功", sizeof("登录成功"), 0);
            cout << user << "登录成功" << endl;
            in.close();
            return true;
        }
    }
    send(fd, "用户名或密码错误", sizeof("用户名或密码错误"), 0);
    return false;
}

void server(){
    printf("服务器启动\n");
    while (1){
        struct sockaddr_in fromaddr;
        socklen_t len = sizeof(fromaddr);
        fd = accept(serverfd, (struct sockaddr*)&fromaddr, &len);
        if (fd == -1){
            printf("客户端连接出错...\n");
            continue;
        }
        char backup[100] = {0};
        char choice[5] = {0};
        bool flag = false;
        while (!flag) {
            recv(fd, choice, sizeof(choice), 0);
            switch (choice[0]) {
                case '1':
                    flag = userRegister();
                    break;
                case '2':
                    flag = userLogin();
                    break;
            }
        }
        recv(fd, backup, sizeof(backup), 0);
        FILE *f = fopen(backup, "wb");
        if (f == NULL){
            send(fd, "open file fail", sizeof("open file fail"), 0);
            cout << "open file fail" << endl;
            continue;
        }
        send(fd, "file uploading...\n", sizeof("file uploading...\n"), 0);
        char buf[1024] = {0};
        int count;
        while ((count = recv(fd, buf, sizeof(buf), 0)) > 0){
            fwrite(buf, count, 1, f);
        }
        cout << "success! Back up in: " << backup << endl;
        fclose(f);
        close(fd);
    }
}

int main() {
    init();
    server();
}
