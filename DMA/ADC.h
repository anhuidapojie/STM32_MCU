#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"
#include "stdio.h"

extern uint16_t DMA_Buf[3];

/*
 * @brief  DMA配置函数
 * @param  无
 * @retval 无
*/
void DMA_Config(void);
