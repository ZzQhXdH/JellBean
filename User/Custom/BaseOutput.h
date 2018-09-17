#ifndef		__BASE_OUTPUT_H
#define		__BASE_OUTPUT_H

#include <stdint.h>

class BaseOutput
{
	public:
		BaseOutput(uint8_t *pBuffer) : pByteArrayBuffer(pBuffer) {}
	
		virtual void write(const uint8_t *byteArray, uint16_t length) = 0;
		
		void format(const char *format, ...);
		
		void buildAndWrite(void);
		
		inline BaseOutput &setAction(uint8_t action)
		{
			mAction = action;
			nIndex = 0;
			return *this;
		}
		
		inline BaseOutput &append1(uint8_t arg)
		{
			pByteArrayBuffer[nIndex] = arg & 0x7F;
			nIndex ++;
			return *this;
		}
		
		inline BaseOutput &append2(uint16_t arg)
		{
			pByteArrayBuffer[nIndex] = (arg >> 7) & 0x7F;
			nIndex ++;
			pByteArrayBuffer[nIndex] = arg & 0x7F;
			nIndex ++;
			return *this;
		}
		
		inline BaseOutput &append4(uint32_t arg)
		{
			pByteArrayBuffer[nIndex] = (arg >> 21) & 0x7F;
			nIndex ++;
			pByteArrayBuffer[nIndex] = (arg >> 14) & 0x7F;
			nIndex ++;
			pByteArrayBuffer[nIndex] = (arg >> 7) & 0x7F;
			nIndex ++;
			pByteArrayBuffer[nIndex] = arg & 0x7F;
			nIndex ++;
			return *this;
		}
		
		void ack(void);
		
		void nck(void);
			
	private:
		uint16_t build(uint8_t *content);
			
	private:
		uint8_t *pByteArrayBuffer;
		uint16_t nIndex;
		uint8_t mAction;
};









#endif
