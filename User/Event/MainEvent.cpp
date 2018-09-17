#include "Event/MainEvent.h"
#include "Driver/MotorManager.h"

void MainEvent::exec(BaseOutput &out)
{
	if (!isCorrect())
	{
		out.nck();
		return;
	}
	out.ack();
	uint8_t action = type();
	switch (action)
	{
		case 0x09: // ��������
			deliver(out, arg(1), arg(2), arg(3), arg(4), arg(5));
			break;
		
		case 0x08: // ����������
			(arg(1) == 0x01) ? MotorManager::instance().vElectorLockOff() : 
							MotorManager::instance().vElectorLockOn();
			break;
	}
}

void MainEvent::deliver(BaseOutput &out, uint8_t n1, 
						uint8_t n2, uint8_t n3, 
						uint8_t n4, uint8_t n5)
{
	MotorManager &manager = MotorManager::instance();
	
	manager.vElectorLockOff(); // ��ֹ���ŵ�����

	MotorManager::Error e = manager.xRotationToClose(); // ����
	
	if (e != MotorManager::Ok) 
	{
		out.setAction(0x89).append1(0xFF).buildAndWrite();
		return;
	}
	
	e = manager.xStepToOrigin(); // ��ԭ��
	
	if (e != MotorManager::Ok) 
	{
		out.setAction(0x89).append1(0xFF).buildAndWrite();
		return;
	}
	
	bool ret = manager.isDropOutChecked(); // ���ϰ���, 
	
	if (ret)
	{
		e = manager.xYAxisToDown();
		if (e != MotorManager::Ok) 
		{
			out.setAction(0x89).append1(0xFF).buildAndWrite();
			return;
		}
		e = manager.xStepToPosition5();
		
		if (e != MotorManager::Ok) 
		{
			out.setAction(0x89).append1(0xFF).buildAndWrite();
			return;
		}		
		
		e = manager.xStepToOrigin();
		
		if (e != MotorManager::Ok) 
		{
			out.setAction(0x89).append1(0xFF).buildAndWrite();
			return;
		}
	}
	
	ret = manager.isDropOutChecked(); // ���ϰ���, 
	
	if (ret)
	{
		out.setAction(0x89).append1(0xFF).buildAndWrite();
		return;
	}
	
	e = manager.xPushNext(); // �Ʊ�
	
	if (e != MotorManager::Ok) 
	{
		out.setAction(0x89).append1(0xFF).buildAndWrite();
		
		if (e == MotorManager::NoCup)
		{
			manager.xPushBack();
		}
		
		return;
	}
	
	e = manager.xDropOut(); // �䱭
	
	if (e != MotorManager::Ok) 
	{
		out.setAction(0x89).append1(0xFF).buildAndWrite();
		return;
	}
	
	if (n1 > 0)
	{
		e = manager.xStepToPosition1();
		if (e != MotorManager::Ok) 
		{
			out.setAction(0x89).append1(0xFF).buildAndWrite();
			return;
		}
		
		for (uint8_t i = 0; i < n1; i ++) // ����һ���Ƕ�
		{
			e = manager.xPowerBox1Toggle();
			
			if (e != MotorManager::Ok) 
			{
				out.setAction(0x89).append1(0xFF).buildAndWrite();
				return;
			}
			
			osDelay(1000);
		}
	}

	if (n2 > 0)
	{
		e = manager.xStepToPosition2();
		
		if (e != MotorManager::Ok) 
		{
			out.setAction(0x89).append1(0xFF).buildAndWrite();
			return;
		}
		
		for (uint8_t i = 0; i < n2; i ++)
		{
			e = manager.xPowerBox2Toggle();
			
			if (e != MotorManager::Ok) 
			{
				out.setAction(0x89).append1(0xFF).buildAndWrite();
				return;
			}
			
			osDelay(1000);
		}
	}

	if (n3 > 0)
	{
		e = manager.xStepToPosition3();
		if (e != MotorManager::Ok) 
		{
			out.setAction(0x89).append1(0xFF).buildAndWrite();
			return;
		}
		
		for (uint8_t i = 0; i < n3; i ++) 
		{
			e = manager.xPowerBox3Toggle();
			if (e != MotorManager::Ok) 
			{
				out.setAction(0x89).append1(0xFF).buildAndWrite();
				return;
			}
			osDelay(1000);
		}
	}

	if (n4 > 0)
	{
		e = manager.xStepToPosition4();
		
		if (e != MotorManager::Ok) 
		{
			out.setAction(0x89).append1(0xFF).buildAndWrite();
			return;
		}	
		
		for (uint8_t i = 0; i < n4; i ++) 
		{
			e = manager.xPowerBox4Toggle();
			if (e != MotorManager::Ok) 
			{
				out.setAction(0x89).append1(0xFF).buildAndWrite();
				return;
			}
			osDelay(1000);
		}
	}

	if (n5 > 0)
	{
		e = manager.xStepToPosition5();
		
		if (e != MotorManager::Ok) 
		{
			out.setAction(0x89).append1(0xFF).buildAndWrite();
			return;
		}
		
		for (uint8_t i = 0; i < n5; i ++) 
		{
			e = manager.xPowerBox5Toggle();
			if (e != MotorManager::Ok) 
			{
				out.setAction(0x89).append1(0xFF).buildAndWrite();
				return;
			}
			osDelay(1000);
		}
	}

	e = manager.xStepForMm(298);
	
	if (e != MotorManager::Ok) 
	{
		out.setAction(0x89).append1(0xFF).buildAndWrite();
		return;
	}
	
	e = manager.xYAxisToDown();
	
	if (e != MotorManager::Ok) 
	{
		out.setAction(0x89).append1(0xFF).buildAndWrite();
		return;
	}
	
	e = manager.xStepForMm(150);
	
	if (e != MotorManager::Ok) 
	{
		out.setAction(0x89).append1(0xFF).buildAndWrite();
		return;
	}
	
	e = manager.xStepToOrigin();
	
	if (e != MotorManager::Ok) 
	{
		out.setAction(0x89).append1(0xFF).buildAndWrite();
		return;
	}
	
	e = manager.xRotationToOpen();
	
	if (e != MotorManager::Ok) 
	{
		out.setAction(0x89).append1(0xFF).buildAndWrite();
		return;
	}
	
	out.setAction(0x89).append1(0x00).buildAndWrite();

	osDelay(10 * 1000);
	
	/**
	 * @brief ���� ���Ҽ���Ƿ����, ����3��
	 */
	for (uint16_t i = 0; i < 3; i ++)
	{
		e = manager.xRotationToClose();
		if (e == MotorManager::Ok)
		{
			break;
		}
		manager.xRotationToOpen2();
		osDelay(1000);
	}
	
	if (e != MotorManager::Ok) // ����ʧ��
	{
		return;
	}

	ret = manager.isDropOutChecked(); // ���ϰ���, 
	
	if (ret)
	{
		e = manager.xYAxisToDown();
		if (e != MotorManager::Ok) 
		{
			return;
		}
		
		e = manager.xStepToPosition5();
		if (e != MotorManager::Ok) 
		{
			return;
		}
		
		e = manager.xStepToOrigin();
		if (e != MotorManager::Ok) 
		{
			return;
		}
	}

}





