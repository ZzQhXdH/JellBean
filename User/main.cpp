#include "stm32f4xx.h"
#include "Driver/LogUart.h"
#include "Task/Task.h"
#include "Driver/MotorManager.h"
#include "Task/ReceiverTask.h"
#include "Driver/CurrentAdc.h"
#include "Task/StatusUpdateTask.h"

class LogTask : public Task
{
	public:
		static LogTask &instance(void) 
		{
			static LogTask task;
			return task;
		}
		
	protected:
		virtual void run(void)
		{
			LogUart &log = LogUart::instance();
			LogEvent env;
			uint8_t bytes[6];
			MotorManager &man = MotorManager::instance();
			while (true)
			{
				//log.write("后面=%d,前面=%d\r\n", MotorManager::instance().isPushBackChecked(), MotorManager::instance().isPushFrontChecked());
				//osDelay(300);
				log.recv(&env);
				if (!env.isCorrect()) 
				{
					log.write(env.error(), LogEvent::ErrorLength);
					continue;
				}
				env.getAck(bytes);
				log.write(bytes, 6);
				env.test();
			}
		}
		
	private:
		LogTask(void) : Task("Log", mLogTaskStack, sizeof (mLogTaskStack)) {}
			
	private:
		uint64_t mLogTaskStack[512 / 8];	
};

class StartTask : public Task
{
	public:
		inline static StartTask &instance(void) {
			static StartTask task;
			return task;
		}
		
	protected:
		virtual void run(void)
		{
			MotorManager::instance();
			CurrentAdc::instance();
			LogTask::instance().start();
			ReceiverTask::instance().start();
			StatusUpdateTask::instance().start();
		}
		
	private:
		StartTask(void) : Task("Start", mStartTaskStack, sizeof (mStartTaskStack)) {}
			
	private:
		uint64_t mStartTaskStack[512 / 8];
};


int main(void)
{
	osKernelInitialize();
	StartTask::instance().start();
	osKernelStart();
}











