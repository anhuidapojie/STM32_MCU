#include "RS485.h"
_RS485_Data RS485Data = {.flag=0}; //RS485数据结构体实例化

void RS485_Config(uint32_t brr)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0}; //GPIO初始化结构体
    USART_InitTypeDef USART_InitStructure = {0}; //USART初始化结构体
    NVIC_InitTypeDef NVIC_InitStructure = {0}; //NVIC初始化结构体

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //使能GPIOG时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE); //使能GPIOG时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //使能USART3时钟

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //USART3 发送引脚TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
    GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化GPIOB10
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //USART3 接收引脚RX
    GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化GPIOB11

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //PG8作为485收发控制引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
    GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化GPIOG8
    RS485_RE(RS485_Recv); //RS485默认设置为接收模式

    USART_InitStructure.USART_BaudRate = brr; //波特率设置
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
    USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //1位停止位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8位数据长度
    USART_Init(RS485_USART,&USART_InitStructure); //初始化USART3

    USART_ITConfig(RS485_USART,USART_IT_RXNE,ENABLE); //使能接收中断
    USART_ITConfig(RS485_USART,USART_IT_IDLE,ENABLE); //使能空闲中断

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //USART3中断通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //子优先级1
    NVIC_Init(&NVIC_InitStructure); //初始化NVIC

    USART_Cmd(RS485_USART,ENABLE); //使能USART3
}

void RS485_SendBuf(uint8_t *buf,uint32_t len)
{
    uint32_t i = 0;
    RS485_RE(RS485_Send); //设置为发送模式
    while (USART_GetFlagStatus(RS485_USART,USART_FLAG_TC) == RESET); //等待发送完成
    for (i = 0; i < len; i++)
    {
        USART_SendData(RS485_USART,buf[i]);
        while (USART_GetFlagStatus(RS485_USART,USART_FLAG_TC) == RESET); //等待发送完成
    }
    RS485_RE(RS485_Recv); //设置为接收模式
}

/*
代码思路：
RS485 配置 (RS485_Config 函数)：
GPIO 配置：
使能 GPIOB 和 GPIOG 的时钟，为 USART3 通信和 RS485 控制引脚提供时钟。
配置 GPIOB 引脚 Pin_10 为 推挽输出模式，作为 USART3 的发送引脚（TX）。
配置 GPIOB 引脚 Pin_11 为 浮空输入模式，作为 USART3 的接收引脚（RX）。
配置 GPIOG 引脚 Pin_8 为 推挽输出模式，用于控制 RS485 的收发模式（通常是 DE/RE 引脚，即控制发送和接收）。
RS485 默认设置为接收模式：通过调用 RS485_RE(RS485_Recv) 设置 RS485 默认工作在接收模式。

USART 配置：
设置 USART3 的波特率、数据格式（8位数据、1位停止位、无校验）、收发模式等。
配置 硬件流控制 为无，表示不使用硬件流控制（如 CTS/RTS）。

中断配置：
启用 USART3 的 接收中断 和 空闲中断。接收中断是在接收到数据时触发，空闲中断是在空闲时触发（例如没有数据传输时）。
配置 NVIC（中断控制器）来使能 USART3 的中断，设置优先级和子优先级。
使能 USART3：通过 USART_Cmd(RS485_USART, ENABLE) 启用 USART3，从而使得该模块能够正常工作。

数据发送函数 (RS485_SendBuf 函数)：
切换到发送模式：首先通过 RS485_RE(RS485_Send) 将 RS485 控制引脚切换为发送模式，允许设备进行数据发送。
等待 USART 发送完成：在发送数据之前，程序会等待 USART_FLAG_TC 标志位被置为设置状态，表示 USART 数据发送完成。
发送数据：通过 USART_SendData 函数发送数据，每发送一个字节，程序都会检查是否发送完成，直到所有数据发送完毕。
切换回接收模式：数据发送完成后，通过 RS485_RE(RS485_Recv) 将 RS485 控制引脚切换回接收模式，允许接收后续的数据。
*/