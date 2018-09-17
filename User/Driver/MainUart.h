#ifndef		__LOG_UART_H
#define		__LOG_UART_H

#include "stm32f4xx.h"
#include "Event/MainEvent.h"
#include "Custom/BaseOutput.h"
#include "Event/Receiver.h"
#include "Event/Sender.h"


#define MAIN_OBJ_NUMBER				sizeof (MainEvent)
#define MAIN_OBJ_SIZE				20
#define MAIN_SIZE					osRtxMessageQueueMemSize(MAIN_OBJ_NUMBER, MAIN_OBJ_SIZE)


class MainUart : public Sender, public Receiver, public BaseOutput 
{
	public:
		static MainUart &instance(void) 
		{
			static MainUart uart;
			return uart;
		}
	
		void write(uint8_t byte);
		
		virtual void write(const uint8_t *byteArray, uint16_t len);
		
	private:
		MainUart(void);
	
	private:
		uint64_t qMqMem[ (MAIN_SIZE + 7) / 8 ];
		uint8_t aBufferMemory[150];
};



#endif
