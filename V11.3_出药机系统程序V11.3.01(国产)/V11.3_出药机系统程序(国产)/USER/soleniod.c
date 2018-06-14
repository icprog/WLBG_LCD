#include "stm32f10x.h"
#include "sys.h"
#include "stm32f10x_dma.h"
#include "declare.h"
#include "stm32f10x_tim.h"

//����ָʾ�ơ��ŷ�������������ƶˡ�ֱ��������ƶˡ�����紫�����˿ڳ�ʼ��
void PORT_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;				//�����ṹ��GPIO_InitStructure
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);			//ʹ��PA, PB,PC�˿�ʱ��

	//****************************************�ŷ�������������ƶ˿ڳ�ʼ��**************************************
	//////PA0-PLUS(�ŷ�����������)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;					//�ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);										//���ݲ�����ʼ��GPIOA
	
	//////PA1-SON(�ŷ�������ʹ��), PA2-CLR(�ŷ��������������)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  				//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;					//�ٶ�Ϊ2MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);										//���ݲ�����ʼ��GPIOA

	//////PB11-SIGN(�ŷ�����������), PB9-TEST_LED(����ָʾ��)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  				//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;					//�ٶ�Ϊ2MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);										//���ݲ�����ʼ��GPIOB
	
	//////PB12-ALM(�ŷ���������������ź�), PB13-EDM(�ŷ������ȫ��ͣ�ź�)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;							//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);										//���ݲ�����ʼ��GPIOB
	
	//**********************************************ֱ������˿ڳ�ʼ��***********************************************
	//////PB14-Dou_A(ҩ��ֱ�����A), PB15-Dou_B(ҩ��ֱ�����B)
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  				//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;					//�ٶ�Ϊ2MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);										//���ݲ�����ʼ��GPIOB

	//////PC6-Gate_A(բ��ֱ�����A), PC7-Gate_B(բ��ֱ�����B),PC13-Working(����ָʾ��)
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  				//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;					//�ٶ�Ϊ2MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);										//���ݲ�����ʼ��GPIOC

	//************************************��紫�����˿ڳ�ʼ��(��紫�����ź���Ϊ�����ź�)***************************
	////// PA3-RST(��������λ�ź�), PA4-Top(������������λ�ź�), PA5-GS_CLOSE(բ�Źر��ź�), PA7-GS_OPEN(բ�Ŵ��ź�), 
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;							//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);										//���ݲ�����ʼ��GPIOA

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
//	GPIO_Init(GPIOA, &GPIO_InitStructure);										//���ݲ�����ʼ��GPIOA

//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;							//��������
//	GPIO_Init(GPIOA, &GPIO_InitStructure);										//���ݲ�����ʼ��GPIOA

////// PB0-DS_OPEN(ҩ���򿪴������ź�)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;							//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);										//���ݲ�����ʼ��GPIOB

	////// PC4-Opposite(��ȫ���䴫�����ź�),PC5-DS_CLOSE(ҩ���ر��ź�),PC10-BUTTON(���԰���)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;							//��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);										//���ݲ�����ʼ��GPIOC
                                                                                                                                                                                                                                               
}

//բ�����ڴ�
void Gate_Opening(void)
{
	Gate_A = 1;			//բ��ֱ�������ת
	Gate_B = 0;	
}

//բ�����ڹر�
void Gate_Closing(void)
{
	Gate_A = 0;			//բ��ֱ�������ת
	Gate_B = 1;	
}

//բ��ֹͣ
void Gate_Stop(void)
{
	Gate_A = 1;			//����ֱ�����
	Gate_B = 1;
}

//բ����ȫ��
void Gate_Opened(void)
{
	InitFlag = 1;										//��ʼ����־λ��1
	TimeoutDly = TIMER7_CNT + 500;	//������ʱʱ��5s
	Gate_Opening();									//բ��ֱ�������ת
	while(InitFlag)
	{
		if(TimeoutDly == TIMER7_CNT)
		{
			Gate_Stop();										//���ֹͣ����
			GateState = 1;									//բ�Ŵ�״̬
			GateStatus = 2;									//բ�Ŵ��쳣����
			InitFlag = 0;										//��־λ����
		}
		else if(GS_OPEN_Level)
		{
			Gate_Stop();										//���ֹͣ����
			GateState = 1;									//բ��Ϊ��״̬				
			GateStatus = GateState;					//բ�Ŵ�
			InitFlag = 0;										//��־λ����
		}
	}
}

//բ����ȫ�ر�
void Gate_Closed(void)
{
	InitFlag = 1;										//��ʼ����־λ��1
	TimeoutDly = TIMER7_CNT + 500;	//������ʱʱ��5s
	Gate_Closing();									//բ��ֱ�������ת
	while(InitFlag)
	{
		if(TimeoutDly == TIMER7_CNT)
		{//��⴫������ʱ
			Gate_Stop();										//���ֹͣ����
			GateState = 1;									//բ�Źر�
			GateStatus = 2;									//բ�Źر��쳣����
			InitFlag = 0;										//��־λ����
		}
		else if(GS_CLOSE_Level)
		{
			Gate_Stop();										//���ֹͣ����
			GateState = 0;									//բ��Ϊ�ر�״̬				
			GateStatus = GateState;					//բ�Źر�
			InitFlag = 0;										//��־λ����
		}
	}
}

//ҩ���򿪽���ʱ
void Dou_Opening(void)
{
	Dou_A = 0;			//��բ��ֱ�����B2 ��ת
	Dou_B = 1;	
}

//ҩ���رս���ʱ
void Dou_Closing(void)
{
	Dou_A = 1;			//����ֱ�����B2 ��ת
	Dou_B = 0;	
}
//ҩ��ֹͣ
void Dou_Stop(void)
{
		Dou_A = 1;			//����ֱ�����B2
		Dou_B = 1;
}

//ҩ����ȫ��
void Dou_Opened(void)
{
	InitFlag = 1;										//��ʼ����־λ��1
	TimeoutDly = TIMER7_CNT + 500;	//������ʱʱ��5s
	Dou_Opening();
	while(InitFlag)
	{
		if(TimeoutDly == TIMER7_CNT)
		{
			Dou_Stop();											//���ֹͣ����
			DouState = 1;										//���Ŵ�
			DouStatus = 2;									//���Ŵ��쳣����
			InitFlag = 0;										//��־λ����
		}
		else if(DS_OPEN_Level)
		{
			Dou_Stop();											//���ֹͣ����
			DouState = 1;										//����Ϊ��״̬				
			DouStatus = DouState;						//����Ϊ��״̬	
			InitFlag = 0;										//��־λ����
		}
	}
}

//ҩ����ȫ�ر�
void Dou_Closed(void)
{
	InitFlag = 1;										//��ʼ����־λ��1
	TimeoutDly = TIMER7_CNT + 500;	//������ʱʱ��5s
	Dou_Closing();
	while(InitFlag)
	{
		if(TimeoutDly == TIMER7_CNT)
		{
			Dou_Stop();										//���ֹͣ����
			DouState = 0;									//���Źر�״̬
			DouStatus = 2;								//���Źر��쳣����
			InitFlag = 0;									//��־λ����
		}
		else if(DS_CLOSE_Level)
		{
			Dou_Stop();										//���ֹͣ����
			DouState = 0;									//����Ϊ�ر�״̬				
			DouStatus = DouState;					//����Ϊ�ر�״̬		
			InitFlag = 0;									//��־λ����
		}
	}
}

//��������λ����
void LiftReset(void)
{
	if(DouState == 1)
	{//ҩ����־λ=1������״̬
		Dou_Closed();							//�ر�ҩ��
	}		
	else
	{
		switch(Slave_ID)
		{
			case 2:  	//��������
				SERVO_FWDRun(3);    	//�������������� 75r/min											
				break;
			
			case 3:		//��������
				SERVO_REVRun(3);    	//�������������� 75r/min												
				break;
			
			default:
				break;
		}
		InitFlag = 1;										//��ʼ����־λ��1
		while(InitFlag)
		{
			if(Bottom_Level)
			{
				SERVO_STOP();														//���������ź��ŷ����ֹͣ
				InitFlag = 0;						//��־λ����
				MachaStatus = 0;				//�������ص�ԭ��					
				delay_ms(100);
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(1);    	//�������������� 25r/min												
						break;
					
					case 3:		//��������
						SERVO_FWDRun(1);    	//�������������� 25r/min											
						break;
					
					default:
						break;
				}
				delay_ms(10);
				SERVO_STOP();														//���������ź��ŷ����ֹͣ
			}
		}
	}
	
}

//��������ź�
void CLRALM(void)
{//��������ź�ʱ�򣺸ߡ����͡�����
//	CLR = 1;
//	delay_ms(10);		
	CLR = 0;
	delay_ms(250);
	CLR = 1;
	delay_ms(10);
	AllSystem_Init();     //ʹ���л�����ʼ��
}

/*************************�ŷ��������********1KHz = 25r/min******************/
//f: 1-25r/min, 8-200r/min, 24-600r/min, 32-800r/min, 40-1000r/min
//����Ƥ��������,�������������У���������������
void SERVO_FWDRun(u8 f)
{
	FWD();																	//��ת(����ʵ������趨)
	TIM2PWM_Set(f*1000,SERVO_ENABLE);			  //fԤ��ֵ�����趨
}

//����Ƥ��������,�������������У��������������� 
void SERVO_REVRun(u8 f)
{
	REV();																	//��ת(����ʵ������趨)
	TIM2PWM_Set(f*1000,SERVO_ENABLE);			  //fԤ��ֵ�����趨
}

//�ŷ����ֹͣ
void SERVO_STOP(void)
{
	TIM2PWM_Set(20*1000,SERVO_DISABLE);  		//�ŷ����ֹͣ
}

//������ʼ��
void AllSystem_Init(void)  
{
	if(Slave_ID == 1)
	{
		SERVO_FWDRun(3);   //��75r/min
		delay_ms(1000);
		SERVO_STOP();
		delay_ms(1000);
		SERVO_REVRun(3);   //��75r/min
		delay_ms(1000);
		SERVO_STOP();
		MachaStatus = 0;
		DouStatus = 0;
		GateStatus = 0;
		ServoStatus = 0;
		Reason = 0;						//�ӻ�����
	}
	else if((Slave_ID == 2) || (Slave_ID == 3))
	{
		Gate_Closed();				//բ�Źر�
		Dou_Closed();					//ҩ���ر�
		LiftReset();
		ServoStatus = 0;			//�ŷ��������ޱ���
		MachaStatus = 0;			//�������ڵײ�
		if((GateStatus == 2)||(DouStatus == 2))
		{
			Reason = 4;						//�ӻ������в��ɻָ��Ĺ���			
		}
		else
		{
			Reason = 0;						//�ӻ�����						
		}
		delay_ms(1000);
	}
	else
	{
	}
}

