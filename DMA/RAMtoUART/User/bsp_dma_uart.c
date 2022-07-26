#include "stm32f4xx_dma.h"
#include "stm32f4xx_usart.h"
#include "bsp_dma_uart.h"

char SendBuff[SENDBUFF_SIZE];

void USART_DMA_Config(void)
{
DMA_InitTypeDef DMA_InitStructure;

/*���� DMA ʱ��*/
RCC_AHB1PeriphClockCmd(DEBUG_USART_DMA_CLK, ENABLE);

/* ��λ��ʼ�� DMA ������ */
DMA_DeInit(DEBUG_USART_DMA_STREAM);

 /* ȷ�� DMA ��������λ��� */
 while (DMA_GetCmdStatus(DEBUG_USART_DMA_STREAM) != DISABLE) {
 }
 
 /*usart1 tx ��Ӧ dma2��ͨ�� 4�������� 7*/
 DMA_InitStructure.DMA_Channel = DEBUG_USART_DMA_CHANNEL;
 /*���� DMA Դ���������ݼĴ�����ַ*/
 DMA_InitStructure.DMA_PeripheralBaseAddr = DEBUG_USART_DR_BASE;
 /*�ڴ��ַ(Ҫ����ı�����ָ��)*/
 DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SendBuff;
 /*���򣺴��ڴ浽����*/
 DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
 /*�����С DMA_BufferSize=SENDBUFF_SIZE*/
 DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
 /*�����ַ����*/
 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
 /*�ڴ��ַ����*/
 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 /*�������ݵ�λ*/
 DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
 /*�ڴ����ݵ�λ 8bit*/
 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
 /*DMA ģʽ������ѭ��*/
 DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
 /*���ȼ�����*/
 DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
 /*���� FIFO*/
 DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
 DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
 /*�洢��ͻ������ 16 ������*/
 DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
 /*����ͻ������ 1 ������*/
 DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
 /*���� DMA2 �������� 7*/
 DMA_Init(DEBUG_USART_DMA_STREAM, &DMA_InitStructure);
 
 /*ʹ�� DMA*/
 DMA_Cmd(DEBUG_USART_DMA_STREAM, ENABLE);
 
 /* �ȴ� DMA ��������Ч*/
 while (DMA_GetCmdStatus(DEBUG_USART_DMA_STREAM) != ENABLE) {
 }
 }
void Delay( uint32_t nCount)
{
for (; nCount != 0; nCount--);
}