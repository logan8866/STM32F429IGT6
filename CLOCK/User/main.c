#include "bsp_clock.h"
#include "bsp_led.h"

int main(void)
{
// 程序来到 main 函数之前，启动文件：statup_stm32f10x_hd.s 已经调用
// SystemInit()函数把系统时钟初始化成 72MHZ
// SystemInit()在 system_stm32f10x.c 中定义
// 如果用户想修改系统时钟，可自行编写程序修改
// 重新设置系统时钟，这时候可以选择使用 HSE 还是 HSI

// 使用 HSE，配置系统时钟为 180M
 HSE_SetSysClock(25, 360, 2, 7);
 
 //系统时钟超频到 216M 爽一下，最高是 216M，别往死里整
//HSE_SetSysClock(25, 432, 2, 9);
 
 // 使用 HSI，配置系统时钟为 180M
 //HSI_SetSysClock(16, 360, 2, 7);
 
 // LED 端口初始化
 LED_GPIO_Config();

 // MCO GPIO 初始化
 MCO1_GPIO_Config();
 MCO2_GPIO_Config();
 
 // MCO1 输出 PLLCLK
 RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_1);
 
 // MCO2 输出 SYSCLK
 RCC_MCO2Config(RCC_MCO2Source_SYSCLK, RCC_MCO1Div_1);
 
 while (1) {
 LED1_ON; // 亮
 Delay(0x01FFFFF);
 LED1_OFF; // 灭
 Delay(0x01FFFFF);
 }
}

void TimingDelay_Decrement(void){
}
