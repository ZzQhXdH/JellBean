#ifndef		__MOTOR_MANAGER_H
#define		__MOTOR_MANAGER_H

#include "stm32f4xx.h"
#include <rtx_os.h>
#include "Driver/CurrentAdc.h"

#define STD_TIME_OUT			10 * 1000

#define POWER_LRT				350
#define DOOR_LRT				170
#define DROP_LRT				80
#define PUSH_LRT				400
#define YAXIS_LRT				100

// 动力盒电机堵转 350
// 门电机堵转 170
// 落杯电机 80
// 推杯电机 400
// Y轴电机 100

/**
 * PD0 <-> 推杯检测				<=> 浮空输入
 * PD1 <-> 取物口槽型开关1		<=> 浮空输入
 * PD2 <-> 取物口槽型开关2		<=> 浮空输入
 * PD3 <-> X轴槽型开关			<=> 浮空输入
 * PD4 <-> 推杯电机微动开关1	<=> 浮空输入
 * PD5 <-> 推杯电机微动开关2	<=> 浮空输入
 * PD6 <-> Y轴微动开关1			<=> 浮空输入
 * PD7 <-> Y轴微动开关2			<=> 浮空输入
 * PD8 <-> 动力盒微动开关1		<=> 浮空输入
 * PD9 <-> 动力盒微动开关2		<=> 浮空输入
 * PD10 <-> 动力盒微动开关3		<=> 浮空输入
 * PD11 <-> 动力盒微动开关4		<=> 浮空输入
 * PD12 <-> 动力盒微动开关5		<=> 浮空输入
 * PD13 <-> 落杯器微动开关		<=> 浮空输入
 * PD14 <-> X轴微动开关			<=> 浮空输入
 * PD15 <-> 门禁开关			<=> 浮空输入
 */

class MotorManager
{
	private:
		enum Dir {
			CW = 0x01,
			CCW = 0x02,
			Brake = 0x03,
			
			Up = CW,
			Down = CCW,
			
			BackUp = CW,
			GoForward = CCW,
			
			Left = CW,
			Right = CCW,
		};
		
	public:
		enum Error {
			Ok = 0x00,
			TimeOut = 0x01,
			NoCup = 0x02, // 推杯电机 => 已经没有杯子了
			LRT = 0x03, // 堵转
		};
	
	public:
		inline static MotorManager &instance(void) {
			static MotorManager man;
			return man;
		}
		
		void vDisableAll(void) { 
			GPIOC->BSRRL = 0x1D0;
			GPIOE->BSRRL = 0xFE10;
		}
		
		/**
		 * @brief 推杯电机管理
		 */
		Error xPushNext(uint32_t timeOut = STD_TIME_OUT * 2);
		
		Error xPushFront(uint32_t timeOut = STD_TIME_OUT * 2);
		
		Error xPushBack(uint32_t timeOut = STD_TIME_OUT * 2);
		
		/**
		 * @brief 落杯电机管理
		 */
		Error xDropOut(uint32_t timeOut = STD_TIME_OUT);
		
		/**
		 * @brief Y轴电机管理
		 */
		Error xYAxisToUp(uint32_t timeOut = STD_TIME_OUT);
		
		Error xYAxisToDown(uint32_t timeOut = STD_TIME_OUT);
		
		/**
		 * @brief 旋转电机管理
		 */
		Error xRotationToOpen(uint32_t timeOut = STD_TIME_OUT);
		 
		Error xRotationToClose(uint32_t timeOut = STD_TIME_OUT);
		
		Error xRotationToOpen2(uint32_t timeOut = STD_TIME_OUT);
		 
		 /**
		  * @brief 步进电机管理
		  */
		Error xStepForSteps(uint32_t steps);
		
		inline Error xStepForMm(uint32_t mm) {
			return xStepForSteps(mm * 10 / 3);
		}
		
		inline Error xStepToPosition1(void) {
			return xStepForMm(171);
		}
		
		inline Error xStepToPosition2(void) {
			return xStepForMm(171 + 110);
		}
		
		inline Error xStepToPosition3(void) {
			return xStepForMm(171 + 110 * 2);
		}
				
