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
 /*��ʼ�� USART1*/
 USARTx_Config();
 
 printf("\r\n ��ӭʹ�ñ��� STM32 F429 �����塣\r\n");
 
 printf("\r\n ����һ�� I2C ����(AT24C02)��д�������� \r\n");
	
 /* I2C ����(AT24C02)��ʼ�� */
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
printf("д�������");
uint8_t I2c_Buf_Write[256];
uint8_t I2c_Buf_Read[256];
 
 for ( i=0; i<=255; i++ ) //��仺��
 {
 I2c_Buf_Write[i] = i;
 
 printf("0x%02X ", I2c_Buf_Write[i]);
 if (i%16 == 15)
 printf("\n\r");
 }
 
 //�� I2c_Buf_Write ��˳�����������д�� EERPOM ��
 //ҳд�뷽ʽ
 // I2C_EE_BufferWrite( I2c_Buf_Write, EEP_Firstpage, 256);
 //�ֽ�д�뷽ʽ
 I2C_EE_ByetsWrite( I2c_Buf_Write, EEP_Firstpage, 256);
 
 printf("д����");
 
 printf("����������");
 //�� EEPROM ��������˳�򱣳ֵ� I2c_Buf_Read ��
 I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256);
 
 //�� I2c_Buf_Read �е�����ͨ�����ڴ�ӡ
 for (i=0; i<256; i++)
 {
 if (I2c_Buf_Read[i] != I2c_Buf_Write[i])
 {
 printf("0x%02X ", I2c_Buf_Read[i]);
 printf("����:I2C EEPROM д������������ݲ�һ��");
 return 0;
 }
 printf("0x%02X ", I2c_Buf_Read[i]);
 if (i%16 == 15)
 printf("\n\r");
 
 }
 printf("I2C(AT24C02)��д���Գɹ�");
 return 1;
 }

void TimingDelay_Decrement(void){
}

