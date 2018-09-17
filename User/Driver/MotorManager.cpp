#include "Driver/MotorManager.h"

#define STEP_PLUS()		GPIOE->ODR ^= 0x04

void MotorManager::vSetDir(Dir dir)
{
	switch (dir)
	{
		case CW: 
			GPIOE->BSRRH = 0x100;
			GPIOE->BSRRL = 0x80;
			break;
		
		case CCW: 
			GPIOE->BSRRL = 0x100;
			GPIOE->BSRRH = 0x80;
			break;
		
		case Brake:
			GPIOE->BSRRH = 0x180;
			break;
	}
}

void MotorManager::vStepSetDir(Dir dir)
{
	switch (dir) 
	{
		case Right: GPIOE->BSRRH = 0x08; return;
		case Left: GPIOE->BSRRL = 0x08; return;
	}
}

MotorManager::Error MotorManager::xPushNext(uint32_t timeOut)
{
	uint32_t counter = 0;
	uint32_t lrt = 0;
	
	if (isPushChecked()) {
		return Ok;
	}
	
	if (isPushFrontChecked()) {
		return NoCup;
	}
	
	vSetDir(GoForward);
	vPushEnable();
	while (!isPushChecked())
	{
		osDelay(20);
		counter += 20;
		if (counter >= timeOut) 
		{
			vPushDisable();
			return TimeOut;
		}
		if (isPushFrontChecked()) 
		{
			vPushDisable();
			return NoCup;
		}
		
		if (isPushLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		
		if (lrt >= 20)
		{
			vPushDisable();
			return LRT;
		}
	}
	counter = 0;
	vSetDir(BackUp);
	while (!isPushBackChecked())
	{
		osDelay(20);
		counter += 20;
		if (counter >= 500) 
		{
			vPushDisable();
			return Ok;
		}
		if (isPushLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 20) 
		{
			vPushDisable();
			return LRT;
		}
	}
	
	vPushDisable();
	return Ok;
}

MotorManager::Error MotorManager::xPushFront(uint32_t timeOut)
{
	uint32_t counter = 0;
	uint32_t lrt = 0;
	vSetDir(GoForward);
	vPushEnable();
	while (!isPushFrontChecked())
	{
		osDelay(20);
		counter += 20;
		if (counter >= timeOut) 
		{
			vPushDisable();
			return TimeOut;
		}
		if (isPushLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		
		if (lrt >= 20) 
		{
			vPushDisable();
			return LRT;
		}
	}
	vPushDisable();
	return Ok;
}

MotorManager::Error MotorManager::xPushBack(uint32_t timeOut)
{
	uint32_t counter = 0;
	uint32_t lrt = 0;
	vSetDir(BackUp);
	vPushEnable();
	while (!isPushBackChecked())
	{
		osDelay(20);
		counter += 20;
		if (counter >= timeOut) 
		{
			vPushDisable();
			return TimeOut;
		}
		if (isPushLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 20) 
		{
			vPushDisable();
			return LRT;
		}
	}
	vPushDisable();
	return Ok;	
}

MotorManager::Error MotorManager::xDropOut(uint32_t timeOut)
{
	uint32_t counter = 0;
	uint32_t lrt = 0;
	vSetDir(CW);
	vDropOutEnable();
	while (!isDropOutChecked())
	{
		osDelay(20);
		counter += 20;
		if (counter >= timeOut) 
		{
			vDropOutDisable();
			return TimeOut;
		}
		if (isDropLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vDropOutDisable();
			return LRT;
		}
	}
	vDropOutDisable();
	return Ok;
}

MotorManager::Error MotorManager::xYAxisToUp(uint32_t timeOut)
{
	uint32_t counter = 0;
	uint32_t lrt = 0;
	vSetDir(Up);
	vYAxisEnable();
	while (!isYAxisUpChecked())
	{
		osDelay(20);
		counter += 20;
		if (counter >= timeOut) {
			vYAxisDisable();
			return TimeOut;
		}
		if (isYAxisLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vYAxisDisable();
			return LRT;
		}
	}
	vYAxisDisable();
	return Ok;
}

MotorManager::Error MotorManager::xYAxisToDown(uint32_t timeOut)
{
	uint32_t counter = 0;
	uint32_t lrt = 0;
	vSetDir(Down);
	vYAxisEnable();
	while (!isYAxisDownChecked())
	{
		osDelay(20);
		counter += 20;
		if (counter >= timeOut) {
			vYAxisDisable();
			return TimeOut;
		}
		if (isYAxisLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vYAxisDisable();
			return LRT;
		}
	}
	vYAxisDisable();
	return Ok;
}

MotorManager::Error MotorManager::xRotationToOpen(uint32_t timeOut)
{
	uint32_t counter = 0;
	uint32_t lrt = 0;
	vSetDir(CCW);
	vRotationEnable();
	while (!isRotationOpenChecked())
	{
		osDelay(20);
		counter += 20;
		if (counter >= timeOut) {
			vRotationDisable();
			return TimeOut;
		}
		if (isDoorLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 20) {
			vRotationDisable();
			return LRT;
		}
	}
	vRotationDisable();
	return Ok;
}

MotorManager::Error MotorManager::xRotationToClose(uint32_t timeOut)
{
	uint32_t counter = 0;
	uint32_t lrt = 0;
	vSetDir(CCW);
	vRotationEnable();
	while (!isRotationCloseChecked())
	{
		osDelay(20);
		counter += 20;
		if (counter >= timeOut) {
			vRotationDisable();
			return TimeOut;
		}
		if (isDoorLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 20) {
			vRotationDisable();
			return LRT;
		}
	}
	vRotationDisable();
	return Ok;
}

MotorManager::Error MotorManager::xRotationToOpen2(uint32_t timeOut)
{
	uint32_t counter = 0;
	uint32_t lrt = 0;
	vSetDir(CW);
	vRotationEnable();
	while (!isRotationOpenChecked())
	{
		osDelay(20);
		counter += 20;
		if (counter >= timeOut) {
			vRotationDisable();
			return TimeOut;
		}
		if (isDoorLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 20) {
			vRotationDisable();
			return LRT;
		}
	}
	vRotationDisable();
	return Ok;	
}

MotorManager::Error MotorManager::xStepForSteps(uint32_t steps)
{
	if (steps == mStepMotorSteps) {
		return Ok;
	}
	uint32_t counter;
	if (steps > mStepMotorSteps) {
		vStepSetDir(Left);
		counter = steps - mStepMotorSteps;
	} else {
		vStepSetDir(Right);
		counter = mStepMotorSteps - steps;
	}
	vStepEnable();
	for (uint32_t i = 0; i < counter; i ++)
	{
		STEP_PLUS();
		osDelay(1);
		STEP_PLUS();
		osDelay(1);
		if (isStepFault()) {
			return LRT;
		}
	}
	mStepMotorSteps = steps;
	return Ok;
}

MotorManager::Error MotorManager::xStepToOrigin(void)
{
	Error e = xYAxisToUp();
	if (e != Ok) {
		return e;
	}
	vStepEnable();
	vStepSetDir(Right);
	while (!isStepOriginChecked()) {
		STEP_PLUS();
		osDelay(1);
	}
	mStepMotorSteps = 0;	
	return Ok;
}

MotorManager::Error MotorManager::xPowerBox1Toggle(uint32_t timeOut)
{
	uint32_t counter = 0;
	uint32_t lrt = 0;
	vSetDir(CCW);
	vPowerBox1Enable();
	while ( isPowerBox1Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox1Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox1Disable();
			return LRT;
		}
	}
	osDelay(500);
	lrt = 0;
	while ( !isPowerBox1Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox1Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox1Disable();
			return LRT;
		}
	}
	osDelay(500);
	lrt = 0;
	while ( isPowerBox1Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox1Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox1Disable();
			return LRT;
		}
	}
	lrt  = 0;
	while ( !isPowerBox1Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox1Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox1Disable();
			return LRT;
		}
	}
	vPowerBox1Disable();
	return Ok;
}

