#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>

using namespace std;

int clientfd;
char IP[50] = "127.0.0.1";

bool check(char *buff){
    for (int i = 0; i < strlen(buff); ++i) {
        if (buff[i] < '1' || buff[i] > '3'){
            return false;
        }
    }
    return true;
}

void getName(char *file){
    int j = 0;
    char res[100];
    for (int i = strlen(file); i >= 0 ; --i) {
        if (file[i] == '/'){
            break;
        }
        j++;
    }
    if (j == 0){
        strcpy(file, "default");
    }
    j--;
    int n = 0;
    for (int i = strlen(file)-j; i < strlen(file); ++i) {
        res[n++] = file[i];
    }
    strcpy(file, res);
}

void init(){
    clientfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(2333);
    addr.sin_addr.s_addr = inet_addr(IP);
    if (connect(clientfd, (struct sockaddr*)&addr, sizeof(addr)) == -1){
        perror("无法连接到服务器");
        exit(-1);
    }
    printf("客户端启动成功\n");
}

void user(){
    string username, passwd;
    cout << "请输入用户名: " << endl;
    cin >> username;
    cout << "请输入密码: " << endl;
    cin >> passwd;
    char buff[1024];
    sprintf(buff, "%s:%s\n", username.c_str(), passwd.c_str());
    send(clientfd, buff, 1024, 0);
}

void start(){
    char rec[100] = {0};
    char sen[100] = {0};
    while (strlen(rec) != 12){
        cout << "**********************\n1.注册\n2.登录\n3.退出\n**********************" << endl;
        cin >> sen;
        while (!check(sen)){
            cout << "wrong choice" << endl;
             cin >> sen;
        }
        if (sen[0] == '3'){
            close(clientfd);
            return;
        }
        send(clientfd, sen, strlen(sen), 0);
        user();
        recv(clientfd, rec, sizeof(rec), 0);
        cout << rec << endl;
    }
    cout << "where do you want to submit? (option: /tmp/1/ /tmp/2/ /tmp/3/)\nyou should input a number: ";
    cin >> sen;
    while (!check(sen)){
        cout << "wrong choice" << endl;
        cin >> sen;
    }
    cout << "Now, input your file name(absolute path!)\n";
    cin >> rec;
    char filename[100];
    char buff[1024] = {0};
    int count;
    FILE *f = fopen(rec, "rb");
    getName(rec);
    sprintf(filename, "/tmp/%s/%s", sen, rec);
    send(clientfd, filename, strlen(filename), 0);
    if (f == NULL){
        cout << "file open fail" << endl;
        exit(1);
    }
    recv(clientfd, buff, 1024, 0);
    printf("%s\n", buff);
    char buffer[1024];
    while ((count = fread(buffer, 1, 1024, f)) > 0){
        send(clientfd, buffer, count, 0);
    }
    printf("file uploaded: %s", filename);
    fclose(f);
    close(clientfd);
}

int main() {
    init();
    start();
}
