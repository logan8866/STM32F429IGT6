#include "stm32f4xx.h"
#include "bsp_uart.h"
#include "bsp_led.h"
#include "stdio.h"

int main(void)
{
char ch;

/* 初始化 RGB 彩灯 */
LED_GPIO_Config();

/* 初始化 USART 配置模式为 115200 8-N-1 */
USARTx_Config();
 
 /* 打印指令输入提示信息 */
 Show_Message();
 while (1)
 {
 /* 获取字符指令 */
 ch=getchar();
 printf("接收到字符：%c\n",ch);
 
 /* 根据字符指令控制 RGB 彩灯颜色 */
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
 /* 如果不是指定指令字符，打印提示信息 */
 Show_Message();
 break;
 }
 }
 }

void TimingDelay_Decrement(){
}
