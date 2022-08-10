#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_spi.h"
#include "bsp_led.h"
#include "bsp_uart.h"
#include "bsp_spi.h"
#include "stdio.h"

/* 获取缓冲区的长度 */
#define TxBufferSize1 (countof(TxBuffer1) - 1)
#define RxBufferSize1 (countof(TxBuffer1) - 1)
#define countof(a) (sizeof(a) / sizeof(*(a)))
#define BufferSize (countof(Tx_Buffer)-1)

#define FLASH_WriteAddress 0x00000
#define FLASH_ReadAddress FLASH_WriteAddress
#define FLASH_SectorToErase FLASH_WriteAddress
 
 
 /* 发送缓冲区初始化 */
 uint8_t Tx_Buffer[] = "感谢您选用秉火 stm32 开发板\r\n";
 uint8_t Rx_Buffer[BufferSize];
 
 //读取的 ID 存储位置
 __IO uint32_t DeviceID = 0;
 __IO uint32_t FlashID = 0;
 __IO TestStatus TransferStatus1 = FAILED;
 
 // 函数原型声明
 void Delay(__IO uint32_t nCount);
 
 /*
 * 函数名：main
 * 描述 ：主函数
 * 输入 ：无
 * 输出 ：无
 */
 int main(void)
 {
 LED_GPIO_Config();
 LED_BLUE;
 
 /* 配置串口 1 为：115200 8-N-1 */
 Debug_USART_Config();
 
 printf("\r\n 这是一个 16M 串行 flash(W25Q128)实验 \r\n");
 
 /* 16M 串行 flash W25Q128 初始化 */
 SPI_FLASH_Init();
 
 Delay( 200 );
 
 /* 获取 SPI Flash ID */
 FlashID = SPI_FLASH_ReadID();
 
 /* 检验 SPI Flash ID */
 if (FlashID == sFLASH_ID)
 {
 printf("\r\n 检测到 SPI FLASH W25Q128 !\r\n");
 
 /* 擦除将要写入的 SPI FLASH 扇区，FLASH 写入前要先擦除 */
 SPI_FLASH_SectorErase(FLASH_SectorToErase);
 
 /* 将发送缓冲区的数据写到 flash 中 */
 SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
 printf("\r\n 写入的数据为：\r\n%s", Tx_Buffer);
 
 /* 将刚刚写入的数据读出来放到接收缓冲区中 */
 SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
 printf("\r\n 读出的数据为：\r\n%s", Rx_Buffer);
 
 /* 检查写入的数据与读出的数据是否相等 */
 TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
 
 if ( PASSED == TransferStatus1 )
 {
 LED_GREEN;
 printf("\r\n16M 串行 flash(W25Q128)测试成功!\n\r");
 }
 else
 {
 LED_RED;
 printf("\r\n16M 串行 flash(W25Q128)测试失败!\n\r");
 }
 }// if (FlashID == sFLASH_ID)
 else
 {
 LED_RED;
 printf("\r\n 获取不到 W25Q128 ID!\n\r");
 }
 
 SPI_Flash_PowerDown();
 while (1);
 }

void TimingDelay_Decrement(void){
}

