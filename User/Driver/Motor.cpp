#include "Driver/Motor.h"


static void initGpio(void);

#define STEP_PLUS()		GPIOE->ODR ^= 0x04

void Motor::vSetDir(Dir dir)
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

void Motor::vPowerBox1On(void)
{
	vPowerBox1On(CCW);
	while (isPowerBox1Checked()) {
		osDelay(20);
	}

	while (!isPowerBox1Checked()) {
		osDelay(20);
	}
	vPowerBox1Off();
}

void Motor::vPowerBox2On(void)
{
	vPowerBox2On(CCW);
	while (isPowerBox2Checked()) {
		osDelay(20);
	}
	while (!isPowerBox2Checked()) {
		osDelay(20);
	}
	vPowerBox2Off();
}

void Motor::vPowerBox3On(void)
{
	vPowerBox3On(CCW);
	while (isPowerBox3Checked()) {
		osDelay(20);
	}
	while (!isPowerBox3Checked()) {
		osDelay(20);
	}
	vPowerBox3Off();
}

void Motor::vPowerBox4On(void)
{
	vPowerBox4On(CCW);
	while (isPowerBox4Checked()) {
		osDelay(20);
	}
	while (!isPowerBox4Checked()) {
		osDelay(20);
	}
	vPowerBox4Off();
}

void Motor::vPowerBox5On(void)
{
	vPowerBox5On(CCW);
	while (isPowerBox5Checked()) {
		osDelay(20);
	}
	while (!isPowerBox5Checked()) {
		osDelay(20);
	}
	vPowerBox5Off();
}

/**
 * @brief ��ת���
 */
void Motor::vRotateTo1(void)
{
	vRotateOn(CCW);
	while (!isRotate1()) {
		osDelay(20);
	}
	vRotateOff();
}

void Motor::vRotateTo2(void)
{
	vRotateOn(CCW);
	while (!isRotate2()) {
		osDelay(20);
	}
	vRotateOff();
}

/**
 * @brief Y��������
 */
void Motor::vYAxisUp(void)
{
	vYAxisOn(Up);
	while (!isY2Contact()) {
		osDelay(20);
	}
	vYAxisOff();
}

/**
 * @brief Y�����½�
 */
void Motor::vYAxisDown(void)
{
	vYAxisOn(Down);
	while (!isY1Contact()) {
		osDelay(20);
	}
	vYAxisOff();
}

/**
 * @brief �Ʊ����ǰ��
 */
void Motor::vPushGoForward(void)
{
	vPushOn(GoForward);
	while (!isPushGoForwardContact()) {
		osDelay(20);
	}
	vPushOff();
}

void Motor::vPushBackUp(void)
{
	vPushOn(BackUp);
	while (!isPushBackUpContact()) {
		osDelay(20);
	}
	vPushOff();
}

void Motor::vPushNext(void)
{
	vPushOn(GoForward);
	while ( (!isPush()) && (!isPushGoForwardContact()) ) {
		osDelay(20);
	}
	vPushOff();
}

/**
 * @brief �䱭
 */
void Motor::vDropOutOn(void)
{
	vDropOutOn(CW);
	while (!isDropOut()) {
		osDelay(20);
	}
	vDropOutOff();
}

/**
 * @brief ����������ұ��ƶ�steps����
 */
void Motor::vStepToRight(uint32_t steps)
{
	vStepSetEnable(true);
	vStepSetDir(Right);
	for (uint32_t i = 0; i < steps; i ++) {
		STEP_PLUS();
		osDelay(1);
	}
}

/**
 * @brief �������������ƶ�steps����
 */
void Motor::vStepToLeft(uint32_t steps)
{
	vStepSetEnable(true);
	vStepSetDir(Left);
	for (uint32_t i = 0; i < steps; i ++) {
		STEP_PLUS();
		osDelay(1);
	}
}

void Motor::vStepToOrigin(void)
{
	vYAxisUp();
	vStepSetEnable(true);
	vStepSetDir(Right);
	while (!isTroughtRight()) {
		STEP_PLUS();
		osDelay(1);
	}
	mCurrentSteps = 0;
}

void Motor::vStepSetDir(Dir dir)
{
	switch (dir) 
	{
		case Right: GPIOE->BSRRH = 0x08; return;
		case Left: GPIOE->BSRRL = 0x08; return;
	}
}

