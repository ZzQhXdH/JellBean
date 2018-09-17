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

// �����е����ת 350
// �ŵ����ת 170
// �䱭��� 80
// �Ʊ���� 400
// Y���� 100

/**
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
			NoCup = 0x02, // �Ʊ���� => �Ѿ�û�б�����
			LRT = 0x03, // ��ת
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
		 * @brief �Ʊ��������
		 */
		Error xPushNext(uint32_t timeOut = STD_TIME_OUT * 2);
		
		Error xPushFront(uint32_t timeOut = STD_TIME_OUT * 2);
		
		Error xPushBack(uint32_t timeOut = STD_TIME_OUT * 2);
		
		/**
		 * @brief �䱭�������
		 */
		Error xDropOut(uint32_t timeOut = STD_TIME_OUT);
		
		/**
		 * @brief Y��������
		 */
		Error xYAxisToUp(uint32_t timeOut = STD_TIME_OUT);
		
		Error xYAxisToDown(uint32_t timeOut = STD_TIME_OUT);
		
		/**
		 * @brief ��ת�������
		 */
		Error xRotationToOpen(uint32_t timeOut = STD_TIME_OUT);
		 
		Error xRotationToClose(uint32_t timeOut = STD_TIME_OUT);
		
		Error xRotationToOpen2(uint32_t timeOut = STD_TIME_OUT);
		 
		 /**
		  * @brief �����������
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
		 * @brief �����е������
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
		 * @brief ���������
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
		
		inline bool isPushChecked(void) { // �Ʊ����, �䱭�����Ƿ��б���
			return (GPIOD->IDR & 0x01) != 0x00;
		}
		
		inline bool isRotationOpenChecked(void) { // ���ȡ����Ƿ��
			return (GPIOD->IDR & 0x04) != 0x00;
		}
		
		inline bool isRotationCloseChecked(void) { // ���ȡ����Ƿ�ر�
			return (GPIOD->IDR & 0x02) != 0x00;
		}
		
		inline bool isStepOriginChecked(void) { // ��ⲽ������Ƿ���ԭ��
			return (GPIOD->IDR & 0x08) != 0x00;
		}
		
		inline bool isPushFrontChecked(void) { // ����Ʊ�����Ƿ�λ��ǰ��
			return (GPIOD->IDR & 0x20) == 0x00;
		}
		
		inline bool isPushBackChecked(void) { // ����Ʊ�����Ƿ�λ�ں���
			return (GPIOD->IDR & 0x10) == 0x00;
		}
		
		inline bool isYAxisUpChecked(void) { // ���Y�����Ƿ�λ������
			return (GPIOD->IDR & 0x80) == 0x00;
		}
		
		inline bool isYAxisDownChecked(void) { // ���Y�����Ƿ�λ������
			return (GPIOD->IDR & 0x40) == 0x00;
		}
		
		inline bool isPowerBox1Checked(void) { // ��⶯���п���1�Ƿ�պ�
			return (GPIOD->IDR & 0x100) == 0x00;
		}
		
		inline bool isPowerBox2Checked(void) { // ��⶯���п���2�Ƿ�պ�
			return (GPIOD->IDR & 0x200) == 0x00;
		}
		
		inline bool isPowerBox3Checked(void) { // ��⶯���п���3�Ƿ�պ�
			return (GPIOD->IDR & 0x400) == 0x00; 
		}
		
		inline bool isPowerBox4Checked(void) { // ��⶯���п���4�Ƿ�պ�
			return (GPIOD->IDR & 0x800) == 0x00;
		}
		
		inline bool isPowerBox5Checked(void) { // ��⶯���п���5�Ƿ�պ�
			return (GPIOD->IDR & 0x1000) == 0x00;
		}
		
		inline bool isDropOutChecked(void) { // ����Ƿ��䱭
			return (GPIOD->IDR & 0x2000) == 0x00; 
		}
		
		inline bool isXAxisChecked(void) { // ���X���΢�������Ƿ�պ�
			return (GPIOD->IDR & 0x4000) == 0x00;
		}
		
		inline bool isDoorCloseChecked(void) { // ����Ž������Ƿ�պ�
			return (GPIOD->IDR & 0x8000) == 0x00; 
		}
		
		inline bool isStepFault(void) { // ��ⲽ������Ƿ����
			return (GPIOE->IDR & 0x20) == 0x00;
		}
		
//#define POWER_LRT				350
//#define DOOR_LRT				170
//#define DROP_LRT				80
//#define PUSH_LRT				400
//#define YAXIS_LRT				100
		
		inline bool isPowerLrt(void) { // �����е���Ƿ��ת
			return CurrentAdc::instance().xGetValue() >= POWER_LRT;
		}
		
		inline bool isDoorLrt(void) { // �ŵ���Ƿ��ת
			return CurrentAdc::instance().xGetValue() >= DOOR_LRT;
		}
		
		inline bool isDropLrt(void) { // �䱭����Ƿ��ת
			return CurrentAdc::instance().xGetValue() >= DROP_LRT;
		}
		
		inline bool isPushLrt(void) { // �Ʊ�����Ƿ��ת
			return CurrentAdc::instance().xGetValue() >= PUSH_LRT;
		} 
		
		inline bool isYAxisLrt(void) { // Y�����Ƿ��ת
			return CurrentAdc::instance().xGetValue() >= YAXIS_LRT;
		}
		
	private:
		MotorManager(void);
		void vInitGpio(void); // ��ʼ��GPIO
		void vSetDir(Dir dir); // ���÷���
		void vStepSetDir(Dir dir); // ���ò�������ķ���
	
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
		uint32_t mStepMotorSteps; // ���������ǰ�Ĳ���
};






#endif
