/*************************************************************************************************
* Description:	socket support head file
* Author:		xiaofei.huang
* Versions:		V1.0
* Date:         2017-07-06
* Modifiy:
* CO.ltd:
* Other:
*************************************************************************************************/
#ifndef _MSOCK_H_ 
#define _MSOCK_H_ 

//#ifdef __cplusplus
//extern "C" {
//#endif 
//

//#include "afxwin.h"
// #include "atlstr.h"
#include <sys/socket.h>

//------------------------------------------- SOCKET通讯宏定义 ------------------------------------------ 
//#define	RECVSIZE		1024							// 接收缓存大小
//#define	SENDSIZE		1024							// 发送缓存大小
//------------------------------------------- SOCKET参数结构体 ------------------------------------------
typedef struct _sock_param								// sock应用变量结构体定义 
{
	SOCKET				 gsocketserve;					// 服务端套接字
	SOCKET				 gsocketclient;					// 客户端套接字
	int					 gsockmode;						// 0 - 阻塞模式， 1 - 非阻塞 
	SOCKADDR_IN			 gaddrserve;					// 服务器地址
	SOCKADDR_IN			 gaddrclient;					// 客户端地址
	//unsigned char		 gseriveIP[4];					// 服务器地址，做客户端时需设置该值，来访问服务器
	//unsigned char		 gclientIP[4];					// 客户端地址

	CString				 gseriveIP;						// 服务器地址，做客户端时需设置该值，来访问服务器
	CString				 gclientIP;						// 客户端地址

	int					 gaddrserveLen;					// 接收数据长度	
	int					 gaddrclientLen;				// 发送数据长度
	unsigned short       gserveport;					// 服务器端口，做客户端时需设置该值，来访问服务器
	unsigned short       gclientport;					// 客户端端口
}Msock;

BOOL	InitSocket();									// 启动套接字命令
/********************************************* UDP客户端 *******************************************/
void    stringconv		(								// IP地址字符串转化
						unsigned char *p,				// p - 待转换的字符
						char *s);						// S - 转化结果

BOOL	udpInitClient	(Msock *p);						// UDP客户端初始化,p - SOCKET通讯结构体

BOOL	udpSendClient	(								// UDP客户端发送程序
						Msock *p,						// p - SOCKET通讯结构体
						char *buf,						// buf - 发送缓存
						int len);						// len - 发送长度

int		udpRecvClient	(								// UDP客户端接收程序
						Msock *p,						// p - SOCKET通讯结构体
						char *buf,						// buf - 接收缓存
						int len);						// len - 接收长度

BOOL	udpClientClose	(Msock *p);						// UDP客户端关闭,p - SOCKET通讯结构体


//#ifdef __cplusplus
//}
//#endif

#endif

