#include "Event/Event.h"




Event::Event(void) : mByteArray(NULL), mLength(0)
{
	
}

Event::Event(const uint8_t *byteArray, uint16_t len) : mByteArray(byteArray), mLength(len)
{
	
}

void Event::getAck(uint8_t *byteArray)
{
	byteArray[0] = 0xE1;
	byteArray[1] = 6;
	byteArray[2] = type() | 0x80;
	byteArray[3] = 0;
	byteArray[4] = 0;
	byteArray[5] = 0xEF;
}

uint8_t Event::xorCheckSum(void)
{
	uint8_t check = 0;
	uint16_t count = mLength - 5;
	const uint8_t *byteArray = mByteArray + 3;
	for (uint16_t i = 0; i < count; i ++) {
		check ^= byteArray[i];
	}
	return check;
}

bool Event::isCorrect(void)
{
	const uint8_t *byteArray = mByteArray;
	uint16_t size = mLength;
		
	return  (0xE1 == byteArray[0]) &&
			(0xEF == byteArray[size - 1]) &&
			(byteArray[1] == size) &&
			(byteArray[size - 2] == xorCheckSum() );
}

