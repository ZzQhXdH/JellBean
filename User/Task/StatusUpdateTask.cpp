#include "Task/StatusUpdateTask.h"
#include "Driver/MotorManager.h"
#include "Driver/MainUart.h"

StatusUpdateTask::StatusUpdateTask(void) : Task("Update", mStack, sizeof (mStack))
{
	
}

void StatusUpdateTask::run(void)
{
	uint8_t s1 = 0;
	uint8_t s2 = 0;
	uint8_t s3 = 0;
	
	MotorManager &manager = MotorManager::instance();
	
	while (true)
	{
		s1 = manager.isPowerBox1Checked() |
			 (manager.isPowerBox2Checked() << 1) |
			 (manager.isPowerBox3Checked() << 2) |
			 (manager.isPowerBox4Checked() << 3) |
			 (manager.isPowerBox5Checked() << 4) ;
		s2 = manager.isDoorCloseChecked() |
			 (manager.isRotationOpenChecked() << 1) |
			 (manager.isPushBackChecked() << 2) |
			 (manager.isPushFrontChecked() << 3) |
			 (manager.isPushChecked() << 4) |
			 (manager.isDropOutChecked() << 5);
		s3 = manager.isYAxisUpChecked() |
			 (manager.isYAxisDownChecked() << 1) |
			 (manager.isStepFault() << 2) |
			 (manager.isStepOriginChecked() << 3);
		MainUart::instance().setAction(0x8A)
							.append1(s1)
							.append1(s2)
							.append1(s3)
							.buildAndWrite();
		osDelay(500);
	}
}









