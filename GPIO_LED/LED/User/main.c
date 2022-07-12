#include "stm32f4xx.h"
#include "bsp_led.h"
#include "stm32f4xx_gpio.h"
 void Delay(__IO uint32_t nCount);
int main(void)
 {
 /* LED ????? */
 LED_GPIO_Config();
 
 /* ?? LED ? */
 while (1) {
 LED1_ON; // ?
 Delay(0xFFFFFF);
 LED1_OFF; // ?
 
 LED2_ON; // ?
 Delay(0xFFFFFF);
 LED2_OFF; // ?
 
 LED3_ON; // ?
 Delay(0xFFFFFF);
 LED3_OFF; // ?
 
 LED4_ON; // ?
 Delay(0xFFFFFF);
 LED4_OFF; // ?
 
 /*???? ??????? ??*/
 LED_RED;
 Delay(0xFFFFFF);
 
 LED_GREEN;
 Delay(0xFFFFFF);
 
 LED_BLUE;
 Delay(0xFFFFFF);
 
 LED_YELLOW;
 Delay(0xFFFFFF);
}
 }

 
 void Delay(__IO uint32_t nCount)
 {
 for (; nCount != 0; nCount--);
 }