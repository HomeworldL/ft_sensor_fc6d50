/***********************************************
*****************Copyright(C)*******************
Filename:
Author:zhouyifan
E-mail:yifanperfect@foxmail.com
Version:v1.0
Date:
***********************************************/
#ifndef UDP_ADAPTER_H
#define UDP_ADAPTER_H
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/un.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>

class UdpAdapter
{
public:
    UdpAdapter();
    ~UdpAdapter();
    void Initialize(const char * remote_ip,int remote_port, int sockmode=0);
    int Send(const char *send_buf,int buf_size);
    int Read(char *rcv_buf,int buf_size);
    int SendTo(const char *send_buf,int buf_size);
    int ReadFrom(char *rcv_buf,int buf_size);
    
private:
    int clien_sockfd_;
    struct sockaddr_in remote_addr_;
    int sockmode_;
};


#endif // UDP_ADAPTER_H