		inline Error xStepToPosition4(void) {
			return xStepForMm(171 + 110 * 3);
		}
				
		inline Error xStepToPosition5(void) {
			return xStepForMm(171 + 110 * 4);
		}
		
		Error xStepToOrigin(void);
		
		/**
		 * @brief 动力盒电机管理
		 */
		Error xPowerBox1Toggle(uint32_t timeOut = STD_TIME_OUT);
		
		Error xPowerBox2Toggle(uint32_t timeOut = STD_TIME_OUT);
		
		Error xPowerBox3Toggle(uint32_t timeOut = STD_TIME_OUT);
		
		Error xPowerBox4Toggle(uint32_t timeOut = STD_TIME_OUT);
		
		Error xPowerBox5Toggle(uint32_t timeOut = STD_TIME_OUT);
		
		Error xPowerBox1Test(uint32_t timeOut = STD_TIME_OUT);
		
		Error xPowerBox2Test(uint32_t timeOut = STD_TIME_OUT);
		
		Error xPowerBox3Test(uint32_t timeOut = STD_TIME_OUT);
		
		Error xPowerBox4Test(uint32_t timeOut = STD_TIME_OUT);
		
		Error xPowerBox5Test(uint32_t timeOut = STD_TIME_OUT);
		/**
		 * @brief 电磁锁管理
		 */
		inline void vElectorLockOn(void) {
			vSetDir(CCW);
			vElectorLockEnable();
			osDelay(3000);
			vElectorLockOff();
		}
		
		inline void vElectorLockOff(void) {
			vElectorLockDisable();
		}
		
		inline bool isPushChecked(void) { // 推杯检测, 落杯器内是否有杯子
			return (GPIOD->IDR & 0x01) != 0x00;
		}
		
		inline bool isRotationOpenChecked(void) { // 检测取物口是否打开
			return (GPIOD->IDR & 0x04) != 0x00;
		}
		
		inline bool isRotationCloseChecked(void) { // 检测取物口是否关闭
			return (GPIOD->IDR & 0x02) != 0x00;
		}
		
		inline bool isStepOriginChecked(void) { // 检测步进电机是否在原点
			return (GPIOD->IDR & 0x08) != 0x00;
		}
		
		inline bool isPushFrontChecked(void) { // 检测推杯电机是否位于前面
			return (GPIOD->IDR & 0x20) == 0x00;
		}
		
		inline bool isPushBackChecked(void) { // 检测推杯电机是否位于后面
			return (GPIOD->IDR & 0x10) == 0x00;
		}
		
		inline bool isYAxisUpChecked(void) { // 检测Y轴电机是否位于上面
			return (GPIOD->IDR & 0x80) == 0x00;
		}
		
		inline bool isYAxisDownChecked(void) { // 检测Y轴电机是否位于下面
			return (GPIOD->IDR & 0x40) == 0x00;
		}
		
		inline bool isPowerBox1Checked(void) { // 检测动力盒开关1是否闭合
			return (GPIOD->IDR & 0x100) == 0x00;
		}
		
		inline bool isPowerBox2Checked(void) { // 检测动力盒开关2是否闭合
			return (GPIOD->IDR & 0x200) == 0x00;
		}
		
		inline bool isPowerBox3Checked(void) { // 检测动力盒开关3是否闭合
			return (GPIOD->IDR & 0x400) == 0x00; 
		}
		
		inline bool isPowerBox4Checked(void) { // 检测动力盒开关4是否闭合
			return (GPIOD->IDR & 0x800) == 0x00;
		}
		
		inline bool isPowerBox5Checked(void) { // 检测动力盒开关5是否闭合
			return (GPIOD->IDR & 0x1000) == 0x00;
		}
		
		inline bool isDropOutChecked(void) { // 检测是否落杯
			return (GPIOD->IDR & 0x2000) == 0x00; 
		}
		
		inline bool isXAxisChecked(void) { // 检测X轴的微动开关是否闭合
			return (GPIOD->IDR & 0x4000) == 0x00;
		}
		
