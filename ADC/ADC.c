#include "ADC.h"

void Light_Config(void)
{
    // 使能GPIOF8和ADC3时钟
    RCC->APB2ENR |=  (1 << 7);
    RCC->APB2ENR |=  (1 << 15);
    //GPIOF模式配置--PF8 模拟输入
    GPIOF->CRH &= ~(0XF << 0);

    //ADC配置
    /*
      参考电压-硬件设计
      ADCCLK=72MHz/6=12MHz <14MHz
      选择通道
      Tconv=1/ADCCLK*239=19.92us
      数据对齐-右对齐
    */
    /*PPRE2高速APB预分频(APB2)位13:11
    由软件置’1’或清’0’来控制高速APB2时钟(PCLK2)的预分频系数。
    */
    RCC->CFGR &= ~(0x3 << 14); //清除原有设置
    RCC->CFGR |=  (0x4 << 14); //PCLK2=HCLK/6

    ADC3->SQR1 &= ~(0xF << 20); //规则通道1个转换
    ADC3->SQR3 &= ~(0x1F << 0); //清除原有设置
    ADC3->SQR3 |= (6 << 0); //选择通道6(PF8)

    ADC3 ->SMPR2 |= (0x7 << 18); //IN6采样时间239.5(配置)周期+12.5(固定)
    //ADC3->CR1 |= (0xF << 16); //独立ADC模式--ADC3没有此位
    ADC3->CR1 &= ~(1 << 8); //关闭扫描模式

    //软件触发 --software start SWSTART
    ADC3->CR2 |= (0x1 << 20); //打开外部触发（SWSATRT放到了外部中断）
    ADC3->CR2 |= (0x7 << 17); //外部事件选择：SWSTART
    ADC3->CR2 |= (0x1 << 22); //打开SWSTART

    ADC3->CR2 &= ~(1 << 11); //数据右对齐
    ADC3->CR2 |= (1 << 1); //连续模式
    //ADC自校准
    ADC3->CR2 &= ~(1 << 0); //关闭ADC
    Delay_nopnms(10); //延时10ms
    ADC3->CR2 |= (1 << 0); //开启ADC

    ADC3->CR2 |= (1 << 3); //开启复位校准
    while(ADC3->CR2 & (1 << 3)); //等待校准完成
    ADC3->CR2 |= (1 << 2); //开启AD校准
    while(ADC3->CR2 & (1 << 2)); //等待校准完成

    //ADC使能
    ADC3->CR2 |= (1 << 0); //开启ADC
}

uint16_t Light_GetData(void)
{
    while ((ADC3—>SR & (1 << 1)) == 0); //等待上一次转换结束
    return ADC3->DR; //返回ADC数据
}