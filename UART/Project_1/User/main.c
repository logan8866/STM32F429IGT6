#include "stm32f4xx.h"
#include "bsp_uart.h"

int main(void)
{
/*��ʼ�� USART ����ģʽΪ 115200 8-N-1���жϽ���*/
Debug_USART_Config();

Usart_SendString( DEBUG_USART,"wangyiqing");

while (1) {
}
}

void TimingDelay_Decrement(){
}
