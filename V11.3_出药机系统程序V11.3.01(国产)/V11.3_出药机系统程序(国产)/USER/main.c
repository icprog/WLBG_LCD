/**********************************************
���ƣ���ҩ��ϵͳ���Ƴ���(������)   ���õ�·�壺Drug Delivery CON_V11.32 PCB
���ܣ�1�������ŷ�����˶� (��������)
			2������ֱ���������ת
			3������紫�����ź�
			
ԭʼ�汾�ţ�V11.3.00   �������ڣ�2017-02-13		������ڣ�2017-12-12       ��ʱ�� 10����

��ʷ�޸İ汾�ţ� ��    �޸����ڣ�
�޸�ԭ��1��

���°汾��: V11.3.01    �޸����ڣ�2018-04-20  ������ڣ�
�޸�ԭ��1�����ӻ�����ʼ������Ƿ����

***********************************************/
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "define.h"
#include "declare.h"

TEST volatile Test;

int main(void)
{		
	
	delay_init();	    	 		//��ʱ������ʼ��	  
	NVIC_Configuration(); 	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	PORT_IO_Init();					//�˿ڳ�ʼ��
	Disable_SON();
	DialSwitch_Init();			//���뿪�ض˿ڳ�ʼ��
	uart_init(9600);	 			//���ڳ�ʼ��Ϊ9600 
  TIM2PWM_Set(20*1000,SERVO_DISABLE);  //�����ŷ����PWM������ʼ������ʼ��Ƶ�� = 20KHz,(Ƶ�ʣ�ռ�ձ�)
 	MYDMA_Config(DMA_UART1_TX,0,(u32)&USART1->DR,(u32)DMA_SEND_BUFF,16);	//DMA1ͨ��4,����Ϊ����1����   	 
 	MYDMA_Config(DMA_UART1_RX,1,(u32)&USART1->DR,(u32)DMA_REV_BUFF,65535);//DMA1ͨ��5,����Ϊ����1����   	 
	DMA_INIT();//DMA �жϳ�ʼ��
	Slave_ID = SW_ID();
	TIM7_Int_Init(10000,71);	//10ms��ʱ
  Coder_InCap_Tim3(10000,71);  //�������������岶���ʼ��
	TEST_LED = 1;					//���Ե���

	delay_ms(1000);		
	delay_ms(1000);			
	delay_ms(1000);		
	delay_ms(1000);			
	
	delay_ms(500);		

	Enable_SON();				//���ŷ�ʹ��	
	delay_ms(110);		
//	DOOR_Check();							//��ȫ�ű������
	AllSystem_Init();     //ʹ���л�����ʼ��
	WORK_LED_CNT = TIMER7_CNT + 50;		//500ms word_led��˸��

///////////////���Գ���//////////////
//	Gate_Opening();							//բ��ֱ�������ת
//	SERVO_REVRun(1);   				  //���������������� 25r/min											
/////////////////////////////////////////////

 	while(1)
	{	
///////////����ҩ��////////
//			Dou_Closed();				
//			delay_ms(1000);		
//			delay_ms(1000);		
//			Dou_Opened();		
//			delay_ms(1000);		
//			delay_ms(1000);		
//////////////////////////////////////		

////////����բ��////////
//			Gate_Closed();				
//			delay_ms(1000);		
//			delay_ms(1000);		
//			Gate_Opened();		
//			delay_ms(1000);		
//			delay_ms(1000);		
///////////////////////////////////
		
		ButtonScan();             //����ɨ�裬���Գ���
		UART1_Com_Check();				//UART1 ͨѶ�������
		AD_Check();								//�ӻ���ַAD��⴦�����
//		DOOR_Check();							//��ȫ�ű������
		ALM_Check();							//�ŷ�����������
	}	 
}

