#include "stm32f4xx_dma.h"
#include "bsp_DMA.h"
#include "bsp_led.h"

 /* 定义 aSRC_Const_Buffer 数组作为 DMA 传输数据源
 const 关键字将 aSRC_Const_Buffer 数组变量定义为常量类型 */
 const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
 0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
 0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
 0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
 0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
 0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
 0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
 0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
 0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80
 };
 /* 定义 DMA 传输目标存储器 */
 uint32_t aDST_Buffer[BUFFER_SIZE];
 
 
void Delay( uint32_t nCount)
{
for (; nCount != 0; nCount--);
}

void DMA_Config(void)
{
DMA_InitTypeDef DMA_InitStructure;
__IO uint32_t Timeout = TIMEOUT_MAX;

/* 使能 DMA 时钟 */
RCC_AHB1PeriphClockCmd(DMA_STREAM_CLOCK, ENABLE);

/* 复位初始化 DMA 数据流 */
 DMA_DeInit(DMA_STREAM);
 
 /* 确保 DMA 数据流复位完成 */
 while (DMA_GetCmdStatus(DMA_STREAM) != DISABLE) {
 }
 
 /* DMA 数据流通道选择 */
 DMA_InitStructure.DMA_Channel = DMA_CHANNEL;
 /* 源数据地址 */
 DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)aSRC_Const_Buffer;
 /* 目标地址 */
 DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)aDST_Buffer;
 /* 存储器到存储器模式 */
 DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;
 /* 数据数目 */
 DMA_InitStructure.DMA_BufferSize = (uint32_t)BUFFER_SIZE;
 /* 使能自动递增功能 */
 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
 /* 使能自动递增功能 */
 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 /* 源数据是字大小(32 位) */
 DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;
 /* 目标数据也是字大小(32 位) */
 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
 /* 一次传输模式，存储器到存储器模式不能使用循环传输 */
 DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
 /* DMA 数据流优先级为高 */
 DMA_InitStructure.DMA_Priority = DMA_Priority_High;
 /* 禁用 FIFO 模式 */
 DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
 DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
 /* 单次模式 */
 DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
 /* 单次模式 */
 DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
 /* 完成 DMA 数据流参数配置 */
 DMA_Init(DMA_STREAM, &DMA_InitStructure);

 /* 清除 DMA 数据流传输完成标志位 */
 DMA_ClearFlag(DMA_STREAM,DMA_FLAG_TCIF);
 
 /* 使能 DMA 数据流，开始 DMA 数据传输 */
 DMA_Cmd(DMA_STREAM, ENABLE);
 
 /* 检测 DMA 数据流是否有效并带有超时检测功能 */
 Timeout = TIMEOUT_MAX;
 while ((DMA_GetCmdStatus(DMA_STREAM) != ENABLE) && (Timeout-- > 0)) {
 }
 
 /* 判断是否超时 */
 if (Timeout == 0) {
 /* 超时就让程序运行下面循环：RGB 彩色灯闪烁 */
 while (1) {
 LED_RED;
 Delay(0xFFFFFF);
 LED_GREEN;
 Delay(0xFFFFFF);
 }
 }
 }
uint8_t Buffercmp(const uint32_t* pBuffer,
uint32_t* pBuffer1, uint16_t BufferLength)
{
/* 数据长度递减 */
while (BufferLength--) {
/* 判断两个数据源是否对应相等 */
if (*pBuffer != *pBuffer1) {
/* 对应数据源不相等马上退出函数，并返回 0 */
return 0;
 }
 /* 递增两个数据源的地址指针 */
 pBuffer++;
 pBuffer1++;
 }
 /* 完成判断并且对应数据相对 */
 return 1;
 }
