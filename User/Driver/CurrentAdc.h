#ifndef		__CURRENT_ADC_H
#define		__CURRENT_ADC_H

#include "stm32f4xx.h"

class CurrentAdc
{
	public:
		static CurrentAdc &instance(void)
		{
			static CurrentAdc adc;
			return adc;
		}
		
		uint16_t xGetValue(void);
		
	private:
		CurrentAdc(void);
};





#endif
