#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_spi.h"
#include "bsp_led.h"
#include "bsp_uart.h"
#include "bsp_spi.h"
#include "stdio.h"

/* ��ȡ�������ĳ��� */
#define TxBufferSize1 (countof(TxBuffer1) - 1)
#define RxBufferSize1 (countof(TxBuffer1) - 1)
#define countof(a) (sizeof(a) / sizeof(*(a)))
#define BufferSize (countof(Tx_Buffer)-1)

#define FLASH_WriteAddress 0x00000
#define FLASH_ReadAddress FLASH_WriteAddress
#define FLASH_SectorToErase FLASH_WriteAddress
 
 
 /* ���ͻ�������ʼ�� */
 uint8_t Tx_Buffer[] = "��л��ѡ�ñ��� stm32 ������\r\n";
 uint8_t Rx_Buffer[BufferSize];
 
 //��ȡ�� ID �洢λ��
 __IO uint32_t DeviceID = 0;
 __IO uint32_t FlashID = 0;
 __IO TestStatus TransferStatus1 = FAILED;
 
 // ����ԭ������
 void Delay(__IO uint32_t nCount);
 
 /*
 * ��������main
 * ���� ��������
 * ���� ����
 * ��� ����
 */
 int main(void)
 {
 LED_GPIO_Config();
 LED_BLUE;
 
 /* ���ô��� 1 Ϊ��115200 8-N-1 */
 Debug_USART_Config();
 
 printf("\r\n ����һ�� 16M ���� flash(W25Q128)ʵ�� \r\n");
 
 /* 16M ���� flash W25Q128 ��ʼ�� */
 SPI_FLASH_Init();
 
 Delay( 200 );
 
 /* ��ȡ SPI Flash ID */
 FlashID = SPI_FLASH_ReadID();
 
 /* ���� SPI Flash ID */
 if (FlashID == sFLASH_ID)
 {
 printf("\r\n ��⵽ SPI FLASH W25Q128 !\r\n");
 
 /* ������Ҫд��� SPI FLASH ������FLASH д��ǰҪ�Ȳ��� */
 SPI_FLASH_SectorErase(FLASH_SectorToErase);
 
 /* �����ͻ�����������д�� flash �� */
 SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
 printf("\r\n д�������Ϊ��\r\n%s", Tx_Buffer);
 
 /* ���ո�д������ݶ������ŵ����ջ������� */
 SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
 printf("\r\n ����������Ϊ��\r\n%s", Rx_Buffer);
 
 /* ���д�������������������Ƿ���� */
 TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
 
 if ( PASSED == TransferStatus1 )
 {
 LED_GREEN;
 printf("\r\n16M ���� flash(W25Q128)���Գɹ�!\n\r");
 }
 else
 {
 LED_RED;
 printf("\r\n16M ���� flash(W25Q128)����ʧ��!\n\r");
 }
 }// if (FlashID == sFLASH_ID)
 else
 {
 LED_RED;
 printf("\r\n ��ȡ���� W25Q128 ID!\n\r");
 }
 
 SPI_Flash_PowerDown();
 while (1);
 }

void TimingDelay_Decrement(void){
}

