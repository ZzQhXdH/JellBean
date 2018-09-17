#ifndef		__DELIVER_OBJECT_H
#define 	__DELIVER_OBJECT_H

#include <stdint.h>

class DeliverObject
{
	public:
		DeliverObject(void);
	
		DeliverObject(uint8_t n1, uint8_t n2, uint8_t n3, uint8_t n4, uint8_t n5);
	
		inline uint8_t number1(void) const {
			return mNumber1;
		}
		
		inline uint8_t number2(void) const {
			return mNumber2;
		}
		
		inline uint8_t number3(void) const {
			return mNumber3;
		}
		
		inline uint8_t number4(void) const {
			return mNumber4;
		}
		
		inline uint8_t number5(void) const {
			return mNumber5;
		}
	
	private:
		uint8_t mNumber1;
		uint8_t mNumber2;
		uint8_t mNumber3;
		uint8_t mNumber4;
		uint8_t mNumber5;
};









#endif
