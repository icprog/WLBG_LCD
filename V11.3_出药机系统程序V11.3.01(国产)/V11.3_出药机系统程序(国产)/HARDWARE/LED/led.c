#include "stm32f10x.h"
#include "led.h"
#include "sys.h"
#include "declare.h"

////ͨѶʹ��LED
//void enable_led(void)
//{
//	u16 tmp;
//	u8 *ptr;
//	
//	ptr = (u8 *)&DMA_REV_BUFF[1];
//	tmp = LRC16(ptr,3);			//����ͨѶ��LRCУ���
//	if(tmp == ((LRC_H <<8) | LRC_L))
//	{//��LRCУ�����ȷ����ִ�����г���
//		if(UART1.Led_Color == 0)						//����3���ֽ� = 0: ���е� 1���̵�  2:��� 
//		{
//			if(UART1.Led_State == 0)					//��4λ�ֽ� = 0�������е�
//			{
//				GreenLed_Disable();
//				RedLed_Disable();
//				BUZZ_Disable();
//				
//				LED_STATE = LED_DISABLE;				//״̬����
//			}
//			else if(UART1.Led_State == 1)			//1�������е�
//			{
//				GreenLed_Enable();
//				RedLed_Enable();
//				BUZZ_Disable();
//				LED_STATE = LED_ENABLE;					//״̬����
//			
//			}
//			else if(UART1.Led_State == 2)			//2�����̵ƽ�����˸
//			{
//				GreenLed_Disable();
//				RedLed_Disable();
//				BUZZ_Disable();
//				LED_STATE = LED_BLINK;					//״̬����
//				Led_Blink_TimeValue = TIMER7_CNT + 50;	//500ms����
//			}
//			else															//�޲���
//			{
//				
//			}
//			LED_COLOR = ALL_LED;
//		}
//		else if(UART1.Led_Color == 1)				//1���̵�
//		{//ʹ���̵� ��������һ��
//			if(UART1.Led_State == 0)					//��4λ�ֽ� = 0����� 
//			{
//				GreenLed_Disable();
//				RedLed_Disable();
//				BUZZ_Disable();
//				LED_STATE = LED_DISABLE;				//״̬����
//			}
//			else if(UART1.Led_State == 1)			//1������
//			{
//				GreenLed_Enable();
//				RedLed_Disable();
//				GREEN_LED_TIM_VALUE = TIMER7_CNT +50;	//��500ms
//				BUZZ_STATE = GREEN_LED_STATE;		//��¼��ǰ״̬Ϊ�̵Ʒ�����״̬
//				BUZZ_Enable();
//				LED_STATE = LED_ENABLE;					//״̬����
//			
//			}
//			else if(UART1.Led_State == 2)			//2����˸
//			{
//				GreenLed_Enable();
//				RedLed_Disable();
//				BUZZ_Disable();
//				LED_STATE = LED_BLINK;					//״̬����
//				Led_Blink_TimeValue = TIMER7_CNT + 50;	//500ms����
//			}
//			else															//�޲���
//			{
//				
//			}
//			LED_COLOR = GREEN_LED;
//		}
//		else if(UART1.Led_Color == 2)				//2�����
//		{//ʹ�ܺ�� ������������
//			if(UART1.Led_State == 0)					//��4λ�ֽ� = 0����� 
//			{
//				GreenLed_Disable();
//				RedLed_Disable();
//				BUZZ_Disable();
//				LED_STATE = LED_DISABLE;				//״̬����
//			}
//			else if(UART1.Led_State == 1)			//1������
//			{
//				GreenLed_Disable();
//				RedLed_Enable();
//				Red_Led_Num = 3;
//				BUZZ_STATE = RED_LED_STATE;			//��¼��ǰ״̬Ϊ��Ʒ�����״̬
//				RED_LED_TIM_VALUE = TIMER7_CNT +50;	//��500ms
////				BUZZ_Enable();
//				LED_STATE = LED_ENABLE;					//״̬����
//			
//			}
//			else if(UART1.Led_State == 2)			//2����˸
//			{
//				GreenLed_Disable();
//				RedLed_Enable();
//				BUZZ_Disable();
//				LED_STATE = LED_BLINK;					//״̬����
//				Led_Blink_TimeValue = TIMER7_CNT + 50;	//500ms����
//			}
//			else															//�޲���
//			{
//				
//			}
//			LED_COLOR = RED_LED;
//		}
//		else
//		{
//			
//		}			
//	}
//	else
//	{//LRCУ�������
//		for(tmp=0;tmp<8;tmp++)
//		{
//			DMA_REV_BUFF[tmp] = 0;		//���ͨѶ������
//		}
//	}
//}

