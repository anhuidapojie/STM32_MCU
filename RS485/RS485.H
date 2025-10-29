#ifndef __RS485_H__
#define __RS485_H__

#include "stm32f10x.h"
#include "stdio.h"

#define RS485_USART USART3 //RS485使用的串口
#define RS485_RE(x) GPIO_WriteBit(GPIOG,GPIO_Pin_8,(BitAction)(x)) //用于控制485收发方向

typedef enum
{
    RS485_Recv=0, 
    RS485_Send
}_RS485_Mode; //RS485工作模式

typedef struct
{
    uint8_t buf[127];  //RS485数据缓存区
    uint8_t flag;      //RS485数据接收完成标志
    uint32_t count;    //RS485数据接收计数
}_RS485_Data; //RS485数据结构体

/*
@brief  RS485串口配置函数
@param  brr: 波特率设置值
@return 无
*/
void RS485_Config(uint32_t brr);

/*
@brief  RS485发送数据函数
@param  buf: 发送数据缓存区
@param  len: 发送数据长度
*/
void RS485_SendBuf(uint8_t *buf,uint32_t len); 