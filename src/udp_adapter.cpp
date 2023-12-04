/***********************************************
*****************Copyright(C)*******************
Filename:
Author:zhouyifan
E-mail:yifanperfect@foxmail.com
Version:v1.0
Date:
***********************************************/
#include "udp_adapter.h"

UdpAdapter::UdpAdapter()
{

}

UdpAdapter::~UdpAdapter()
{

}

void UdpAdapter::Initialize(const char *remote_ip, int remote_port, int sockmode)
{
    // create socket
    clien_sockfd_ = socket(AF_INET, SOCK_DGRAM, 0); // IPv4 Internet protocols  用于IPV4  SOCK_DGRAM:UDP  SOCK_STREAM:TCP

    if (clien_sockfd_ == -1)
    {
        perror("socket error:");
    }

    memset(&remote_addr_, 0, sizeof(remote_addr_));
    remote_addr_.sin_family = AF_INET;
    remote_addr_.sin_addr.s_addr = inet_addr(remote_ip); // inet_pton?
    remote_addr_.sin_port = htons(remote_port);

    //connect
    int ret = connect(clien_sockfd_,(struct sockaddr*)&remote_addr_,sizeof (struct sockaddr));
    if(ret < 0){
        perror("connect error");
    }

    sockmode_ = sockmode; // 阻塞、非阻塞
    printf("connect ...\n");

}

int UdpAdapter::Send(const char *send_buf, int buf_size)
{
    return send(clien_sockfd_,send_buf, buf_size, 0);
}

int UdpAdapter::Read(char *rcv_buf, int buf_size)
{
    return recv(clien_sockfd_, rcv_buf, buf_size, MSG_DONTWAIT); // MSG_DONTWAIT
}

int UdpAdapter::SendTo(const char *send_buf, int buf_size)
{
    return sendto(clien_sockfd_,send_buf, buf_size, 0, (struct sockaddr*)&remote_addr_,sizeof (struct sockaddr));
}

int UdpAdapter::ReadFrom(char *rcv_buf, int buf_size)
{
    // return recvfrom(clien_sockfd_, rcv_buf, buf_size,MSG_DONTWAIT, (struct sockaddr*)&remote_addr_,sizeof(struct sockaddr));
    return recv(clien_sockfd_, rcv_buf, buf_size, sockmode_);
}



