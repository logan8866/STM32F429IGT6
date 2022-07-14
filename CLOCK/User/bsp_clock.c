#include "bsp_clock.h"
#include "stm32f4xx_rcc.h"

 /*
 * m: VCO 输入时钟 分频因子，取值 2~63
 * n: VCO 输出时钟 倍频因子，取值 192~432
 * p: SYSCLK 时钟分频因子 ，取值 2，4，6，8
 * q: OTG FS,SDIO,RNG 时钟分频因子，取值 4~15
 * 函数调用举例，使用 HSE 设置时钟
 * SYSCLK=HCLK=180M,PCLK2=HCLK/2=90M,PCLK1=HCLK/4=45M
 * HSE_SetSysClock(25, 360, 2, 7);
 * HSE 作为时钟来源，经过 PLL 倍频作为系统时钟，这是通常的做法
 
 * 系统时钟超频到 216M 爽一下
 * HSE_SetSysClock(25, 432, 2, 9);
 */
 void HSE_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q)
 {
 __IO uint32_t HSEStartUpStatus = 0;
 
 // 使能 HSE，开启外部晶振，秉火 F429 使用 HSE=25M
 RCC_HSEConfig(RCC_HSE_ON);
 
 // 等待 HSE 启动稳定
 HSEStartUpStatus = RCC_WaitForHSEStartUp();
 
 if (HSEStartUpStatus == SUCCESS) {
 // 调压器电压输出级别配置为 1，以便在器件为最大频率
 // 工作时使性能和功耗实现平衡
 RCC->APB1ENR |= RCC_APB1ENR_PWREN;
 PWR->CR |= PWR_CR_VOS;
 
 // HCLK = SYSCLK / 1
 RCC_HCLKConfig(RCC_SYSCLK_Div1);
 
 // PCLK2 = HCLK / 2
 RCC_PCLK2Config(RCC_HCLK_Div2);
 
// PCLK1 = HCLK / 4
 RCC_PCLK1Config(RCC_HCLK_Div4);
 
 // 如果要超频就得在这里下手啦
 // 设置 PLL 来源时钟，设置 VCO 分频因子 m，设置 VCO 倍频因子 n，
 // 设置系统时钟分频因子 p，设置 OTG FS,SDIO,RNG 分频因子 q
 RCC_PLLConfig(RCC_PLLSource_HSE, m, n, p, q);
 
 // 使能 PLL
 RCC_PLLCmd(ENABLE);
 
 // 等待 PLL 稳定
 while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
 }
 
 /*-----------------------------------------------------*/
  //开启 OVER-RIDE 模式，以能达到更高频率
 PWR->CR |= PWR_CR_ODEN;
 while ((PWR->CSR & PWR_CSR_ODRDY) == 0) {
 }
 PWR->CR |= PWR_CR_ODSWEN;
 while ((PWR->CSR & PWR_CSR_ODSWRDY) == 0) {
 }
 // 配置 FLASH 预取指,指令缓存,数据缓存和等待状态
 FLASH->ACR = FLASH_ACR_PRFTEN
 | FLASH_ACR_ICEN
 | FLASH_ACR_DCEN
 | FLASH_ACR_LATENCY_5WS;
 /*-----------------------------------------------------*/
 
 // 当 PLL 稳定之后，把 PLL 时钟切换为系统时钟 SYSCLK
 RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
 
 // 读取时钟切换状态位，确保 PLLCLK 被选为系统时钟
 while (RCC_GetSYSCLKSource() != 0x08) {
 }
 } else {
 // HSE 启动出错处理
 
 while (1) {
 }
 }
 }