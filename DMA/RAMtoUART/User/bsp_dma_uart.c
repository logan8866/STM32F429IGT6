#include "stm32f4xx_dma.h"
#include "stm32f4xx_usart.h"
#include "bsp_dma_uart.h"

char SendBuff[SENDBUFF_SIZE];

void USART_DMA_Config(void)
{
DMA_InitTypeDef DMA_InitStructure;

/*开启 DMA 时钟*/
RCC_AHB1PeriphClockCmd(DEBUG_USART_DMA_CLK, ENABLE);

/* 复位初始化 DMA 数据流 */
DMA_DeInit(DEBUG_USART_DMA_STREAM);

 /* 确保 DMA 数据流复位完成 */
 while (DMA_GetCmdStatus(DEBUG_USART_DMA_STREAM) != DISABLE) {
 }
 
 /*usart1 tx 对应 dma2，通道 4，数据流 7*/
 DMA_InitStructure.DMA_Channel = DEBUG_USART_DMA_CHANNEL;
 /*设置 DMA 源：串口数据寄存器地址*/
 DMA_InitStructure.DMA_PeripheralBaseAddr = DEBUG_USART_DR_BASE;
 /*内存地址(要传输的变量的指针)*/
 DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SendBuff;
 /*方向：从内存到外设*/
 DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
 /*传输大小 DMA_BufferSize=SENDBUFF_SIZE*/
 DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
 /*外设地址不增*/
 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
 /*内存地址自增*/
 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 /*外设数据单位*/
 DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
 /*内存数据单位 8bit*/
 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
 /*DMA 模式：不断循环*/
 DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
 /*优先级：中*/
 DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
 /*禁用 FIFO*/
 DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
 DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
 /*存储器突发传输 16 个节拍*/
 DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
 /*外设突发传输 1 个节拍*/
 DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
 /*配置 DMA2 的数据流 7*/
 DMA_Init(DEBUG_USART_DMA_STREAM, &DMA_InitStructure);
 
 /*使能 DMA*/
 DMA_Cmd(DEBUG_USART_DMA_STREAM, ENABLE);
 
 /* 等待 DMA 数据流有效*/
 while (DMA_GetCmdStatus(DEBUG_USART_DMA_STREAM) != ENABLE) {
 }
 }
void Delay( uint32_t nCount)
{
for (; nCount != 0; nCount--);
}