void BeltClearALM(void)
{
	switch(ALMState)
	{
		case 1:		//���յ������źţ���CLR = 1
			DlyTime = TIMER7_CNT + 1;									//��ʱ10ms
			ALMState = 2;															//case 2: ��ʱ10ms���
			break;
		
		case 2:		//case 2: ��ʱ10ms���
			if(DlyTime == TIMER7_CNT)
			{//clr = 1,��ʱ10msʱ�䵽
				CLR = 0;																//CLR = 0,��ʱ150ms
				DlyTime = TIMER7_CNT + 15;							//150ms��ʱ
				ALMState = 3;														//case 3:150ms��ʱ���
			}
			break;
			
		case 3:		//case 3:150ms��ʱ���
			if(DlyTime == TIMER7_CNT)
			{//clr = 0;��ʱ150msʱ�䵽
				CLR = 1;																//CLR = 1,��ʱ110ms
				DlyTime = TIMER7_CNT + 11;							//��ʱ110ms
				ALMState = 4;														//case 4:��ʱ110ms���
			}
			break;
			
		case 4:		//case 4:��ʱ110ms���
			if(DlyTime == TIMER7_CNT)
			{//clr = 1,��ʱ110ms���
				if(ALM_Level == 0)
				{//�������źŲ������
					Reason = 4;			//�ŷ����������ֲ�������Ĵ����ź�
				}
				else
				{//�����źſ����
					Reason = 0;			//�ŷ����������������
					ServoStatus = 0;						//�ŷ��ޱ���
				}
			}
			break;
			
		default:
			break;
	}
}

void DOOR_Check(void)
{
	switch(EDMState)
	{
		case 0:		//�жϰ�ȫ��ͣ�ź�			
			if(EDM_Level == 1)
			{//��ȫ��ͣ�ź� = 1
				EDMDlyTime = TIMER7_CNT + 30;		//��ʱ300ms
				EDMState = EDM_DLY_300ms;				//case 1:EDM�źŷ������
			}
			break;
			
		case EDM_DLY_300ms:	//case 1:EDM�źŷ������
			if(EDMDlyTime == TIMER7_CNT)
			{
				if(EDM_Level == 1)
				{//��ȫ��ͣ�ź� = 1,ȷ����EDM�ź�
					SERVO_STOP(); 								//���ֹͣ
					Disable_SON();
					LiftRunState = 0;
					Test.State = 0;								//���������ж�
					TEST_LED = 1;									//���Ե���					
					Test.LEDEnable = DISABLE;			//ֹͣ��˸
					Reason = 5;										//��ͣ�źţ��������Ŵ򿪱���
					EDMState = DoorOpen;				
				} 
				else
				{
					EDMState = 0;				
				}
			}
			break;
		
		case DoorOpen:
			if(EDM_Level == 0)
			{
				EDMState = DoorClose;
			}
			break;
		
		case DoorClose:		//�ű�����
			Enable_SON();				//���ŷ�ʹ��	
			EDMDlyTime = TIMER7_CNT + 11;		//��ʱ110ms
			EDMState = EDM_DLY_110ms;
			break;			
		
		case EDM_DLY_110ms:		//���110ms
			AllSystem_Init();     //ʹ���л�����ʼ��
			EDMState = 0;					//���ص���ʼ״̬
			break;

		default:
			SERVO_STOP(); 								//���ֹͣ
			LiftRunState = 0;
			Test.State = 0;								//���������ж�
			TEST_LED = 1;									//���Ե���					
			Test.LEDEnable = DISABLE;			//ֹͣ��˸
			AllSystem_Init();     //ʹ���л�����ʼ��
			EDMState = 0;					//���ص���ʼ״̬
			break;
	}	
}

void ALM_Check(void)
{
	switch(ALMState)
	{
		case 0:		//�ŷ������������źų�ʼ״̬,�ж��Ƿ����ŷ�����
			if(ALM_Level == 0)
			{//���ŷ������ź�
				ALMDlyTime = TIMER7_CNT + 10;		//�����ź���ʱ100ms
				ALMState = ALM_DLY_100ms;				//case 1:ALM�����ź���ʱ100msʱ����
			}
			break;
		
		case ALM_DLY_100ms:
			if(ALMDlyTime == TIMER7_CNT)
			{
				if(ALM_Level == 0)
				{
					SERVO_STOP(); 								//���ֹͣ
					Disable_SON();
					LiftRunState = 0;
					RunFlag = 0;
					Test.State = 0;								//���������ж�
					TEST_LED = 1;									//���Ե���					
					Test.LEDEnable = DISABLE;			//ֹͣ��˸
					ServoStatus = 1;							//�ŷ��б���
					Reason = 4;										//�ŷ��������
					ALMDlyTime = TIMER7_CNT + 1000;		//���������Ҫ��ʱ10s
					ALMState = ALM_DLY_10s;									
				}
				else
				{
					ALMState = 0;
				}
			}
			break;
			
		case ALM_DLY_10s:
			if(ALMDlyTime == TIMER7_CNT)
			{
				CLR = 1;
				ALMDlyTime = TIMER7_CNT + 1;		//��ʱ10ms
				ALMState = ALM_DLY_10ms;
			}
			break;
			
		case ALM_DLY_10ms:
			if(ALMDlyTime == TIMER7_CNT)
			{
				CLR = 0;
				ALMDlyTime = TIMER7_CNT + 15;		//��ʱ150ms
				ALMState = ALM_DLY_150ms;
			}
			break;
			
		case ALM_DLY_150ms:
			if(ALMDlyTime == TIMER7_CNT)
			{
				CLR = 1;
				ALMDlyTime = TIMER7_CNT + 10;		//��ʱ100ms
				ALMState = ALM_DLY_1000us;			
			}
			break;
			
		case ALM_DLY_1000us:
			
			if(ALMDlyTime == TIMER7_CNT)
			{
				if(ALM_Level == 1)
				{//�������
					Enable_SON();				//���ŷ�ʹ��	
					Reason = 0;										//�ŷ�����������
					ALMDlyTime = TIMER7_CNT + 11;		//��ʱ110ms
					ALMState = ALM_DLY_110ms;
				}
				else
				{//����δ���
					ServoStatus = 1;							//�ŷ��б���
					Reason = 4;										//�ŷ��������
				}
			}
			break;
			
		case ALM_DLY_110ms:
			AllSystem_Init();     //ʹ���л�����ʼ��
			ALMState = 0;					//���ص���ʼ״̬			
			break;

			
		default:
			SERVO_STOP(); 								//���ֹͣ
			LiftRunState = 0;
			Test.State = 0;								//���������ж�
			TEST_LED = 1;									//���Ե���					
			Test.LEDEnable = DISABLE;			//ֹͣ��˸
			AllSystem_Init();     //ʹ���л�����ʼ��
			ALMState = 0;					//���ص���ʼ״̬
			break;		
	}
}

//////////����Ƥ�����ư�/////////////////
void TransmitBeltControl(void)
{
	u16 RunTime;    		//�ŷ��������ʱ��
	u8	RunMode;				//����ģʽ
	u16 ID = 0x0011;		//����Ƥ���Ļ��������ַ��0x0011
	RunTime = DMA_REV_BUFF[5];		//�ŷ��������ʱ��
	RunMode = DMA_REV_BUFF[4];		//�ŷ��������ģʽ
		if(RunFlag == 1)
		{
			RunFlag = 0;
			if(ID == ( (DMA_REV_BUFF[2]<<8)|(DMA_REV_BUFF[3]) ))
			{//��ID=0x0011,����Ƥ��������ȷ
				switch(RunMode)
				{//�жϵ�5�ֽڣ��˶�ģʽ
					case 0:		//ֹͣ 
						SERVO_STOP(); 															//���ֹͣ
						MachaStatus = 0;						//���״̬��0����ֹͣ 2��������Ƥ����ת 3��������Ƥ����ת
						Reason = 0;									//����
						ServoStatus = 0;						//�ŷ��ޱ���
						break;
					
					case 0x10:		//�����˶� 200r/min
						SERVO_FWDRun(8);
						DlyTime = TIMER7_CNT + (RunTime * 100);
						MachaStatus = 2;						//���״̬��0����ֹͣ 2��������Ƥ����ת 3��������Ƥ����ת
						ServoStatus = 0;						//�ŷ��ޱ���
						break;
					
					case 0x11:		//�����˶� 600r/min
						SERVO_FWDRun(24);
						DlyTime = TIMER7_CNT + (RunTime * 100);
						MachaStatus = 2;						//���״̬��0����ֹͣ 2��������Ƥ����ת 3��������Ƥ����ת
						ServoStatus = 0;						//�ŷ��ޱ���
						break;
					
					case 0x12:		//�����˶� 800r/min
						SERVO_FWDRun(32);
						DlyTime = TIMER7_CNT + (RunTime * 100);
						MachaStatus = 2;						//���״̬��0����ֹͣ 2��������Ƥ����ת 3��������Ƥ����ת
						ServoStatus = 0;						//�ŷ��ޱ���
						break;
					
					case 0x13:		//�����˶� 1000r/min
						SERVO_FWDRun(40);
						DlyTime = TIMER7_CNT + (RunTime * 100);
						MachaStatus = 2;						//���״̬��0����ֹͣ 2��������Ƥ����ת 3��������Ƥ����ת
						ServoStatus = 0;						//�ŷ��ޱ���
						break;
					
					case 0x20:		//�����˶� 200r/min
						SERVO_REVRun(8);
						DlyTime = TIMER7_CNT + (RunTime * 100);
						MachaStatus = 3;						//���״̬��0����ֹͣ 2��������Ƥ����ת 3��������Ƥ����ת
						ServoStatus = 0;						//�ŷ��ޱ���
						break;
					
					case 0x21:		//�����˶� 600r/min
						SERVO_REVRun(24);
						DlyTime = TIMER7_CNT + (RunTime * 100);
						MachaStatus = 3;						//���״̬��0����ֹͣ 2��������Ƥ����ת 3��������Ƥ����ת
						ServoStatus = 0;						//�ŷ��ޱ���
						break;
					
					case 0x22:		//�����˶� 800r/min
						SERVO_REVRun(32);
						DlyTime = TIMER7_CNT + (RunTime * 100);
						MachaStatus = 3;						//���״̬��0����ֹͣ 2��������Ƥ����ת 3��������Ƥ����ת
						ServoStatus = 0;						//�ŷ��ޱ���
						break;
					
					case 0x23:		//�����˶� 1000r/min
						SERVO_REVRun(40);
						DlyTime = TIMER7_CNT + (RunTime * 100);
						MachaStatus = 3;						//���״̬��0����ֹͣ 2��������Ƥ����ת 3��������Ƥ����ת
						ServoStatus = 0;						//�ŷ��ޱ���
						break;
										
					default:
						SERVO_STOP(); 															//���ֹͣ
						MachaStatus = 0;						//���״̬��0����ֹͣ 2��������Ƥ����ת 3��������Ƥ����ת
						Reason = 0;									//����
						ServoStatus = 0;						//�ŷ��ޱ���
						break;
				}
			}
			else
			{
			
			}		
		}
		if(RunTime)			//�������ʱ��DMA_REV_BUFF[5]
		{//RunTime>0��������N���ֹͣ
			if(DlyTime == TIMER7_CNT)			//��ʱʱ��
			{//��ʱʱ�䵽
				SERVO_STOP(); 															//���ֹͣ			
				MachaStatus = 0;						//���״̬��0����ֹͣ 2��������Ƥ����ת 3��������Ƥ����ת
				Reason = 0;									//����
				ServoStatus = 0;						//�ŷ��ޱ���
			}
		}
}

