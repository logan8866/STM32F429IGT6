#include "stm32f4xx.h"
void HSE_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q);
void HSI_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q);
void Delay(__IO uint32_t nCount);
void MCO1_GPIO_Config(void);
void MCO2_GPIO_Config(void);