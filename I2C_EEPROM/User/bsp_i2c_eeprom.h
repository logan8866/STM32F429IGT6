#include "stm32f4xx.h"
/* STM32 I2C 速率 */
#define I2C_Speed 400000

/* STM32 自身的 I2C 地址，这个地址只要与 STM32 外挂的 I2C 器件地址不一样即可 */
#define I2C_OWN_ADDRESS7 0X0A
#define EEPROM_ADDRESS 0XA0
#define  EEP_Firstpage 0x00

/*I2C 接口*/
#define EEPROM_I2C I2C1
#define EEPROM_I2C_CLK RCC_APB1Periph_I2C1

#define EEPROM_I2C_SCL_PIN GPIO_Pin_6
#define EEPROM_I2C_SCL_GPIO_PORT GPIOB
#define EEPROM_I2C_SCL_GPIO_CLK RCC_AHB1Periph_GPIOB
#define EEPROM_I2C_SCL_SOURCE GPIO_PinSource6
#define EEPROM_I2C_SCL_AF GPIO_AF_I2C1

#define EEPROM_I2C_SDA_PIN GPIO_Pin_7
#define EEPROM_I2C_SDA_GPIO_PORT GPIOB
#define EEPROM_I2C_SDA_GPIO_CLK RCC_AHB1Periph_GPIOB
#define EEPROM_I2C_SDA_SOURCE GPIO_PinSource7
#define EEPROM_I2C_SDA_AF GPIO_AF_I2C1
/*通讯等待超时时间*/
#define I2CT_FLAG_TIMEOUT ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))
//等待 Standby 状态的最大次数
#define MAX_TRIAL_NUMBER 300
/* AT24C01/02 每页有 8 个字节 */
#define I2C_PageSize 8
void I2C_GPIO_Config(void);
void I2C_Mode_Config(void);
void I2C_EE_Init(void);
uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);
uint32_t I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr);
uint8_t I2C_EE_WaitEepromStandbyState(void);
uint8_t I2C_EE_ByetsWrite(uint8_t* pBuffer,uint8_t WriteAddr,uint16_t NumByteToWrite);
uint8_t I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr,uint8_t NumByteToWrite);
void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, u16 NumByteToWrite);
uint8_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr,u16 NumByteToRead);

