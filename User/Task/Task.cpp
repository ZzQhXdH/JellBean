#include "Task/Task.h"

Task::Task(const char *name, uint64_t *stack, uint32_t stackSize, osPriority_t priority)
{
	mAttr.name = name;
	mAttr.attr_bits = osThreadDetached;
	mAttr.cb_mem = mCbBlock;
	mAttr.cb_size = osRtxThreadCbSize;
	mAttr.stack_mem = stack;
	mAttr.stack_size = stackSize;
	mAttr.priority = priority;
	mAttr.tz_module = 0;
	mAttr.reserved = 0;
	mId = NULL;
}

void Task::startFunction(void *arg)
{
	((Task *) arg)->run();
	((Task *) arg)->mId = NULL;
	osThreadExit();
}

void Task::start(void)
{
	if (mId != NULL) {
		osThreadTerminate(mId);
	}
	osThreadNew(startFunction, this, &mAttr);
}

void Task::stop(void)
{
	osThreadTerminate(mId);
}









