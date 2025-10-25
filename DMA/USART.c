#include "USART.h"

void USART1_Config(uint32_t brrVal)
{
	//1）时钟使能 GPIOA USART1
	RCC->APB2ENR |= (1 << 2)|(1 << 14);
	//2）GPIO模式配置
	/*
		PA9 	接 USART1_TX  复用推挽输出
		PA10 	接 USART_RX 	浮空输入
	*/
	GPIOA->CRH &= ~(0XFF << 4);
	GPIOA->CRH |= (0X4B << 4);
	//3）USART模式配置
	/*
		8-0-1协议
		波特率
		单工or双工
			对USART的配置
	*/
		//8-0-1协议
	USART1->CR1 &= ~(1 << 12);//一个起始位，8个数据位
	USART1->CR1 &= ~(1 << 10);//无校验
	USART1->CR2 &= ~(0X3 << 12);//停止位1位
		//全双工
	USART1->CR1 |= (1 << 3);//使能发送器
	USART1->CR1 |= (1 << 2);//使能接收器
		//波特率
	USART1->BRR = 72000000 / brrVal;
	//4）USART使能
	USART1->CR1 |= (1 << 13);
}

void USART1_SendByte(uint8_t byte)
{
    while ((USART1->SR & (1 << 6)) == 0); //等待发送缓冲区为空
    USART1->DR = byte; //发送数据
}

void USART_SendString(char* str)
{
    while (*str != '\0')
    {
        USART1_SendByte((uint8_t)(*str));
        str++;
    }
}

void USART_SendBuf(char *str, uint32_t len)
{
    while (len--)
    {
        USART1_SendByte((uint8_t)(*str));
        str++;
    }
}

int fputc(int c, FILE* stream)
{
    USART1_SendByte(c);
    return c;
}

//阻塞型接收
uint8_t USART_GetByte(void)
{
    while ((USART1->SR & (1 << 5)) == 0); //等待接收缓冲区非空
    return USART1->DR; //返回接收到的数据
}