/* File Name: server.c */  
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<errno.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<netinet/in.h>  
#include<arpa/inet.h>
#include<unistd.h>
#include"chat.h"
#define DEFAULT_PORT 4567  //监听端口号
#define MAXLINE 4096  
int main(int argc, char** argv)  
{  
    //struct sockaddr_in from;
    //int fromlen=sizeof(from);
    Packet package;
    Data data;
    Kind kind;
    int    socket_fd, connect_fd;  
    struct sockaddr_in     servaddr;  
    char    buff[4096];  
    int     n;  
    //初始化Socket  
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  
    printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);  
    exit(0);  
    }  
    //初始化  
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。  
    servaddr.sin_port = htons(DEFAULT_PORT);//设置的端口为DEFAULT_PORT  
    //printf("%s",inet_ntoa(servaddr.sin_addr));
    //将本地地址绑定到所创建的套接字上  
    if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){  
    printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);  
    exit(0);  
    }  
    //开始监听是否有客户端连接  
    if( listen(socket_fd, 10) == -1){  
    printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);  
    exit(0);  
    }  
    printf("======waiting for client's request======\n");  
    while(1){  
//阻塞直到有客户端连接，不然多浪费CPU资源。  
        //if( (connect_fd = accept(socket_fd, (struct sockaddr*)&from, (unsigned int*)&fromlen)) == -1){ 
        if( (connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1){ 
        printf("accept socket error: %s(errno: %d)",strerror(errno),errno);  
        continue;  
    }  
//接受客户端传过来的数据  
    if(read(connect_fd, &package,sizeof(Packet))<0) printf("failed\n");
    printf("succed\n");
    parse_packet(package,&kind,&data);
    buff[n] = '\0';  
    printf("id_to: %s\nid_from：%s\n:%d\n", data.message.id_to,data.message.id_from,kind);  
    close(connect_fd);  
    }  
    close(socket_fd);  
}
