/*************************************************************************************************
* Description:	delay us support file
* Author:		xiaofei.huang
* Versions:		V1.0
* Date:         2017-07-06
* Modifiy:
* CO.ltd:
* Other:
*************************************************************************************************/
#include"delay.h"
#include "stdafx.h"
/*************************************************************************************************
* Function Name  : Delayus.
* Description    : 微妙级延时函数.
* Input          : time   - 延时参数，以秒为单位，最小设置值 0.000001秒 = 1微妙.
*                ：buf     - 发送数据缓存.
*                ：len     - 发送数据长度.
* Return         : none.
**************************************************************************************************/
void Delayus(double time)
{
	LARGE_INTEGER  litmp;
	LONGLONG       QPart1, QPart2;;
	double         dfMinus, dfFreq, dfTim;

	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;		// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;				// 获得初始值
	do {
		QueryPerformanceCounter(&litmp);
		QPart2 = litmp.QuadPart;			// 获得中止值 
		dfMinus = (double)(QPart2 - QPart1);
		dfTim = dfMinus / dfFreq;			// 获得对应的时间值，单位为秒
	} while (dfTim < time);
}



