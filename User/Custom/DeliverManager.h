#ifndef		__DELIVER_MANAGER_H
#define		__DELIVER_MANAGER_H

#include "Driver/MotorManager.h"
#include "Custom/DeliverObject.h"
#include "Custom/DeliverListener.h"


class DeliverManager
{
	public:
		inline static DeliverManager &instance(void) {
			static DeliverManager man;
			return man;
		}
		
	public:
		void deliver(const DeliverObject &obj, DeliverListener *listener);
		
	private:
		DeliverManager(void);
};

#endif
