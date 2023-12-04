/*************************************************************************************************
* Description:	com support file for NBIT sensor 
* Author:		xiaofei.huang
* Versions:		V1.0
* Date:         2017-07-06
* Modifiy:
* CO.ltd:       
* Other:
*************************************************************************************************/
#include "stdafx.h"
#include "comm.h"

/*************************************************************************************************
* Function Name  : open_com.
* Description    : 打开串口.
* Input          : com    - 串口号.
* Return         : HANDLE - 设备句柄.
**************************************************************************************************/
HANDLE open_com(LPCWSTR com)
{
	HANDLE hCOM = INVALID_HANDLE_VALUE;
#ifdef SYNC
	hCOM = CreateFile(com,							// 串口名称,如："COM1"					
		GENERIC_READ|GENERIC_WRITE,					// 允许读、写,如：GENERIC_READ|GENERIC_WRITE
		0,											// 独占方式：0
		NULL,										// NULL
		OPEN_EXISTING,								// 打开而不是创建:OPEN_EXISTING
		0,											// 同步方式:0,	异步方式：FILE_FLAG_OVERLAPPED
		NULL);										// NULL
#else
	hCOM = CreateFile(com,							// 串口名称,如："COM1"					
		GENERIC_READ|GENERIC_WRITE,					// 允许读、写,如：GENERIC_READ|GENERIC_WRITE
		0,											// 独占方式：0
		NULL,										// NULL
		OPEN_EXISTING,								// 打开而不是创建:OPEN_EXISTING
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,	// 同步方式:0,	异步方式：FILE_FLAG_OVERLAPPED
		NULL);										// NULL
#endif
	return hCOM;	
}
/*************************************************************************************************
* Function Name  : config_com.
* Description    : 配置串口.
* Input          : hcom    - 设备句柄.
*                ：bufsize - 收发数据缓存大小
*                ：baud    - 波特率
* Return         : BOOL    - TRUE - 配置成功 ，FALSE - 配置失败.
**************************************************************************************************/
BOOL config_com(HANDLE hcom,DWORD bufsize,DWORD baud)
{
	DCB				Mcomm_DCB;  // 
	COMMTIMEOUTS	Mcomm_TIMES;// 超时参数

	if(hcom == INVALID_HANDLE_VALUE) return FALSE;

	GetCommState(hcom,&Mcomm_DCB);
	Mcomm_DCB.BaudRate = baud;							// 波特率
	Mcomm_DCB.ByteSize = 8;								// 每个字节有8 位
	Mcomm_DCB.Parity   = NOPARITY;						// 无奇偶校验位
	Mcomm_DCB.StopBits = ONESTOPBIT;					// 一个停止位

	if (!SetCommState(hcom, &Mcomm_DCB))				// 设置串口参数,并检查设置是否成功
	{
		return FALSE;
	}

	Mcomm_TIMES.ReadIntervalTimeout = 100;				// 读间隔超时	
	if (!SetCommTimeouts(hcom, &Mcomm_TIMES))			// 设置超时参数，并检查设置是否成功
	{
		return FALSE;
	}

	//---------------------------------------- 初始化配置参数 --------------------------------------------
	SetupComm(hcom, bufsize, bufsize);					// 设置输入输出缓存大小

	PurgeComm(hcom,PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR|PURGE_RXCLEAR);		// 清除缓存内容

	return TRUE;
}
/*************************************************************************************************
* Function Name  : close_com.
* Description    : 关闭串口.
* Input          : hcom    - 设备句柄.
* Return         : BOOL    - TRUE - 配置成功 ，FALSE - 配置失败.
**************************************************************************************************/
BOOL close_com(HANDLE hcom)
{
	if(hcom == INVALID_HANDLE_VALUE) return FALSE;

	CloseHandle(hcom);

	return TRUE;
}
/*************************************************************************************************
* Function Name  : com_send_data.
* Description    : 发送数据.
* Input          : hcom    - 设备句柄.
*                ：buf     - 发送数据缓存
*                ：len     - 发送数据长度
* Return         : BOOL    - TRUE - 配置成功 ，FALSE - 配置失败.
**************************************************************************************************/
BOOL com_send_data(HANDLE hcom,char* buf,DWORD len)
{
	BOOL ret = FALSE;
#ifdef  SYNC
	COMSTAT ComStat;										// 设备当前状态
	DWORD	ErrorFlags;										// 错误
	DWORD   WritenSize = 0;

	if(hcom == INVALID_HANDLE_VALUE) return FALSE;

	if(len == 0) return FALSE;

	ClearCommError(hcom,&ErrorFlags,&ComStat);				// 清除硬件的通讯错误以及获取通讯设备的当前状态

	ret = WriteFile(hcom, buf, len,&WritenSize,NULL);		// 写数据

	if(!ret)											    // 判断是否写成功
	{
		return FALSE;
	}
#else	
	if (hcom == INVALID_HANDLE_VALUE)	return FALSE;
	if (len == 0)						return FALSE;

	// OVERLAPPED: 异步输入输出的信息的结构体
	//           : 要求操作系统传送数据,并且在传送完毕时进行通知
	OVERLAPPED	EventWrite;										// 事件定义
	memset(&EventWrite,0,sizeof(OVERLAPPED));
	EventWrite.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);		// 创建一个事件

	COMSTAT		ComStat;										// 设备当前状态
	DWORD		ErrorFlags;										// 错误状态
	ClearCommError(hcom,&ErrorFlags,&ComStat);					// 清除硬件的通讯错误以及获取通讯设备的当前状态

	DWORD		dword;											// 错误
	ret = WriteFile(hcom, buf, len,&dword,&EventWrite);			// 写数据

	if(!ret)													// 判断是否写成功
	{
		if(GetLastError() == ERROR_IO_PENDING)
		{
			WaitForSingleObject(EventWrite.hEvent,1000);		// 超时等待
		}
	}
#endif
	return TRUE;
}
/*************************************************************************************************
* Function Name  : com_read_data.
* Description    : 读数据.
* Input          : hcom    - 设备句柄.
*                ：buf     - 发送数据缓存
*                ：len     - 发送数据长度
* Return         : BOOL    - TRUE - 配置成功 ，FALSE - 配置失败.
**************************************************************************************************/
BOOL com_read_data(HANDLE hcom,char *buf,DWORD len)
{
	BOOL ret = FALSE;
#ifdef SYNC
	DWORD   ReadSize = 0;

	if(hcom == INVALID_HANDLE_VALUE) return FALSE;

	if(!len)  return FALSE;

	ret = ReadFile(hcom, buf, len,&ReadSize,NULL);

	if(!ret) return FALSE;

#else 
	if(hcom == INVALID_HANDLE_VALUE)	return FALSE;
	if(len <= 0)						return FALSE;

	OVERLAPPED	EventRead;
	memset(&EventRead,0,sizeof(OVERLAPPED));
	EventRead.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	DWORD		ErrorFlags;
	COMSTAT		ComStat;									// 设备当前状态
	ClearCommError(hcom,&ErrorFlags,&ComStat);				// 清除硬件的通讯错误以及获取通讯设备的当前状态

	DWORD		dword;
	ret = ReadFile(hcom, buf, len,&dword,&EventRead);
	if(!ret)											   
	{													  
		if(GetLastError()==ERROR_IO_PENDING)
		{
			WaitForSingleObject(EventRead.hEvent,2000);
		}	
	}		

	PurgeComm(hcom,PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
#endif
	return ComStat.cbInQue;
}




