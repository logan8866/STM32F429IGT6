#include "bsp_interrupt.h"
#include "bsp_led.h"

int main(void)
{
/* LED 端口初始化 */
LED_GPIO_Config();

/* 初始化 EXTI 中断，按下按键会触发中断，
* 触发中断会进入 stm32f4xx_it.c 文件中的函数
* KEY1_IRQHandler 和 KEY2_IRQHandler，处理中断，反转 LED 灯。
*/
EXTI_Key_Config();

/* 等待中断，由于使用中断方式，CPU 不用轮询按键 */
while (1) {
}
}

void TimingDelay_Decrement(void){
}
