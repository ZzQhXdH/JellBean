#ifndef		__LOG_EVENT_H
#define		__LOG_EVENT_H

#include "Event/Event.h"

class LogEvent : public Event
{
	public:
		enum {
			ErrorLength = 0x05
		};
	
	public:
		const uint8_t *error(void);
	
		void test(void);
	
		void testStepMotor(void); // �����������
		
		void testDropMotor(void); // �䱭�������
		
		void testPushMotor(void); // �Ʊ��������
	
		void testRotationMotor(void); // ��ת�������
	
		void testYAxisMotor(void); // Y��������
	
		void testPowerBoxMotor(void); // �����е������
	
		void testElectorLock(void); // ���������
	
		void testDeliver(void);
		
};




#endif
