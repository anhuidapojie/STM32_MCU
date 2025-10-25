#ifndef _DELAY_H_
#define _DELAY_H_

#include "stm32f10x.h"

#define Delay_nopnms(x) Delay_nopnus(x*1000);
/*
@brief 微秒级延时
@param timers 微秒数
*/
void Delay_nopnus(uint32_t timers);

#endif