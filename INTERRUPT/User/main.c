#include "bsp_interrupt.h"
#include "bsp_led.h"

int main(void)
{
/* LED �˿ڳ�ʼ�� */
LED_GPIO_Config();

/* ��ʼ�� EXTI �жϣ����°����ᴥ���жϣ�
* �����жϻ���� stm32f4xx_it.c �ļ��еĺ���
* KEY1_IRQHandler �� KEY2_IRQHandler�������жϣ���ת LED �ơ�
*/
EXTI_Key_Config();

/* �ȴ��жϣ�����ʹ���жϷ�ʽ��CPU ������ѯ���� */
while (1) {
}
}

void TimingDelay_Decrement(void){
}
