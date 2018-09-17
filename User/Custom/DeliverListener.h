#ifndef		__DELIVER_LISTENER_H
#define		__DELIVER_LISTENER_H

#include "Driver/LogUart.h"

class DeliverListener
{
	public:
		virtual void onStart(void); // 开始出货
		 
		virtual void onInit(void); // 初始化完成
	
		virtual void onPushCup(void); // 开始推杯
	
		virtual void onDropCup(void); // 开始落杯
	
		virtual void onStepMotorMoved(uint32_t steps); // 步进电机移动到离原点steps步数
		
		virtual void onToppleCup(void); // 倒杯
	
		virtual void onStepMotorOrigin(void); // 回归原点
	
		
};



#endif
