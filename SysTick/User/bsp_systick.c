#include "stm32f4xx_it.h"
#include "bsp_systick.h"

int TimingDelay = 0;

void SysTick_Init(void)
{
/* SystemFrequency / 1000 1ms 中断一次
* SystemFrequency / 100000 10us 中断一次
 * SystemFrequency / 1000000 1us 中断一次
 */
 if (SysTick_Config(SystemCoreClock / 100000)) {
 /* Capture error */
 while (1);
 }
 }
void Delay_us(__IO u32 nTime)
{
TimingDelay = nTime;
 
 while (TimingDelay != 0);
 }

void TimingDelay_Decrement(void)
{
if (TimingDelay != 0x00) {
 TimingDelay--;
}
}
 

