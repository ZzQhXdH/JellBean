#ifndef		__EVENT_H
#define		__EVENT_H

#include "Task/Task.h"


class Event
{
	public:
		Event(void);
		
		Event(const uint8_t *byteArray, uint16_t len);
		
		inline void set(const uint8_t *byteArray, uint16_t len) {
			mByteArray = byteArray;
			mLength = len;
		}
	
		inline uint8_t arg(uint16_t index) const {
			return mByteArray[index + 2];
		}
	
		inline const uint8_t *data(void) const {
			return mByteArray;
		}
		
		inline uint16_t length() const {
			return mLength;
		}
		
		inline uint8_t type() const {
			return mByteArray[2];
		}
		
		uint8_t xorCheckSum(void);
		
		void getAck(uint8_t *byteArray);
		
		bool isCorrect(void);
		
	private:
		const uint8_t *mByteArray;
		uint16_t mLength;
};



#endif
