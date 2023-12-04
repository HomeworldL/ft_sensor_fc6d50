/*************************************************************************************************
* Description:	delay us support head file
* Author:		xiaofei.huang
* Versions:		V1.0
* Date:         2017-07-06
* Modifiy:
* CO.ltd:
* Other:
*************************************************************************************************/
#ifndef _DELAY_H_ 
#define _DELAY_H_ 

#ifdef __cplusplus
extern "C" {
#endif _

// #include "WTypes.h"

void Delayus(double time);	// 微秒级延时函数 time - 延时参数 最小可设置：0.000001秒 = 1微妙

#ifdef __cplusplus
}
#endif

#endif


