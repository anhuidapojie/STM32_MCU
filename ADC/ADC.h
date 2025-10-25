#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"
#include "stdio.h"

/*
@brief 光敏电阻配置
@param 无
@return 无
*/
void Light_Config(void);

/**
 * @brief 获取光敏电阻数据
 * @param 无
 * @return 光敏电阻数据(0~4095)
 */
uint16_t Light_GetData(void);
