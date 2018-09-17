#ifndef		__DELIVER_LISTENER_H
#define		__DELIVER_LISTENER_H

#include "Driver/LogUart.h"

class DeliverListener
{
	public:
		virtual void onStart(void); // ��ʼ����
		 
		virtual void onInit(void); // ��ʼ�����
	
		virtual void onPushCup(void); // ��ʼ�Ʊ�
	
		virtual void onDropCup(void); // ��ʼ�䱭
	
		virtual void onStepMotorMoved(uint32_t steps); // ��������ƶ�����ԭ��steps����
		
		virtual void onToppleCup(void); // ����
	
		virtual void onStepMotorOrigin(void); // �ع�ԭ��
	
		
};



#endif
