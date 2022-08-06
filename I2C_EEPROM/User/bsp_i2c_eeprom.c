#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"
#include "bsp_i2c_eeprom.h"
#include "stm32f4xx_usart.h"
#include "stdio.h"

void I2C_GPIO_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
 
 /*使能 I2C 外设时钟 */
 RCC_APB1PeriphClockCmd(EEPROM_I2C_CLK, ENABLE);
 
 /*使能 I2C 引脚的 GPIO 时钟*/
 RCC_AHB1PeriphClockCmd(EEPROM_I2C_SCL_GPIO_CLK |
 EEPROM_I2C_SDA_GPIO_CLK, ENABLE);
 
 /* 连接引脚源 PXx 到 I2C_SCL*/
 GPIO_PinAFConfig(EEPROM_I2C_SCL_GPIO_PORT, EEPROM_I2C_SCL_SOURCE,
 EEPROM_I2C_SCL_AF);
 /* 连接引脚源 PXx 到 to I2C_SDA*/
 GPIO_PinAFConfig(EEPROM_I2C_SDA_GPIO_PORT, EEPROM_I2C_SDA_SOURCE,
 EEPROM_I2C_SDA_AF);
 
 /*配置 SCL 引脚 */
 GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
 
 /*配置 SDA 引脚 */
 GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_PIN;
 GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
}

///////////////////////////////////////////////////////////////////////////////////
void I2C_Mode_Config(void)
{
I2C_InitTypeDef I2C_InitStructure;
 
 /* I2C 配置 */
 /*I2C 模式*/
 I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
 /*占空比*/
 I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
 /*I2C 自身地址*/
 I2C_InitStructure.I2C_OwnAddress1 =I2C_OWN_ADDRESS7;
 /*使能响应*/
 I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
 /* I2C 的寻址模式 */
 I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
 /* 通信速率 */
 I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
 /*写入配置*/
 I2C_Init(EEPROM_I2C, &I2C_InitStructure);
 /* 使能 I2C */
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
 /* 使用串口 printf 输出错误信息，方便调试 */
 printf("I2C 等待超时!errorCode = %d",errorCode);
 return 0;
 }
