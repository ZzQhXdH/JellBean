#ifndef		__RECEIVER_TASK_H
#define		__RECEIVER_TASK_H

#include "Task/Task.h"

class ReceiverTask : public Task
{
	public:
		static ReceiverTask &instance(void)
		{
			static ReceiverTask task;
			return task;
		}
	
	private:
		ReceiverTask(void);
		
	private:
		virtual void run(void);
	
	private:
		uint64_t mTaskStack[ (1024 + 7) / 8 ];
};



#endif
