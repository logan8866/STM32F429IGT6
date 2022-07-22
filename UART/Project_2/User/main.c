#include "stm32f4xx.h"
#include "bsp_uart.h"
#include "bsp_led.h"
#include "stdio.h"

int main(void)
{
char ch;

/* ��ʼ�� RGB �ʵ� */
LED_GPIO_Config();

/* ��ʼ�� USART ����ģʽΪ 115200 8-N-1 */
USARTx_Config();
 
 /* ��ӡָ��������ʾ��Ϣ */
 Show_Message();
 while (1)
 {
 /* ��ȡ�ַ�ָ�� */
 ch=getchar();
 printf("���յ��ַ���%c\n",ch);
 
 /* �����ַ�ָ����� RGB �ʵ���ɫ */
 switch (ch)
 {
 case '1':
 LED_RED;
 break;
 case '2':
 LED_GREEN;
 break;
 case '3':
 LED_BLUE;
 break;

 default:
 /* �������ָ��ָ���ַ�����ӡ��ʾ��Ϣ */
 Show_Message();
 break;
 }
 }
 }

void TimingDelay_Decrement(){
}
