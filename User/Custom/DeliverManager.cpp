#include "Custom/DeliverManager.h"



void DeliverManager::deliver(const DeliverObject &obj, DeliverListener *listener)
{
	MotorManager &manager = MotorManager::instance();
	manager.vElectorLockOff();
	manager.xRotationToClose();
	manager.xStepToOrigin(); // 回原点
	manager.xPushNext();
	manager.xDropOut();
	
	if (obj.number1() > 0)
	{
		manager.xStepToPosition1();
		for (uint8_t i = 0; i < obj.number1(); i ++) // 出第一种糖豆
		{
			manager.xPowerBox1Toggle();
			osDelay(1000);
		}
	}

	if (obj.number2() > 0)
	{
		manager.xStepToPosition2();
		for (uint8_t i = 0; i < obj.number2(); i ++)
		{
			manager.xPowerBox2Toggle();
			osDelay(1000);
		}
	}

	if (obj.number3() > 0)
	{
		manager.xStepToPosition3();
		for (uint8_t i = 0; i < obj.number3(); i ++) 
		{
			manager.xPowerBox3Toggle();
			osDelay(1000);
		}
	}

	if (obj.number4() > 0)
	{
		manager.xStepToPosition4();
		for (uint8_t i = 0; i < obj.number4(); i ++) 
		{
			manager.xPowerBox4Toggle();
			osDelay(1000);
		}
	}

	if (obj.number5() > 0)
	{
		manager.xStepToPosition5();
		for (uint8_t i = 0; i < obj.number5(); i ++) 
		{
			manager.xPowerBox5Toggle();
			osDelay(1000);
		}
	}

	manager.xStepForMm(298);
	manager.xYAxisToDown();
	manager.xStepForMm(150);
	manager.xStepToOrigin();
	manager.xRotationToOpen();
}


DeliverManager::DeliverManager(void) 
{
}
















