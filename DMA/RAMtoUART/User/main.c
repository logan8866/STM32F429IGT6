#include "stdio.h"
#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_dma_uart.h"
#include "bsp_uart.h"

extern char SendBuff[SENDBUFF_SIZE];

int main(void)
{
uint16_t i;
/* ��ʼ�� USART */
USARTx_Config();

/* ����ʹ�� DMA ģʽ */
USART_DMA_Config();

/* ���� RGB ��ɫ�� */
LED_GPIO_Config();
LED_GREEN;
 
 printf("HELLO UART DMA TEST");
 
 /*��佫Ҫ���͵�����*/
 for (i=0; i<SENDBUFF_SIZE; i++) {
 SendBuff[i] = 'A'+i;
 }

 
 /* USART1 �� DMA ���� TX ���� */
 USART_DMACmd(DEBUG_USART, USART_DMAReq_Tx, ENABLE);
 
 /* ��ʱ CPU �ǿ��еģ����Ը����������� */
 //����ͬʱ���� LED
 while (1) {
 LED_BLUE;
 Delay(0xFFFFFF);
 LED_RED;
 Delay(0xFFFFFF);
 }
 }

 void TimingDelay_Decrement(void){
 }