void Motor::vStepSetEnable(bool flag)
{
	if (flag) {
		GPIOE->BSRRH = 0x10;
	} else {
		GPIOE->BSRRL = 0x10;
	}
}

void Motor::vMoveTo(uint32_t offsetSteps, uint32_t currentSteps)
{
	if (offsetSteps > currentSteps) {
		vStepToLeft(offsetSteps - currentSteps);
	} else {
		vStepToRight(currentSteps - offsetSteps);
	}
	mCurrentSteps = offsetSteps;
}

void Motor::vMoveTo(uint32_t steps)
{
	if (steps > mCurrentSteps) {
		vStepToLeft(steps - mCurrentSteps);
	} else {
		vStepToRight(mCurrentSteps - steps);
	}
	mCurrentSteps = steps;	
}

Motor::Motor(void)
{
	initGpio();
	
	vPowerBox1Off();
	vPowerBox2Off();
	vPowerBox3Off();
	vPowerBox4Off();
	vPowerBox5Off();
	
	vRotateOff();
	vDropOutOff();
	vYAxisOff();
	vPushOff();
	vElectorLockOff();
	vOpenIrda();
	vRotateTo1();
	vStepToOrigin();
	
}

/**
 * @brief ��ʼ��Gpio
 * PC4 <-> �Ʊ��������
 * PC6 <-> �䱭���
 * PC7 <-> ȡ�����ת���
 * PC8 <-> �����
 * 
 * PE11 <-> �����е��1
 * PE12 <-> �����е��2
 * PE13 <-> �����е��3
 * PE14 <-> �����е��4
 * PE15 <-> �����е��5
 *
 * PE7 <-> ����ѡ��1
 * PE8 <-> ����ѡ��2
 * PE9 <-> Y����
 * PE10 <-> �Ʊ����
 *
 * PD0 <-> �Ʊ����
 * PD4 <-> �Ʊ����΢������1
 * PD5 <-> �Ʊ����΢������2
 * PD6 <-> Y��΢������1
 * PD7 <-> Y��΢������2
 *
 * PD8 <-> ������΢������1
 * PD9 <-> ������΢������2
 * PD10 <-> ������΢������3
 * PD11 <-> ������΢������4
 * PD12 <-> ������΢������5
 * PD13 <-> �䱭��΢������
 * PD14 <-> X��΢������
 * PD15 <-> �Ž�����
 *
 * PE2 <-> ���������������
 * PE3 <-> ��������������
 * PE4 <-> �������ʹ��
 * PE5 <-> ���������������
 * 
 * PD3 <-> X����Ϳ���
 * PD2 <-> ȡ��ڲ��Ϳ���2
 * PD1 <-> ȡ��ڲ��Ϳ���1
 */
static void initGpio(void)
{
	RCC->AHB1ENR |= 0x1C;
	
	GPIOC->MODER &= ~0x3F000;
	GPIOC->MODER |= 0x15000;
	GPIOC->OSPEEDR |= 0x3F000;
	GPIOC->PUPDR &= ~0x3F000;
	GPIOC->OTYPER &= ~0x1C0;
	
	GPIOC->MODER &= ~0x300;
	GPIOC->MODER |= 0x100;
	GPIOC->OSPEEDR |= 0x300;
	GPIOC->PUPDR &= ~0x300;
	GPIOC->OTYPER &= ~0x10;
	
	GPIOE->MODER &= ~0xFFFFC000;
	GPIOE->MODER |= 0x55554000;
	GPIOE->OSPEEDR |= 0xFFFFC000;
	GPIOE->PUPDR &= ~0xFFFFC000;
	GPIOE->OTYPER &= ~0xFF80;
	
	GPIOD->MODER &= ~0xFFFFFF03;
	GPIOD->PUPDR &= ~0xFFFFFF03;
	
	GPIOE->MODER &= ~0xFF0;
	GPIOE->MODER |= 0x150;
	GPIOE->PUPDR &= ~0xFF0;
	GPIOE->OSPEEDR |= 0xFF0;
	GPIOE->OTYPER &= ~0x1C;
	
	GPIOD->MODER &= ~0xFC;
	GPIOD->PUPDR &= ~0xFC;
}






