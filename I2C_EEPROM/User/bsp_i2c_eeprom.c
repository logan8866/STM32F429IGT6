#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"
#include "bsp_i2c_eeprom.h"
#include "stm32f4xx_usart.h"
#include "stdio.h"

void I2C_GPIO_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
 
 /*ʹ�� I2C ����ʱ�� */
 RCC_APB1PeriphClockCmd(EEPROM_I2C_CLK, ENABLE);
 
 /*ʹ�� I2C ���ŵ� GPIO ʱ��*/
 RCC_AHB1PeriphClockCmd(EEPROM_I2C_SCL_GPIO_CLK |
 EEPROM_I2C_SDA_GPIO_CLK, ENABLE);
 
 /* ��������Դ PXx �� I2C_SCL*/
 GPIO_PinAFConfig(EEPROM_I2C_SCL_GPIO_PORT, EEPROM_I2C_SCL_SOURCE,
 EEPROM_I2C_SCL_AF);
 /* ��������Դ PXx �� to I2C_SDA*/
 GPIO_PinAFConfig(EEPROM_I2C_SDA_GPIO_PORT, EEPROM_I2C_SDA_SOURCE,
 EEPROM_I2C_SDA_AF);
 
 /*���� SCL ���� */
 GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
 
 /*���� SDA ���� */
 GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_PIN;
 GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
}

///////////////////////////////////////////////////////////////////////////////////
void I2C_Mode_Config(void)
{
I2C_InitTypeDef I2C_InitStructure;
 
 /* I2C ���� */
 /*I2C ģʽ*/
 I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
 /*ռ�ձ�*/
 I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
 /*I2C �����ַ*/
 I2C_InitStructure.I2C_OwnAddress1 =I2C_OWN_ADDRESS7;
 /*ʹ����Ӧ*/
 I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
 /* I2C ��Ѱַģʽ */
 I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
 /* ͨ������ */
 I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
 /*д������*/
 I2C_Init(EEPROM_I2C, &I2C_InitStructure);
 /* ʹ�� I2C */
 I2C_Cmd(EEPROM_I2C, ENABLE);
 }
/////////////////////////////////////////////////////////////////////
 void I2C_EE_Init(void)
 {
 I2C_GPIO_Config();
 
 I2C_Mode_Config();
 }
//////////////////////////////////////////////////////////////////////
 static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
 {
 /* ʹ�ô��� printf ���������Ϣ��������� */
 printf("I2C �ȴ���ʱ!errorCode = %d",errorCode);
 return 0;
 }