////LED��˸ʹ�ܼ��
//void Led_State_Check(void)
//{
//	switch(LED_STATE)
//	{
//		case 0: //  LED_STATE = 0������
//				GreenLed_Disable();
//				RedLed_Disable();
//				BUZZ_Disable();

//			break;
//		case 1:	//  LED_STATE = 1������
//			if(UART1.Led_Color == 0)   //���е�
//			{
//				GreenLed_Enable();
//				RedLed_Enable();
//			}
//			if(UART1.Led_Color == 1)	 //�̵�
//			{
//				GreenLed_Enable();
//				RedLed_Disable();
//			}
//			if(UART1.Led_Color == 2)	 //���
//			{
//				GreenLed_Disable();
//				RedLed_Enable();
//			}
//			else
//			{
//				
//			}
//			break;
//		case 2:		//  LED_STATE = 2������
//			if(UART1.Led_Color == 0)   //���е�
//			{
//				if(Led_Blink_TimeValue == TIMER7_CNT)
//				{
//					if(GREEN)
//					{
//						GREEN = 0;
//						RED = 1;
//					}
//					else
//					{
//						GREEN = 1;
//						RED = 0;
//					}
//					Led_Blink_TimeValue = TIMER7_CNT + 50;
//				}
//			}
//			if(UART1.Led_Color == 1)	 //�̵�
//			{
//				if(Led_Blink_TimeValue == TIMER7_CNT)
//				{
//					if(GREEN)
//					{
//						GREEN = 0;
//						RED = 0;
//					}
//					else
//					{
//						GREEN = 1;
//						RED = 0;
//					}
//					Led_Blink_TimeValue = TIMER7_CNT + 50;
//				}
//			}
//			if(UART1.Led_Color == 2)	 //���
//			{
//				if(Led_Blink_TimeValue == TIMER7_CNT)
//				{
//					if(RED)
//					{
//						RED = 0;
//						GREEN = 0;
//					}
//					else
//					{
//						RED = 1;
//						GREEN = 0;
//					}
//					Led_Blink_TimeValue = TIMER7_CNT + 50;	//500ms����
//				}
//			}
//			break;
//		default:
//			break;
//	}
//}

////��������ʱ���
//void Buzz_Tim_Check(void)
//{
//	switch(BUZZ_STATE)
//	{
//		case GREEN_LED_STATE:
//				if((BUZZ == 1)&&(GREEN_LED_TIM_VALUE == TIMER7_CNT))
//				{
//					BUZZ_Disable();
//				}
//			break;
//		case RED_LED_STATE:
//				if(Red_Led_Num)
//				{//3��û������
//					switch(RED_LED_BUZZ_STATE)
//					{
//						case RED_LED_BUZZ_DISABLE:
//							if(RED_LED_TIM_VALUE == TIMER7_CNT)
//							{//500ms������� ����ʹ�ܷ�����
//								BUZZ_Enable();
//								RED_LED_TIM_VALUE = TIMER7_CNT + 50;		//��500ms
//								RED_LED_BUZZ_STATE = RED_LED_BUZZ_ENABLE;
//							}
//							break;
//						case RED_LED_BUZZ_ENABLE:
//								if(RED_LED_TIM_VALUE == TIMER7_CNT)
//								{//500msʹ����� �رշ�����
//									BUZZ_Disable();
//									RED_LED_TIM_VALUE = TIMER7_CNT + 50;
//									RED_LED_BUZZ_STATE = RED_LED_BUZZ_DISABLE;
//									Red_Led_Num--;
//								}
//							break;
//						default:
//							break;
//					}
//				}
//			break;
//		default:
//			break;
//	}
//}


