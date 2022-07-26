#include "stm32f4xx_it.h"

//���Ŷ���
/*******************************************************/
#define USARTx USART1

/* ��ͬ�Ĵ��ڹ��ص����߲�һ����ʱ��ʹ�ܺ���Ҳ��һ������ֲʱҪע��
* ���� 1 �� 6 �� RCC_APB2PeriphClockCmd
* ���� 2/3/4/5/7 �� RCC_APB1PeriphClockCmd
*/
#define USARTx_CLK RCC_APB2Periph_USART1
 #define USARTx_CLOCKCMD RCC_APB2PeriphClockCmd
 #define USARTx_BAUDRATE 115200 //���ڲ�����

 
 #define USARTx_RX_GPIO_PORT GPIOA
 #define USARTx_RX_GPIO_CLK RCC_AHB1Periph_GPIOA
 #define USARTx_RX_PIN GPIO_Pin_10
 #define USARTx_RX_AF GPIO_AF_USART1
 #define USARTx_RX_SOURCE GPIO_PinSource10
 
 #define USARTx_TX_GPIO_PORT GPIOA
 #define USARTx_TX_GPIO_CLK RCC_AHB1Periph_GPIOA
 #define USARTx_TX_PIN GPIO_Pin_9
 #define USARTx_TX_AF GPIO_AF_USART1
 #define USARTx_TX_SOURCE GPIO_PinSource9
 
 /************************************************************/

#define USARTx_IRQHandler USART1_IRQHandler
#define USARTx_IRQ USART1_IRQn
 
static void NVIC_Configuration(void);
void USARTx_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void DEBUG_USART_IRQHandler(void);
void Show_Message(void);
