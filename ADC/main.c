#include "stm32f10x.h"
#include "ADC.h"
#include "Delay.h"
#include "stdio.h"

int mian(void)
{
    USART_Config(115200); //初始化串口
    Light_Config();       //初始化光敏电阻ADC
    printf("Light ADC Test\r\n");

    uint16_t ADvalue = 0; //存储ADC值
    float TempV = 0;   //存储电压值
    float TempR = 0;   //存储电阻值
    while (1)
    {
        ADvalue = Light_GetData(); //ADvalue = ADC3->DR;
        printf("ADvalue=%d ", ADvalue);
        TempV = (float)ADvalue * 3.3 / 4096; //计算电压值
        printf("Voltage=%.2fV ", TempV);
        TempR = (TempV * 36000) / (3.3 - TempV); //计算电阻值
        printf("Resistance=%.2fOhm\r\n", TempR);
        Delay_nopnms(1000); //延时1秒
    }
}