///////////////////////////////////////////////////////////////////////
 uint32_t I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr)
 {
 /* 产生 I2C 起始信号 */
 I2C_GenerateSTART(EEPROM_I2C, ENABLE);
 
 /*设置超时等待时间*/
 uint32_t I2CTimeout = I2CT_FLAG_TIMEOUT;
 /* 检测 EV5 事件并清除标志*/
 while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
 }
 
 /* 发送 EEPROM 设备地址 发送R/W方向*/
 I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS,
 I2C_Direction_Transmitter);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 /* 检测 EV6 事件并清除标志*/
 while (!I2C_CheckEvent(EEPROM_I2C,
 I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
 }
 
 /* 发送要写入的 EEPROM 内部地址(即 EEPROM 内部存储器的地址) */
 I2C_SendData(EEPROM_I2C, WriteAddr);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 /* 检测 EV8 事件并清除标志*/
 while (!I2C_CheckEvent(EEPROM_I2C,
 I2C_EVENT_MASTER_BYTE_TRANSMITTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
 }
 /* 发送一字节要写入的数据 */
 I2C_SendData(EEPROM_I2C, *pBuffer);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 /* 检测 EV8 事件并清除标志*/
 while (!I2C_CheckEvent(EEPROM_I2C,
 I2C_EVENT_MASTER_BYTE_TRANSMITTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
 }
 
 /* 发送停止信号 */
 I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
 
 return 1;
}
////////////////////////////////////////////////////////////////////////////////
static uint8_t I2C_EE_WaitEepromStandbyState(void)
{
 __IO uint16_t tmpSR1 = 0;
 __IO uint32_t EETrials = 0;
 
 /*总线忙时等待 */
 uint32_t I2CTimeout = I2CT_LONG_TIMEOUT;
 while (I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(20);
 }
 
 /* 等待从机应答，最多等待 300 次 */
 while (1)
 {
 /*开始信号 */
 I2C_GenerateSTART(EEPROM_I2C, ENABLE);
 
 /* 检测 EV5 事件并清除标志*/
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(21);
 }
 
 /* 发送 EEPROM 设备地址 */
 I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDRESS,I2C_Direction_Transmitter);
 
 /* 等待 ADDR 标志 */
 I2CTimeout = I2CT_LONG_TIMEOUT;
 do
 {
 /* 获取 SR1 寄存器状态 */
 tmpSR1 = EEPROM_I2C->SR1;
 
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(22);
 }
 /* 一直等待直到 addr 及 af 标志为 1  收到地址且已经应答*/
 while ((tmpSR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0);
 
 /*检查 addr 标志是否为 1  收到地址且响应地址*/
 if (tmpSR1 & I2C_SR1_ADDR)
 {
 /* 清除 addr 标志该标志通过读 SR1 及 SR2 清除 */
 (void)EEPROM_I2C->SR2;
 
 /*产生停止信号 */
 I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
 
 /* 退出函数 */
 return 1;
 }
 else//收到地址但未响应
 {
 /*清除 af 标志 */
 I2C_ClearFlag(EEPROM_I2C, I2C_FLAG_AF);
 }
 
 /*检查等待次数*/
 if (EETrials++ == MAX_TRIAL_NUMBER)
 {
 /* 等待 MAX_TRIAL_NUMBER 次都还没准备好，退出等待 */
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
 
 /*每写一个字节调用一次 I2C_EE_ByteWrite 函数*/
 for (i=0; i<NumByteToWrite; i++)
 {
 /*等待 EEPROM 准备完毕*/
 I2C_EE_WaitEepromStandbyState();
 /*按字节写入数据*/
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
 
 /* 产生 I2C 起始信号 */
 I2C_GenerateSTART(EEPROM_I2C, ENABLE);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 
 /* 检测 EV5 事件并清除标志 */
 while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(5);
 }
 
 /* 发送 EEPROM 设备地址 */
 I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDRESS,I2C_Direction_Transmitter);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 
 /* 检测 EV6 事件并清除标志*/
 while (!I2C_CheckEvent(EEPROM_I2C,
 I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(6);
 }
 /* 发送要写入的 EEPROM 内部地址(即 EEPROM 内部存储器的地址) */
 I2C_SendData(EEPROM_I2C, WriteAddr);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 
 /* 检测 EV8 事件并清除标志*/
 while (! I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(7);
 }
 /* 循环发送 NumByteToWrite 个数据 */
 while (NumByteToWrite--)
 {
 /* 发送缓冲区中的数据 */
 I2C_SendData(EEPROM_I2C, *pBuffer);
 
 /* 指向缓冲区中的下一个数据 */
 pBuffer++;
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 
 /* 检测 EV8 事件并清除标志*/
 while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(8);
 }
 }
 /* 发送停止信号 */
 I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
 return 1;
 }
 
 /////////////////////////////////////////////////////////////////////////////////////
 void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, u16 NumByteToWrite)
 {
 uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
 
 /*mod 运算求余，若 writeAddr 是 I2C_PageSize 整数倍，运算结果 Addr 值为 0*/
 Addr = WriteAddr % I2C_PageSize;
 
 /*差 count 个数据，刚好可以对齐到页地址*/
 count = I2C_PageSize - Addr;
 /*计算出要写多少整数页*/
 NumOfPage = NumByteToWrite / I2C_PageSize;
 /*mod 运算求余，计算出剩余不满一页的字节数*/
 NumOfSingle = NumByteToWrite % I2C_PageSize;
 
 /* Addr=0,则 WriteAddr 刚好按页对齐 aligned */
 if (Addr == 0)
 {
 /* 如果 NumByteToWrite < I2C_PageSize */
 if (NumOfPage == 0)
 {
 I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
 I2C_EE_WaitEepromStandbyState();
 }
 /* 如果 NumByteToWrite > I2C_PageSize */
 else
 {
 /*先把整数页都写了*/
 while (NumOfPage--)
 {
 I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
 I2C_EE_WaitEepromStandbyState();
 WriteAddr += I2C_PageSize;
 pBuffer += I2C_PageSize;
 }
 
 /*若有多余的不满一页的数据，把它写完*/
 if (NumOfSingle!=0)
 {
 I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
 I2C_EE_WaitEepromStandbyState();
 }
 }
 }
 /* 如果 WriteAddr 不是按 I2C_PageSize 对齐 */
 else
 {
 /* 如果 NumByteToWrite < I2C_PageSize */
 if (NumOfPage== 0)
 {
 I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
 I2C_EE_WaitEepromStandbyState();
 }
 /* 如果 NumByteToWrite > I2C_PageSize */
 else
 {
 /*地址不对齐多出的 count 分开处理，不加入这个运算*/
 NumByteToWrite -= count;
 NumOfPage = NumByteToWrite / I2C_PageSize;
 NumOfSingle = NumByteToWrite % I2C_PageSize;
 
 /*先把 WriteAddr 所在页的剩余字节写了*/
 if (count != 0)
 {
 I2C_EE_PageWrite(pBuffer, WriteAddr, count);
 I2C_EE_WaitEepromStandbyState();
 
 /*WriteAddr 加上 count 后，地址就对齐到页了*/
 WriteAddr += count;
 pBuffer += count;
 }
 /*把整数页都写了*/
 while (NumOfPage--)
 {
 I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
 I2C_EE_WaitEepromStandbyState();
 WriteAddr += I2C_PageSize;
 pBuffer += I2C_PageSize;
 }
 /*若有多余的不满一页的数据，把它写完*/
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
 
 /* 产生 I2C 起始信号 */
 I2C_GenerateSTART(EEPROM_I2C, ENABLE);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;

 /* 检测 EV5 事件并清除标志*/
 while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(10);
 }
 
 /* 发送 EEPROM 设备地址 */
 I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDRESS,I2C_Direction_Transmitter);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 
 /* 检测 EV6 事件并清除标志*/
 while (!I2C_CheckEvent(EEPROM_I2C,
 I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(11);
 }
 /*通过重新设置 PE 位清除 EV6 事件 */
 I2C_Cmd(EEPROM_I2C, ENABLE);
 
 /* 发送要读取的 EEPROM 内部地址(即 EEPROM 内部存储器的地址) */
 I2C_SendData(EEPROM_I2C, ReadAddr);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 
 /* 检测 EV8 事件并清除标志*/
 while (!I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(12);
 }
 /* 产生第二次 I2C 起始信号 */
 I2C_GenerateSTART(EEPROM_I2C, ENABLE);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 
 /* 检测 EV5 事件并清除标志*/
 while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(13);
 }
 /* 发送 EEPROM 设备地址 */
 I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Receiver);
 
 I2CTimeout = I2CT_FLAG_TIMEOUT;
 
 /* 检测 EV6 事件并清除标志*/
 while (!I2C_CheckEvent(EEPROM_I2C,
 I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(14);
 }
 /* 读取 NumByteToRead 个数据*/
 while (NumByteToRead)
 {
 /*若 NumByteToRead=1，表示已经接收到最后一个数据了，
 发送非应答信号，结束传输*/
 if (NumByteToRead == 1)
 {
 /* 发送非应答信号 */
 I2C_AcknowledgeConfig(EEPROM_I2C, DISABLE);
 
 /* 发送停止信号 */
 I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
 }
 
 I2CTimeout = I2CT_LONG_TIMEOUT;
 while (I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)
 {
 if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
 }
 {
 /*通过 I2C，从设备中读取一个字节的数据 */
 *pBuffer = I2C_ReceiveData(EEPROM_I2C);
 
 /* 存储数据的指针指向下一个地址 */
 pBuffer++;
 
 /* 接收数据自减 */
 NumByteToRead--;
 }
 }
 
 /* 使能应答，方便下一次 I2C 传输 */
 I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);
 return 1;
 }
