/*************************************************************************************************
* Description:	socket support file
* Author:		xiaofei.huang
* Versions:		V1.0
* Date:         2017-07-06
* Modifiy:
* CO.ltd:
* Other:
*************************************************************************************************/
// #include "stdafx.h"
#include "sock.h"
/*************************************************************************************************
* Function Name  : stringconv.
* Description    : IP地址字符串转化.
* Input          : p  - IP字符串输入.
* Return         : s  - 转化结果.
**************************************************************************************************/
void stringconv(unsigned char *p, char *s)
{
	int LoopCount = 0;
	int AddCount  = 0;
	int Temp[3]	  = {0};

	for (int i = 0; i < 4; i++)
	{
		Temp[LoopCount] = (p[i] / 100);

		if (Temp[LoopCount]) LoopCount++;

		Temp[LoopCount] = (p[i] - (p[i] / 100) * 100) / 10;

		if (Temp[LoopCount]) LoopCount++;

		Temp[LoopCount] = (p[i] - (p[i] / 10) * 10);

		if (Temp[LoopCount]) LoopCount++;

		if (LoopCount == 0)
		{
			s[AddCount + LoopCount] = '0';

			AddCount++;

			s[AddCount + LoopCount] = '.';

			AddCount++;
		}
		else
		{
			for (int k = 0; k < LoopCount; k++)
			{
				s[AddCount + k] = Temp[k];
			}

			s[AddCount + LoopCount] = '.';

			LoopCount++;

			AddCount += LoopCount;
		}

		LoopCount = 0;
	}

	s[AddCount - 1] = 0;
}
/*************************************************************************************************
* Function Name  : InitSocket.
* Description    : 启动Socket套接字.
* Input          : NONE.
* Return         : BOOL    - TRUE - 配置成功 ，FALSE - 配置失败.
**************************************************************************************************/
BOOL InitSocket()
{
	WORD	wVersionRequested;
	WSADATA wsaData;
	int		err=0;
	//---------------- Windows异步套接字的启动命令 -----------------
	wVersionRequested = MAKEWORD( 2, 2 );			// 版本
	err = WSAStartup( wVersionRequested, &wsaData );// 启动 

	if (err != 0)									// 判断启动是否成功
	{
		return false;
	}
	//--------------------------- 版本判断 -------------------------
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		 HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		WSACleanup();
		return false; 
	}

	return true;
}
/*************************************************************************************************
* Function Name  : udpInitClient.
* Description    : udp客户端初始化.
* Input          : Msock   - sock参数.
* Return         : BOOL    - TRUE - 配置成功 ，FALSE - 配置失败.
**************************************************************************************************/
BOOL udpInitClient(Msock *p)
{
	char m_Addr[32];
	char m_Addrcopy[32];



	//strncpy_s(m_Addrcopy, ip, 32);
    //strncpy(m_Addrcopy, (LPCSTR)p->gseriveIP, 32);
	int length = p->gseriveIP.GetLength();
	for (int i = 0; i < length; i++)
	{
		m_Addrcopy[i] = p->gseriveIP.GetAt(i);
	}
	m_Addrcopy[length] = '\0';

	for(int i = 0;i < 32;i += 2)
	{
		m_Addr[i/2] = m_Addrcopy[i];
	}
	//stringconv(p->gseriveIP, m_Addr);
	//---------------------- 建立客户端地址 ---------------------------
	p->gaddrserve.sin_family = AF_INET;

    p->gaddrserve.sin_addr.s_addr = inet_addr(m_Addrcopy);//(m_Addr);
															// IP地址：0.0.0.0： INADDR_ANY -指定地址为0.0.0.0的地址
															// htonl - 将主机数转换成无符号长整型的网络字节顺序
															// 可写成：inet_addr("0.0.0.0");
	p->gaddrserve.sin_port		  = htons(p->gserveport);	// 端口

    // htons - 将主机的无符号短整形数转换成网络字节顺序
	//---------- 创建一个与指定传送服务提供者捆绑的套接口 -----------
	p->gsocketclient = WSASocket(AF_INET,					// af   - 地址族规范。目前仅支持AF_INET格式
								 SOCK_DGRAM,				// type - 新套接口的类型描述 SOCK_STREAM - 是数据流,一般是tcp/ip协议的编程 SOCK_DGRAM - 是数据抱,是udp协议网络编程
								 IPPROTO_UDP,				// protocol - 套接口使用的特定协议，如果调用者不愿指定协议则定为0。
								 NULL,						// lpProtocolInfo - 一个指向PROTOCOL_INFO结构的指针，该结构定义所创建套接口的特性
								 0,							// g - 保留给未来使用的套接字组。套接口组的标识符。
								 0);						// iflags - 套接口属性描述。


	if(p->gsocketclient == INVALID_SOCKET)					// 判断创建是否成功
	{
		return false;
	}	
	ioctlsocket(p->gsocketclient,FIONBIO,(u_long FAR*)&p->gsockmode);
	//--------------------- 初始化发送和接收缓存 ---------------------
	p->gaddrserveLen = sizeof(p->gaddrserve);

	return true;
}
/*************************************************************************************************
* Function Name  : udpSendClient.
* Description    : UDP客户端发送数据.
* Input          : Msock   - sock参数.
*                ：buf     - 发送数据缓存.
*                ：len     - 发送数据长度.
* Return         : BOOL    - TRUE - 配置成功 ，FALSE - 配置失败.
**************************************************************************************************/
BOOL udpSendClient(Msock *p,char *buf,int len)
{
	int ret;

	ret = sendto(p->gsocketclient, buf, len, 0, (sockaddr*)&p->gaddrserve, p->gaddrserveLen);

	if (ret == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}
/*************************************************************************************************
* Function Name  : udpSendClient.
* Description    : UDP客户端接收数据.
* Input          : Msock   - sock参数.
*                ：buf     - 接收数据缓存.
*                ：len     - 接收数据长度.
* Return         : int     - 接收到的数据长度.
**************************************************************************************************/
int udpRecvClient(Msock *p, char *buf,int len)
{
	int ret = SOCKET_ERROR;

	ret = recvfrom(p->gsocketclient, buf, len,0, (sockaddr*)&p->gaddrserve,&p->gaddrserveLen);
	if (ret == SOCKET_ERROR)
	{
		return 0;
	}

	return ret;
}
/*************************************************************************************************
* Function Name  : udpClientClose.
* Description    : UDP客户端断开连接.
* Input          : Msock   - sock参数.
* Return         : BOOL    - TRUE - 配置成功 ，FALSE - 配置失败.
**************************************************************************************************/
BOOL udpClientClose(Msock *p)
{
	closesocket(p->gsocketclient);
	WSACleanup();
	return true;
}


