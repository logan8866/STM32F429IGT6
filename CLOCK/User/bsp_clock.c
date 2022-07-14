#include "bsp_clock.h"
#include "stm32f4xx_rcc.h"

 /*
 * m: VCO ����ʱ�� ��Ƶ���ӣ�ȡֵ 2~63
 * n: VCO ���ʱ�� ��Ƶ���ӣ�ȡֵ 192~432
 * p: SYSCLK ʱ�ӷ�Ƶ���� ��ȡֵ 2��4��6��8
 * q: OTG FS,SDIO,RNG ʱ�ӷ�Ƶ���ӣ�ȡֵ 4~15
 * �������þ�����ʹ�� HSE ����ʱ��
 * SYSCLK=HCLK=180M,PCLK2=HCLK/2=90M,PCLK1=HCLK/4=45M
 * HSE_SetSysClock(25, 360, 2, 7);
 * HSE ��Ϊʱ����Դ������ PLL ��Ƶ��Ϊϵͳʱ�ӣ�����ͨ��������
 
 * ϵͳʱ�ӳ�Ƶ�� 216M ˬһ��
 * HSE_SetSysClock(25, 432, 2, 9);
 */
 void HSE_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q)
 {
 __IO uint32_t HSEStartUpStatus = 0;
 
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