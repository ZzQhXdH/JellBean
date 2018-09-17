#include "Task/ReceiverTask.h"
#include "Driver/MainUart.h"


ReceiverTask::ReceiverTask(void) : Task("receiver", mTaskStack, sizeof(mTaskStack))
{
	
}

void ReceiverTask::run(void)
{
	MainUart& uart = MainUart::instance();
	MainEvent env;
	while (true)
	{
		uart.recv(&env);
		env.exec(uart);
	}
}