///////////////////////////////////////////////////////////////////////
 uint32_t I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr)
 {
 /* ���� I2C ��ʼ�ź� */
 I2C_GenerateSTART(EEPROM_I2C, ENABLE);
 
 /*���ó�ʱ�ȴ�ʱ��*/
 uint32_t I2CTimeout = I2CT_FLAG_TIMEOUT;
 /* ��� EV5 �¼��������־*/
 while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
 }
 
 /* ���� EEPROM �豸��ַ ����R/W����*/
 I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS,
 I2C_Direction_Transmitter);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 /* ��� EV6 �¼��������־*/
 while (!I2C_CheckEvent(EEPROM_I2C,
 I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
 }
 
 /* ����Ҫд��� EEPROM �ڲ���ַ(�� EEPROM �ڲ��洢���ĵ�ַ) */
 I2C_SendData(EEPROM_I2C, WriteAddr);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 /* ��� EV8 �¼��������־*/
 while (!I2C_CheckEvent(EEPROM_I2C,
 I2C_EVENT_MASTER_BYTE_TRANSMITTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
 }
 /* ����һ�ֽ�Ҫд������� */
 I2C_SendData(EEPROM_I2C, *pBuffer);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 /* ��� EV8 �¼��������־*/
 while (!I2C_CheckEvent(EEPROM_I2C,
 I2C_EVENT_MASTER_BYTE_TRANSMITTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
 }
 
 /* ����ֹͣ�ź� */
 I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
 
 return 1;
}
////////////////////////////////////////////////////////////////////////////////
static uint8_t I2C_EE_WaitEepromStandbyState(void)
{
 __IO uint16_t tmpSR1 = 0;
 __IO uint32_t EETrials = 0;
 
 /*����æʱ�ȴ� */
 uint32_t I2CTimeout = I2CT_LONG_TIMEOUT;
 while (I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(20);
 }
 
 /* �ȴ��ӻ�Ӧ�����ȴ� 300 �� */
 while (1)
 {
 /*��ʼ�ź� */
 I2C_GenerateSTART(EEPROM_I2C, ENABLE);
 
 /* ��� EV5 �¼��������־*/
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(21);
 }
 
 /* ���� EEPROM �豸��ַ */
 I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDRESS,I2C_Direction_Transmitter);
 
 /* �ȴ� ADDR ��־ */
 I2CTimeout = I2CT_LONG_TIMEOUT;
 do
 {
 /* ��ȡ SR1 �Ĵ���״̬ */
 tmpSR1 = EEPROM_I2C->SR1;
 
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(22);
 }
 /* һֱ�ȴ�ֱ�� addr �� af ��־Ϊ 1  �յ���ַ���Ѿ�Ӧ��*/
 while ((tmpSR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0);
 
 /*��� addr ��־�Ƿ�Ϊ 1  �յ���ַ����Ӧ��ַ*/
 if (tmpSR1 & I2C_SR1_ADDR)
 {
 /* ��� addr ��־�ñ�־ͨ���� SR1 �� SR2 ��� */
 (void)EEPROM_I2C->SR2;
 
 /*����ֹͣ�ź� */
 I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
 
 /* �˳����� */
 return 1;
 }
 else//�յ���ַ��δ��Ӧ
 {
 /*��� af ��־ */
 I2C_ClearFlag(EEPROM_I2C, I2C_FLAG_AF);
 }
 
 /*���ȴ�����*/
 if (EETrials++ == MAX_TRIAL_NUMBER)
 {
 /* �ȴ� MAX_TRIAL_NUMBER �ζ���û׼���ã��˳��ȴ� */
 return I2C_TIMEOUT_UserCallback(23);
 }
 }
 }
/////////////////////////////////////////////////////////////////////////
uint8_t I2C_EE_ByetsWrite(uint8_t* pBuffer,uint8_t WriteAddr,
 uint16_t NumByteToWrite)
 {
 uint16_t i;
 uint8_t res;
 
 /*ÿдһ���ֽڵ���һ�� I2C_EE_ByteWrite ����*/
 for (i=0; i<NumByteToWrite; i++)
 {
 /*�ȴ� EEPROM ׼�����*/
 I2C_EE_WaitEepromStandbyState();
 /*���ֽ�д������*/
 res = I2C_EE_ByteWrite(pBuffer++,WriteAddr++);
 }
 return res;
 }
//////////////////////////////////////////////////////////////////////////
 uint8_t I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr,
 uint8_t NumByteToWrite)
 {
 uint32_t I2CTimeout = I2CT_LONG_TIMEOUT;

 while (I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(4);
 }
 
 /* ���� I2C ��ʼ�ź� */
 I2C_GenerateSTART(EEPROM_I2C, ENABLE);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 
 /* ��� EV5 �¼��������־ */
 while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(5);
 }
 
 /* ���� EEPROM �豸��ַ */
 I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDRESS,I2C_Direction_Transmitter);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 
 /* ��� EV6 �¼��������־*/
 while (!I2C_CheckEvent(EEPROM_I2C,
 I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(6);
 }
 /* ����Ҫд��� EEPROM �ڲ���ַ(�� EEPROM �ڲ��洢���ĵ�ַ) */
 I2C_SendData(EEPROM_I2C, WriteAddr);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 
 /* ��� EV8 �¼��������־*/
 while (! I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(7);
 }
 /* ѭ������ NumByteToWrite ������ */
 while (NumByteToWrite--)
 {
 /* ���ͻ������е����� */
 I2C_SendData(EEPROM_I2C, *pBuffer);
 
 /* ָ�򻺳����е���һ������ */
 pBuffer++;
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 
 /* ��� EV8 �¼��������־*/
 while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(8);
 }
 }
 /* ����ֹͣ�ź� */
 I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
 return 1;
 }
 
 /////////////////////////////////////////////////////////////////////////////////////
 void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, u16 NumByteToWrite)
 {
 uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
 
 /*mod �������࣬�� writeAddr �� I2C_PageSize �������������� Addr ֵΪ 0*/
 Addr = WriteAddr % I2C_PageSize;
 
 /*�� count �����ݣ��պÿ��Զ��뵽ҳ��ַ*/
 count = I2C_PageSize - Addr;
 /*�����Ҫд��������ҳ*/
 NumOfPage = NumByteToWrite / I2C_PageSize;
 /*mod �������࣬�����ʣ�಻��һҳ���ֽ���*/
 NumOfSingle = NumByteToWrite % I2C_PageSize;
 
 /* Addr=0,�� WriteAddr �պð�ҳ���� aligned */
 if (Addr == 0)
 {
 /* ��� NumByteToWrite < I2C_PageSize */
 if (NumOfPage == 0)
 {
 I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
 I2C_EE_WaitEepromStandbyState();
 }
 /* ��� NumByteToWrite > I2C_PageSize */
 else
 {
 /*�Ȱ�����ҳ��д��*/
 while (NumOfPage--)
 {
 I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
 I2C_EE_WaitEepromStandbyState();
 WriteAddr += I2C_PageSize;
 pBuffer += I2C_PageSize;
 }
 
 /*���ж���Ĳ���һҳ�����ݣ�����д��*/
 if (NumOfSingle!=0)
 {
 I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
 I2C_EE_WaitEepromStandbyState();
 }
 }
 }
 /* ��� WriteAddr ���ǰ� I2C_PageSize ���� */
 else
 {
 /* ��� NumByteToWrite < I2C_PageSize */
 if (NumOfPage== 0)
 {
 I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
 I2C_EE_WaitEepromStandbyState();
 }
 /* ��� NumByteToWrite > I2C_PageSize */
 else
 {
 /*��ַ���������� count �ֿ������������������*/
 NumByteToWrite -= count;
 NumOfPage = NumByteToWrite / I2C_PageSize;
 NumOfSingle = NumByteToWrite % I2C_PageSize;
 
 /*�Ȱ� WriteAddr ����ҳ��ʣ���ֽ�д��*/
 if (count != 0)
 {
 I2C_EE_PageWrite(pBuffer, WriteAddr, count);
 I2C_EE_WaitEepromStandbyState();
 
 /*WriteAddr ���� count �󣬵�ַ�Ͷ��뵽ҳ��*/
 WriteAddr += count;
 pBuffer += count;
 }
 /*������ҳ��д��*/
 while (NumOfPage--)
 {
 I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
 I2C_EE_WaitEepromStandbyState();
 WriteAddr += I2C_PageSize;
 pBuffer += I2C_PageSize;
 }
 /*���ж���Ĳ���һҳ�����ݣ�����д��*/
 if (NumOfSingle != 0)
 {
 I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
 I2C_EE_WaitEepromStandbyState();
 }
 }
 }
 }
