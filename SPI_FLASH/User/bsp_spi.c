
/**
* @brief SPI_FLASH 初始化
* @param 无
* @retval 无
*/
void SPI_FLASH_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
 
 /* 使能 FLASH_SPI 及 GPIO 时钟 */
 /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO,
 SPI_FLASH_SPI_MISO_GPIO 和 SPI_FLASH_SPI_SCK_GPIO 时钟使能 */
 RCC_AHB1PeriphClockCmd (FLASH_SPI_SCK_GPIO_CLK | FLASH_SPI_MISO_GPIO_CLK|
 FLASH_SPI_MOSI_GPIO_CLK|FLASH_CS_GPIO_CLK, ENABLE);
 
 /*!< SPI_FLASH_SPI 时钟使能 */
 FLASH_SPI_CLK_INIT(FLASH_SPI_CLK, ENABLE);
 
 //设置引脚复用
 GPIO_PinAFConfig(FLASH_SPI_SCK_GPIO_PORT,FLASH_SPI_SCK_PINSOURCE,
 FLASH_SPI_SCK_AF);
 GPIO_PinAFConfig(FLASH_SPI_MISO_GPIO_PORT,FLASH_SPI_MISO_PINSOURCE,
 FLASH_SPI_MISO_AF);
 GPIO_PinAFConfig(FLASH_SPI_MOSI_GPIO_PORT,FLASH_SPI_MOSI_PINSOURCE,
 FLASH_SPI_MOSI_AF);
 
 /*!< 配置 SPI_FLASH_SPI 引脚: SCK */
 GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 
 GPIO_Init(FLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
 
 /*!< 配置 SPI_FLASH_SPI 引脚: MISO */
 GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
 GPIO_Init(FLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
 
 /*!< 配置 SPI_FLASH_SPI 引脚: MOSI */
 GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
 GPIO_Init(FLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
 
 /*!< 配置 SPI_FLASH_SPI 引脚: CS */
 GPIO_InitStructure.GPIO_Pin = FLASH_CS_PIN;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStructure);
 
 /* 停止信号 FLASH: CS 引脚高电平*/
 SPI_FLASH_CS_HIGH();
 /*为方便讲解，以下省略 SPI 模式初始化部分*/
 //......
 }
/**
* @brief SPI_FLASH 引脚初始化
* @param 无
* @retval 无
*/
void SPI_FLASH_Init(void)
{
/*为方便讲解，省略了 SPI 的 GPIO 初始化部分*/
//......
 
 SPI_InitTypeDef SPI_InitStructure;
 /* FLASH_SPI 模式配置 */
 // FLASH 芯片 支持 SPI 模式 0 及模式 3，据此设置 CPOL CPHA
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
 
 /* 使能 FLASH_SPI */
 SPI_Cmd(FLASH_SPI, ENABLE);
 }
#define Dummy_Byte 0xFF
/**
* @brief 使用 SPI 发送一个字节的数据
* @param byte：要发送的数据
* @retval 返回接收到的数据
*/
u8 SPI_FLASH_SendByte(u8 byte)
{
SPITimeout = SPIT_FLAG_TIMEOUT;
 
 /* 等待发送缓冲区为空，TXE 事件 */
 while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET)
 {
 if ((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
 }
 
 /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
 SPI_I2S_SendData(FLASH_SPI, byte);
 
 SPITimeout = SPIT_FLAG_TIMEOUT;
 
 /* 等待接收缓冲区非空，RXNE 事件 */
 while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET)
 {
 if ((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
 }
 
 /* 读取数据寄存器，获取接收缓冲区数据 */
 return SPI_I2S_ReceiveData(FLASH_SPI);
 }
 
 /**
 * @brief 使用 SPI 读取一个字节的数据
 * @param 无
 * @retval 返回接收到的数据
 */
 u8 SPI_FLASH_ReadByte(void)
 {
 return (SPI_FLASH_SendByte(Dummy_Byte));
 }
/**
* @brief 读取 FLASH ID
* @param 无
* @retval FLASH ID
*/
u32 SPI_FLASH_ReadID(void)
{
u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

 /* 开始通讯：CS 低电平 */
 SPI_FLASH_CS_LOW();
 
 /* 发送 JEDEC 指令，读取 ID */
 SPI_FLASH_SendByte(W25X_JedecDeviceID);
 
 /* 读取一个字节数据 */
 Temp0 = SPI_FLASH_SendByte(Dummy_Byte);
 
 /* 读取一个字节数据 */
 Temp1 = SPI_FLASH_SendByte(Dummy_Byte);
 
 /* 读取一个字节数据 */
 Temp2 = SPI_FLASH_SendByte(Dummy_Byte);
 
 /* 停止通讯：CS 高电平 */
 SPI_FLASH_CS_HIGH();
 
 /*把数据组合起来，作为函数的返回值*/
 Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
 
 return Temp;
 }
/**
* @brief 向 FLASH 发送 写使能 命令
* @param none
* @retval none
*/
void SPI_FLASH_WriteEnable(void)
{
/* 通讯开始：CS 低 */
SPI_FLASH_CS_LOW();
 
 /* 发送写使能命令*/
 SPI_FLASH_SendByte(W25X_WriteEnable);
 
 /*通讯结束：CS 高 */
 SPI_FLASH_CS_HIGH();
 }
/*WIP(BUSY)标志：FLASH 内部正在写入*/
#define WIP_Flag 0x01

/**
* @brief 等待 WIP(BUSY)标志被置 0，即等待到 FLASH 内部数据写入完毕
* @param none
* @retval none
*/
void SPI_FLASH_WaitForWriteEnd(void)
 {
 u8 FLASH_Status = 0;
 /* 选择 FLASH: CS 低 */
 SPI_FLASH_CS_LOW();
 
 /* 发送 读状态寄存器 命令 */
 SPI_FLASH_SendByte(W25X_ReadStatusReg);
 
 SPITimeout = SPIT_FLAG_TIMEOUT;
 /* 若 FLASH 忙碌，则等待 */
 do
 {
 /* 读取 FLASH 芯片的状态寄存器 */
 FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
 if ((SPITimeout--) == 0)
 {
 SPI_TIMEOUT_UserCallback(4);
 return;
 }
 }
 while ((FLASH_Status & WIP_Flag) == SET); /* 正在写入标志 */
 
 /* 停止信号 FLASH: CS 高 */
 SPI_FLASH_CS_HIGH();
 }
/**
* @brief 擦除 FLASH 扇区
* @param SectorAddr：要擦除的扇区地址
* @retval 无
*/
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
/* 发送 FLASH 写使能命令 */
SPI_FLASH_WriteEnable();
 SPI_FLASH_WaitForWriteEnd();
 /* 擦除扇区 */
 /* 选择 FLASH: CS 低电平 */
 SPI_FLASH_CS_LOW();
 /* 发送扇区擦除指令*/
 SPI_FLASH_SendByte(W25X_SectorErase);
 /*发送擦除扇区地址的高位*/
 SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
 /* 发送擦除扇区地址的中位 */
 SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
 /* 发送擦除扇区地址的低位 */
 SPI_FLASH_SendByte(SectorAddr & 0xFF);
 /* 停止信号 FLASH: CS 高电平 */
 SPI_FLASH_CS_HIGH();
 /* 等待擦除完毕*/
 SPI_FLASH_WaitForWriteEnd();
 }

/**
* @brief 对 FLASH 按页写入数据，调用本函数写入数据前需要先擦除扇区
* @param pBuffer，要写入数据的指针
* @param WriteAddr，写入地址
* @param NumByteToWrite，写入数据长度，必须小于等于页大小
* @retval 无
*/
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
 /* 发送 FLASH 写使能命令 */
 SPI_FLASH_WriteEnable();
 
 /* 选择 FLASH: CS 低电平 */
 SPI_FLASH_CS_LOW();
 /* 写送写指令*/
 SPI_FLASH_SendByte(W25X_PageProgram);
 /*发送写地址的高位*/
 SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
 /*发送写地址的中位*/
 SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
 /*发送写地址的低位*/
 SPI_FLASH_SendByte(WriteAddr & 0xFF);
 
 if (NumByteToWrite > SPI_FLASH_PerWritePageSize)
 {
 NumByteToWrite = SPI_FLASH_PerWritePageSize;
 FLASH_ERROR("SPI_FLASH_PageWrite too large!");
 }
 
 /* 写入数据*/
 while (NumByteToWrite--)
 {
 /* 发送当前要写入的字节数据 */
 SPI_FLASH_SendByte(*pBuffer);
 /* 指向下一字节数据 */
 pBuffer++;
 }
 
 /* 停止信号 FLASH: CS 高电平 */
 SPI_FLASH_CS_HIGH();
 
 /* 等待写入完毕*/
 SPI_FLASH_WaitForWriteEnd();
 }

/**
* @brief 对 FLASH 写入数据，调用本函数写入数据前需要先擦除扇区
* @param pBuffer，要写入数据的指针
* @param WriteAddr，写入地址
* @param NumByteToWrite，写入数据长度
* @retval 无
*/
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
 u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
 
 /*mod 运算求余，若 writeAddr 是 SPI_FLASH_PageSize 整数倍，运算结果 Addr 值为
/
 Addr = WriteAddr % SPI_FLASH_PageSize;
 
 /*差 count 个数据值，刚好可以对齐到页地址*/
 count = SPI_FLASH_PageSize - Addr;
 /*计算出要写多少整数页*/
 NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
 /*mod 运算求余，计算出剩余不满一页的字节数*/
 NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
 
 /* Addr=0,则 WriteAddr 刚好按页对齐 aligned */
 if (Addr == 0)
 {
 /* NumByteToWrite < SPI_FLASH_PageSize */
 if (NumOfPage == 0)
 {
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
 }
 else /* NumByteToWrite > SPI_FLASH_PageSize */
 {
 /*先把整数页都写了*/
 while (NumOfPage--)
 {
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
 WriteAddr += SPI_FLASH_PageSize;
 pBuffer += SPI_FLASH_PageSize;
 }
 
 /*若有多余的不满一页的数据，把它写完*/
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
 }
 }
 /* 若地址与 SPI_FLASH_PageSize 不对齐 */
 else
 {
 /* NumByteToWrite < SPI_FLASH_PageSize */
 if (NumOfPage == 0)
 {
 /*当前页剩余的 count 个位置比 NumOfSingle 小，写不完*/
 if (NumOfSingle > count)
 {
 temp = NumOfSingle - count;
 
 /*先写满当前页*/
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
 WriteAddr += count;
 pBuffer += count;
 
 /*再写剩余的数据*/
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
 }
 else /*当前页剩余的 count 个位置能写完 NumOfSingle 个数据*/
 {
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
 }
 }
 else /* NumByteToWrite > SPI_FLASH_PageSize */
 {
 /*地址不对齐多出的 count 分开处理，不加入这个运算*/
 NumByteToWrite -= count;
 NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
 NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
 
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
 WriteAddr += count;
 pBuffer += count;
 
 /*把整数页都写了*/
 while (NumOfPage--)
 {
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
 WriteAddr += SPI_FLASH_PageSize;
 pBuffer += SPI_FLASH_PageSize;
 }
 /*若有多余的不满一页的数据，把它写完*/
 if (NumOfSingle != 0)
 {
 SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
 }
 }
 }
 }
/**
* @brief 读取 FLASH 数据
* @param pBuffer，存储读出数据的指针
* @param ReadAddr，读取地址
* @param NumByteToRead，读取数据长度
* @retval 无
*/
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
 /* 选择 FLASH: CS 低电平 */
 SPI_FLASH_CS_LOW();
 
 /* 发送 读 指令 */
 SPI_FLASH_SendByte(W25X_ReadData);
 
 /* 发送 读 地址高位 */
 SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
 /* 发送 读 地址中位 */
 SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
 /* 发送 读 地址低位 */
 SPI_FLASH_SendByte(ReadAddr & 0xFF);
 
 /* 读取数据 */
 while (NumByteToRead--)
 {
 /* 读取一个字节*/
 *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
 /* 指向下一个字节缓冲区 */
 pBuffer++;
 }
 
 /* 停止信号 FLASH: CS 高电平 */
 SPI_FLASH_CS_HIGH();
 }