MotorManager::Error MotorManager::xPowerBox2Toggle(uint32_t timeOut)
{
	uint32_t lrt = 0;
	uint32_t counter = 0;
	vSetDir(CCW);
	vPowerBox2Enable();
	while ( isPowerBox2Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox2Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox2Disable();
			return LRT;
		}
	}
	osDelay(500);
	lrt = 0;
	while ( !isPowerBox2Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox2Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox2Disable();
			return LRT;
		}
	}
	osDelay(500);
	lrt = 0;
	while ( isPowerBox2Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox2Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox2Disable();
			return LRT;
		}
	}
	lrt = 0;
	while ( !isPowerBox2Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox2Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox2Disable();
			return LRT;
		}
	}
	vPowerBox2Disable();
	return Ok;
}

MotorManager::Error MotorManager::xPowerBox3Toggle(uint32_t timeOut)
{
	uint32_t counter = 0;
	uint32_t lrt = 0;
	vSetDir(CCW);
	vPowerBox3Enable();
	while ( isPowerBox3Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox3Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox3Disable();
			return LRT;
		}
	}
	osDelay(500);
	lrt = 0;
	while ( !isPowerBox3Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox3Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox3Disable();
			return LRT;
		}
	}
	osDelay(500);
	lrt = 0;
	while ( isPowerBox3Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox3Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox3Disable();
			return LRT;
		}
	}
	lrt = 0;
	while ( !isPowerBox3Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox3Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox3Disable();
			return LRT;
		}
	}
	vPowerBox3Disable();
	return Ok;
}

