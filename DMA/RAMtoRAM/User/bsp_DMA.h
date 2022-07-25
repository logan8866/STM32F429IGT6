#include "stm32f4xx.h"
/* 相关宏定义，使用存储器到存储器传输必须使用 DMA2 */
#define DMA_STREAM DMA2_Stream0
#define DMA_CHANNEL DMA_Channel_0
#define DMA_STREAM_CLOCK RCC_AHB1Periph_DMA2
#define DMA_FLAG_TCIF DMA_FLAG_TCIF0

#define BUFFER_SIZE 32
#define TIMEOUT_MAX 10000 /* Maximum timeout value */
void Delay( uint32_t nCount);
void DMA_Config(void);
uint8_t Buffercmp(const uint32_t* pBuffer,uint32_t* pBuffer1, uint16_t BufferLength);
