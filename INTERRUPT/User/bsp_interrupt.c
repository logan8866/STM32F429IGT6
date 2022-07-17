#include "misc.h"
#include "bsp_interrupt.h"
#include "bsp_led.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"

static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 配置 NVIC 为优先级组 1 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* 配置中断源：按键 1 */
	NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
	/* 配置抢占优先级：1 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* 配置子优先级：1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/* 使能中断通道 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* 配置中断源：按键 2，其他使用上面相关配置 */
	NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI_Key_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;

/*开启按键 GPIO 口的时钟*/
RCC_AHB1PeriphClockCmd(KEY1_INT_GPIO_CLK|KEY2_INT_GPIO_CLK ,ENABLE);

/* 使能 SYSCFG 时钟 ，使用 GPIO 外部中断时必须使能 SYSCFG 时钟*/
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
 
 /* 配置 NVIC */
 NVIC_Configuration();
 
 /* 选择按键 1 的引脚 */
 GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
 /* 设置引脚为输入模式 */
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
 /* 设置引脚不上拉也不下拉 */
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 /* 使用上面的结构体初始化按键 */
 GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);
 
 /* 连接 EXTI 中断源 到 key1 引脚 */
 SYSCFG_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE,
 KEY1_INT_EXTI_PINSOURCE);
 
 /* 选择 EXTI 中断源 */
 EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
 /* 中断模式 */
 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
 /* 下降沿触发 */
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
 /* 使能中断/事件线 */
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 EXTI_Init(&EXTI_InitStructure);
 
 /* 选择按键 2 的引脚 */
 GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
 /* 其他配置与上面相同 */
 GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);
 
 /* 连接 EXTI 中断源 到 key2 引脚 */
 SYSCFG_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE,
 KEY2_INT_EXTI_PINSOURCE);
 
 /* 选择 EXTI 中断源 */
 EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
 /* 上升沿触发 */
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 EXTI_Init(&EXTI_InitStructure);
 }

 
void KEY1_IRQHandler(void)
{
//确保是否产生了 EXTI Line 中断
if (EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) {
// LED1 取反
LED1_TOGGLE;
//清除中断标志位
EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
}
}

void KEY2_IRQHandler(void)
{
//确保是否产生了 EXTI Line 中断
if (EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) {
// LED2 取反
LED2_TOGGLE;
//清除中断标志位
EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);
}
}
