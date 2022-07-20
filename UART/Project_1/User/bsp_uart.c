#include "bsp_uart.h"
#include "stm32f4xx_it.h"

static void NVIC_Configuration(void)
{
NVIC_InitTypeDef NVIC_InitStructure;

/* Ƕ�������жϿ�������ѡ�� */
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

/* ���� USART Ϊ�ж�Դ */
NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
 /* �������ȼ�Ϊ 1 */
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
 /* �����ȼ�Ϊ 1 */
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
 /* ʹ���ж� */
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 /* ��ʼ������ NVIC */
 NVIC_Init(&NVIC_InitStructure);
 }

void Debug_USART_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
/* ʹ�� USART GPIO ʱ�� */
RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK |
DEBUG_USART_TX_GPIO_CLK,
ENABLE);

 /* ʹ�� USART ʱ�� */
 RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
 
 /* ���� PXx �� USARTx_Tx*/
 GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,
 DEBUG_USART_RX_SOURCE,
 DEBUG_USART_RX_AF);
 
 /* ���� PXx �� USARTx__Rx*/
 GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,
 DEBUG_USART_TX_SOURCE,
 DEBUG_USART_TX_AF);
 
 /* GPIO ��ʼ�� */
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 
 /* ���� Tx ����Ϊ���ù��� */
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN ;
 GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
 
 /* ���� Rx ����Ϊ���ù��� */
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
 GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
 
 /* ���ô� DEBUG_USART ģʽ */
 /* ���������ã�DEBUG_USART_BAUDRATE */
 USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
 /* �ֳ�(����λ+У��λ)��8 */
 USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 /* ֹͣλ��1 ��ֹͣλ */
 USART_InitStructure.USART_StopBits = USART_StopBits_1;
 /* У��λѡ�񣺲�ʹ��У�� */
 USART_InitStructure.USART_Parity = USART_Parity_No;
 /* Ӳ�������ƣ���ʹ��Ӳ���� */
 USART_InitStructure.USART_HardwareFlowControl =
 USART_HardwareFlowControl_None;
 /* USART ģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
 USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 /* ��� USART ��ʼ������ */
 USART_Init(DEBUG_USART, &USART_InitStructure);
 
 /* Ƕ�������жϿ����� NVIC ���� */
 NVIC_Configuration();
 
 /* ʹ�ܴ��ڽ����ж� */
 USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);
 
 /* ʹ�ܴ��� */
 USART_Cmd(DEBUG_USART, ENABLE);
 }

/***************** ����һ���ַ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
/* ����һ���ֽ����ݵ� USART */
USART_SendData(pUSARTx,ch);

/* �ȴ��������ݼĴ���Ϊ�� */
while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

 
 /***************** �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
 unsigned int k=0;
 do {
 Usart_SendByte( pUSARTx, *(str + k) );
 k++;
 } while (*(str + k)!='\0');
 
 /* �ȴ�������� */
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

