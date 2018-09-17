#include "Driver/LogUart.h"
#include <stdio.h>
#include <stdarg.h>

static uint64_t RecvBuffer[512 / 8];
static uint64_t RecvSingleBuffer[64 / 8];

static LogEvent MsgRecv;

LogUart::LogUart(void) : Sender("Log"), Receiver("Log", qMqMem, LOG_UART_OBJ_NUMBER, LOG_UART_OBJ_SIZE)
{
	RCC->AHB1ENR |= 0x02; // Enable GpioB
	RCC->APB1ENR |= 0x40000; // Enable USART3
	RCC->AHB1ENR |= 0x200000; // Enable DMA1
	
	GPIOB->MODER &= ~0xF00000;
	GPIOB->MODER |= 0xA00000;
	
	GPIOB->PUPDR &= ~0xF00000;
	GPIOB->PUPDR |= 0x500000;
	
	GPIOB->OSPEEDR |= 0xA00000;
	
	GPIOB->OTYPER &= ~0xC00;
	
	GPIOB->AFR[1] &= ~0xFF00;
	GPIOB->AFR[1] |= 0x7700;
	
	USART3->CR1 = 0x00;
	USART3->CR2 = 0x00;
	USART3->CR3 = 0x40;
	USART3->BRR = 42000000 / 9600;
	USART3->CR1 = 0x201C;
	
	DMA1->LIFCR = 0xFC0;
	DMA1_Stream1->CR = 0x00;
	DMA1_Stream1->M0AR = (uint32_t) &RecvBuffer;
	DMA1_Stream1->PAR = (uint32_t) &USART3->DR;
	DMA1_Stream1->NDTR = sizeof (RecvBuffer);
	DMA1_Stream1->CR = 	(0x04 << 25) | 
						(0x00 << 23) | 
						(0x00 << 21) | 
						(0x00 << 19) | 
						(0x00 << 18) | 
						(0x02 << 16) | 
						(0x00 << 15) | 
						(0x00 << 13) | 
						(0x00 << 11) | 
						(0x01 << 10) | 
						(0x00 << 9) | 
						(0x01 << 8) |
						(0x00 << 6) |
						(0x00 << 5) | 
						(0x01 << 0) ; 
	NVIC_EnableIRQ(USART3_IRQn);	
}

void LogUart::write(uint8_t byte) 
{
	while (0 == (USART3->SR & 0x80));
	USART3->DR = byte;
}

void LogUart::write(const uint8_t *byteArray, uint32_t len)
{
	acquire();
	while (len --) {
		write(*byteArray ++);
	}
	release();
}

void LogUart::write(const char *format, ...)
{
	static char buffer[100];
	va_list ap;
	
	va_start(ap, format);
	int len = vsprintf(buffer, format, ap);
	write((const uint8_t *) buffer, len);
}

static inline void vBufferOver(uint32_t index, uint32_t lastIndex)
{
	uint32_t k = 0;
	uint8_t *p1 = (uint8_t *) RecvSingleBuffer;
	uint8_t *p2 = (uint8_t *) RecvBuffer;
	for (uint32_t i = lastIndex; i < sizeof (RecvBuffer); i ++, k ++) {
		p1[k] = p2[i];
	}
	for (uint32_t i = 0; i < index; i ++, k ++) {
		p1[k] = p2[i];
	}
	MsgRecv.set(p1, k);
}

extern "C" void USART3_IRQHandler(void)
{
	static uint32_t lastIndex;
	uint32_t index;
	if (0 == (USART3->SR & 0x10)) {
		return ;
	}
	index = USART3->DR;
	index = sizeof(RecvBuffer) - DMA1_Stream1->NDTR;
	if (index > lastIndex) {
		MsgRecv.set((const uint8_t *) (RecvBuffer) + lastIndex, index - lastIndex);
	} else {
		vBufferOver(index, lastIndex);
	}
	LogUart::instance().put(&MsgRecv);
	lastIndex = index;
}












