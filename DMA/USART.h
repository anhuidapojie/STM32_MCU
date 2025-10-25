#ifndef _USART1_H_
#define _USART1_H_

#include "stm32f10x.h"
#include "stdio.h"

/*
@brief USART1配置
@param brrVal 波特率设置值
 */
void USART1_Config(uint32_t brrVal);

/*
@brief 发送一个字节数据
@param byte 要发送的字节
 */
void USART1_SendByte(uint8_t byte);

/*
@brief 发送一个字符串
@param str 要发送的字符串
 */
void USART_SendString(char *str);

/*
@brief 发送指定长度的数据
@param str 要发送的数据
@param len 要发送的数据长度
 */
void USART_SendBuf(char *str,uint32_t len);

/*
@brief 接收一个字节数据
@return 接收到的字节数据
 */
uint8_t USART_GetByte(void);

#endif