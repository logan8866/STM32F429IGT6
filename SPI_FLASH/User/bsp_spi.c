#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_flash.h"
#include "bsp_spi.h"
#include "stdio.h"
__IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;
/**
* @brief SPI_FLASH ��ʼ��
* @param ��
* @retval ��
*/
void SPI_FLASH_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
 
 /* ʹ�� FLASH_SPI �� GPIO ʱ�� */
 /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO,
 SPI_FLASH_SPI_MISO_GPIO �� SPI_FLASH_SPI_SCK_GPIO ʱ��ʹ�� */
 RCC_AHB1PeriphClockCmd (FLASH_SPI_SCK_GPIO_CLK | FLASH_SPI_MISO_GPIO_CLK|
 FLASH_SPI_MOSI_GPIO_CLK|FLASH_CS_GPIO_CLK, ENABLE);
 
 /*!< SPI_FLASH_SPI ʱ��ʹ�� */
 FLASH_SPI_CLK_INIT(FLASH_SPI_CLK, ENABLE);
 
 //�������Ÿ���
 GPIO_PinAFConfig(FLASH_SPI_SCK_GPIO_PORT,FLASH_SPI_SCK_PINSOURCE,
 FLASH_SPI_SCK_AF);
 GPIO_PinAFConfig(FLASH_SPI_MISO_GPIO_PORT,FLASH_SPI_MISO_PINSOURCE,
 FLASH_SPI_MISO_AF);
 GPIO_PinAFConfig(FLASH_SPI_MOSI_GPIO_PORT,FLASH_SPI_MOSI_PINSOURCE,
 FLASH_SPI_MOSI_AF);
 
 /*!< ���� SPI_FLASH_SPI ����: SCK */
 GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 
 GPIO_Init(FLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
 
 /*!< ���� SPI_FLASH_SPI ����: MISO */
 GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
 GPIO_Init(FLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
 
 /*!< ���� SPI_FLASH_SPI ����: MOSI */
 GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
 GPIO_Init(FLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
 
 /*!< ���� SPI_FLASH_SPI ����: CS */
 GPIO_InitStructure.GPIO_Pin = FLASH_CS_PIN;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStructure);
 
 /* ֹͣ�ź� FLASH: CS ���Ÿߵ�ƽ*/
 SPI_FLASH_CS_HIGH();
 
 SPI_InitTypeDef SPI_InitStructure;
 /* FLASH_SPI ģʽ���� */
 // FLASH оƬ ֧�� SPI ģʽ 0 ��ģʽ 3���ݴ����� CPOL CPHA
 SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
 SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
 SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
 SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
 SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
 SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
 SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
 SPI_InitStructure.SPI_CRCPolynomial = 7;
 SPI_Init(FLASH_SPI, &SPI_InitStructure);
 
 /* ʹ�� FLASH_SPI */
 SPI_Cmd(FLASH_SPI, ENABLE);
}
/////////////////////////////////////////////////////////////////////////////////////
u8 SPI_FLASH_SendByte(u8 byte)
{
 uint32_t SPITimeout = SPIT_FLAG_TIMEOUT;
 
 /* �ȴ����ͻ�����Ϊ�գ�TXE �¼� */
 while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET)
 {
 if ((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
 }
 
 /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
 SPI_I2S_SendData(FLASH_SPI, byte);
 
 SPITimeout = SPIT_FLAG_TIMEOUT;
 
 /* �ȴ����ջ������ǿգ�RXNE �¼� */
 while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET)
 {
 if ((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
 }
 
 /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
 return SPI_I2S_ReceiveData(FLASH_SPI);
 }

 /////////////////////////////////////////////////////////////////////////////////
 u8 SPI_FLASH_ReadByte(void)
 {
 return (SPI_FLASH_SendByte(Dummy_Byte));
 }

///////////////////////////////////////////////////////////////////////////////////
u32 SPI_FLASH_ReadID(void)
{
u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

 /* ��ʼͨѶ��CS �͵�ƽ */
 SPI_FLASH_CS_LOW();
 
 /* ���� JEDEC ָ���ȡ ID */
 SPI_FLASH_SendByte(W25X_JedecDeviceID);
 
 /* ��ȡһ���ֽ����� */
 Temp0 = SPI_FLASH_SendByte(Dummy_Byte);
 printf("%d\n",Temp0);
 
 /* ��ȡһ���ֽ����� */
 Temp1 = SPI_FLASH_SendByte(Dummy_Byte);
	printf("%d\n",Temp1);
 
 /* ��ȡһ���ֽ����� */
 Temp2 = SPI_FLASH_SendByte(Dummy_Byte);
	printf("%d\n",Temp2);
 
 /* ֹͣͨѶ��CS �ߵ�ƽ */
 SPI_FLASH_CS_HIGH();
 
 /*�����������������Ϊ�����ķ���ֵ*/
 Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
 
 return Temp;
 }
//////////////////////////////////////////////////////////////////////////
 u32 SPI_FLASH_ReadDeviceID(void)
{
  u32 Temp = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  SPI_FLASH_SendByte(W25X_DeviceID);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  
  /* Read a byte from the FLASH */
  Temp = SPI_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  return Temp;
}
////////////////////////////////////////////////////////
void SPI_FLASH_WriteEnable(void)
{
/* ͨѶ��ʼ��CS �� */
SPI_FLASH_CS_LOW();
 
 /* ����дʹ������*/
 SPI_FLASH_SendByte(W25X_WriteEnable);
 
 /*ͨѶ������CS �� */
 SPI_FLASH_CS_HIGH();
 }

/////////////////////////////////////////////////////////////////////
void SPI_FLASH_WaitForWriteEnd(void)
 {
 u8 FLASH_Status = 0;
 /* ѡ�� FLASH: CS �� */
 SPI_FLASH_CS_LOW();
 
 /* ���� ��״̬�Ĵ��� ���� */
 SPI_FLASH_SendByte(W25X_ReadStatusReg);
 
 uint32_t SPITimeout = SPIT_FLAG_TIMEOUT;
 /* �� FLASH æµ����ȴ� */
 do
 {
 /* ��ȡ FLASH оƬ��״̬�Ĵ��� */
 FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
 if ((SPITimeout--) == 0)
 {
 SPI_TIMEOUT_UserCallback(4);
 return;
 }
 }
 while ((FLASH_Status & WIP_Flag) == SET); /* ����д���־ */
 
 /* ֹͣ�ź� FLASH: CS �� */
 SPI_FLASH_CS_HIGH();
 }
//////////////////////////////////////////////////////////////////
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
/* ���� FLASH дʹ������ */
SPI_FLASH_WriteEnable();
 SPI_FLASH_WaitForWriteEnd();
 /* �������� */
 /* ѡ�� FLASH: CS �͵�ƽ */
 SPI_FLASH_CS_LOW();
 /* ������������ָ��*/
 SPI_FLASH_SendByte(W25X_SectorErase);
 /*���Ͳ���������ַ�ĸ�λ*/
 SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
 /* ���Ͳ���������ַ����λ */
 SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
 /* ���Ͳ���������ַ�ĵ�λ */
 SPI_FLASH_SendByte(SectorAddr & 0xFF);
 /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
 SPI_FLASH_CS_HIGH();
 /* �ȴ��������*/
 SPI_FLASH_WaitForWriteEnd();
 }

///////////////////////////////////////////////////////////////////////////////////////
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
 /* ���� FLASH дʹ������ */
 SPI_FLASH_WriteEnable();
 
 /* ѡ�� FLASH: CS �͵�ƽ */
 SPI_FLASH_CS_LOW();
 /* д��дָ��*/
 SPI_FLASH_SendByte(W25X_PageProgram);
 /*����д��ַ�ĸ�λ*/
 SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
 /*����д��ַ����λ*/
 SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
 /*����д��ַ�ĵ�λ*/
 SPI_FLASH_SendByte(WriteAddr & 0xFF);
 
 if (NumByteToWrite > SPI_FLASH_PerWritePageSize)
 {
 NumByteToWrite = SPI_FLASH_PerWritePageSize;
 printf("SPI_FLASH_PageWrite too large!");
 }
 
 /* д������*/
 while (NumByteToWrite--)
 {
 /* ���͵�ǰҪд����ֽ����� */
 SPI_FLASH_SendByte(*pBuffer);
 /* ָ����һ�ֽ����� */
 pBuffer++;
 }
 
 /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
 SPI_FLASH_CS_HIGH();
 
 /* �ȴ�д�����*/
 SPI_FLASH_WaitForWriteEnd();
 }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
 u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
 
 /*mod �������࣬�� writeAddr �� SPI_FLASH_PageSize �������������� Addr ֵΪAddr = WriteAddr % SPI_FLASH_PageSize;�� count ������ֵ���պÿ��Զ��뵽ҳ��ַ*/
 Addr = WriteAddr % SPI_FLASH_PageSize;
	count = SPI_FLASH_PageSize - Addr;
 /*�����Ҫд��������ҳ*/
 NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
 /*mod �������࣬�����ʣ�಻��һҳ���ֽ���*/
 NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
 
 /* Addr=0,�� WriteAddr �պð�ҳ���� aligned */
 if (Addr == 0)
 {
 /* NumByteToWrite < SPI_FLASH_PageSize */
 if (NumOfPage == 0)
 {
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
 }
 else /* NumByteToWrite > SPI_FLASH_PageSize */
 {
 /*�Ȱ�����ҳ��д��*/
 while (NumOfPage--)
 {
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
 WriteAddr += SPI_FLASH_PageSize;
 pBuffer += SPI_FLASH_PageSize;
 }
 
 /*���ж���Ĳ���һҳ�����ݣ�����д��*/
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
 }
 }
 /* ����ַ�� SPI_FLASH_PageSize ������ */
 else
 {
 /* NumByteToWrite < SPI_FLASH_PageSize */
 if (NumOfPage == 0)
 {
 /*��ǰҳʣ��� count ��λ�ñ� NumOfSingle С��д����*/
 if (NumOfSingle > count)
 {
 temp = NumOfSingle - count;
 
 /*��д����ǰҳ*/
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
 WriteAddr += count;
 pBuffer += count;
 
 /*��дʣ�������*/
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
 }
 else /*��ǰҳʣ��� count ��λ����д�� NumOfSingle ������*/
 {
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
 }
 }
 else /* NumByteToWrite > SPI_FLASH_PageSize */
 {
 /*��ַ���������� count �ֿ������������������*/
 NumByteToWrite -= count;
 NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
 NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
 
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
 WriteAddr += count;
 pBuffer += count;
 
 /*������ҳ��д��*/
 while (NumOfPage--)
 {
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
 WriteAddr += SPI_FLASH_PageSize;
 pBuffer += SPI_FLASH_PageSize;
 }
 /*���ж���Ĳ���һҳ�����ݣ�����д��*/
 if (NumOfSingle != 0)
 {
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
 }
 }
 }
 }
////////////////////////////////////////////////////////////////////////////////////
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
 /* ѡ�� FLASH: CS �͵�ƽ */
 SPI_FLASH_CS_LOW();
 
 /* ���� �� ָ�� */
 SPI_FLASH_SendByte(W25X_ReadData);
 
 /* ���� �� ��ַ��λ */
 SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
 /* ���� �� ��ַ��λ */
 SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
 /* ���� �� ��ַ��λ */
 SPI_FLASH_SendByte(ReadAddr & 0xFF);
 
 /* ��ȡ���� */
 while (NumByteToRead--)
 {
 /* ��ȡһ���ֽ�*/
 *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
 /* ָ����һ���ֽڻ����� */
 pBuffer++;
 }
 
 /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
 SPI_FLASH_CS_HIGH();
}

uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  printf("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  return 0;
}
void SPI_Flash_PowerDown(void)   
{ 
  /* ѡ�� FLASH: CS �� */
  SPI_FLASH_CS_LOW();

  /* ���� ���� ���� */
  SPI_FLASH_SendByte(W25X_PowerDown);

  /* ֹͣ�ź�  FLASH: CS �� */
  SPI_FLASH_CS_HIGH();
} 