		inline bool isDoorCloseChecked(void) { // 检测门禁开关是否闭合
			return (GPIOD->IDR & 0x8000) == 0x00; 
		}
		
		inline bool isStepFault(void) { // 检测步进电机是否故障
			return (GPIOE->IDR & 0x20) == 0x00;
		}
		
//#define POWER_LRT				350
//#define DOOR_LRT				170
//#define DROP_LRT				80
//#define PUSH_LRT				400
//#define YAXIS_LRT				100
		
		inline bool isPowerLrt(void) { // 动力盒电机是否堵转
			return CurrentAdc::instance().xGetValue() >= POWER_LRT;
		}
		
		inline bool isDoorLrt(void) { // 门电机是否堵转
			return CurrentAdc::instance().xGetValue() >= DOOR_LRT;
		}
		
		inline bool isDropLrt(void) { // 落杯电机是否堵转
			return CurrentAdc::instance().xGetValue() >= DROP_LRT;
		}
		
		inline bool isPushLrt(void) { // 推杯电机是否堵转
			return CurrentAdc::instance().xGetValue() >= PUSH_LRT;
		} 
		
		inline bool isYAxisLrt(void) { // Y轴电机是否堵转
			return CurrentAdc::instance().xGetValue() >= YAXIS_LRT;
		}
		
	private:
		MotorManager(void);
		void vInitGpio(void); // 初始化GPIO
		void vSetDir(Dir dir); // 设置方向
		void vStepSetDir(Dir dir); // 设置步进电机的方向
	
	private:
		inline void vIrdaEnable(void) {
			GPIOC->BSRRL = 0x10;
		}
		
		inline void vIrdaDisable(void) {
			GPIOC->BSRRH = 0x10;
		}
	
		inline void vDropOutEnable(void) {
			GPIOC->BSRRH = 0x40;
		}
		
		inline void vDropOutDisable(void) {
			GPIOC->BSRRL = 0x40;
		}
		
		inline void vRotationEnable(void) {
			GPIOC->BSRRH = 0x80;
		}
		
		inline void vRotationDisable(void) {
			GPIOC->BSRRL = 0x80;
		}
		
		inline void vElectorLockEnable(void) {
			GPIOC->BSRRH = 0x100;
		}
		
		inline void vElectorLockDisable(void) {
			GPIOC->BSRRL = 0x100;
		}
		
		inline void vStepEnable(void) {
			GPIOE->BSRRH = 0x10;
		}
	
		inline void vStepDisable(void) {
			GPIOE->BSRRL = 0x10;
		}
		
		inline void vYAxisEnable(void) {
			GPIOE->BSRRH = 0x400;
		}
		
		inline void vYAxisDisable(void) {
			GPIOE->BSRRL = 0x400;
		}
		
		inline void vPushEnable(void) {
			GPIOE->BSRRH = 0x200;
		}
		
		inline void vPushDisable(void) {
			GPIOE->BSRRL = 0x200;
		}
		
		inline void vPowerBox1Enable(void) {
			GPIOE->BSRRH = 0x800;
		}
		
		inline void vPowerBox1Disable(void) {
			GPIOE->BSRRL = 0x800;
		}
		
		inline void vPowerBox2Enable(void) {
			GPIOE->BSRRH = 0x1000;
		}
		
		inline void vPowerBox2Disable(void) {
			GPIOE->BSRRL = 0x1000;
		}
		
		inline void vPowerBox3Enable(void) {
			GPIOE->BSRRH = 0x2000;
		}
		
		inline void vPowerBox3Disable(void) {
			GPIOE->BSRRL = 0x2000;
		}
		
		inline void vPowerBox4Enable(void) {
			GPIOE->BSRRH = 0x4000;
		}
		
		inline void vPowerBox4Disable(void) {
			GPIOE->BSRRL = 0x4000;
		}
		
		inline void vPowerBox5Enable(void) {
			GPIOE->BSRRH = 0x8000;
		}
		
		inline void vPowerBox5Disable(void) {
			GPIOE->BSRRL = 0x8000;
		}
		
	private:
		uint32_t mStepMotorSteps; // 步进电机当前的步数
};






#endif
