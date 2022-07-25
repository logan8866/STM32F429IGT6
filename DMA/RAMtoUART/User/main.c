#include "stm32f4xx.h"
#include "bsp_uart.h"

int main(void)
{
/*初始化 USART 配置模式为 115200 8-N-1，中断接收*/
Debug_USART_Config();

Usart_SendString( DEBUG_USART,"wangyiqing");

while (1) {
}
}

void TimingDelay_Decrement(){
}
