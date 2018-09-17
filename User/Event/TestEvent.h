#ifndef		__TEST_EVENT_H
#define		__TEST_EVENT_H

#include "Event/Event.h"

class TestEvent : public Event
{
	public:
		void exec(void);
};

#endif
