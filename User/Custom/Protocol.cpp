#include "Custom/Protocol.h"
#include "Event/Event.h"

namespace Protocol {
	
	uint8_t getXorCheckSum(const uint8_t *byteArray, uint16_t count)
	{
		uint8_t check = 0;
		for (uint16_t i = 0; i < count; i ++) {
			check ^= byteArray[i];
		}
		return check;
	}
	
	bool isCorrect(const uint8_t *byteArray, uint16_t size)
	{
		return  (0xE1 == byteArray[0]) &&
				(0xEF == byteArray[size - 1]) &&
				(byteArray[1] == size) &&
				(byteArray[size - 2] == getXorCheckSum(byteArray + 3, size - 5) );
	}
	
	bool isCorrect(const Event &env)
	{
		const uint8_t *byteArray = env.data();
		uint16_t size = env.length();
		
		return  (0xE1 == byteArray[0]) &&
				(0xEF == byteArray[size - 1]) &&
				(byteArray[1] == size) &&
				(byteArray[size - 2] == getXorCheckSum(byteArray + 3, size - 5) );
	}
	
	void xCreateCustom(uint8_t *byteArray, uint8_t type, const uint8_t *args, uint16_t argCount)
	{
		byteArray[0] = 0xE1;
		byteArray[1] = argCount + 5;
		byteArray[2] = type;
		
		uint8_t check = 0;
		for (uint16_t i = 0; i < argCount; i ++)
		{
			byteArray[3 + i] = args[i];
			check ^= args[i];
		}
		byteArray[argCount + 3] = check;
		byteArray[argCount + 4] = 0xEF;
	}
	
	void xCreateAck(uint8_t *byteArray, uint8_t type)
	{
		byteArray[0] = 0xE1;
		byteArray[1] = 6;
		byteArray[2] = type;
		byteArray[3] = 0;
		byteArray[4] = 0;
		byteArray[5] = 0xEF;
	}
}










