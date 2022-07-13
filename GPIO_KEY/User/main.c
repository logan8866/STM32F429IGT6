#include "stm32f4xx.h"
#include "bsp_key.h"
/** �������±��ú�
* ��������Ϊ�ߵ�ƽ������ KEY_ON=1�� KEY_OFF=0
* ����������Ϊ�͵�ƽ���Ѻ����ó� KEY_ON=0 ��KEY_OFF=1 ����
*/
#define KEY_ON 1
#define KEY_OFF 0

/**
* @brief ����Ƿ��а�������
* @param GPIOx:����Ķ˿�, x �����ǣ�A...K��
* @param GPIO_PIN:����Ķ˿�λ�� ������ GPIO_PIN_x��x ������ 0...15��
* @retval ������״̬
* @arg KEY_ON:��������
* @arg KEY_OFF:����û����
*/
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
/*����Ƿ��а������� */
if (GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON ) {
/*�ȴ������ͷ� */
while (GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);
return KEY_ON;
} else
return KEY_OFF;
}

 int main(void)
{
 /* LED �˿ڳ�ʼ�� */
 LED_GPIO_Config();
 
 /*��ʼ������*/
 Key_GPIO_Config();
 
 /* ��ѯ����״̬��������������ת LED */
 while (1) {
 if ( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON ) {
 /*LED1 ��ת*/
 LED1_TOGGLE;
 }
 
 if ( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON ) {
 /*LED2 ��ת*/
 LED2_TOGGLE;
 }
}
}