#include "stm32f4xx_it.h"
#include "bsp_systick.h"
#include "bsp_led.h"

extern int TimingDelay;

int main(void)
{
/* LED 端口初始化 */
LED_GPIO_Config();

/* 配置 SysTick 为 10us 中断一次,时间到后触发定时中断，
*进入 stm32fxx_it.c 文件的 SysTick_Handler 处理，通过数中断次数计时
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
