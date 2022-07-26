#include "stdio.h"
#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_dma_uart.h"
#include "bsp_uart.h"

extern char SendBuff[SENDBUFF_SIZE];

int main(void)
{
uint16_t i;
/* 初始化 USART */
USARTx_Config();

/* 配置使用 DMA 模式 */
USART_DMA_Config();

/* 配置 RGB 彩色灯 */
LED_GPIO_Config();
LED_GREEN;
 
 printf("HELLO UART DMA TEST");
 
 /*填充将要发送的数据*/
 for (i=0; i<SENDBUFF_SIZE; i++) {
 SendBuff[i] = 'A'+i;
 }

 
 /* USART1 向 DMA 发出 TX 请求 */
 USART_DMACmd(DEBUG_USART, USART_DMAReq_Tx, ENABLE);
 
 /* 此时 CPU 是空闲的，可以干其他的事情 */
 //例如同时控制 LED
 while (1) {
 LED_BLUE;
 Delay(0xFFFFFF);
 LED_RED;
 Delay(0xFFFFFF);
 }
 }

 void TimingDelay_Decrement(void){
 }
