#ifndef		__MAIN_EVENT_H
#define		__MAIN_EVENT_H

#include "Event/Event.h"
#include "Custom/BaseOutput.h"

class MainEvent : public Event
{
	public:
		void exec(BaseOutput &out);
	
	private:
		void deliver(BaseOutput &out,uint8_t n1, uint8_t n2, uint8_t n3, uint8_t n4, uint8_t n5);
};




#endif
