#include "Driver/MainUart.h"
#include <stdio.h>
#include <stdarg.h>


static uint64_t RecvBuffer[512 / 8];
static uint64_t RecvSingleBuffer[64 / 8];

static MainEvent MsgRecv;

/**
 * @brief MainUart Init
 * TX <-> PC10 <-> UART5_TX <-> AF8
 * RX <-> PC11 <-> UART5_RX <-> AF8 <-> DMA1_Stream2 Channel4
 */
MainUart::MainUart(void) : 
	Sender("Main"), 
	Receiver("Main", qMqMem, MAIN_OBJ_NUMBER, MAIN_OBJ_SIZE),
	BaseOutput(aBufferMemory)
{
	RCC->AHB1ENR |= 0x04;
	
	GPIOC->MODER &= ~0xF00000;
	GPIOC->MODER |= 0xA00000;
	GPIOC->PUPDR &= ~0xF00000;
	GPIOC->PUPDR |= 0x500000;
	GPIOC->OSPEEDR |= 0xF00000;
	GPIOC->OTYPER &= ~0xC00;
	GPIOC->AFR[1] &= ~0xFF00;
	GPIOC->AFR[1] |= 0x8800;
	
	
	/**
	 * @brief 
	 */
	RCC->APB1ENR |= 0x80000;
	
	UART4->CR1 = 0x00;
	UART4->CR2 = 0x00;
	UART4->CR3 = 0x40;
	UART4->BRR = 42000000 / 9600;
	UART4->CR1 = 0x201C;
	
	RCC->AHB1ENR |= 0x200000; // DMA1
	
	DMA1->LIFCR = 0x3F0000;
	DMA1_Stream2->CR = 0x00;
	DMA1_Stream2->M0AR = (uint32_t) &RecvBuffer;
	DMA1_Stream2->PAR = (uint32_t) &UART4->DR;
	DMA1_Stream2->NDTR = sizeof(RecvBuffer);
	DMA1_Stream2->CR = 	(0x04 << 25) | // ????
						(0x00 << 23) | // ???????
						(0x00 << 21) | // ??????
						(0x00 << 19) | // ??M0
						(0x00 << 18) | // ???????
						(0x02 << 16) | // ????
						(0x00 << 15) | // ??????PSIZE??
						(0x00 << 13) | // ??????8?
						(0x00 << 11) | // ?????8?
						(0x01 << 10) | // ???????????????
						(0x00 << 9) | // ???????
						(0x01 << 8) | // ????
						(0x00 << 6) | // ??????: ??????
						(0x00 << 5) | // ???????
						(0x01 << 0) ; // ??DMA
	NVIC_EnableIRQ(UART4_IRQn);
}

void MainUart::write(uint8_t byte)
{
	while (0 == (UART4->SR & 0x80));
	UART4->DR = byte;
}

void MainUart::write(const uint8_t *byteArray, uint16_t len)
{
	acquire();
	while (len --)
	{
		write(*byteArray);
		byteArray ++;
	}
	release();
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


extern "C" void UART4_IRQHandler(void)
{
	static uint32_t lastIndex;
	uint32_t index;
	if (0 == (UART4->SR & 0x10)) {
		return ;
	}
	index = UART4->DR;
	index = sizeof(RecvBuffer) - DMA1_Stream2->NDTR;
	if (index > lastIndex) {
		MsgRecv.set((const uint8_t *) (RecvBuffer) + lastIndex, index - lastIndex);
	} else {
		vBufferOver(index, lastIndex);
	}
	MainUart::instance().put(&MsgRecv);
	lastIndex = index;	
}