MotorManager::Error MotorManager::xPowerBox4Toggle(uint32_t timeOut)
{
	uint32_t counter = 0;
	uint32_t lrt = 0;
	vSetDir(CCW);
	vPowerBox4Enable();
	while ( isPowerBox4Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox4Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox4Disable();
			return LRT;
		}
	}
	osDelay(500);
	lrt = 0;
	while ( !isPowerBox4Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox4Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox4Disable();
			return LRT;
		}
	}
	osDelay(500);
	lrt = 0;
	while ( isPowerBox4Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox4Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox4Disable();
			return LRT;
		}
	}
	lrt = 0;
	while ( !isPowerBox4Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox4Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox4Disable();
			return LRT;
		}
	}
	
	vPowerBox4Disable();
	return Ok;
}

MotorManager::Error MotorManager::xPowerBox5Toggle(uint32_t timeOut)
{
	uint32_t counter = 0;
	uint32_t lrt = 0;
	vSetDir(CCW);
	vPowerBox5Enable();
	while ( isPowerBox5Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox5Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox5Disable();
			return LRT;
		}
	}
	osDelay(500);
	lrt = 0;
	while ( !isPowerBox5Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox5Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox5Disable();
			return LRT;
		}
	}
	osDelay(500);
	lrt = 0;
	while ( isPowerBox5Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox5Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox5Disable();
			return LRT;
		}
	}
	lrt = 0;
	while ( !isPowerBox5Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox5Disable();
			return TimeOut;
		} 
		if (isPowerLrt()) {
			lrt ++;
		} else {
			lrt = 0;
		}
		if (lrt >= 5) {
			vPowerBox5Disable();
			return LRT;
		}
	}
	vPowerBox5Disable();
	return Ok;
}


MotorManager::Error MotorManager::xPowerBox1Test(uint32_t timeOut)
{
	uint32_t counter = 0;
	vSetDir(CCW);
	vPowerBox1Enable();
	while ( isPowerBox1Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox1Disable();
			return TimeOut;
		} 
	}
	while ( !isPowerBox1Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox1Disable();
			return TimeOut;
		} 
	}
	vPowerBox1Disable();
	return Ok;
}


MotorManager::Error MotorManager::xPowerBox2Test(uint32_t timeOut)
{
	uint32_t counter = 0;
	vSetDir(CCW);
	vPowerBox2Enable();
	while ( isPowerBox2Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox2Disable();
			return TimeOut;
		} 
	}
	osDelay(500);
	while ( !isPowerBox2Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox2Disable();
			return TimeOut;
		} 
	}
	vPowerBox2Disable();
	return Ok;
}


