#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_DMA.h"

extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
extern uint32_t aDST_Buffer[BUFFER_SIZE];

int main(void)
{
/* �����űȽϽ������ */
uint8_t TransferStatus;

/* LED �˿ڳ�ʼ�� */
LED_GPIO_Config();

/* ���� RGB ��ɫ��Ϊ��ɫ */
 LED_GREEN;
 
 /* ����ʱ���� */
 Delay(0xFFFFFF);
 
 /* DMA �������� */
 DMA_Config();
 
 /* �ȴ� DMA ������� */
 while (DMA_GetFlagStatus(DMA_STREAM,DMA_FLAG_TCIF)==DISABLE) {
 
 }
 
 /* �Ƚ�Դ�����봫������� */
 TransferStatus=Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE);
 
 /* �ж�Դ�����봫������ݱȽϽ��*/
 if (TransferStatus==0) { 
 /* Դ�����봫������ݲ����ʱ RGB ��ɫ����ʾ��ɫ */
 LED_RED;
 } else {
 /* Դ�����봫����������ʱ RGB ��ɫ����ʾ��ɫ */
 LED_BLUE;
 }
 
 while (1) {
 }
 }

void TimingDelay_Decrement(void){
}
