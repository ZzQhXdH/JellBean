#include "Event/LogEvent.h"
#include "Driver/Motor.h"
#include "Custom/DeliverManager.h"
#include "Driver/MotorManager.h"

const uint8_t ErrorResult[] = {
	0xE1, 0x05, 0x7F, 0x00, 0xEF,
};

const uint8_t *LogEvent::error(void) {
	return ErrorResult;
}

void LogEvent::test(void)
{
	uint8_t types = type();
	switch (types)
	{
		case 0x02: testStepMotor(); break;
		
		case 0x03: testDropMotor(); break;
		
		case 0x04: testPushMotor(); break;
		
		case 0x05: testRotationMotor(); break;
		
		case 0x06: testYAxisMotor(); break;
		
		case 0x07: testPowerBoxMotor(); break;
		
		case 0x08: testElectorLock(); break;
		
		case 0x09: testDeliver(); break;
	}
}

void LogEvent::testElectorLock(void)
{
	uint8_t arg1 = arg(1);
	switch (arg1) 
	{
		case 0x01: MotorManager::instance().vElectorLockOff(); break;
		case 0x02: MotorManager::instance().vElectorLockOn(); break;
	}
}

void LogEvent::testPowerBoxMotor(void)
{
	uint8_t arg1 = arg(1);
	switch (arg1)
	{
		case 0x01: MotorManager::instance().xPowerBox1Test(); break;
		case 0x02: MotorManager::instance().xPowerBox2Test(); break;
		case 0x03: MotorManager::instance().xPowerBox3Test(); break;
		case 0x04: MotorManager::instance().xPowerBox4Test(); break;
		case 0x05: MotorManager::instance().xPowerBox5Test(); break;
	}
}

void LogEvent::testYAxisMotor(void)
{
	uint8_t arg1 = arg(1);
	switch (arg1)
	{
		case 0x01: MotorManager::instance().xYAxisToUp(); break;
		case 0x02: MotorManager::instance().xYAxisToDown(); break;
	}	
}

void LogEvent::testRotationMotor(void)
{
	uint8_t arg1 = arg(1);
	switch (arg1)
	{
		case 0x01: Motor::instance().vRotateOff(); break;
		case 0x02: Motor::instance().vRotateOn(Motor::CCW); break;
		case 0x03: Motor::instance().vRotateOn(Motor::CW); break;
		case 0x04: MotorManager::instance().xRotationToOpen(); break;
		case 0x05: MotorManager::instance().xRotationToClose(); break;
	}	
}

void LogEvent::testPushMotor(void)
{
	uint8_t arg1 = arg(1);
	MotorManager::Error e;
	switch (arg1)
	{
		case 0x01: e = MotorManager::instance().xPushNext(); break;
		case 0x02: e = MotorManager::instance().xPushBack(); break;
	}
	
	if (e == MotorManager::NoCup)
	{
		MotorManager::instance().xPushBack();
	}
}

void LogEvent::testDropMotor(void)
{
	uint8_t arg1 = arg(1);
	switch (arg1)
	{
		case 0x01: MotorManager::instance().xDropOut(); break;
		case 0x02: Motor::instance().vDropOutOn(Motor::CCW); break;
		case 0x03: Motor::instance().vDropOutOn(Motor::CW); break;
		case 0x04: Motor::instance().vDropOutOff(); break;
	}
}

void LogEvent::testStepMotor(void)
{
	uint8_t arg1 = arg(1);
	switch (arg1)
	{
		case 0x01: MotorManager::instance().xStepToOrigin(); break;
		
		case 0x02: {
			uint32_t step = ( arg(2) << 21 ) | ( arg(3) << 14 ) | ( arg(4) << 7) | arg(5);
			MotorManager::instance().xStepForMm(step);
		} break;
		
		case 0x03: {
			uint32_t step = ( arg(2) << 21 ) | ( arg(3) << 14 ) | ( arg(4) << 7) | arg(5);
			MotorManager::instance().xStepForSteps(step);
		} break;
		
		case 0x04: {
			MotorManager::instance().xStepToPosition1();
		} break;
		
		case 0x05: {
			MotorManager::instance().xStepToPosition2();
		} break;
				
		case 0x06: {
			MotorManager::instance().xStepToPosition3();
		} break;
		
		case 0x07: {
			MotorManager::instance().xStepToPosition4();
		} break;
				
		case 0x08: {
			MotorManager::instance().xStepToPosition5();
		} break;
	}
}

void LogEvent::testDeliver(void)
{
	DeliverObject obj( arg(1), arg(2), arg(3), arg(4), arg(5) );
	DeliverManager::instance().deliver(obj, NULL);
}






