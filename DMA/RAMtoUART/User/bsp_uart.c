#include "bsp_uart.h"
#include "stm32f4xx_it.h"

static void NVIC_Configuration(void)
{
NVIC_InitTypeDef NVIC_InitStructure;

/* 嵌套向量中断控制器组选择 */
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

/* 配置 USART 为中断源 */
NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
 /* 抢断优先级为 1 */
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
 /* 子优先级为 1 */
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
 /* 使能中断 */
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 /* 初始化配置 NVIC */
 NVIC_Init(&NVIC_InitStructure);
 }

void Debug_USART_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
/* 使能 USART GPIO 时钟 */
RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK |
DEBUG_USART_TX_GPIO_CLK,
ENABLE);

 /* 使能 USART 时钟 */
 RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
 
 /* 连接 PXx 到 USARTx_Tx*/
 GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,
 DEBUG_USART_RX_SOURCE,
 DEBUG_USART_RX_AF);
 
 /* 连接 PXx 到 USARTx__Rx*/
 GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,
 DEBUG_USART_TX_SOURCE,
 DEBUG_USART_TX_AF);
 
 /* GPIO 初始化 */
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 
 /* 配置 Tx 引脚为复用功能 */
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN ;
 GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
 
 /* 配置 Rx 引脚为复用功能 */
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
 GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
 
 /* 配置串 DEBUG_USART 模式 */
 /* 波特率设置：DEBUG_USART_BAUDRATE */
 USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
 /* 字长(数据位+校验位)：8 */
 USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 /* 停止位：1 个停止位 */
 USART_InitStructure.USART_StopBits = USART_StopBits_1;
 /* 校验位选择：不使用校验 */
 USART_InitStructure.USART_Parity = USART_Parity_No;
 /* 硬件流控制：不使用硬件流 */
 USART_InitStructure.USART_HardwareFlowControl =
 USART_HardwareFlowControl_None;
 /* USART 模式控制：同时使能接收和发送 */
 USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 /* 完成 USART 初始化配置 */
 USART_Init(DEBUG_USART, &USART_InitStructure);
 
 /* 嵌套向量中断控制器 NVIC 配置 */
 NVIC_Configuration();
 
 /* 使能串口接收中断 */
 USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);
 
 /* 使能串口 */
 USART_Cmd(DEBUG_USART, ENABLE);
 }

/***************** 发送一个字符 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
/* 发送一个字节数据到 USART */
USART_SendData(pUSARTx,ch);

/* 等待发送数据寄存器为空 */
while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

 
 /***************** 发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
 unsigned int k=0;
 do {
 Usart_SendByte( pUSARTx, *(str + k) );
 k++;
 } while (*(str + k)!='\0');
 
 /* 等待发送完成 */
 while (USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET) {
 }
}
 void DEBUG_USART_IRQHandler(void)
 {
 uint8_t ucTemp;
 if (USART_GetITStatus(DEBUG_USART,USART_IT_RXNE)!=RESET) {
 ucTemp = USART_ReceiveData( DEBUG_USART );
 USART_SendData(DEBUG_USART,ucTemp);
 }
 
 }

