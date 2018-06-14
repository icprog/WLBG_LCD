#include "timer.h"
#include "stm32f10x_tim.h"
#include "declare.h"

//ͨ�ö�ʱ��7�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM7_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM7��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM7�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM7, ENABLE);  //ʹ��TIMx					 
}

//��ʱ��7�жϷ������
void TIM7_IRQHandler(void)   //TIM7�ж�
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //���TIM7�����жϷ������
	{
		TIMER7_CNT++;						//10msһ������
		if(WORK_LED_CNT == TIMER7_CNT)
		{
			WORK_LED = !WORK_LED;
			WORK_LED_CNT = TIMER7_CNT + 50;
		}

		if(Test.LEDEnable == ENABLE)   //�����ʼ������Գ�������˸����ָʾ��
		{
			if(Test.LEDTime == TIMER7_CNT)
			{
				TEST_LED 		 = !TEST_LED;
				Test.LEDTime = TIMER7_CNT + 50;		//500ms test_led��˸��
			}
		}
		
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
	}
}

//ʹ��TIM2��ʱ������PWM��ʼ��
//f:PWM�����Ƶ�ʣ�duty:ռ�ձ�
void TIM2PWM_Set(u32 f, u8 duty)
{	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;			
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);			//�򿪶�ʱ��ʱ��
	
	//��ʱ��2����
	TIM_TimeBaseStructure.TIM_Period = 99;									//20KHz,16λ��ֵ�����65536
	TIM_TimeBaseStructure.TIM_Prescaler = (72000000 / (100 * f)) - 1;		//16λ��ֵ�����65536
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;					//ʱ�ӷ�Ƶϵ��������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ������ģʽ
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);					//��ʼ����ʱ��
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//����ʱ��TIM2ΪPWM1ģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //ʹ������Ƚ�״̬
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	//ch1������
	TIM_OCInitStructure.TIM_Pulse = duty;										//ch1ռ�ձ�50%
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);									
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);				//ʹ�ܵ�Ԥװ�ؼĴ���
		
	TIM_OCInitStructure.TIM_Pulse = duty;
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);									
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);				//ʹ�ܵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM2,ENABLE);											//ʹ��TIM2��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_ITConfig(TIM2,TIM_IT_CC1,ENABLE);										//����ʱ���ж�
	TIM_CtrlPWMOutputs(TIM2,ENABLE);												//ʹ��TIM2������������
	TIM_Cmd(TIM2,ENABLE);																		//ʹ�ܶ�ʱ��2
}

//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 
		
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)//����1���������¼� PA6
	{
		//�������
		FeedbackNowNum ++;
		if(FeedbackNowNum == FeedbackHopeNum)
		{//������ʵ�ʷ�������ֵ = ������������������ֵ
			PulseCntComplete = 1;							 //��������������
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1); 		//����жϱ�־λ
	}
	
}

//�ŷ�������������� �������벶׽��ʼ��
void Coder_InCap_Tim3(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;				//�����ṹ��GPIO_InitStructure	
	TIM_ICInitTypeDef  TIM3_ICInitStructure;								//ͨ�������ʼ���ṹ
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;					//��ʱ����ʼ���ṹ
  NVIC_InitTypeDef NVIC_InitStructure;										//NVIC�жϼĴ������ʼ���ṹ����

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		//TIM3ʱ�ӿ���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//ʹ��PA�˿�ʱ��
	
	//PA6-Feedback_plus(�������ŷ�������巴���ź�)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);										//���ݲ�����ʼ��GPIOA
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	
	//TIM3�����ʼ��	 
	TIM_TimeBaseStructure.TIM_Period 				= arr; //�趨�������Զ���װֵ ���10ms���  
	TIM_TimeBaseStructure.TIM_Prescaler 		=	psc; 	//Ԥ��Ƶ��,1M�ļ���Ƶ��,1us��1.	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	//TIM3ͨ���Ĳ�׽��ʼ��
	TIM3_ICInitStructure.TIM_Channel 			= TIM_Channel_1; 					//CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPolarity 	= TIM_ICPolarity_Rising;	//�����ز���
	TIM3_ICInitStructure.TIM_ICSelection 	= TIM_ICSelection_DirectTI;  //�ܽ���Ĵ�����Ӧ��ϵ
	TIM3_ICInitStructure.TIM_ICPrescaler 	= TIM_ICPSC_DIV1;	 				//���������Ƶ,����Ƶ 
	TIM3_ICInitStructure.TIM_ICFilter 		= 0x00;										//IC1F=0000 ���������˲��� ���˲� �����������������϶������ȶ� 0x0~0xf
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);												//��ʼ��

//	TIM3_ICInitStructure.TIM_Channel 			= TIM_Channel_2; 					//ѡ������� IC2ӳ�䵽TI2��
//	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��3
	TIM_ITConfig( TIM3,TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	


	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
}

