#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_DMA.h"

extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
extern uint32_t aDST_Buffer[BUFFER_SIZE];

int main(void)
{
/* 定义存放比较结果变量 */
uint8_t TransferStatus;

/* LED 端口初始化 */
LED_GPIO_Config();

/* 设置 RGB 彩色灯为紫色 */
 LED_GREEN;
 
 /* 简单延时函数 */
 Delay(0xFFFFFF);
 
 /* DMA 传输配置 */
 DMA_Config();
 
 /* 等待 DMA 传输完成 */
 while (DMA_GetFlagStatus(DMA_STREAM,DMA_FLAG_TCIF)==DISABLE) {
 
 }
 
 /* 比较源数据与传输后数据 */
 TransferStatus=Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE);
 
 /* 判断源数据与传输后数据比较结果*/
 if (TransferStatus==0) { 
 /* 源数据与传输后数据不相等时 RGB 彩色灯显示红色 */
 LED_RED;
 } else {
 /* 源数据与传输后数据相等时 RGB 彩色灯显示蓝色 */
 LED_BLUE;
 }
 
 while (1) {
 }
 }

void TimingDelay_Decrement(void){
}
