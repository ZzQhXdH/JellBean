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
 * @brief 旋转电机
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
 * @brief Y轴电机上升
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
 * @brief Y轴电机下降
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
 * @brief 推杯电机前进
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
 * @brief 落杯
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
 * @brief 步进电机往右边移动steps步数
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
 * @brief 步进电机往左边移动steps步数
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
 * @brief 初始化Gpio
 * PC4 <-> 推杯检测红外打开
 * PC6 <-> 落杯电机
 * PC7 <-> 取物口旋转电机
 * PC8 <-> 电磁锁
 * 
 * PE11 <-> 动力盒电机1
 * PE12 <-> 动力盒电机2
 * PE13 <-> 动力盒电机3
 * PE14 <-> 动力盒电机4
 * PE15 <-> 动力盒电机5
 *
 * PE7 <-> 方向选择1
 * PE8 <-> 方向选择2
 * PE9 <-> Y轴电机
 * PE10 <-> 推杯电机
 *
 * PD0 <-> 推杯检测
 * PD4 <-> 推杯电机微动开关1
 * PD5 <-> 推杯电机微动开关2
 * PD6 <-> Y轴微动开关1
 * PD7 <-> Y轴微动开关2
 *
 * PD8 <-> 动力盒微动开关1
 * PD9 <-> 动力盒微动开关2
 * PD10 <-> 动力盒微动开关3
 * PD11 <-> 动力盒微动开关4
 * PD12 <-> 动力盒微动开关5
 * PD13 <-> 落杯器微动开关
 * PD14 <-> X轴微动开关
 * PD15 <-> 门禁开关
 *
 * PE2 <-> 步进电机脉冲引脚
 * PE3 <-> 步进电机方向控制
 * PE4 <-> 步进电机使能
 * PE5 <-> 步进电机故障输入
 * 
 * PD3 <-> X轴槽型开关
 * PD2 <-> 取物口槽型开关2
 * PD1 <-> 取物口槽型开关1
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






