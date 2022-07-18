#include "stm32f4xx_it.h"
#include "bsp_systick.h"
#include "bsp_led.h"

extern int TimingDelay;

int main(void)
{
/* LED �˿ڳ�ʼ�� */
LED_GPIO_Config();

/* ���� SysTick Ϊ 10us �ж�һ��,ʱ�䵽�󴥷���ʱ�жϣ�
*���� stm32fxx_it.c �ļ��� SysTick_Handler ����ͨ�����жϴ�����ʱ
*/
SysTick_Init();
 
 while (1) {
 
 LED_RED;
 Delay_us(300000); // 10000 * 10us = 1000ms
 
 LED_GREEN;
 Delay_us(300000); // 10000 * 10us = 1000ms
 
 LED_BLUE;
 Delay_us(300000); // 10000 * 10us = 1000ms
 }
 }
