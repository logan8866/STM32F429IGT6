#include "stm32f4xx.h"
#include "bsp_key.h"
/** 按键按下标置宏
* 按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
* 若按键按下为低电平，把宏设置成 KEY_ON=0 ，KEY_OFF=1 即可
*/
#define KEY_ON 1
#define KEY_OFF 0

/**
* @brief 检测是否有按键按下
* @param GPIOx:具体的端口, x 可以是（A...K）
* @param GPIO_PIN:具体的端口位， 可以是 GPIO_PIN_x（x 可以是 0...15）
* @retval 按键的状态
* @arg KEY_ON:按键按下
* @arg KEY_OFF:按键没按下
*/
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
/*检测是否有按键按下 */
if (GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON ) {
/*等待按键释放 */
while (GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);
return KEY_ON;
} else
return KEY_OFF;
}

 int main(void)
{
 /* LED 端口初始化 */
 LED_GPIO_Config();
 
 /*初始化按键*/
 Key_GPIO_Config();
 
 /* 轮询按键状态，若按键按下则反转 LED */
 while (1) {
 if ( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON ) {
 /*LED1 反转*/
 LED1_TOGGLE;
 }
 
 if ( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON ) {
 /*LED2 反转*/
 LED2_TOGGLE;
 }
}
}