//void LeftLiftClearALM(void)
//{
//	switch(ALMState)
//	{
//		case 1:		//���յ������źţ���CLR = 1
//			DlyTime = TIMER7_CNT + 1;									//��ʱ10ms
//			ALMState = 2;															//case 2: ��ʱ10ms���
//			break;
//		
//		case 2:		//case 2: ��ʱ10ms���
//			if(DlyTime == TIMER7_CNT)
//			{//clr = 1,��ʱ10msʱ�䵽
//				CLR = 0;																//CLR = 0,��ʱ150ms
//				DlyTime = TIMER7_CNT + 15;							//150ms��ʱ
//				ALMState = 3;														//case 3:150ms��ʱ���
//			}
//			break;
//			
//		case 3:		//case 3:150ms��ʱ���
//			if(DlyTime == TIMER7_CNT)
//			{//clr = 0;��ʱ150msʱ�䵽
//				CLR = 1;																//CLR = 1,��ʱ110ms
//				DlyTime = TIMER7_CNT + 11;							//��ʱ110ms
//				ALMState = 4;														//case 4:��ʱ110ms���
//			}
//			break;
//			
//		case 4:		//case 4:��ʱ110ms���
//			if(DlyTime == TIMER7_CNT)
//			{//clr = 1,��ʱ110ms���
//				if(ALM_Level == 0)
//				{//�������źŲ������
//					Reason = 4;			//�ŷ����������ֲ�������Ĵ����ź�
//				}
//				else
//				{//�����źſ����
//					Gate_Closing();				//�ر�բ��
//					Dou_Closing();				//���ر�
//					ALMState = 5;					//case 5:բ�Ŵ������źż��
//				}
//			}
//			break;
//			
//		case 5:		//case 5:բ�Ŵ������źż��
//			if(GS_CLOSE_Level == 1)
//			{//��⵽բ�Źرմ������źż��
//				Gate_Stop();											//����⵽�رմ�����ʱ�����ֹͣ����
//				GateState = 0;										//բ��Ϊ�ر�״̬
//				GateStatus = GateState;						//բ��״̬����: 0����բ�Źر� 1����բ�Ŵ� 2����բ�Ŵ򿪻��߹ر��쳣����
//			}
//			if(DS_CLOSE_Level == 1)				//����⵽ҩ���رմ�����ʱ
//			{
//				Dou_Stop();									//����⵽ҩ���رմ�����ʱ�����ֹͣ����
//				DouState = 0;								//ҩ��Ϊ�ر�״̬
//				DouStatus = DouState;				//ҩ��״̬������0����ҩ����ɵ�ҩ���� 1����δ��ɵ�ҩ���� 2����ҩ���򿪻�ر��쳣����
//				SERVO_REVRun(1);						//������������������ 25r/min
//				ALMState = 6;								//case 6: ��������λ�������źż��
//			}
//			break;
//			
//		case 6:			//case 6: ��������λ�������źż��
//			if(Bottom_Level == 1)
//			{//��⵽��λ��������ֹͣ���
//				SERVO_STOP(); 							//���ֹͣ	�������ص�ԭ������������΢С����
//				DlyTime = TIMER7_CNT + 15;	//150ms��ʱ
//				ALMState = 7;								//case 7: ��������λ��ֹͣ��ʱ���
//			}
//			break;
//			
//		case 7:			//case 7: ��������λֹͣ��ʱ���
//			if(DlyTime == TIMER7_CNT)
//			{//��������λͣ150ms�����س�150ms
//				SERVO_FWDRun(1);						//�����������˶� 25r/min
//				DlyTime = TIMER7_CNT + 15;	//��������λͣ150ms
//				ALMState = 8;								//case 8: ��������λ�س���ʱʱ�估���
//			}
//			break;
//			
//		case 8:			//case 8: ��������λ�س���ʱʱ�估���
//			if(DlyTime == TIMER7_CNT)
//			{//�������س�ʱ�䵽��ֹͣ���
//				SERVO_STOP(); 															//���ֹͣ
//				Reason = 0;									//����
//				ServoStatus = 0;						//�ŷ��������
//				MachaStatus = 0;						//������״̬��0�����ص���ʼ��, 1����δ�ص���ʼ��
//			}
//			break;
//		
//		default:
//			break;
//	}	
//}

