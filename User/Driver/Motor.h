#ifndef		__MOTO_H
#define		__MOTO_H

#include <rtx_os.h>
#include "stm32f4xx.h"

#define STD_POSITION1	1138
#define STD_POSITION2	1871
#define STD_POSITION3	2605
#define STD_POSITION4	3338
#define	STD_POSITION5	4071

class Motor
{
	public:
		enum Dir {
			CW = 0x01,
			CCW = 0x02,
			Brake = 0x03,
			
			Up = CW,
			Down = CCW,
			
			BackUp = CW,
			GoForward = CCW,
			
			Left = 0x01,
			Right = 0x02,
		};
	
	public:
		inline static Motor &instance(void) {
			static Motor moto;
			return moto;
		}
		
		void vSetDir(Dir dir);
		
		/**
		 * @brief 落杯电机
		 */
		inline void vDropOutOn(Dir dir) {
			vSetDir(dir);
			GPIOC->BSRRH = 0x40;
		}
		
		void vDropOutOn(void);
		
		inline void vDropOutOff(void) {
			vSetDir(Brake);
			GPIOC->BSRRL = 0x40;
		}
		
		/**
		 * @brief 取物口旋转电机
		 */
		inline void vRotateOn(Dir dir) {
			vSetDir(dir);
			GPIOC->BSRRH = 0x80;
		}
		
		inline void vRotateOff(void) {
			vSetDir(Brake);
			GPIOC->BSRRL = 0x80;
		}
		
		void vRotateTo1(void);
		
		void vRotateTo2(void);
		
		inline bool isRotate2(void) {
			return (GPIOD->IDR & 0x04) != 0x00;
		}
		
		inline bool isRotate1(void) {
			return (GPIOD->IDR & 0x02) != 0x00;
		}
		
		/**
		 * @brief Y轴电机
		 */
		inline void vYAxisOn(Dir dir) {
			vSetDir(dir);
			GPIOE->BSRRH = 0x400;
		}
		
		inline void vYAxisOff(void) {
			vSetDir(Brake);
			GPIOE->BSRRL = 0x400;
		}
		
		void vYAxisUp(void);
		
		void vYAxisDown(void);
		
		/**
		 * @brief 推杯电机
		 */
		inline void vPushOn(Dir dir) {
			vSetDir(dir);
			GPIOE->BSRRH = 0x200;
		}
		
		inline void vPushOff(void) {
			vSetDir(Brake);
			GPIOE->BSRRL = 0x200;
		}
		
		void vPushNext(void);
		
		void vPushGoForward(void);
		
		void vPushBackUp(void);
		
		/**
		 * @brief 打开红外检测
		 */
		inline void vOpenIrda(void) {
			GPIOC->BSRRL = 0x10;
		}
		
		/**
		 * @brief 关闭红外检测
		 */
		inline void vCloseIrda(void) {
			GPIOC->BSRRH = 0x10;
		}
		
		/**
		 * @brief 电磁锁
		 */
		inline void vElectorLockOn(void) {
			vSetDir(CCW);
			GPIOC->BSRRH = 0x100;
		}
		
		inline void vElectorLockOff(void) {
			vSetDir(Brake);
			GPIOC->BSRRL = 0x100;
		}
		
		/**
		 * @brief 动力盒电机1
		 */
		inline void vPowerBox1On(Dir dir) {
			GPIOE->BSRRH = 0x800;
			vSetDir(dir);
		}
		
		inline void vPowerBox1Off(void) {
			GPIOE->BSRRL = 0x800;
			vSetDir(Brake);
		}
		
		void vPowerBox1On(void);
		
		/**
		 * @brief 动力盒电机2
		 */
		inline void vPowerBox2On(Dir dir) {
			GPIOE->BSRRH = 0x1000;
			vSetDir(dir);
		}
		
		inline void vPowerBox2Off(void) {
			GPIOE->BSRRL = 0x1000;
			vSetDir(Brake);
		}
		
		void vPowerBox2On(void);
		
		/**
		 * @brief 动力盒电机3
		 */
		inline void vPowerBox3On(Dir dir) {
			GPIOE->BSRRH = 0x2000;
			vSetDir(dir);
		}
		
		inline void vPowerBox3Off(void) {
			GPIOE->BSRRL = 0x2000;
			vSetDir(Brake);
		}
		
		void vPowerBox3On(void);
		
