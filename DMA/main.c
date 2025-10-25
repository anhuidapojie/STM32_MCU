#include "stdio.h"
#include "stm32f10x.h"
#include "usart1.h"
#include "ADC.h"
#include "Delay.h"

int main(void)
{
	USART1_Config(115200);
	DMA_Config();
	
	printf("printf demo\r\n");

	while(1)
	{
		printf("光敏的AD值：%d\r\n",DMA_Buf[1]);
		printf("热敏的AD值：%d\r\n",DMA_Buf[0]);
		printf("电位器的AD值：%d\r\n",DMA_Buf[2]);
		Delay_nopnms(1000);
	}
}