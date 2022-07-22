#include "bsp_uart.h"
#include "stm32f4xx_it.h"
#include "stdio.h"

static void NVIC_Configuration(void)
{
NVIC_InitTypeDef NVIC_InitStructure;

/* 嵌套向量中断控制器组选择 */
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

/* 配置 USART 为中断源 */
NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQ;
 /* 抢断优先级为 1 */
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
 /* 子优先级为 1 */
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
 /* 使能中断 */
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 /* 初始化配置 NVIC */
 NVIC_Init(&NVIC_InitStructure);
 }

void USARTx_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
/* 使能 USART GPIO 时钟 */
RCC_AHB1PeriphClockCmd(USARTx_RX_GPIO_CLK |
USARTx_TX_GPIO_CLK,
ENABLE);

 /* 使能 USART 时钟 */
 RCC_APB2PeriphClockCmd(USARTx_CLK, ENABLE);
 
 /* 连接 PXx 到 USARTx_Tx*/
 GPIO_PinAFConfig(USARTx_RX_GPIO_PORT,
 USARTx_RX_SOURCE,
 USARTx_RX_AF);
 
 /* 连接 PXx 到 USARTx__Rx*/
 GPIO_PinAFConfig(USARTx_TX_GPIO_PORT,
 USARTx_TX_SOURCE,
 USARTx_TX_AF);
 
 /* GPIO 初始化 */
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 
 /* 配置 Tx 引脚为复用功能 */
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN ;
 GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure);
 
 /* 配置 Rx 引脚为复用功能 */
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
 GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);
 
 /* 配置串 USARTx 模式 */
 /* 波特率设置：USARTx_BAUDRATE */
 USART_InitStructure.USART_BaudRate = USARTx_BAUDRATE;
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
 USART_Init(USARTx, &USART_InitStructure);
 
 /* 嵌套向量中断控制器 NVIC 配置 */
 NVIC_Configuration();
 
 /* 使能串口接收中断 */
 USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
 
 /* 使能串口 */
 USART_Cmd(USARTx, ENABLE);
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
void USARTx_IRQHandler(void)
{
uint8_t ucTemp;
if (USART_GetITStatus(USARTx,USART_IT_RXNE)!=RESET) {
ucTemp = USART_ReceiveData( USARTx );
USART_SendData(USARTx,ucTemp);
}
}
 
///重定向 c 库函数 printf 到串口，重定向后可使用 printf 函数
int fputc(int ch, FILE *f)
{
/* 发送一个字节数据到串口 */
USART_SendData(USARTx, (uint8_t) ch);

/* 等待发送完毕 */
while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);

return (ch);
}
 
///重定向 c 库函数 scanf 到串口，重写向后可使用 scanf、getchar 等函数
int fgetc(FILE *f)
{
/* 等待串口输入数据 */
while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);

return (int)USART_ReceiveData(USARTx);
}

void Show_Message(void)
{
printf("\r\n 这是一个通过串口通信指令控制 RGB 彩灯实验 \n");
printf("使用 USART1 参数为：%d 8-N-1 \n",USARTx_BAUDRATE);
printf("开发板接到指令后控制 RGB 彩灯颜色，指令对应如下：\n");
printf(" 指令 ------ 彩灯颜色 \n");
printf(" 1 ------ 红 \n");
printf(" 2 ------ 绿 \n");
printf(" 3 ------ 蓝 \n");
}