MotorManager::Error MotorManager::xPowerBox3Test(uint32_t timeOut)
{
	uint32_t counter = 0;
	vSetDir(CCW);
	vPowerBox3Enable();
	while ( isPowerBox3Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox3Disable();
			return TimeOut;
		} 
	}
	while ( !isPowerBox3Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox3Disable();
			return TimeOut;
		} 
	}
	vPowerBox3Disable();
	return Ok;
}


MotorManager::Error MotorManager::xPowerBox4Test(uint32_t timeOut)
{
	uint32_t counter = 0;
	vSetDir(CCW);
	vPowerBox4Enable();
	while ( isPowerBox4Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox4Disable();
			return TimeOut;
		} 
	}
	while ( !isPowerBox4Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox4Disable();
			return TimeOut;
		} 
	}
	vPowerBox4Disable();
	return Ok;
}


MotorManager::Error MotorManager::xPowerBox5Test(uint32_t timeOut)
{
	uint32_t counter = 0;
	vSetDir(CCW);
	vPowerBox5Enable();
	while ( isPowerBox5Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox5Disable();
			return TimeOut;
		} 
	}
	while ( !isPowerBox5Checked() )
	{
		osDelay(20);
		counter += 20;
		if (counter > timeOut) {
			vPowerBox5Disable();
			return TimeOut;
		} 
	}
	vPowerBox5Disable();
	return Ok;
}

MotorManager::MotorManager(void)
{
	vInitGpio();
	vDisableAll();
	vIrdaEnable();
	xRotationToClose();
	xStepToOrigin();
	mStepMotorSteps = 0;
}

/**
 * PC4 <-> �Ʊ��������		<=> �������
 * PC6 <-> �䱭���				<=> �������
 * PC7 <-> ȡ�����ת���		<=> �������
 * PC8 <-> �����				<=> �������
 *
 * PD0 <-> �Ʊ����				<=> ��������
 * PD1 <-> ȡ��ڲ��Ϳ���1		<=> ��������
 * PD2 <-> ȡ��ڲ��Ϳ���2		<=> ��������
 * PD3 <-> X����Ϳ���			<=> ��������
 * PD4 <-> �Ʊ����΢������1	<=> ��������
 * PD5 <-> �Ʊ����΢������2	<=> ��������
 * PD6 <-> Y��΢������1			<=> ��������
 * PD7 <-> Y��΢������2			<=> ��������
 * PD8 <-> ������΢������1		<=> ��������
 * PD9 <-> ������΢������2		<=> ��������
 * PD10 <-> ������΢������3		<=> ��������
 * PD11 <-> ������΢������4		<=> ��������
 * PD12 <-> ������΢������5		<=> ��������
 * PD13 <-> �䱭��΢������		<=> ��������
 * PD14 <-> X��΢������			<=> ��������
 * PD15 <-> �Ž�����			<=> ��������
 *
 * PE2 <-> ���������������		<=> �������
 * PE3 <-> ��������������		<=> �������
 * PE4 <-> �������ʹ��			<=> �������
 * PE5 <-> ���������������		<=> �������
 * PE7 <-> ����ѡ��1			<=> �������
 * PE8 <-> ����ѡ��2			<=> �������
 * PE9 <-> Y����				<=> �������
 * PE10 <-> �Ʊ����			<=> �������
 * PE11 <-> �����е��1			<=> �������
 * PE12 <-> �����е��2			<=> �������
 * PE13 <-> �����е��3			<=> �������
 * PE14 <-> �����е��4			<=> �������
 * PE15 <-> �����е��5			<=> �������
 */
 
void MotorManager::vInitGpio(void)
{
	RCC->AHB1ENR |= 0x1C;
	
	GPIOC->MODER &= ~0x3F300; 
	GPIOC->MODER |= 0x15100;
	GPIOC->PUPDR &= ~0x3F300;
	GPIOC->OSPEEDR |= 0x3F300;
	
	GPIOD->MODER = 0x00; // ����ģʽ
	GPIOD->PUPDR = 0x00; // ����
	
	GPIOE->MODER &= ~0xFFFFCFF0;
	GPIOE->MODER |= 0x55554150;
	GPIOE->PUPDR &= ~0xFFFFCFF0;
	GPIOE->OSPEEDR |= 0xFFFFCFF0;
}












