#include "bsp_uart.h"
#include "stm32f4xx_it.h"
#include "stdio.h"

static void NVIC_Configuration(void)
{
NVIC_InitTypeDef NVIC_InitStructure;

/* Ƕ�������жϿ�������ѡ�� */
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

/* ���� USART Ϊ�ж�Դ */
NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQ;
 /* �������ȼ�Ϊ 1 */
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
 /* �����ȼ�Ϊ 1 */
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
 /* ʹ���ж� */
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 /* ��ʼ������ NVIC */
 NVIC_Init(&NVIC_InitStructure);
 }

void USARTx_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
/* ʹ�� USART GPIO ʱ�� */
RCC_AHB1PeriphClockCmd(USARTx_RX_GPIO_CLK |
USARTx_TX_GPIO_CLK,
ENABLE);

 /* ʹ�� USART ʱ�� */
 RCC_APB2PeriphClockCmd(USARTx_CLK, ENABLE);
 
 /* ���� PXx �� USARTx_Tx*/
 GPIO_PinAFConfig(USARTx_RX_GPIO_PORT,
 USARTx_RX_SOURCE,
 USARTx_RX_AF);
 
 /* ���� PXx �� USARTx__Rx*/
 GPIO_PinAFConfig(USARTx_TX_GPIO_PORT,
 USARTx_TX_SOURCE,
 USARTx_TX_AF);
 
 /* GPIO ��ʼ�� */
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 
 /* ���� Tx ����Ϊ���ù��� */
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN ;
 GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure);
 
 /* ���� Rx ����Ϊ���ù��� */
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
 GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);
 
 /* ���ô� USARTx ģʽ */
 /* ���������ã�USARTx_BAUDRATE */
 USART_InitStructure.USART_BaudRate = USARTx_BAUDRATE;
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
 USART_Init(USARTx, &USART_InitStructure);
 
 /* Ƕ�������жϿ����� NVIC ���� */
 NVIC_Configuration();
 
 /* ʹ�ܴ��ڽ����ж� */
 USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
 
 /* ʹ�ܴ��� */
 USART_Cmd(USARTx, ENABLE);
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
void USARTx_IRQHandler(void)
{
uint8_t ucTemp;
if (USART_GetITStatus(USARTx,USART_IT_RXNE)!=RESET) {
ucTemp = USART_ReceiveData( USARTx );
USART_SendData(USARTx,ucTemp);
}
}
 
///�ض��� c �⺯�� printf �����ڣ��ض�����ʹ�� printf ����
int fputc(int ch, FILE *f)
{
/* ����һ���ֽ����ݵ����� */
USART_SendData(USARTx, (uint8_t) ch);

/* �ȴ�������� */
while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);

return (ch);
}
 
///�ض��� c �⺯�� scanf �����ڣ���д����ʹ�� scanf��getchar �Ⱥ���
int fgetc(FILE *f)
{
/* �ȴ������������� */
while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);

return (int)USART_ReceiveData(USARTx);
}

void Show_Message(void)
{
printf("\r\n ����һ��ͨ������ͨ��ָ����� RGB �ʵ�ʵ�� \n");
printf("ʹ�� USART1 ����Ϊ��%d 8-N-1 \n",USARTx_BAUDRATE);
printf("������ӵ�ָ������ RGB �ʵ���ɫ��ָ���Ӧ���£�\n");
printf(" ָ�� ------ �ʵ���ɫ \n");
printf(" 1 ------ �� \n");
printf(" 2 ------ �� \n");
printf(" 3 ------ �� \n");
}
