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
	
		void testStepMotor(void); // 步进电机测试
		
		void testDropMotor(void); // 落杯电机测试
		
		void testPushMotor(void); // 推杯电机测试
	
		void testRotationMotor(void); // 旋转电机测试
	
		void testYAxisMotor(void); // Y轴电机测试
	
		void testPowerBoxMotor(void); // 动力盒电机测试
	
		void testElectorLock(void); // 电磁锁测试
	
		void testDeliver(void);
		
};




#endif
