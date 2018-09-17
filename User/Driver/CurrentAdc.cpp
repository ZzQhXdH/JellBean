#include "Driver/CurrentAdc.h"

static uint16_t AdcValue;

/**
 * @brief PA0 <=> ADC
 */
CurrentAdc::CurrentAdc(void)
{
	RCC->AHB1ENR |= 0x01;
	RCC->APB2ENR |= 0x100; // ADC1
	
	GPIOA->MODER |= 0x03; // PA0 模拟模式
	GPIOA->PUPDR &= ~0x03;
	GPIOA->PUPDR |= 0x02;
	
	RCC->AHB1ENR |= 0x400000; // 启动DMA2 Stream0 Channel 0
	DMA2->LIFCR = 0x3F;
	DMA2_Stream0->CR = 0x00;
	DMA2_Stream0->M0AR = (uint32_t) &AdcValue;
	DMA2_Stream0->NDTR = 1;
	DMA2_Stream0->PAR = (uint32_t) &ADC1->DR;
	DMA2_Stream0->CR = 0x2901;
	
	RCC->APB2RSTR |= 0x100;
	RCC->APB2RSTR &= ~0x100;
	ADC->CCR = 0x03 << 16; // CLK = 21MHz
	ADC1->CR1 = 0x00; // 12Bit 非扫描模式
	ADC1->CR2 = 0x02 | 0x300; // 连续转换模式, DMA开启
	ADC1->SQR1 &= ~(0x0F << 20); // 1个转换序列
	ADC1->SMPR2 |= 0x07; // 通道1 480个周期
	ADC1->CR2 |= 0x01; // 开启AD
	ADC1->SQR3 = 0x00;
	ADC1->CR2 |= (0x01 << 30); // 开始转换
}

uint16_t CurrentAdc::xGetValue(void)
{
	return AdcValue;
}







