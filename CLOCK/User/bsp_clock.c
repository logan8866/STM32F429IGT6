#include "bsp_clock.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"

 void HSE_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q)
 {
 __IO uint32_t HSEStartUpStatus = 0;
	RCC_DeInit();
 
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
 
 
void HSI_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q)
{
__IO uint32_t HSIStartUpStatus = 0;

// 把 RCC 外设初始化成复位状态
RCC_DeInit();

//使能 HSI, HSI=16M
RCC_HSICmd(ENABLE);

// 等待 HSI 就绪
HSIStartUpStatus = RCC->CR & RCC_CR_HSIRDY;

// 只有 HSI 就绪之后则继续往下执行
if (HSIStartUpStatus == RCC_CR_HSIRDY) {
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
RCC_PLLConfig(RCC_PLLSource_HSI, m, n, p, q);

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
|FLASH_ACR_DCEN
|FLASH_ACR_LATENCY_5WS;
/*-----------------------------------------------------*/

// 当 PLL 稳定之后，把 PLL 时钟切换为系统时钟 SYSCLK
RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

// 读取时钟切换状态位，确保 PLLCLK 被选为系统时钟
while (RCC_GetSYSCLKSource() != 0x08) {
}
} else {
// HSI 启动出错处理
while (1) {
}
}
}


void Delay(__IO uint32_t nCount)
{
for (; nCount != 0; nCount--);
}


// MCO1 PA8 GPIO 初始化
void MCO1_GPIO_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

// MCO1 GPIO 配置
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// MCO2 PC9 GPIO 初始化
void MCO2_GPIO_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

// MCO2 GPIO 配置
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(GPIOC, &GPIO_InitStructure);
}