//////////�����������ư�///����������������SERVO_REVRun()  ����������������SERVO_FWDRun()/////////////  
//////////�����������ư�   ����������������SERVO_FWDRun()  ����������������SERVO_REVRun()
void HoisterControl(void)
{
		switch(LiftRunState)
		{////�ж�����������״̬
			case 0:     //��������ʼ״̬���ж�����������״̬
				if(RunFlag == 1)
				{//�������б�־λ=1������������
					switch(ID)
					{
						case 0x0021:		//����������:ȷ��ҩ���Źرա������ٽ���������ȫ���𡪡������˶����趨���롪��ͣ��3s(�ɵ�)���������½�����������ȫ�����λ�á�������ʹ��������λ
							if(DouState == 1)
							{//ҩ����־λ=1������״̬
								Dou_Closed();																		//�ر�ҩ��
							}		
							if(Distance == 0)																	//����=0��ʹҩ����λ
							{
								switch(Slave_ID)
								{
									case 2:  	//��������
										SERVO_FWDRun(3);														//������������������ 75r/min
										break;
									
									case 3:		//��������
										SERVO_REVRun(3);    												//������������������ 75r/min												
										break;
									
									default:
										break;
								}
								LiftRunState = Lift_RST_Check;									//case 1:��������λ���������
							}
							else if((0 < Distance) && ( Distance < 135))			//�����������߶� < 135
							{ 
								PulseCntComplete = 0;							 							//����������������־λ
								FeedbackNowNum = 0;						
								FeedbackHopeNum = 2400 * Distance / 108;  	 		//�趨���������� = 2400 / 108 * ��������(mm), ������������Pr0.11=2400
								switch(Slave_ID)
								{
									case 2:  	//��������
										SERVO_REVRun(3);    												//�������������� 75r/min												
										break;
									
									case 3:		//��������
										SERVO_FWDRun(3);    												//�������������� 75r/min											
										break;
									
									default:
										break;
								}
								LiftRunState = Distance_Short_FeedbackPulse;		//case 4: ���������δ��ȫ������������
							}
							else if((135 <= Distance) && (Distance < 2095))		//����������ȫ����
							{//�ȵ����˶�75r/min,����������ȫ����								
								PulseCntComplete = 0;							 							//����������������־λ
								FeedbackNowNum = 0;		
								FeedbackHopeNum = 3000;													//3000p = 135mm ������ȫ����߶�
								switch(Slave_ID)
								{
									case 2:  	//��������
										SERVO_REVRun(3);    												//������������������ 75r/min										
										break;
									
									case 3:		//��������
										SERVO_FWDRun(3);    												//������������������ 75r/min											
										break;
									
									default:
										break;
								}
								LiftRunState = Distance_3000_Up_FeedbackPulse;	//case 6: ����������պ���ȫ������������=3000
							}
							else if(Distance == 0x0fff)
							{
								CLRALM();																				//��������ź�			
							}
							break;
							
					case 0x0022:		//բ�ſ���
						if(GateAction == 0)		
						{//բ�Źر�
							Gate_Closing();										//բ��ֱ�������ת
							LiftRunState = Gate_Close_Check;	//case 71: բ�Źرռ��					
						}
						else if(GateAction==1)	
						{//բ�Ŵ�
							if(MachaStatus == 0)
							{
								Gate_Opening();										//բ��ֱ����� 
								TimeoutDly = TIMER7_CNT + 500;		//������ʱʱ��5s
								Reason = 6;
								LiftRunState = Gate_Open_Check;		//case 72: բ�Źرռ��					
							}
						}
						else
						{						
						}
						break;
																
					case 0x0023:		//ҩ������
						if(DouAction == 0)		
						{//ҩ���ر�
							Dou_Closing();										//ҩ��ֱ�����B2
							LiftRunState = Dou_Close_Check;		//case 73: ҩ���رռ��					
						}
						else if(DouAction==1)	
						{//ҩ����
							Dou_Opening();										//ҩ��ֱ�����B2
							LiftRunState = Dou_Open_Check;		//case 74: ҩ���رռ��					
						}
						else
						{
						
						}
						break;
					
					case 0x0024:		//��������
						TimeoutDly = TIMER7_CNT + 500;	//������ʱʱ��5s
						if(DouState == 1)
						{//���ҩ�����Ǵ򿪵ģ��͹رն���
							Dou_Closing();														//�رն���
							LiftRunState = LinkageBefor_Dou_Close_Check;		//case 75: ���Ͽ��ƣ���������֮ǰ���ҩ���Ƿ�ر�״̬													
						}
						else
						{
							LiftRunState = LinkageBefor_Dou_Close_Check;		//case 75: ���Ͽ��ƣ���������֮ǰ���ҩ���Ƿ�ر�״̬													
						}
						Reason = 6;										//�ӻ��豸æµ���ӻ���æ�ڴ���һ����ʱ����������������ڴӻ�����ʱ������Ϣ

						break;
						
					case 0x0031:		//����������:ȷ��ҩ���Źرա������ٽ���������ȫ���𡪡������˶����趨���롪��ͣ��3s(�ɵ�)���������½�����������ȫ�����λ�á�������ʹ��������λ
						if(DouState)
						{//ҩ����־λ=1������״̬
							Dou_Closed();																		//�ر�ҩ��
						}													
						if(Distance == 0)																	//����=0��ʹҩ����λ
						{
							switch(Slave_ID)
							{
								case 2:  	//��������
									SERVO_FWDRun(3);														//������������������ 75r/min
									break;
								
								case 3:		//��������
									SERVO_REVRun(3);    												//������������������ 75r/min												
									break;
								
								default:
									break;
							}
							LiftRunState = Lift_RST_Check;									//case 1:��������λ���������
						}
						else if((0 < Distance) && ( Distance < 135))			//������δ��ȫ���𣬼��� 0 < Distance < 135
						{
							PulseCntComplete = 0;							 							//����������������־λ
							FeedbackNowNum = 0;						
							FeedbackHopeNum = 2400 * Distance / 108;  	 		//�趨���������� = 2400/108 * ��������(mm), ������������Pr0.11=2400
							switch(Slave_ID)
							{
								case 2:  	//��������
									SERVO_REVRun(3);    												//�������������� 75r/min												
									break;
								
								case 3:		//��������
									SERVO_FWDRun(3);    												//�������������� 75r/min											
									break;
								
								default:
									break;
							}
							MachaStatus = 1;																//������������ԭ��
							LiftRunState = Distance_Short_FeedbackPulse;		//case 4: ���������δ��ȫ������������
						}
						else if((135 <= Distance) && (Distance < 2095))		//����������ȫ����, 135 =< Distance < 2095
						{//�ȵ����˶�75r/min(�ɵ�),����������ȫ����								
							PulseCntComplete = 0;							 							//����������������־λ
							FeedbackNowNum = 0;		
							FeedbackHopeNum = 3000;													//��ȫ����������
							switch(Slave_ID)
							{
								case 2:  	//��������
									SERVO_REVRun(3);    												//������������������ 75r/min(�ɵ�)												
									break;
								
								case 3:		//��������
									SERVO_FWDRun(3);    												//������������������ 75r/min(�ɵ�)										
									break;
								
								default:
									break;
							}
							MachaStatus = 1;																//������������ԭ��
							LiftRunState = Distance_3000_Up_FeedbackPulse;	//case 6: ����������պ���ȫ������������=3000
						}
						else if(Distance == 0x0fff)
						{
							CLRALM();																				//��������ź�			
						}
						break;
						
				case 0x0032:		//բ�ſ���
					if(GateAction == 0)		
					{//բ�Źر�
						Gate_Closing();										//բ��ֱ�������ת
						LiftRunState = Gate_Close_Check;	//case 71: բ�Źرռ��					
					}
					else if(GateAction==1)	
					{//բ�Ŵ�
						if(MachaStatus == 0)
						{
							Gate_Opening();										//բ��ֱ����� 
							TimeoutDly = TIMER7_CNT + 500;		//������ʱʱ��5s
							Reason = 6;
							LiftRunState = Gate_Open_Check;		//case 72: բ�Ŵ򿪼��					
						}
					}
					else
					{
					
					}
					break;
										
				case 0x0033:		//ҩ������
					if(DouAction == 0)							
					{//ҩ���ر�
						Dou_Closing();										//ҩ��ֱ�����B2
						LiftRunState = Dou_Close_Check;		//case 73: ҩ���رռ��					
					}
					else if(DouAction==1)	
					{//ҩ����
						Dou_Opening();										//ҩ��ֱ�����B2
						LiftRunState = Dou_Open_Check;		//case 74: ҩ���رռ��					
					}
					else
					{
					
					}
					break;
				
				case 0x0034:		//��������  ���ȱ�֤ҩ�����ǹر�״̬
					TimeoutDly = TIMER7_CNT + 500;	//������ʱʱ��5s
					if(DouState == 1)
					{//���ҩ�����Ǵ򿪵ģ��͹رն���
						Dou_Closing();																	//�رն���
						LiftRunState = LinkageBefor_Dou_Close_Check;		//case 75: ���Ͽ��ƣ���������֮ǰ���ҩ���Ƿ�ر�״̬													
					}
					else
					{
						LiftRunState = LinkageBefor_Dou_Close_Check;		//case 75: ���Ͽ��ƣ���������֮ǰ���ҩ���Ƿ�ر�״̬													
					}
					Reason = 6;										//�ӻ��豸æµ���ӻ���æ�ڴ���һ����ʱ����������������ڴӻ�����ʱ������Ϣ
					break;
					
					default:
						break;
				}	
//					RunFlag = 0;					//����ӻ���Ӧ�����ʶ
			}
			else
			{
//					RunFlag = 0;					//����ӻ���Ӧ�����ʶ
			}
			break;
					
			case Lift_RST_Check:		//case 1:��������λ���������
				if(Bottom_Level == 1)
				{//��⵽��λ��������ֹͣ���
					delay_ms(10);	
					if(Bottom_Level == 1)
					{
						SERVO_STOP(); 										//���ֹͣ	�������ص�ԭ������������΢С����
						DlyTime = TIMER7_CNT + 15;				//150ms��ʱ
						LiftRunState = Distance_Return;		//case 2:0����س�
					}
				}
				break;
							
			case Distance_Return:   //case 2: ��������λ��س�ʱ���ж�
				if(DlyTime == TIMER7_CNT)
				{//��������λͣ150ms�����س�150ms
					PulseCntComplete = 0;							 	//����������������־λ
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_REVRun(1);    	//�������������� 25r/min												
							break;
						
						case 3:		//��������
							SERVO_FWDRun(1);    	//�������������� 25r/min											
							break;
						
						default:
							break;
					}
					DlyTime = TIMER7_CNT + 1;						//��������λͣ10ms
					LiftRunState = Lift_Return_End;			//case 3: �س̽�����ֹͣ��������ص������ж�����������״̬
				}
			break;
			
			case Lift_Return_End:		//case 3: �س̽�����ֹͣ��������ص������ж�����������״̬
				if(DlyTime == TIMER7_CNT)
				{//�س̽�����ֹͣ��������������ж�����״̬
					SERVO_STOP(); 				//���ֹͣ			
					MachaStatus = 0;			//�������ص�ԭ��
					DouStatus = DouState;				//��ҩ���
					GateStatus = GateState;				//��ҩ���
					ServoStatus = 0;			//�ŷ��������ޱ���
					LiftRunState	= 0;		//����������״̬�����ж�
					RunFlag = 0;					//����ӻ���Ӧ�����ʶ
					Distance_Reg = 0;
					if((DouStatus == 2)||(GateStatus == 2))
					{
						Reason = 4;						//�ӻ�����
					}
					else
					{
						Reason = 0;						//�ӻ�����
					}
				}
				break;
				
			case Distance_Short_FeedbackPulse:	//case 4:������δ��ȫ����ʱ�������������
				if(PulseCntComplete == 1)
				{//�����������������
					PulseCntComplete = 0;							 	//����������������־λ
					SERVO_STOP(); 										 	//�����������趨���룬���ֹͣ
					MachaStatus = 1;										//������δ��ԭ��
					DouStatus = 0;											//��ҩ���
					ServoStatus = 0;										//�ŷ��������ޱ���
					Reason = 6;													//�ӻ�æµ
					DlyTime = TIMER7_CNT + 300;				 	//��ʱ3s
					LiftRunState = Distance_Short_Dly;	//case 5: 0 < ���롡< 135mm ������δ��ȫ����߶���ʱʱ��
				}
				break;
			
			case Distance_Short_Dly:	//case 5: 0 < ���� < 580  ������δ��ȫ���𣬵�λ��ʱ3s
				if(DlyTime == TIMER7_CNT)
				{//3s��ʱ��
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_FWDRun(3);														//������������������ 75r/min
							break;
						
						case 3:		//��������
							SERVO_REVRun(3);    												//������������������ 75r/min												
							break;
						
						default:
							break;
					}
					LiftRunState = Lift_RST_Check;	//case 1:��������λ���������
				}
				break;	
							
			case Distance_3000_Up_FeedbackPulse:	//case 6: ���������ϵ��պ���ȫ���������������
				if(PulseCntComplete == 1)
				{//�������պ���ȫ����ʱ����������������ɣ��ٸ����˶����趨���� ���������� = 2050 ����������=155mm
					PulseCntComplete = 0;				 			//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 200;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_REVRun(6);    					//�����������ŷ������1�μ��� 150r/min												
							break;
						
						case 3:		//��������
							SERVO_FWDRun(6);    					//�����������ŷ������1�μ��� 150r/min										
							break;
						
						default:
							break;
					}
					MachaStatus = 1;									//������������ԭ��
					DouStatus = 0;										//ҩ���ر�
					ServoStatus = 0;									//�ŷ��������ޱ���
					Reason = 6;												//�ӻ�æµ
					LiftRunState = Up_Accelerate_1;		//case 7:�������ŷ����1���ٽ׶�,�������������
				}
				else
				{//�������պ�����ķ���������2050δ����
					
				}
				break;
				
			case Up_Accelerate_1:		//case 7:�������ŷ����1���ٽ׶�,�������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 300;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_REVRun(10);    					//�����������ŷ������2�μ��� 250r/min										
							break;
						
						case 3:		//��������
							SERVO_FWDRun(10);    					//�����������ŷ������2�μ��� 250r/min										
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Accelerate_2;		//case 8:�����������ŷ������2�μ���,�������������
				}
				else
				{
				}
				break;

			case Up_Accelerate_2:		//case 8:�������ŷ����2���ٽ׶�,�������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 400;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_REVRun(14);    					//�����������ŷ������3�μ��� 350r/min										
							break;
						
						case 3:		//��������
							SERVO_FWDRun(14);    					//�����������ŷ������3�μ��� 350r/min										
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Accelerate_3;		//case 9:�����������ŷ������3�μ���,�������������
				}
				else
				{
				}
				break;
				
			case Up_Accelerate_3:		//case 9:�����������ŷ������3�μ���,�������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 500;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(18);							//�������ŷ������4�μ��� 450r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(18);							//�������ŷ������4�μ��� 450r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(18);							//�������ŷ������4�μ��� 450r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(18);							//�������ŷ������4�μ��� 450r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Accelerate_4;		//case 10:�����������ŷ������4�μ���,�������������
				}
				else
				{
				}
				break;

			case Up_Accelerate_4:		//case 10:�����������ŷ������4�μ���,�������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 600;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(22);							//�������ŷ������5�μ��� 550r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(22);							//�������ŷ������5�μ��� 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(22);							//�������ŷ������5�μ��� 550r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(22);							//�������ŷ������5�μ��� 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Accelerate_5;		//case 11:�����������ŷ������5�μ���,�������������
				}
				else
				{
				}
				break;

			case Up_Accelerate_5:		//case 11:�����������ŷ������5�μ���,�������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 700;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(24);							//�������ŷ������6�μ��� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(26);							//�������ŷ������6�μ��� 650r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(24);							//�������ŷ������6�μ��� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(26);							//�������ŷ������6�μ��� 650r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Accelerate_6;		//case 12:�����������ŷ������6�μ���,�������������
				}
				else
				{
				}
				break;

			case Up_Accelerate_6:		//case 12:�����������ŷ������6�μ���,�������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 800;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(24);							//�������ŷ������7�μ��� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(30);							//�������ŷ������7�μ��� 750r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(24);							//�������ŷ������7�μ��� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(30);							//�������ŷ������7�μ��� 750r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Accelerate_7;		//case 13:�����������ŷ������7�μ���,�������������
				}
				else
				{
				}
				break;
				
			case Up_Accelerate_7:		//case 13:�����������ŷ������7�μ���,�������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 												//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = ((2400 * Distance / 108)  - 3000) - 8450;  //�����˶��������������У��趨���������� = 13 * ��������(mm) - 2050(��ȫ�����������), ������������Pr0.11=2400
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(24);							//�������ŷ�������� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(32);							//�������ŷ�������� 800r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(24);							//�������ŷ�������� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(32);							//�������ŷ�������� 800r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Uniform;							//case 20:�����������ŷ���������˶����������������
				}
				else
				{
				}
				break;
			
			case Up_Uniform:				//case 20:�����������ŷ���������˶����������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;				 			//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 700;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(24);							//�����������ŷ������1�μ��� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(30);							//�����������ŷ������1�μ��� 750r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(24);							//�����������ŷ������1�μ��� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(30);							//�����������ŷ������1�μ��� 750r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_1;		//case 21:�����������ŷ������1�μ��٣��������������
				}
				else
				{
				}
				break;
			
			case Up_Decelerate_1:			//case 21:�����������ŷ������1�μ��٣��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 650;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(24);							//�����������ŷ������2�μ��� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(26);							//�����������ŷ������2�μ��� 650r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(24);							//�����������ŷ������2�μ��� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(26);							//�����������ŷ������2�μ��� 650r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_2;		//case 22:�������ŷ����2���ٽ׶Σ��������������
				}
				else
				{
				}
				break;
			
			case Up_Decelerate_2:			//case 22:�������ŷ����2���ٽ׶Σ��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 600;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(22);							//�����������ŷ������3�μ��� 550r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(22);							//�����������ŷ������3�μ��� 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(22);							//�����������ŷ������3�μ��� 550r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(22);							//�����������ŷ������3�μ��� 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_3;		//case 23:�������ŷ����3���ٽ׶Σ��������������
				}
				else
				{
				}
				break;

			case Up_Decelerate_3:			//case 23:�������ŷ����3���ٽ׶Σ��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 550;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(18);							//�����������ŷ������4�μ��� 450r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(18);							//�����������ŷ������4�μ��� 450r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(18);							//�����������ŷ������4�μ��� 450r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(18);							//�����������ŷ������4�μ��� 450r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_4;		//case 24:������������4�μ��٣��������������
				}
				else
				{
				}
				break;

			case Up_Decelerate_4:			//case 24:�������ŷ����3���ٽ׶Σ��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 500;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_REVRun(14);    					//�������ŷ������5�μ��� 350r/min												
							break;
						
						case 3:		//��������
							SERVO_FWDRun(14);    					//�������ŷ������5�μ��� 350r/min										
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_5;		//case 25:������������5�μ��٣��������������
				}
				else
				{
				}
				break;

			case Up_Decelerate_5:			//case 25:�������ŷ����5���ٽ׶Σ��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 450;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_REVRun(10);    					//�������ŷ������6�μ��� 250r/min												
							break;
						
						case 3:		//��������
							SERVO_FWDRun(10);    					//�������ŷ������6�μ��� 250r/min										
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_6;		//case 26:������������6�μ��٣��������������
				}
				else
				{
				}
				break;

			case Up_Decelerate_6:			//case 26:�������ŷ����6���ٽ׶Σ��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 400;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_REVRun(8);    					//�������ŷ������7�μ��� 200r/min												
							break;
						
						case 3:		//��������
							SERVO_FWDRun(8);    					//�������ŷ������7�μ��� 200r/min										
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_7;		//case 27:������������7�μ��٣��������������
				}
				else
				{
				}
				break;

				
			case Up_Decelerate_7:			//case 27:�������ŷ����7���ٽ׶Σ��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 350;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_REVRun(6);    					//�������ŷ������8�μ��� 150r/min												
							break;
						
						case 3:		//��������
							SERVO_FWDRun(6);    					//�������ŷ������8�μ��� 150r/min										
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_8;		//case 28:������������8�μ��٣��������������
				}
				else
				{
				}
				break;

			case Up_Decelerate_8:			//case 28:�������ŷ����8���ٽ׶Σ��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 300;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_REVRun(5);    					//������������9�μ��� 125r/min												
							break;
						
						case 3:		//��������
							SERVO_FWDRun(5);    					//������������9�μ��� 125r/min									
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_9;		//case 29:������������9�μ��٣��������������
				}
				else
				{
				}
				break;

			case Up_Decelerate_9:			//case 29:�������ŷ����9���ٽ׶Σ��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 						//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 250;  											//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_REVRun(4);    					//������������10�μ��� 100r/min												
							break;
						
						case 3:		//��������
							SERVO_FWDRun(4);    					//������������10�μ��� 100r/min									
							break;
						
						default:
							break;
					}
					LiftRunState = Up_Decelerate_10;		//case 30:�������ŷ����10���ٽ׶Σ��������������
				}
				else
				{
				}
				break;
	
			case Up_Decelerate_10:			//case 30:�������ŷ����10���ٽ׶Σ��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 						//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 200;  											//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_REVRun(3);    					//������������11�μ��� 75r/min												
							break;
						
						case 3:		//��������
							SERVO_FWDRun(3);    					//������������11�μ��� 75r/min									
							break;
						
						default:
							break;
					}
					LiftRunState = Distance_Long_FeedbackPulse;		//case 40:�����������趨λ��ʱ�������������
				}
				else
				{
				}
				break;

