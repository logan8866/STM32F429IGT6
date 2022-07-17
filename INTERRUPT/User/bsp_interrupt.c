#include "misc.h"
#include "bsp_interrupt.h"
#include "bsp_led.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"

static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* ���� NVIC Ϊ���ȼ��� 1 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* �����ж�Դ������ 1 */
	NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
	/* ������ռ���ȼ���1 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* ���������ȼ���1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/* ʹ���ж�ͨ�� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* �����ж�Դ������ 2������ʹ������������� */
	NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI_Key_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;

/*�������� GPIO �ڵ�ʱ��*/
RCC_AHB1PeriphClockCmd(KEY1_INT_GPIO_CLK|KEY2_INT_GPIO_CLK ,ENABLE);

/* ʹ�� SYSCFG ʱ�� ��ʹ�� GPIO �ⲿ�ж�ʱ����ʹ�� SYSCFG ʱ��*/
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
 
 /* ���� NVIC */
 NVIC_Configuration();
 
 /* ѡ�񰴼� 1 ������ */
 GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
 /* ��������Ϊ����ģʽ */
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
 /* �������Ų�����Ҳ������ */
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 /* ʹ������Ľṹ���ʼ������ */
 GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);
 
 /* ���� EXTI �ж�Դ �� key1 ���� */
 SYSCFG_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE,
 KEY1_INT_EXTI_PINSOURCE);
 
 /* ѡ�� EXTI �ж�Դ */
 EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
 /* �ж�ģʽ */
 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
 /* �½��ش��� */
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
 /* ʹ���ж�/�¼��� */
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 EXTI_Init(&EXTI_InitStructure);
 
 /* ѡ�񰴼� 2 ������ */
 GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
 /* ����������������ͬ */
 GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);
 
 /* ���� EXTI �ж�Դ �� key2 ���� */
 SYSCFG_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE,
 KEY2_INT_EXTI_PINSOURCE);
 
 /* ѡ�� EXTI �ж�Դ */
 EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
 /* �����ش��� */
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 EXTI_Init(&EXTI_InitStructure);
 }

 
void KEY1_IRQHandler(void)
{
//ȷ���Ƿ������ EXTI Line �ж�
if (EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) {
// LED1 ȡ��
LED1_TOGGLE;
//����жϱ�־λ
EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
}
}

void KEY2_IRQHandler(void)
{
//ȷ���Ƿ������ EXTI Line �ж�
if (EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) {
// LED2 ȡ��
LED2_TOGGLE;
//����жϱ�־λ
EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);
}
}
