#include "bsp_clock.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"

 void HSE_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q)
 {
 __IO uint32_t HSEStartUpStatus = 0;
	RCC_DeInit();
 
 // ʹ�� HSE�������ⲿ���񣬱��� F429 ʹ�� HSE=25M
 RCC_HSEConfig(RCC_HSE_ON);
 
 // �ȴ� HSE �����ȶ�
 HSEStartUpStatus = RCC_WaitForHSEStartUp();
 
 if (HSEStartUpStatus == SUCCESS) {
 // ��ѹ����ѹ�����������Ϊ 1���Ա�������Ϊ���Ƶ��
 // ����ʱʹ���ܺ͹���ʵ��ƽ��
 RCC->APB1ENR |= RCC_APB1ENR_PWREN;
 PWR->CR |= PWR_CR_VOS;
 
 // HCLK = SYSCLK / 1
 RCC_HCLKConfig(RCC_SYSCLK_Div1);
 
 // PCLK2 = HCLK / 2
 RCC_PCLK2Config(RCC_HCLK_Div2);
 
// PCLK1 = HCLK / 4
 RCC_PCLK1Config(RCC_HCLK_Div4);
 
 // ���Ҫ��Ƶ�͵�������������
 // ���� PLL ��Դʱ�ӣ����� VCO ��Ƶ���� m������ VCO ��Ƶ���� n��
 // ����ϵͳʱ�ӷ�Ƶ���� p������ OTG FS,SDIO,RNG ��Ƶ���� q
 RCC_PLLConfig(RCC_PLLSource_HSE, m, n, p, q);
 
 // ʹ�� PLL
 RCC_PLLCmd(ENABLE);
 
 // �ȴ� PLL �ȶ�
 while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
 }
 
 /*-----------------------------------------------------*/
  //���� OVER-RIDE ģʽ�����ܴﵽ����Ƶ��
 PWR->CR |= PWR_CR_ODEN;
 while ((PWR->CSR & PWR_CSR_ODRDY) == 0) {
 }
 PWR->CR |= PWR_CR_ODSWEN;
 while ((PWR->CSR & PWR_CSR_ODSWRDY) == 0) {
 }
 // ���� FLASH Ԥȡָ,ָ���,���ݻ���͵ȴ�״̬
 FLASH->ACR = FLASH_ACR_PRFTEN
 | FLASH_ACR_ICEN
 | FLASH_ACR_DCEN
 | FLASH_ACR_LATENCY_5WS;
 /*-----------------------------------------------------*/
 
 // �� PLL �ȶ�֮�󣬰� PLL ʱ���л�Ϊϵͳʱ�� SYSCLK
 RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
 
 // ��ȡʱ���л�״̬λ��ȷ�� PLLCLK ��ѡΪϵͳʱ��
 while (RCC_GetSYSCLKSource() != 0x08) {
 }
 } else {
 // HSE ����������
 
 while (1) {
 }
 }
 }
 
 
void HSI_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q)
{
__IO uint32_t HSIStartUpStatus = 0;

// �� RCC �����ʼ���ɸ�λ״̬
RCC_DeInit();

//ʹ�� HSI, HSI=16M
RCC_HSICmd(ENABLE);

// �ȴ� HSI ����
HSIStartUpStatus = RCC->CR & RCC_CR_HSIRDY;

// ֻ�� HSI ����֮�����������ִ��
if (HSIStartUpStatus == RCC_CR_HSIRDY) {
// ��ѹ����ѹ�����������Ϊ 1���Ա�������Ϊ���Ƶ��
// ����ʱʹ���ܺ͹���ʵ��ƽ��
RCC->APB1ENR |= RCC_APB1ENR_PWREN;
PWR->CR |= PWR_CR_VOS;

// HCLK = SYSCLK / 1
RCC_HCLKConfig(RCC_SYSCLK_Div1);

// PCLK2 = HCLK / 2
RCC_PCLK2Config(RCC_HCLK_Div2);

// PCLK1 = HCLK / 4
RCC_PCLK1Config(RCC_HCLK_Div4);

// ���Ҫ��Ƶ�͵�������������
// ���� PLL ��Դʱ�ӣ����� VCO ��Ƶ���� m������ VCO ��Ƶ���� n��
// ����ϵͳʱ�ӷ�Ƶ���� p������ OTG FS,SDIO,RNG ��Ƶ���� q
RCC_PLLConfig(RCC_PLLSource_HSI, m, n, p, q);

// ʹ�� PLL
RCC_PLLCmd(ENABLE);

// �ȴ� PLL �ȶ�
while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
}

/*-----------------------------------------------------*/
//���� OVER-RIDE ģʽ�����ܴﵽ����Ƶ��
PWR->CR |= PWR_CR_ODEN;
while ((PWR->CSR & PWR_CSR_ODRDY) == 0) {
}
PWR->CR |= PWR_CR_ODSWEN;
while ((PWR->CSR & PWR_CSR_ODSWRDY) == 0) {
}
// ���� FLASH Ԥȡָ,ָ���,���ݻ���͵ȴ�״̬
FLASH->ACR = FLASH_ACR_PRFTEN
| FLASH_ACR_ICEN
|FLASH_ACR_DCEN
|FLASH_ACR_LATENCY_5WS;
/*-----------------------------------------------------*/

// �� PLL �ȶ�֮�󣬰� PLL ʱ���л�Ϊϵͳʱ�� SYSCLK
RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

// ��ȡʱ���л�״̬λ��ȷ�� PLLCLK ��ѡΪϵͳʱ��
while (RCC_GetSYSCLKSource() != 0x08) {
}
} else {
// HSI ����������
while (1) {
}
}
}


void Delay(__IO uint32_t nCount)
{
for (; nCount != 0; nCount--);
}


// MCO1 PA8 GPIO ��ʼ��
void MCO1_GPIO_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

// MCO1 GPIO ����
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// MCO2 PC9 GPIO ��ʼ��
void MCO2_GPIO_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

// MCO2 GPIO ����
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(GPIOC, &GPIO_InitStructure);
}