//			case Up_Decelerate_11:			//case 29:�������ŷ����9���ٽ׶Σ��������������
//				if(PulseCntComplete == 1)
//				{
//					PulseCntComplete = 0;							 						//����������������־λ				
//					FeedbackNowNum = 0;						
//					FeedbackHopeNum = 150;  											//���÷���������
//					SERVO_REVRun(2);															//������������9�μ��� 100r/min
//					LiftRunState = Up_Decelerate_12;		//case 40:�����������趨λ��ʱ�������������
//				}
//				else
//				{
//				}
//				break;

//			case Up_Decelerate_12:			//case 29:�������ŷ����9���ٽ׶Σ��������������
//				if(PulseCntComplete == 1)
//				{
//					PulseCntComplete = 0;							 						//����������������־λ				
//					FeedbackNowNum = 0;						
//					FeedbackHopeNum = 100;  											//���÷���������
//					SERVO_REVRun(1);															//������������9�μ��� 100r/min
//					LiftRunState = Distance_Long_FeedbackPulse;		//case 40:�����������趨λ��ʱ�������������
//				}
//				else
//				{
//				}
//				break;
				
			case Distance_Long_FeedbackPulse:		//case 40:�����������趨λ��ʱ�������������
				if(PulseCntComplete == 1)
				{//�����������趨λ�ã��������������
					PulseCntComplete = 0;							 //����������������־λ				
					SERVO_STOP(); 										 //�����������趨���룬���ֹͣ
					DlyTime = TIMER7_CNT + 300;				 //��ʱ3s
					LiftRunState = Distance_Long_Dly;	 //case 41: �������ڵ�������λ��ֹͣʱ���ж�					
				}
				else
				{
				}
				break;

			case Distance_Long_Dly:		//case 41: �������ڵ�������λ��ֹͣʱ���ж�
				if(DlyTime == TIMER7_CNT)
				{//��ʱ3sʱ�䵽
					PulseCntComplete = 0;							  //����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 100;  						//���÷��������� 300
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_FWDRun(2);    					//�������½��ŷ������1�μ��� 50r/min									
							break;
						
						case 3:		//��������
							SERVO_REVRun(2);    					//�������½��ŷ������1�μ��� 50r/min												
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Accelerate_1;		//case 42:���������µ�1���ٽ׶Σ��жϷ���������
				}				
				else
				{
				}
				break;
				
			case Down_Accelerate_1:		//case 42:���������µ�1���ٽ׶Σ��жϷ���������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 	//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 200;  			 			//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_FWDRun(6);    					//�������½��ŷ������2�μ��� 150r/min									
							break;
						
						case 3:		//��������
							SERVO_REVRun(6);    					//�������½��ŷ������2�μ��� 150r/min												
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Accelerate_2;	 	//case 43:�������½��ŷ������2�μ���,�������������
				}
				else
				{
				}
				break;
			
			case Down_Accelerate_2:			//case 43:�������½��ŷ������2�μ���,�������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 300;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_FWDRun(10);    					//�������½��ŷ������3�μ��� 250r/min									
							break;
						
						case 3:		//��������
							SERVO_REVRun(10);    					//�������½��ŷ������3�μ��� 250r/min												
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Accelerate_3;	//case 44:�������½��ŷ������3�μ���,�������������
				}
				else
				{
				}
				break;

			case Down_Accelerate_3:			//case 44:�������½��ŷ������3�μ���,�������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 400;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_FWDRun(14);    					//�������½��ŷ������4�μ��� 350r/min									
							break;
						
						case 3:		//��������
							SERVO_REVRun(14);    					//�������½��ŷ������4�μ��� 350r/min												
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Accelerate_4;	//case 45:�������½��ŷ������4�μ���,�������������
				}
				else
				{
				}
				break;
				
			case Down_Accelerate_4:		//case 45:�������½��ŷ������4�μ���,�������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 500;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(18);							//�������½��ŷ������5�μ��� 450r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(18);							//�������½��ŷ������5�μ��� 450r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(18);							//�������½��ŷ������5�μ��� 450r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(18);							//�������½��ŷ������5�μ��� 450r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Accelerate_5;	//case 46:�������ŷ���������˶����������������
				}
				else
				{
				}
				break;

			case Down_Accelerate_5:		//case 46:�������½��ŷ������4�μ���,�������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 600;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(22);							//�������½��ŷ������6�μ��� 550r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(22);							//�������½��ŷ������6�μ��� 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(22);							//�������½��ŷ������6�μ��� 550r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(22);							//�������½��ŷ������6�μ��� 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Accelerate_6;	//case 47:�������½��ŷ������6�μ���,�������������
				}
				else
				{
				}
				break;

			case Down_Accelerate_6:		//case 47:�������½��ŷ������6�μ���,�������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 700;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(24);							//�������½��ŷ������7�μ��� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(26);							//�������½��ŷ������7�μ��� 650r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(24);							//�������½��ŷ������7�μ��� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(26);							//�������½��ŷ������7�μ��� 650r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Accelerate_7;	//case 48:�������½��ŷ������7�μ���,�������������
				}
				else
				{
				}
				break;

			case Down_Accelerate_7:		//case 48:�������½��ŷ������7�μ���,�������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 800;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(22);							//�������½��ŷ������6�μ��� 550r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(22);							//�������½��ŷ������6�μ��� 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(22);							//�������½��ŷ������6�μ��� 550r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(22);							//�������½��ŷ������6�μ��� 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					switch(SpeedGread)
					{
						case 1:		//�ٶ�1 400r/min
							SERVO_FWDRun(16);				 			//�������½��ŷ�������� 100r/min
							break;
						
						case 2:		//�ٶ�2 600r/min
							SERVO_FWDRun(24);				 			//�������½��ŷ������7�μ��� 600r/min
							break;
						
						case 3:		//�ٶ�3 800r/min
							SERVO_FWDRun(30);				 			//�������½��ŷ������7�μ��� 750r/min
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Accelerate_8;	//case 49:�������ŷ���������˶����������������
				}
				else
				{
				}
				break;

			case Down_Accelerate_8:		//case 49:�������½��ŷ������4�μ���,�������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 												//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = ((2400 * Distance / 108)  - 3000) - 8500;  //�����˶��������������У��趨���������� = 22 * ��������(mm) - 2050(��ȫ�����������), ������������Pr0.11=2400
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������½��ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(24);							//�������½��ŷ�������� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(32);							//�������½��ŷ�������� 800r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������½��ŷ�������� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(24);							//�������½��ŷ�������� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(32);							//�������½��ŷ�������� 800r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Uniform;			//case 60:�������ŷ���������˶����������������
				}
				else
				{
				}
				break;
				
			case Down_Uniform:				//case 60:�������ŷ���������˶����������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 700;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������½��ŷ������1�μ��� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(24);							//�������½��ŷ������1�μ��� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(30);							//�������½��ŷ������1�μ��� 750r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������½��ŷ������1�μ��� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(24);							//�������½��ŷ������1�μ��� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(30);							//�������½��ŷ������1�μ��� 750r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Decelerate_1;	//case 61:�������½��ŷ������1�μ��٣��������������
				}
				else
				{
				}
				break;
			
			case Down_Decelerate_1:		//case 61:�������½��ŷ������1�μ��٣��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 650;  					//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������½��ŷ������2�μ��� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(24);							//�������½��ŷ������2�μ��� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(26);							//�������½��ŷ������2�μ��� 650r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������½��ŷ������2�μ��� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(24);							//�������½��ŷ������2�μ��� 600r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(26);							//�������½��ŷ������2�μ��� 650r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Decelerate_2;	//case 62:�������½��ŷ������2�μ��٣��������������
				}
				else
				{
				}
				break;
			
			case Down_Decelerate_2:		//case 62:�������½��ŷ������2�μ��٣��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 								//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 600;  													//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������½��ŷ������3�μ��� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(22);							//�������½��ŷ������3�μ��� 550r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(22);							//�������½��ŷ������3�μ��� 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������½��ŷ������3�μ��� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(22);							//�������½��ŷ������3�μ��� 550r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(22);							//�������½��ŷ������3�μ��� 550r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Decelerate_3;	//case 63: �������½��ŷ������3�μ��٣��������������
				}
				else
				{
				}
				break;

			case Down_Decelerate_3:		//case 63:�������½��ŷ������3�μ��٣��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 								//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 550;  													//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_FWDRun(16);							//�������½��ŷ������4�μ��� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_FWDRun(18);							//�������½��ŷ������4�μ��� 450r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_FWDRun(18);							//�������½��ŷ������4�μ��� 450r/min
									break;
								
								default:
									break;
							}
							break;
						
						case 3:		//��������
							switch(SpeedGread)
							{
								case 1:		//�ٶ�1 400r/min
									SERVO_REVRun(16);							//�������½��ŷ������4�μ��� 400r/min
									break;
								
								case 2:		//�ٶ�2 600r/min
									SERVO_REVRun(18);							//�������½��ŷ������4�μ��� 450r/min
									break;
								
								case 3:		//�ٶ�3 800r/min
									SERVO_REVRun(18);							//�������½��ŷ������4�μ��� 450r/min
									break;
								
								default:
									break;
							}
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Decelerate_4;	//case 64:�������½��ŷ������4�μ��٣��������������
				}
				else
				{
				}
				break;

			case Down_Decelerate_4:		//case 64:�������½��ŷ������4�μ��٣��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 								//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 500;  													//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_FWDRun(14);				 			//�������½��ŷ������5�μ��� 350r/min
							break;
						
						case 3:		//��������
							SERVO_REVRun(14);    					//�������½��ŷ������5�μ��� 350r/min									
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Decelerate_5;	//case 65:�������½��ŷ������5�μ��٣��������������
				}
				else
				{
				}
				break;

			case Down_Decelerate_5:		//case 65:�������½��ŷ������5�μ��٣��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 								//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 450;  													//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_FWDRun(10);				 			//�������½��ŷ������6�μ��� 250r/min
							break;
						
						case 3:		//��������
							SERVO_REVRun(10);    					//�������½��ŷ������6�μ��� 250r/min									
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Decelerate_6;	//case 66:�������½��ŷ������6�μ��٣��������������
				}
				else
				{
				}
				break;

			case Down_Decelerate_6:		//case 66:�������½��ŷ������6�μ��٣��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 								//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 400;  													//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_FWDRun(6);				 			//�������½��ŷ������7�μ��� 150r/min
							break;
						
						case 3:		//��������
							SERVO_REVRun(6);    					//�������½��ŷ������7�μ��� 150r/min									
							break;
						
						default:
							break;
					}
					LiftRunState = Down_Decelerate_7;	//case 67:�������½��ŷ������7�μ��٣��������������
				}
				else
				{
				}
				break;

			case Down_Decelerate_7:		//case 67:�������½��ŷ������7�μ��٣��������������
				if(PulseCntComplete == 1)
				{
					PulseCntComplete = 0;							 								//����������������־λ				
					FeedbackNowNum = 0;						
					FeedbackHopeNum = 350;  													//���÷���������
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_FWDRun(3);				 			//�������½��ŷ������8�μ��� 75r/min
							break;
						
						case 3:		//��������
							SERVO_REVRun(3);    					//�������½��ŷ������8�μ��� 75r/min									
							break;
						
						default:
							break;
					}
					LiftRunState = Distance_3000_Down_FeedbackPulse;	//case 70: �������˶��������������˶����պ������λ�ã��жϷ���������
				}
				else
				{
				}
				break;
				
				case Distance_3000_Down_FeedbackPulse:		//case 70: �������˶��������������˶����պ������λ�ã��жϷ���������
				if(PulseCntComplete == 1)
				{//�������ص��պ������λ�ã��������������
					PulseCntComplete = 0;							 //����������������־λ				
					switch(Slave_ID)
					{
						case 2:  	//��������
							SERVO_FWDRun(3);				 			//�������½��ŷ������8�μ��� 75r/min
							break;
						
						case 3:		//��������
							SERVO_REVRun(3);    					//�������½��ŷ������8�μ��� 75r/min									
							break;
						
						default:
							break;
					}
					LiftRunState = Lift_RST_Check;		 //case 1:��������λ���������									
				}
				else
				{
				}
				break;

			case Gate_Close_Check:	//case 71:բ�ſ��ƣ�բ�Źرռ��
				if(GS_CLOSE_Level == 1)
				{//��⵽���Ŵ�����
					Gate_Stop();								//����⵽���Ŵ�����ʱ�����ֹͣ����
					GateState = 0;							//բ��Ϊ�ر�״̬
					GateStatus = 0;							//բ�Źر�
					LiftRunState	= 0;					//����������״̬�����ж�
					RunFlag = 0;								//����ӻ���Ӧ�����ʶ
					Reason = 0;									//�ӻ�����
				}
				else
				{
				}
				break;
			
			case Gate_Open_Check:		//case 72: բ�ſ��ƣ�բ�Ŵ򿪼��
				if(TimeoutDly == TIMER7_CNT)
				{
					Gate_Stop();								//���ֹͣ����
					GateState = 1;							//բ�Ŵ�״̬
					GateStatus = 2;							//բ�Ŵ��쳣����
					LiftRunState	= 0;					//����������״̬�����ж�
					RunFlag = 0;								//����ӻ���Ӧ�����ʶ
					Reason = 4;									//�ӻ����ϱ���						
				}
				else if(GS_OPEN_Level)
				{//��⵽���Ŵ�����
					Gate_Stop();								//���ֹͣ����
					GateState = 1;							//բ��Ϊ��״̬				
					GateStatus = GateState;			//բ��Ϊ��״̬
					LiftRunState	= 0;					//����������״̬�����ж�
					RunFlag = 0;								//����ӻ���Ӧ�����ʶ
					Reason = 0;									//�ӻ�����						
				}
				break;
				
			case Dou_Close_Check:		//case 73: ҩ�����ƣ�ҩ���رռ��
				if(DS_CLOSE_Level == 1)				//��δ��⵽ҩ�����Ŵ�����ʱ�������������
				{
					Dou_Stop();									//����⵽ҩ�����Ŵ�����ʱ�����ֹͣ����
					DouState = 0;								//ҩ��Ϊ�ر�״̬
					DouStatus = 0;							//��ҩ���
					LiftRunState	= 0;					//����������״̬�����ж�
					RunFlag = 0;								//����ӻ���Ӧ�����ʶ
					Reason = 0;									//�ӻ�����
				}
				else
				{
				}
				break;
			
			case Dou_Open_Check:		//case 74: ҩ�����ƣ�ҩ���򿪼��
				if(DS_OPEN_Level == 1)				//��δ��⵽ҩ�����Ŵ�����ʱ�������������
				{
					Dou_Stop();									//����⵽ҩ�����Ŵ�����ʱ�����ֹͣ����
					DouState = 1;								//ҩ��Ϊ��״̬
					DouStatus = 1;							//��ҩδ���
					LiftRunState	= 0;					//����������״̬�����ж�
					RunFlag = 0;								//����ӻ���Ӧ�����ʶ
					Reason = 0;									//�ӻ�����
				}
				else
				{
				}
				break;
				
		case LinkageBefor_Dou_Close_Check:			//case: 75 //���Ͽ��ƣ������Ͽ���֮ǰ���ȼ��ҩ���Ƿ�ر�
			if(TimeoutDly == TIMER7_CNT)
			{//���ʱ�䳬ʱ
				Dou_Stop();							//���ֹͣ����
				DouState = 0;						//���Źر�״̬
				DouStatus = 2;					//���Źر��쳣����
				LiftRunState = 0;				//case 0: ��ʼ״̬
				RunFlag	= 0;						//����������б�־λ
				Reason = 4;							//�ӻ�����
			}
			else if(DS_CLOSE_Level)
			{//��⵽���Źش�����
				Dou_Stop();										//���ֹͣ����
				DouState = 0;									//����Ϊ�ر�״̬				
				DouStatus = DouState;							//����״̬
				if(GateState == 1)
				{
					Gate_Closing();						//�ر�բ��
					TimeoutDly = TIMER7_CNT + 500;		//�賬ʱʱ��Ϊ5s
					LiftRunState	= Linkage_Gate_Close_Check;		//case 76: ����������״̬�����ж�					
				}
			}
			
			break;
				
		case Linkage_Gate_Close_Check:			//case 76:����������״̬�����ж�
			if(TimeoutDly == TIMER7_CNT)
			{//��ʱ5s
				Gate_Stop();						//���ֹͣ����
				GateState = 0;					//բ�Źر�״̬
				GateStatus = 2;					//բ�Źر��쳣����
				LiftRunState = 0;				//case 0: ��ʼ״̬
				RunFlag	= 0;						//����������б�־λ
				Reason = 4;							//�ӻ�����
			}
			if(Oppsite_Level == 1)
			{//��⵽���䴫�����źţ�������ҩ״̬
				Gate_Stop();
				GateState = 1;
				GateStatus = 3;					//������ҩ�����բ�Ŵ��ڴ�״̬
				LiftRunState	= Linkage_OppsiteCLR_Check;		//case 77: ������բ�ŷ�����ҩ������
			}
			else if(GS_CLOSE_Level == 1)
			{//��⵽���Ŵ�����
				Gate_Stop();															//����⵽���Ŵ�����ʱ�����ֹͣ����
				GateState = 0;														//բ��Ϊ�ر�״̬
				GateStatus = GateState;											//բ��״̬����
				LiftRunState	= Linkage_Distance_Check;		//case 79: ���������������ж�
			}
			break;
			
		case Linkage_OppsiteCLR_Check:				//case 77: ������բ�ŷ�����ҩ������
			if(Oppsite_Level == 0)
			{//����ҩ���
				delay_ms(500);		
				Gate_Closing();					//�ر�բ��
				LiftRunState	= Linkage_OppsiteCLR_After;		//case 78: ��ҩ�������ż��
			}
			break;

		case	Linkage_OppsiteCLR_After:				//case 78: ��ҩ�������ż��			
			if(GS_CLOSE_Level == 1)
			{//��⵽���Ŵ�����
				Gate_Stop();															//����⵽���Ŵ�����ʱ�����ֹͣ����
				GateState = 0;														//բ��Ϊ�ر�״̬
				GateStatus = GateState;											//բ��״̬����
				LiftRunState	= Linkage_Distance_Check;		//case 79: ���������������ж�
			}
			break;
			
		case Linkage_Distance_Check:		//case 79: ���������������ж�
			if((Distance >= 700) && (Distance <= 2095))									
			{//����������ȫ����
				PulseCntComplete = 0;							 		//����������������־λ
				FeedbackNowNum = 0;					
				FeedbackHopeNum = 3000;
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(3);    							//������������������ 75r/min										
						break;
					
					case 3:		//��������
						SERVO_FWDRun(3);    							//������������������ 75r/min											
						break;
					
					default:
						break;
				}
				MachaStatus = 1;											//����������������ԭ��
				LiftRunState = Linkage_3000_Up_Check;	//case 78: ���Ͽ��ƣ���������������ȫ�����λ�ã���ⷴ��������					
			}
			else
			{
				LiftRunState	= 0;					//����������״̬����
				RunFlag	= 0;								//����������б�־λ
				Reason = 0;									//�ӻ�����
			}
			break;


			case Linkage_3000_Up_Check:		//case 78: ���Ͽ��ƣ���������������ȫ�����λ�ã���ⷴ��������
			if(PulseCntComplete == 1)
			{//�������պ���ȫ����ʱ����������������ɣ��ٸ����˶����趨���� ���������� = 3000 ����������=155mm
				PulseCntComplete = 0;							 			//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 200;  								//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(6);    							//�������ŷ������1�μ��� 150r/min										
						break;
					
					case 3:		//��������
						SERVO_FWDRun(6);    							//�������ŷ������1�μ��� 150r/min											
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Accelerate_1;	//case 79:�����������ŷ������1�μ���,�������������
			}
			else
			{//�������պ�����ķ���������3000δ����
				
			}
			break;
			
		case Linkage_Up_Accelerate_1:		//case 79:�����������ŷ������1�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 300;  								//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(10);    							//�������ŷ������2�μ��� 250r/min										
						break;
					
					case 3:		//��������
						SERVO_FWDRun(10);    							//�������ŷ������2�μ��� 250r/min											
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Accelerate_2;	//case 80:�����������ŷ������2�μ���,�������������
			}
			else
			{
			}
			break;
		
		case Linkage_Up_Accelerate_2:		//case 80:�����������ŷ������2�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 400;  								//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(14);    							//�������ŷ������3�μ��� 350r/min										
						break;
					
					case 3:		//��������
						SERVO_FWDRun(14);    							//�������ŷ������3�μ��� 350r/min											
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Accelerate_3;	//case 81:�������ŷ����3���ٽ׶�,�������������
			}
			else
			{
			}
			break;

		case Linkage_Up_Accelerate_3:		//case 81:�������ŷ����3���ٽ׶�,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 500;  								//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							//�������ŷ�������� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(18);							//�������ŷ������4�μ��� 450r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(18);							//�������ŷ������4�μ��� 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							//�������ŷ�������� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(18);							//�������ŷ������4�μ��� 450r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(18);							//�������ŷ������4�μ��� 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Accelerate_4;	//case 82:�������ŷ����4���ٽ׶�,�������������
			}
			else
			{
			}
			break;

		case Linkage_Up_Accelerate_4:		//case 82:�����������ŷ������4�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 600;  								//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							//�������ŷ������5�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(22);							//�������ŷ������5�μ��� 550r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(22);							//�������ŷ������5�μ��� 550r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							//�������ŷ������5�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(22);							//�������ŷ������5�μ��� 550r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(22);							//�������ŷ������5�μ��� 550r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Accelerate_5;	//case 83:�������ŷ����5���ٽ׶�,�������������
			}
			else
			{
			}
			break;

		case Linkage_Up_Accelerate_5:		//case 83:�����������ŷ������5�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 700;  								//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							//�������ŷ������6�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(24);							//�������ŷ������6�μ��� 600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(26);							//�������ŷ������6�μ��� 650r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							//�������ŷ������6�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(24);							//�������ŷ������6�μ��� 600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(26);							//�������ŷ������6�μ��� 650r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Accelerate_6;	//case 84:�������ŷ����6���ٽ׶�,�������������
			}
			else
			{
			}
			break;

		case Linkage_Up_Accelerate_6:		//case 84:�����������ŷ������6�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 1000;  								//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							//�������ŷ������7�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(24);							//�������ŷ������7�μ��� 600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(38);							//�������ŷ������7�μ��� 950r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							//�������ŷ������7�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(24);							//�������ŷ������7�μ��� 600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(38);							//�������ŷ������7�μ��� 750r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Accelerate_7;	//case 85:�������ŷ����7���ٽ׶�,�������������
			}
			else
			{
			}
			break;
			
		case Linkage_Up_Accelerate_7:		//case 85:�������ŷ����7���ٽ׶�,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 	//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = ((2400 * Distance / 108 )  - 3000) - 9100;  //�����˶��������������У��趨���������� = 13 * ��������(mm) - 2050(��ȫ�����������), ������������Pr0.11=2400
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							//�������ŷ�������� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(24);							//�������ŷ�������� 600r/min
								break;
							
							case 3:		//�ٶ�3 1000r/min
								SERVO_REVRun(40);							//�������ŷ�������� 1000r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							//�������ŷ�������� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(24);							//�������ŷ�������� 600r/min
								break;
							
							case 3:		//�ٶ�3 1000r/min
								SERVO_FWDRun(40);							//�������ŷ�������� 1000r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Uniform;		//case 90:�����������ŷ���������˶����������������
			}
			else
			{
			}
			break;
		
		case Linkage_Up_Uniform:				//case 90:�����������ŷ���������˶����������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 		//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 800;  						 	//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							 //�������ŷ������1�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(24);							 //�������ŷ������1�μ��� 600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(38);							 //�������ŷ������1�μ��� 750r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							 //�������ŷ������1�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(24);							 //�������ŷ������1�μ��� 600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(38);							 //�������ŷ������1�μ��� 750r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Decelerate_1;	 //case 91:�����������ŷ������1�μ��٣��������������
			}
			else
			{
			}
			break;
		
		case Linkage_Up_Decelerate_1:		//case 91:�����������ŷ������1�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 	  //����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 750;  						 	//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							 //�������ŷ������2�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(24);							 //�������ŷ������2�μ��� 600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(34);							 //�������ŷ������2�μ��� 650r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							 //�������ŷ������2�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(24);							 //�������ŷ������2�μ��� 600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(34);							 //�������ŷ������2�μ��� 650r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Decelerate_2;	 //case 92:�����������ŷ������2�μ��٣��������������
			}
			else
			{
			}
			break;
		
		case Linkage_Up_Decelerate_2:		 //case 92:�������ŷ������2�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;											//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 700; 										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							 //�������ŷ������3�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(22);							 //�������ŷ������3�μ��� 550r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(30);							 //�������ŷ������3�μ��� 550r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							 //�������ŷ������3�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(22);							 //�������ŷ������3�μ��� 550r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(30);							 //�������ŷ������3�μ��� 550r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Decelerate_3;		//case 93:�����������ŷ������3�μ��٣��������������
			}
			else
			{
			}
			break;

		case Linkage_Up_Decelerate_3:		//case 93:�����������ŷ������3�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 650;  									//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							 //�������ŷ������4�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(18);							 //�������ŷ������4�μ��� 450r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(26);							 //�������ŷ������4�μ��� 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							 //�������ŷ������4�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(18);							 //�������ŷ������4�μ��� 450r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(26);							 //�������ŷ������4�μ��� 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Decelerate_4;		//case 94:������������4�μ��٣��������������
			}
			else
			{
			}
			break;

		case Linkage_Up_Decelerate_4:		//case 94:�����������ŷ������3�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 600;  									//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							 //�������ŷ������4�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(18);							 //�������ŷ������4�μ��� 450r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(22);							 //�������ŷ������4�μ��� 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							 //�������ŷ������4�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(18);							 //�������ŷ������4�μ��� 450r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(22);							 //�������ŷ������4�μ��� 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Decelerate_5;		//case 95:������������5�μ��٣��������������
			}
			else
			{
			}
			break;

		case Linkage_Up_Decelerate_5:		//case 95:�����������ŷ������5�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 550;  									//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							 //�������ŷ������4�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(18);							 //�������ŷ������4�μ��� 450r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(18);							 //�������ŷ������4�μ��� 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							 //�������ŷ������4�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(18);							 //�������ŷ������4�μ��� 450r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(18);							 //�������ŷ������4�μ��� 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Decelerate_6;		//case 96:������������6�μ��٣��������������
			}
			else
			{
			}
			break;

		case Linkage_Up_Decelerate_6:		//case 96:�����������ŷ������6�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 500;  									//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(14);    					//�������ŷ������7�μ��� 200r/min												
						break;
					
					case 3:		//��������
						SERVO_FWDRun(14);    					//�������ŷ������7�μ��� 200r/min										
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Decelerate_7;		//case 97:������������7�μ��٣��������������
			}
			else
			{
			}
			break;

		case Linkage_Up_Decelerate_7:		//case 97:�����������ŷ������7�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 450;  								//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(10);    					//�������ŷ������8�μ��� 150r/min												
						break;
					
					case 3:		//��������
						SERVO_FWDRun(10);    					//�������ŷ������8�μ��� 150r/min										
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Up_Decelerate_8;	//case 98:������������8�μ��٣��������������
			}
			else
			{
			}
			break;

		case Linkage_Up_Decelerate_8:		//case 98:�����������ŷ������8�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 400;  								//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(6);    					//�������ŷ������9�μ��� 125r/min												
						break;
					
					case 3:		//��������
						SERVO_FWDRun(6);    					//�������ŷ������9�μ��� 125r/min										
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_FeedbackPulse;	//case 99:������������9�μ��٣��������������
			}
			else
			{
			}
			break;

		case Linkage_FeedbackPulse:		//case 110: ���Ͽ��ƣ� �������������е��趨λ�ã��������������
			if(PulseCntComplete == 1)
			{//���Ͽ��ƣ������������趨λ�ã��������������
				PulseCntComplete = 0;							 //����������������־λ				
				SERVO_STOP(); 										 //�����������趨�߶ȣ����ֹͣ			
				DlyTime = TIMER7_CNT + 100;				 //�����������趨�߶ȣ�ͣ1s����ҩ����ҩ
				LiftRunState = Lift_Stop_Dly;			 //case 111: �������ƣ������������趨�߶ȣ���ʱʱ����		
			}			
			else
			{
			}
			break;
		
		case Lift_Stop_Dly:		//case 111: �������ƣ������������趨�߶ȣ���ʱʱ����	
			if(DlyTime == TIMER7_CNT)
			{//���Ͽ��ƣ�������ֹͣʱ���ѵ�����ҩ��
				Dou_Opening();													//��ҩ����ҩ
				TimeoutDly = TIMER7_CNT + 500;	//������ʱʱ��5s				
				LiftRunState = Linkage_Dou_Open_Check;	//case 112: �������ƣ���ҩ���������źż��	
			}
			else
			{
			}
			break;
			
		case Linkage_Dou_Open_Check:	//case 112: �������ƣ���ҩ���������źż��	
			if(TimeoutDly == TIMER7_CNT)
			{//��������ⳬʱ
				Dou_Stop();										//����⵽ҩ�����Ŵ�����ʱ�����ֹͣ����
				DouState = 1;									//ҩ����״̬
				DouStatus = 2;								//ҩ�����쳣����
				LiftRunState	= 0;					//����������״̬�����ж�
				RunFlag = 0;								//����ӻ���Ӧ�����ʶ
				Reason = 4;									//�ӻ����ϱ���						
			}
			else if(DS_OPEN_Level == 1)					//��δ��⵽ҩ�����Ŵ�����ʱ
			{
				Dou_Stop();										//����⵽ҩ�����Ŵ�����ʱ�����ֹͣ����
				DouState = 1;									//ҩ��Ϊ��״̬
				DouStatus = DouState;					//��ҩ��
				DlyTime = TIMER7_CNT + 200;		//��ҩ����ҩ�趨ʱ��2s
				LiftRunState = Dou_Open_Dly;	//case 113: �������ƣ�ҩ����ҩ��ʱʱ����						
			}
			else
			{
			}
			break;

		case Dou_Open_Dly:	 //case 113: �������ƣ�ҩ����ҩ��ʱʱ����	
			if(DlyTime == TIMER7_CNT)
			{//ҩ����ʱ�䵽
				Dou_Closing();														//�ر�ҩ��
				TimeoutDly = TIMER7_CNT + 500;	//������ʱʱ��5s				
				LiftRunState = Linkage_Dou_Close_Check;		//case 114: �������ƣ��ر�ҩ���������źż��	
			}
			else
			{
			}
			break;
			
		case Linkage_Dou_Close_Check:		//case 114:���Ͽ��ƣ��������ƣ��ر�ҩ���������źż��
			if(TimeoutDly == TIMER7_CNT)
			{
				Dou_Stop();																	//����⵽ҩ���رմ�����ʱ�����ֹͣ����
				DouState = 0;																//ҩ��Ϊ�ر�״̬
				DouStatus = 2;															//ҩ��״̬������0����ҩ����ɵ�ҩ���� 1����δ��ɵ�ҩ���� 2����ҩ���򿪻�ر��쳣����
				PulseCntComplete = 0;							 					//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 100;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
//						SERVO_FWDRun(2);    					//�������½��ŷ������1�μ��� 50r/min									
						SERVO_FWDRun(6);    					//�������½��ŷ������1�μ��� 50r/min									
						break;
					
					case 3:		//��������
//						SERVO_FWDRun(2);    					//�������½��ŷ������1�μ��� 50r/min									
						SERVO_REVRun(6);    					//�������½��ŷ������1�μ��� 50r/min												
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_1;		//case 115:���������µ�1���ٽ׶Σ��жϷ���������
			}
			if(DS_CLOSE_Level == 1)				//��δ��⵽ҩ�����Ŵ�����ʱ
			{
				Dou_Stop();																	//����⵽ҩ���رմ�����ʱ�����ֹͣ����
				DouState = 0;																//ҩ��Ϊ�ر�״̬
				DouStatus = DouState;															//ҩ��״̬������0����ҩ����ɵ�ҩ���� 1����δ��ɵ�ҩ���� 2����ҩ���򿪻�ر��쳣����
				PulseCntComplete = 0;							 					//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 100;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
//						SERVO_FWDRun(2);    					//�������½��ŷ������1�μ��� 50r/min									
						SERVO_FWDRun(6);    					//�������½��ŷ������1�μ��� 50r/min									
						break;
					
					case 3:		//��������
//						SERVO_REVRun(2);    					//�������½��ŷ������1�μ��� 50r/min												
						SERVO_REVRun(6);    					//�������½��ŷ������1�μ��� 50r/min												
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_1;		//case 115:���������µ�1���ٽ׶Σ��жϷ���������
			}				
			else
			{
			}
			break;
			
		case Linkage_Down_Accelerate_1:		//case 115:���������µ�1���ٽ׶Σ��жϷ���������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 200;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(6);    					//�������½��ŷ������2�μ��� 150r/min									
						break;
					
					case 3:		//��������
						SERVO_REVRun(6);    					//�������½��ŷ������2�μ��� 150r/min												
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_2;		//case 116:�������½��ŷ������2�μ���,�������������
			}
			else
			{
			}
			break;
		
		case Linkage_Down_Accelerate_2:			//case 116:�������½��ŷ������2�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 300;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(10);    					//�������½��ŷ������3�μ��� 250r/min									
						break;
					
					case 3:		//��������
						SERVO_REVRun(10);    					//�������½��ŷ������3�μ��� 250r/min												
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_3;		//case 117:�������½��ŷ������3�μ���,�������������
			}
			else
			{
			}
			break;

		case Linkage_Down_Accelerate_3:		//case 117:�������½��ŷ������3�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;				 								//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 400;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(14);    					//�������½��ŷ������4�μ��� 350r/min									
						break;
					
					case 3:		//��������
						SERVO_REVRun(14);    					//�������½��ŷ������4�μ��� 350r/min												
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_4;		//case 118:�������½��ŷ������4�μ���,�������������
			}
			else
			{
			}
			break;
			
		case Linkage_Down_Accelerate_4:		//case 118:�������½��ŷ������4�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;												//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 500;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							//�������½��ŷ������5�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(18);							//�������½��ŷ������5�μ��� 450r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(18);							//�������½��ŷ������5�μ��� 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							//�������½��ŷ������5�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(18);							//�������½��ŷ������5�μ��� 450r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(18);							//�������½��ŷ������5�μ��� 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_5;		//case 119:�������½��ŷ������5�μ���,�������������
			}
			else
			{
			}
			break;

		case Linkage_Down_Accelerate_5:		//case 119:�������½��ŷ������5�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;												//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 600;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							//�������½��ŷ������5�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(22);							//�������½��ŷ������5�μ��� 550r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(22);							//�������½��ŷ������5�μ��� 550r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							//�������½��ŷ������6�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(22);							//�������½��ŷ������6�μ��� 550r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(22);							//�������½��ŷ������6�μ��� 550r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_6;		//case 120:�������½��ŷ������6�μ���,�������������
			}
			else
			{
			}
			break;

		case Linkage_Down_Accelerate_6:		//case 120:�������½��ŷ������4�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;												//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 700;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							//�������½��ŷ������7�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(24);							//�������½��ŷ������7�μ��� 600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(26);							//�������½��ŷ������7�μ��� 650r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							//�������½��ŷ������7�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(24);							//�������½��ŷ������7�μ��� 600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(26);							//�������½��ŷ������7�μ��� 650r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_7;		//case 121:�������½��ŷ������7�μ���,�������������
			}
			else
			{
			}
			break;

		case Linkage_Down_Accelerate_7:		//case 121:�������½��ŷ������7�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;												//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 800;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							//�������½��ŷ������8�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(24);							//�������½��ŷ������8�μ��� 600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(30);							//�������½��ŷ������8�μ��� 750r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							//�������½��ŷ������8�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(24);							//�������½��ŷ������8�μ��� 600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(30);							//�������½��ŷ������8�μ��� 750r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Accelerate_8;		//case 122:�������½��ŷ������8�μ���,�������������
			}
			else
			{
			}
			break;

		case Linkage_Down_Accelerate_8:		//case 122:�������½��ŷ������8�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;												//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = ((2400 * Distance / 108)  - 3000) - 8500;  //�����˶��������������У��趨���������� = 13 * ��������(mm) - 2050(��ȫ�����������), ������������Pr0.11=2400
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							//�������½��ŷ��������400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(24);							//�������½��ŷ��������600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(32);							//�������½��ŷ��������800r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							//�������½��ŷ��������400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(24);							//�������½��ŷ��������600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(32);							//�������½��ŷ��������800r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Uniform;				//case 130:�������ŷ���������˶����������������
			}
			else
			{
			}
			break;
			
		case Linkage_Down_Uniform:				//case 130:�������ŷ���������˶����������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 700;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							//�������½��ŷ������1�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(24);							//�������½��ŷ������1�μ��� 600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(30);							//�������½��ŷ������1�μ��� 750r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							//�������½��ŷ������1�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(24);							//�������½��ŷ������1�μ��� 600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(30);							//�������½��ŷ������1�μ��� 750r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Decelerate_1;	//case 131:�������½��ŷ������1�μ��٣��������������
			}
			else
			{
			}
			break;
		
		case Linkage_Down_Decelerate_1:		//case 131:�������½��ŷ������1�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 650;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							//�������½��ŷ������2�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(24);							//�������½��ŷ������2�μ��� 600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(26);							//�������½��ŷ������2�μ��� 650r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							//�������½��ŷ������2�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(24);							//�������½��ŷ������2�μ��� 600r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(26);							//�������½��ŷ������2�μ��� 650r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Decelerate_2;		//case 132:�������½��ŷ������2�μ��٣��������������
			}
			else
			{
			}
			break;
		
		case Linkage_Down_Decelerate_2:		//case 132:�������½��ŷ������2�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 600;  									//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							//�������½��ŷ������3�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(22);							//�������½��ŷ������3�μ��� 550r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(22);							//�������½��ŷ������3�μ��� 550r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							//�������½��ŷ������3�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(22);							//�������½��ŷ������3�μ��� 550r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(22);							//�������½��ŷ������3�μ��� 550r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Decelerate_3;		//case 133:�������½��ŷ������3�μ��٣��������������
			}
			else
			{
			}
			break;

		case Linkage_Down_Decelerate_3:		//case 133:�������½��ŷ������3�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 550;  									//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_FWDRun(16);							//�������½��ŷ������4�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_FWDRun(18);							//�������½��ŷ������4�μ��� 450r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_FWDRun(18);							//�������½��ŷ������4�μ��� 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					case 3:		//��������
						switch(SpeedGread)
						{
							case 1:		//�ٶ�1 400r/min
								SERVO_REVRun(16);							//�������½��ŷ������4�μ��� 400r/min
								break;
							
							case 2:		//�ٶ�2 600r/min
								SERVO_REVRun(18);							//�������½��ŷ������4�μ��� 450r/min
								break;
							
							case 3:		//�ٶ�3 800r/min
								SERVO_REVRun(18);							//�������½��ŷ������4�μ��� 450r/min
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Decelerate_4;		//case 134:�������½��ŷ������4�μ��٣��������������
			}
			else
			{
			}
			break;

		case Linkage_Down_Decelerate_4:		//case 134:�������½��ŷ������4�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 500;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(14);				 			//�������½��ŷ������5�μ��� 350r/min
						break;
					
					case 3:		//��������
						SERVO_REVRun(14);    					//�������½��ŷ������5�μ��� 350r/min									
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Decelerate_5;		//case 135:�������½��ŷ������5�μ��٣��������������
			}
			else
			{
			}
			break;

		case Linkage_Down_Decelerate_5:		//case 135:�������½��ŷ������5�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 450;  									//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(10);				 			//�������½��ŷ������6�μ��� 250r/min
						break;
					
					case 3:		//��������
						SERVO_REVRun(10);    					//�������½��ŷ������6�μ��� 250r/min									
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Decelerate_6;		//case 136:�������½��ŷ������6�μ��٣��������������
			}
			else
			{
			}
			break;

		case Linkage_Down_Decelerate_6:		//case 136:�������½��ŷ������6�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 400;  									//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(6);				 			//�������½��ŷ������7�μ��� 150r/min
						break;
					
					case 3:		//��������
						SERVO_REVRun(6);    					//�������½��ŷ������7�μ��� 150r/min									
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_Down_Decelerate_7;		//case 137:�������½��ŷ������7�μ��٣��������������
			}
			else
			{
			}
			break;

		case Linkage_Down_Decelerate_7:		//case 137:�������½��ŷ������7�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 350;  									//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(3);				 			//�������½��ŷ������8�μ��� 75r/min
						break;
					
					case 3:		//��������
						SERVO_REVRun(3);    					//�������½��ŷ������8�μ��� 75r/min									
						break;
					
					default:
						break;
				}
				LiftRunState = Linkage_3000_Down_Check;		//case 140: ���Ͽ��ƣ������������˶����պ������λ�ã��жϷ���������
			}
			else
			{
			}
			break;
			
		case Linkage_3000_Down_Check:		//case 140:���Ͽ��ƣ��������������е���ȫ�����λ�ã���ⷴ��������
			if(PulseCntComplete == 1)
			{//�������պ���ȫ����ʱ����������������ɣ��ٵ����˶����趨����
				PulseCntComplete = 0;							//����������������־λ				
				FeedbackNowNum = 0;						
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(3);				 			//�������½��ŷ���������˶�75r/min
						break;
					
					case 3:		//��������
						SERVO_REVRun(3);    					//�������½��ŷ���������˶�75r/min									
						break;
					
					default:
						break;
				}
				LiftRunState = Lift_RST_Check;		//case 1: ���Ͽ��ƣ���������λ�������źż��
			}
			else
			{
			}
			break;
						
		default:
			AllSystem_Init();     //ʹ���л�����ʼ��
			LiftRunState = 0;				
			break;
		}
}		

void AD_Check(void)
{
	if(Slave_ID == 1)
	{//�ӻ���ַ = 1(���ʹ�)
		TransmitBeltControl();		//����Ƥ�����Ƴ���
	}
	else if((Slave_ID == 2)||(Slave_ID == 3))
	{//�ӻ���ַ = 2(��������) || �ӻ���ַ = 3(��������)
		HoisterControl();				//���������Ƴ���
	}
	else
	{
	}
}