///////////////////////////////////////////////////////////////////////////////
uint8_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr,
 u16 NumByteToRead)
 {
 uint32_t I2CTimeout = I2CT_LONG_TIMEOUT;
 
 while (I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);
 }
 
 /* ���� I2C ��ʼ�ź� */
 I2C_GenerateSTART(EEPROM_I2C, ENABLE);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;

 /* ��� EV5 �¼��������־*/
 while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(10);
 }
 
 /* ���� EEPROM �豸��ַ */
 I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDRESS,I2C_Direction_Transmitter);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 
 /* ��� EV6 �¼��������־*/
 while (!I2C_CheckEvent(EEPROM_I2C,
 I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(11);
 }
 /*ͨ���������� PE λ��� EV6 �¼� */
 I2C_Cmd(EEPROM_I2C, ENABLE);
 
 /* ����Ҫ��ȡ�� EEPROM �ڲ���ַ(�� EEPROM �ڲ��洢���ĵ�ַ) */
 I2C_SendData(EEPROM_I2C, ReadAddr);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 
 /* ��� EV8 �¼��������־*/
 while (!I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(12);
 }
 /* �����ڶ��� I2C ��ʼ�ź� */
 I2C_GenerateSTART(EEPROM_I2C, ENABLE);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 
 /* ��� EV5 �¼��������־*/
 while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(13);
 }
 /* ���� EEPROM �豸��ַ */
 I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Receiver);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 
 /* ��� EV6 �¼��������־*/
 while (!I2C_CheckEvent(EEPROM_I2C,
 I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(14);
 }
 /* ��ȡ NumByteToRead ������*/
 while (NumByteToRead)
 {
 /*�� NumByteToRead=1����ʾ�Ѿ����յ����һ�������ˣ�
 ���ͷ�Ӧ���źţ���������*/
 if (NumByteToRead == 1)
 {
 /* ���ͷ�Ӧ���ź� */
 I2C_AcknowledgeConfig(EEPROM_I2C, DISABLE);
 
 /* ����ֹͣ�ź� */
 I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
 }
 
 I2CTimeout = I2CT_LONG_TIMEOUT;
 while (I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
 }
 {
 /*ͨ�� I2C�����豸�ж�ȡһ���ֽڵ����� */
 *pBuffer = I2C_ReceiveData(EEPROM_I2C);
 
 /* �洢���ݵ�ָ��ָ����һ����ַ */
 pBuffer++;
 
 /* ���������Լ� */
 NumByteToRead--;
 }
 }
 
 /* ʹ��Ӧ�𣬷�����һ�� I2C ���� */
 I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);
 return 1;
 }
