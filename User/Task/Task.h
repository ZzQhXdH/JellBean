#ifndef		__TASK_TASK_H
#define		__TASK_TASK_H

#include <rtx_os.h>
#include <stdint.h>


class Task
{
	public:
		void start();
		void stop();
	
	protected:
		virtual void run(void) = 0;
	
	protected:
		Task(const char *name, uint64_t *stack, uint32_t stackSize, osPriority_t priority = osPriorityHigh);
	
	private:
		static void startFunction(void *);
	
	private:
		osThreadAttr_t mAttr;
		osThreadId_t mId;
		uint64_t mCbBlock[ (osRtxThreadCbSize + 7) / 8];
};






#endif
