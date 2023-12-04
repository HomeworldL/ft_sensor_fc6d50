/*************************************************************************************************
* Description:	com support head file
* Author:		xiaofei.huang
* Versions:		V1.0
* Date:         2017-07-06
* Modifiy:
* CO.ltd:
* Other:
*************************************************************************************************/
#ifndef _COMM_H_ 
#define _COMM_H_ 

#ifdef __cplusplus
extern "C" {
#endif 

#include "WTypes.h"

//#define SYNC									// 定义串口工作在同步方式，不定义该宏则工作在异步方式
//------------------------------------端口操作------------------------------------------------------
HANDLE	open_com		(LPCWSTR com);			// 打开设备，nPORT - 串口号

BOOL	config_com		(						// 配置串口参数
						HANDLE hcom,			// hCOM  - 设备句柄,
						DWORD  bufsize,			// nBUFF -缓存大小
						DWORD  baud);			// nBAUD -波特率

BOOL	close_com		(HANDLE hCOM);			// 关闭设备，hCOM - 设备句柄

BOOL	com_send_data	(						// 发送数据
						HANDLE hcom,			// hCOM    - 设备句柄,
						char*  buf,				// SendBuf - 发送数据缓存
						DWORD  len);			// SendLen - 发送数据长度

BOOL	com_read_data	(						// 读取数据
						HANDLE hcom,			// hCOM    - 设备句柄
						char*  buf,				// ReadBuf - 接受缓存
						DWORD  len);			// ReadLen - 接收缓存大小



#ifdef __cplusplus
}
#endif

#endif









