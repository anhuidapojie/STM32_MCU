#include "ADC.h"
#include "Delay.h"

void DMA_Config(void)
{
	//1)时钟使能--GPIOF ADC3
	RCC->APB2ENR |= (1 << 7);
	RCC->APB2ENR |= (1 << 15);
	//2)GPIO模式配置--PF6\7\8 模拟输入
	GPIOF->CRL &= ~((uint32_t)0XFF << 24);
	GPIOF->CRH &= ~(0XF << 0);
	//3)ADC配置
	RCC->CFGR &= ~(0X3 << 14);
	RCC->CFGR |= (0X2 << 14);//6分频，ADCCLK=12MHz
	
	ADC3->SQR1 &= ~(0XF << 20);
	ADC3->SQR1 |= (0X2 << 20);//规则通道3个转换
	ADC3->SQR3 &= ~(0X1F << 0);
	ADC3->SQR3 |= (4 << 0);//第一个转换通道位IN4--热敏元件
	ADC3->SQR3 &= ~(0X1F << 5);
	ADC3->SQR3 |= (6 << 5);//第二个转换通道位IN6--光敏元件
	ADC3->SQR3 &= ~(0X1F << 10);
	ADC3->SQR3 |= (5 << 10);//第三个转换通道位IN5--电位器
	
	//现在对于我们而言采样不重要
	ADC3->SMPR2 |= (0X7 << 18);//IN6 采样Tconv = 239.5(配置) + 12.5(固定)
	
	//ADC3->CR1 &= ~(0XF << 16);//ADC独立模式--ADC3没有此位
	ADC3->CR1 |= (1 << 8);//开启扫描模式
	
	//软件触发--software start SWSTART
	ADC3->CR2 |= (0X1 << 20);//打开外部触发（SWSATRT放到了外部触发中）
	ADC3->CR2 |= (0X7 << 17);//选择SWSTART
	ADC3->CR2 |= (0X1 << 22);//打开SWSTART
	
	ADC3->CR2 &= ~(1 << 11);//数据右对齐
	ADC3->CR2 |= (1 << 1);//连续模式
	//4)ADC自校准
	ADC3->CR2 &= ~(1 << 0);//关闭ADC
	Delay_nopnms(10);
	ADC3->CR2 |= (1 << 0);//打开ADC
	
	ADC3->CR2 |= (1 << 3);//启动复位校准
	while((ADC3->CR2 & (1 << 3)));//等待校准结束
	ADC3->CR2 |= (1 << 2);//启动AD校准
	while((ADC3->CR2 & (1 << 2)));//等待校准结束
	
	//5)ADC使能
	ADC3->CR2 |= (1 << 0);//打开ADC

  RCC->AHBENR |= (1 << 1); //使能DMA2时钟
  //DMA2通道配置
  DMA2_Channel5->CNDTR = 3; //传输数据量3个
  DMA2_Channel5->CPAR = (uint32_t)(&ADC3->DR); //外设地址
  DMA2_Channel5->CMAR = (uint32_t)DMA_Buf; //存储器地址
  DMA2_Channel5->CCR = 0x5A1; //配置寄存器
  //使能DMA2
  //在第2不的0X5A1中已经有了
}
