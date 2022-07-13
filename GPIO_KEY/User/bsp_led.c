#include "stm32f4xx_rcc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "bsp_led.h"


 void LED_GPIO_Config(void)
 {
 GPIO_InitTypeDef GPIO_InitStructure;

 RCC_AHB1PeriphClockCmd ( LED1_GPIO_CLK|
 LED2_GPIO_CLK|
 LED3_GPIO_CLK|
 LED4_GPIO_CLK,
 ENABLE);
 
 GPIO_InitStructure.GPIO_Pin = LED1_PIN;
 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 

 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 

 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 
 GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
 
 GPIO_InitStructure.GPIO_Pin = LED2_PIN;
 GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
 
 GPIO_InitStructure.GPIO_Pin = LED3_PIN;
 GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = LED4_PIN;
 GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStructure);

 LED_RED;
 
 LED4_ON;
 }
 