		/**
		 * @brief 动力盒电机4
		 */
		inline void vPowerBox4On(Dir dir) {
			GPIOE->BSRRH = 0x4000;
			vSetDir(dir);
		}
		
		inline void vPowerBox4Off(void) {
			GPIOE->BSRRL = 0x4000;
			vSetDir(Brake);
		}
		
		void vPowerBox4On(void);
		
		/**
		 * @brief 动力盒电机5
		 */
		inline void vPowerBox5On(Dir dir) {
			GPIOE->BSRRH = 0x8000;
			vSetDir(dir);
		}
		
		inline void vPowerBox5Off(void) {
			GPIOE->BSRRL = 0x8000;
			vSetDir(Brake);
		}
		
		void vPowerBox5On(void);
		
		inline bool isPowerBox1Checked(void) {
			return (GPIOD->IDR & 0x100) == 0x00;
		}
		
		inline bool isPowerBox2Checked(void) {
			return (GPIOD->IDR & 0x200) == 0x00;
		}
		
		inline bool isPowerBox3Checked(void) {
			return (GPIOD->IDR & 0x400) == 0x00;
		}
		
		inline bool isPowerBox4Checked(void) {
			return (GPIOD->IDR & 0x800) == 0x00;
		}
		
		inline bool isPowerBox5Checked(void) {
			return (GPIOD->IDR & 0x1000) == 0x00;
		}
		
		/**
		 * @brief 步进电机
		 * 40 * 3
		 */
		void vStepToRight(uint32_t steps);
		
		void vStepToLeft(uint32_t steps);
		
		void vMoveTo(uint32_t offsetSteps, uint32_t currentSteps);
		
		void vMoveTo(uint32_t steps);
		
		inline void vMoveToMm(uint32_t mm) {
			vMoveTo( mm * 800 / 120 );
		}
		
		inline void vMoveTo1(void) {
			vYAxisUp();
			vMoveTo(STD_POSITION1);
		}
		
		inline void vMoveTo2(void) {
			vYAxisUp();
			vMoveTo(STD_POSITION2);
		}
		
		inline void vMoveTo3(void) {
			vYAxisUp();
			vMoveTo(STD_POSITION3);
		}
		
		inline void vMoveTo4(void) {
			vYAxisUp();
			vMoveTo(STD_POSITION4);
		}
		
		inline void vMoveTo5(void) {
			vYAxisUp();
			vMoveTo(STD_POSITION5);
		}
		
		inline bool isStepFault(void) {
			return (GPIOE->IDR & 0x20) == 0x00;
		}
		
		inline void vDislodgeFault(void) {
			vStepSetEnable(false);
			osDelay(4000);
			vStepSetEnable(true);
		}
		
		void vStepToOrigin(void);
		
		void vStepSetDir(Dir dir);
		
		void vStepSetEnable(bool flag);
		
		inline bool isRight(void) { // 微动开关
			return (GPIOD->IDR & 0x4000) == 0x00;
		}
		
		inline bool isTroughtRight(void) { // 槽型开关
			return (GPIOD->IDR & 0x08) != 0x00;
		}
		
		/**
		 * @brief Y轴微动开关1是否闭合
		 */
		inline bool isY1Contact(void) {
			return (GPIOD->IDR & 0x40) == 0x00;
		}
		
		/**
		 * @brief Y轴微动开关2是否闭合
		 */
		inline bool isY2Contact(void) {
			return (GPIOD->IDR & 0x80) == 0x00;
		}
		
		/**
		 * @brief 推杯电机前进的位置开关是否闭合
		 */
		inline bool isPushGoForwardContact(void) {
			return (GPIOD->IDR & 0x20) == 0x00;
		}
		
		/**
		 * @brief 推杯电机后退的位置开关是否闭合
		 */
		inline bool isPushBackUpContact(void) {
			return (GPIOD->IDR & 0x10) == 0x00;
		}
		
		/**
		 * @brief 检测是否落杯
		 */
		inline bool isDropOut(void) {
			return (GPIOD->IDR & 0x2000) == 0x00;
		}
		
		/**
		 * @brief 检测推杯
		 */
		inline bool isPush(void) {
			return (GPIOD->IDR & 0x01) != 0x00;
		}
		
	private:
		Motor(void);
	
	private:
		uint32_t mCurrentSteps; // 相对于原点左边的偏移步数
};





#endif
