#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_i2c.h"
#include "bsp_i2c_eeprom.h"
#include "bsp_led.h"
#include "bsp_uart.h"
#include "stdio.h"
uint8_t I2C_Test(void);

int main(void)
{
LED_GPIO_Config();
 
 LED_BLUE;
 /*初始化 USART1*/
 USARTx_Config();
 
 printf("\r\n 欢迎使用秉火 STM32 F429 开发板。\r\n");
 
 printf("\r\n 这是一个 I2C 外设(AT24C02)读写测试例程 \r\n");
	
 /* I2C 外设(AT24C02)初始化 */
 I2C_EE_Init();
	
 uint8_t k = I2C_Test();
 
 if (k ==1)
 {
 LED_GREEN;
 }
 else
 {
 LED_RED;
 }
 
 while (1)
 {
 }
 
 }

uint8_t I2C_Test(void)
{
u16 i;
printf("写入的数据");
uint8_t I2c_Buf_Write[256];
uint8_t I2c_Buf_Read[256];
 
 for ( i=0; i<=255; i++ ) //填充缓冲
 {
 I2c_Buf_Write[i] = i;
 
 printf("0x%02X ", I2c_Buf_Write[i]);
 if (i%16 == 15)
 printf("\n\r");
 }
 
 //将 I2c_Buf_Write 中顺序递增的数据写入 EERPOM 中
 //页写入方式
 // I2C_EE_BufferWrite( I2c_Buf_Write, EEP_Firstpage, 256);
 //字节写入方式
 I2C_EE_ByetsWrite( I2c_Buf_Write, EEP_Firstpage, 256);
 
 printf("写结束");
 
 printf("读出的数据");
 //将 EEPROM 读出数据顺序保持到 I2c_Buf_Read 中
 I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256);
 
 //将 I2c_Buf_Read 中的数据通过串口打印
 for (i=0; i<256; i++)
 {
 if (I2c_Buf_Read[i] != I2c_Buf_Write[i])
 {
 printf("0x%02X ", I2c_Buf_Read[i]);
 printf("错误:I2C EEPROM 写入与读出的数据不一致");
 return 0;
 }
 printf("0x%02X ", I2c_Buf_Read[i]);
 if (i%16 == 15)
 printf("\n\r");
 
 }
 printf("I2C(AT24C02)读写测试成功");
 return 1;
 }

void TimingDelay_Decrement(void){
}

