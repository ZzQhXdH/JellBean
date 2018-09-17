#ifndef		__STATUS_UPDATE_TASK_H
#define		__STATUS_UPDATE_TASK_H

#include "Task/Task.h"

class StatusUpdateTask : public Task
{
	public:
		static StatusUpdateTask &instance(void)
		{
			static StatusUpdateTask INSTANCE;
			return INSTANCE;
		}
	
	private:
		StatusUpdateTask(void);
		
		uint64_t mStack[512 / 8];
		
		
	protected:
		virtual void run(void);
};




#endif
