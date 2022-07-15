#include "bsp_clock.h"
#include "bsp_led.h"

int main(void)
{
// �������� main ����֮ǰ�������ļ���statup_stm32f10x_hd.s �Ѿ�����
// SystemInit()������ϵͳʱ�ӳ�ʼ���� 72MHZ
// SystemInit()�� system_stm32f10x.c �ж���
// ����û����޸�ϵͳʱ�ӣ������б�д�����޸�
// ��������ϵͳʱ�ӣ���ʱ�����ѡ��ʹ�� HSE ���� HSI

// ʹ�� HSE������ϵͳʱ��Ϊ 180M
 HSE_SetSysClock(25, 360, 2, 7);
 
 //ϵͳʱ�ӳ�Ƶ�� 216M ˬһ�£������ 216M������������
//HSE_SetSysClock(25, 432, 2, 9);
 
 // ʹ�� HSI������ϵͳʱ��Ϊ 180M
 //HSI_SetSysClock(16, 360, 2, 7);
 
 // LED �˿ڳ�ʼ��
 LED_GPIO_Config();

 // MCO GPIO ��ʼ��
 MCO1_GPIO_Config();
 MCO2_GPIO_Config();
 
 // MCO1 ��� PLLCLK
 RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_1);
 
 // MCO2 ��� SYSCLK
 RCC_MCO2Config(RCC_MCO2Source_SYSCLK, RCC_MCO1Div_1);
 
 while (1) {
 LED1_ON; // ��
 Delay(0x01FFFFF);
 LED1_OFF; // ��
 Delay(0x01FFFFF);
 }
}

void TimingDelay_Decrement(void){
}
