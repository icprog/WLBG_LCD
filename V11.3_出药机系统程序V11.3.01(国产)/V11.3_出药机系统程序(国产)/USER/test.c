#include "stm32f10x.h"
#include "sys.h"
#include "declare.h"
#include "stm32f10x_tim.h"
#include "delay.h"

//����Ƥ�����԰������
void BeltButton_Check(void)
{
	switch(Test.ButtonState)
	{
		case 0:		//��������5s
			if(BUTTON_Level == 1)
			{
				Test.ButtonDlyTime 	 = TIMER7_CNT + 10;		//ȥ��100ms
				Test.ButtonState = 1;					//case 1:���ȥ����ʱʱ��100ms		
			}
			break;
			
		case 1:		//case 1:���ȥ����ʱʱ��100ms	
			if(Test.ButtonDlyTime == TIMER7_CNT)
			{
				Test.ButtonState = 2;					//��ǰ����״̬Ϊ�ߵ�ƽ���������ɿ�
			}
			break;
			
		case 2:
			if(BUTTON_Level == 0)
			{//�����ٴα����£�ֹͣ���Գ��򣬲����س�ʼ״̬
				SERVO_STOP(); 								//���ֹͣ
				TEST_LED = 1;									//���Ե���					
				Test.LEDEnable = DISABLE;			//ֹͣ��˸
				Test.State = 0;								//���������ж�
			}
			break;
			
		default:
			break;

	}
}
	
//���������԰������
void LiftButton_Check(void)
{
	switch(Test.ButtonState)
	{
		case 0:		//��������5s
			if(BUTTON_Level == 1)
			{
				Test.ButtonDlyTime 	 = TIMER7_CNT + 10;		//ȥ��100ms
				Test.ButtonState = 1;					//case 1:���ȥ����ʱʱ��100ms		
			}
			break;
			
		case 1:		//case 1:���ȥ����ʱʱ��100ms	
			if(Test.ButtonDlyTime == TIMER7_CNT)
			{
				Test.ButtonState = 2;					//��ǰ����״̬Ϊ�ߵ�ƽ���������ɿ�
			}
			break;
		
//		case 2:
//			if(BUTTON_Level == 0)
//			{
//				Test.ButtonDlyTime 	 = TIMER7_CNT + 10;		//ȥ��100ms
//				Test.ButtonState = 3;					//case 1:���ȥ����ʱʱ��100ms		
//			}
//			break;
		
		case 2:
//			if(Test.ButtonDlyTime == TIMER7_CNT)
//			{
				if(BUTTON_Level == 0)
				{//�����ٴα�����
					SERVO_STOP(); 								//���ֹͣ
					TEST_LED = 1;									//���Ե���					
					Test.LEDEnable = DISABLE;			//ֹͣ��˸
					Test.State = 0;								//���������ж�
					AllSystem_Init();     				//ʹ���л�����ʼ��
				}
//			}
			break;
		
		default:
			break;
	}

}

//����Ƥ�����Գ���,����Ƥ������ת����ת30s--ͣ3s--��ת30s
void TransmitBelt_Test(void)
{
	switch(Test.State)
	{
		case 0:    //�޲���
			TEST_LED = 1;																//���Ե���
			if(BUTTON_Level == 0)												//�������������
			{
				Test.ButtonDlyTime 	 = TIMER7_CNT + 10;		//ȥ��100ms
				Test.State = Down_DLY_100ms;              //����״̬1:�жϰ�������ʱ��
			}
			else
			{
				Test.State = 0;
			}
			break;

		case Down_DLY_100ms:		//case 1:�������·���ʱ���ж�
			if(Test.ButtonDlyTime == TIMER7_CNT)
			{//�������� ��ʱ100ms(����)
				if(BUTTON_Level == 0)
				{//ȷ������������
					Test.ButtonDownTime = TIMER7_CNT + 500;	//����԰�������5s�������������
					Test.State = BUTTON_DOWN_5s;            //����״̬2:�жϰ�������ʱ��
				}
				else
				{
					Test.State = 0;               					//����״̬0:�޲���					
				}
			}			
			break;
			
		case BUTTON_DOWN_5s:		//case 2: ���԰�������5sʱ����
			if(Test.ButtonDownTime == TIMER7_CNT)
			{//����ʱ��5s��
				Test.State = TEST_FWDRUN;					//����״̬3�����Գ��� �����ת
				Test.ButtonState = 0;							//��ǰ����״̬Ϊ�͵�ƽ��������
				Test.LEDEnable = ENABLE;
				Test.LEDTime = TIMER7_CNT + 50;		//500ms test_led��˸��
				WORK_LED_CNT = TIMER7_CNT + 50;
				WORK_LED = 0;
				TEST_LED = 1;
			}
			else if(BUTTON_Level == 1)
			{//5sʱ��δ�����ɿ�����
				Test.State = 0;										//����״̬0, ��5S�������������㣬�����ж�״̬
			}
			break;
			
		case TEST_FWDRUN:		 	//case 3: ������Գ��� �����ת
			SERVO_FWDRun(40);   							//���������1000r/min
			Test.RunTime = TIMER7_CNT + 3000;	//����Ƥ���������ʱ������Ϊ30s
			Test.State = TEST_RUN_TIME;				//case 4: ����30sת���ȴ�ʱ��
			BeltButton_Check();
			break;
		
		case TEST_RUN_TIME: 		//case 4: ����30s����ʱ����
			if(Test.RunTime == TIMER7_CNT)                                                                                                                                                                       
			{//���30s����ʱ�䵽
				SERVO_STOP(); 																//���ֹͣ
				Test.StopTime = TIMER7_CNT + 300;							//���ֹͣ3s��ʱ
				Test.State = FWDRUN_STOP_TIME;								//case 5: ���ֹͣ3sʱ�䶨ʱ		
			}
			BeltButton_Check();
      break;
			
		case FWDRUN_STOP_TIME: 		//case 5:���ֹͣʱ���ж�	
			if(Test.StopTime == TIMER7_CNT)
			{//ֹͣʱ�䵽
				SERVO_REVRun(40);   							//���������ת 1000r/min
				Test.RunTime = TIMER7_CNT + 3000;	//����Ƥ���������ʱ������Ϊ30s			
				Test.State = TEST_REVRUN;					//case 6: �������30sת���ȴ�ʱ��	
			}	
			BeltButton_Check();			
			break;

		case TEST_REVRUN:		 //case 6: �������30sת���ȴ�ʱ��	
			if(Test.RunTime == TIMER7_CNT)
			{//���30s����ʱ�䵽
				SERVO_STOP(); 										//���ֹͣ
				Test.StopTime = TIMER7_CNT + 300;	//���ֹͣ3s��ʱ
				Test.State = REVRUN_STOP_TIME;		//case 7: ���ֹͣʱ�䶨ʱ		
			}
			BeltButton_Check();
			break;
		
		case REVRUN_STOP_TIME:			//case 7: ���ֹͣʱ�䶨ʱ
			if(Test.StopTime == TIMER7_CNT)
			{//ֹͣʱ�䵽
				Test.State = 3;										//���ص����ת״̬				
			}
			BeltButton_Check();
			break;
		
		default:		//���ִ���״̬��ֹͣ���ת�����Żس�ʼ״̬
			SERVO_STOP(); 								//���ֹͣ
			TEST_LED = 1;									//���Ե���			
			Test.LEDEnable = DISABLE;			//ֹͣ��˸
			Test.State = 0;								//���������ж�״̬
			break;
	}			
}

//�����������Գ���  ���°���5S������Գ��򡪡���բ�š���3S�����ر�բ�š�����������4�ų�ҩ�ڡ���1S������ҩ������2S�����ر�ҩ�������ص���ʼλ�á����ظ����϶���
//////////�����������ư�///����������������SERVO_REVRun()  ����������������SERVO_FWDRun()/////////////  
//////////�����������ư�   ����������������SERVO_FWDRun()  ����������������SERVO_REVRun()
void Hoister_Test(void)
{
		switch(Test.State)
	{
		case 0:    //case 0:�޲���
			TEST_LED = 1;																//���Ե���
			if(BUTTON_Level == 0)												//�������������
			{
				Test.ButtonDlyTime 	 = TIMER7_CNT + 10;		//ȥ��100ms
				Test.State = Down_DLY_100ms;              //case 1:�жϰ�������ʱ��
			}
			else
			{
				Test.State = 0;
			}
			break;

		case Down_DLY_100ms:		//case 1:����ʱ���ж�
			if(Test.ButtonDlyTime == TIMER7_CNT)
			{//��������
				if(BUTTON_Level == 0)
				{//��ʱ100ms
					Test.ButtonDownTime = TIMER7_CNT + 500;	//����԰�������5s�������������
					Test.State = BUTTON_DOWN_5s;            //case 2:�жϰ�������ʱ��
				}
				else
				{
					Test.State = 0;               					//����״̬0:�޲���					
				}
			}
			
			break;
			
		case BUTTON_DOWN_5s:     //case 2: ��������5s
			if(Test.ButtonDownTime == TIMER7_CNT)
			{//����ʱ��5s��
				Test.State = TEST_LIFT_START;				//case 3�����������Գ���ʼ 
				Test.ButtonState = 0;								//��ǰ����״̬Ϊ�͵�ƽ��������
				Gate_Opening();											//��բ��				
				Test.LEDEnable = ENABLE;
				Test.LEDTime = TIMER7_CNT + 50;			//500ms test_led��˸��
				WORK_LED_CNT = TIMER7_CNT + 50;
				WORK_LED = 1;
				TEST_LED = 0;
			}
			else if(BUTTON_Level == 1)
			{//5sʱ��δ�����ɿ�����
				Test.State = 0;				//����״̬0, ��5S�������������㣬�����ж�״̬
			}
			break;
			
		case TEST_LIFT_START:			//case 3:������Գ��� բ�Ŵ򿪴������źż��
			if(GS_OPEN_Level == 1)
			{//��⵽բ�Ŵ򿪴������źż��
				Gate_Stop();											//����⵽���Ŵ�����ʱ�����ֹͣ����
				GateState = 1;										//բ��Ϊ��״̬
				Test.DlyTime = TIMER7_CNT +300;		//բ�Ŵ�3s(ʱ��ɵ�)
				Test.State = TEST_GATE_OPEN_DLY4;	//case 4: բ�Ŵ���ʱʱ����		
			}
			LiftButton_Check();
			break;
			
		case TEST_GATE_OPEN_DLY4:				//case 4: ��ҩ��4��բ�Ŵ���ʱʱ����		
			if(Test.DlyTime == TIMER7_CNT)
			{//բ�Ŵ�3s,�ر�բ��
				Gate_Closing();												//�ر�բ��
				Test.State = TEST_GATE_CLOSE_CHECK4;	//case 5����ҩ��4��բ�Źرմ��������								
			}
			LiftButton_Check();
			break;
			
		case TEST_GATE_CLOSE_CHECK4:		//case 5����ҩ��4��բ�Źرմ��������					
			if(GS_CLOSE_Level == 1)
			{//��⵽բ�Źرմ������źż��
				Gate_Stop();												//����⵽���Ŵ�����ʱ�����ֹͣ����
				GateState = 0;											//բ��Ϊ�ر�״̬
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 3000;
				PulseCntComplete = 0;							 				//����������������־λ				
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
				Test.State = TEST_3000_UP_CHECK4;		//case 6:  ��ҩ��4��������: �������պ���ȫ����λ�ã�����������
			}
			LiftButton_Check();
			break;
		
		case TEST_3000_UP_CHECK4:		//case 6:  ��ҩ��4��2095 ������: �������պ���ȫ����λ�ã�����������
			if(PulseCntComplete == 1)
			{//�������պ���ȫ����ʱ����������������ɣ��ٸ����˶����趨����
				PulseCntComplete = 0;							 				//����������������־λ				
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
				Test.State = TEST_Win4_Up_Accelerate_1;		//case 7:�����������ŷ������1�μ���,�������������
			}
			LiftButton_Check();
			break;
				
		case TEST_Win4_Up_Accelerate_1:		//case 7:�����������ŷ������1�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
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
				Test.State = TEST_Win4_Up_Accelerate_2;		//case 8:�����������ŷ������2�μ���,�������������
			}
			LiftButton_Check();
			break;
			
		case TEST_Win4_Up_Accelerate_2:		//case 8:�����������ŷ������2�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
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
				Test.State = TEST_Win4_Up_Accelerate_3;		//case 9:�����������ŷ������3�μ���,�������������
			}
			LiftButton_Check();
			break;
		
		case TEST_Win4_Up_Accelerate_3:		//case 9:�����������ŷ������3�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 							 //����������������־λ				
				FeedbackNowNum = 0;									
				FeedbackHopeNum = 500;  								//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(18);							//�������ŷ������4�μ��� 450r/min
						break;
					
					case 3:		//��������
						SERVO_FWDRun(18);							//�������ŷ������4�μ��� 450r/min							
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Accelerate_4;		//case 10:�����������ŷ������3�μ���,�������������
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Up_Accelerate_4:		//case 10:�����������ŷ������3�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 							 //����������������־λ				
				FeedbackNowNum = 0;									
				FeedbackHopeNum = 600;  								//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(22);							//�������ŷ������5�μ��� 550r/min
						break;
					
					case 3:		//��������
						SERVO_FWDRun(22);							//�������ŷ������5�μ��� 550r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Accelerate_5;		//case 11:�����������ŷ������5�μ���,�������������
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Up_Accelerate_5:		//case 11:�����������ŷ������5�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//����������������־λ				
				FeedbackNowNum = 0;									
				FeedbackHopeNum = 700;  								//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(26);							//�������ŷ������6�μ��� 650r/min
						break;
					
					case 3:		//��������
						SERVO_FWDRun(26);							//�������ŷ������6�μ��� 650r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Accelerate_6;		//case 11:�����������ŷ������6�μ���,�������������
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Up_Accelerate_6:		//case 11:�����������ŷ������6�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 			//����������������־λ				
				FeedbackNowNum = 0;									
				FeedbackHopeNum = 1000;  								//���÷��������� ���ת��1000r/min
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(38);							//�������ŷ������7�μ��� 950r/min
						break;
					
					case 3:		//��������
						SERVO_FWDRun(38);							//�������ŷ������7�μ��� 950r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Accelerate_7;		//case 12:�����������ŷ������3�μ���,�������������
			}
			LiftButton_Check();
			break;
			
		case TEST_Win4_Up_Accelerate_7:		//case 12:�����������ŷ������3�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 							 //����������������־λ				
				FeedbackNowNum = 0;									
				FeedbackHopeNum = (46444  - 3000) - 9100;  //���÷���������	(������)    1000r/min //2090mm / (0.045mm/p) = 46444p ����2090mm��Ҫ46444������, ������������Pr0.11=2400
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(40);							//�������ŷ������8�μ��� 1000r/min
						break;
					
					case 3:		//��������
						SERVO_FWDRun(40);							//�������ŷ������8�μ��� 1000r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_2;		//case 23:�����������ŷ������2�μ��٣��������������
			}
			LiftButton_Check();
			break;
					
		case TEST_Win4_Up_Decelerate_2:		//case 23:�����������ŷ������2�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 800;  									//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(38);							//�������ŷ������1�μ��� 650r/min
						break;
					
					case 3:		//��������
						SERVO_FWDRun(38);							//�������ŷ������1�μ��� 650r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_3;		//case 24:�����������ŷ������3�μ��٣��������������
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Up_Decelerate_3:		//case 24:�����������ŷ������3�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 		//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 750;  							//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(34);							//�������ŷ������1�μ��� 650r/min
						break;
					
					case 3:		//��������
						SERVO_FWDRun(34);							//�������ŷ������1�μ��� 650r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_4;		//case 25:������������4�μ��٣��������������
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Up_Decelerate_4:		//case 25:�����������ŷ������2�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 700;  									//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(30);							//�������ŷ������1�μ��� 650r/min
						break;
					
					case 3:		//��������
						SERVO_FWDRun(30);							//�������ŷ������1�μ��� 650r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_5;		//case 26:�����������ŷ������3�μ��٣��������������
			}
			LiftButton_Check();				
			break;

		case TEST_Win4_Up_Decelerate_5:		//case 26:�����������ŷ������2�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 650;  									//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(26);							//�������ŷ������1�μ��� 250r/min
						break;
					
					case 3:		//��������
						SERVO_FWDRun(26);							//�������ŷ������1�μ��� 250r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_6;		//case 27:�����������ŷ������3�μ��٣��������������
			}
			LiftButton_Check();				
			break;

		case TEST_Win4_Up_Decelerate_6:		//case 27:�����������ŷ������2�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 600;  									//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(22);							//�������ŷ������1�μ��� 650r/min
						break;
					
					case 3:		//��������
						SERVO_FWDRun(22);							//�������ŷ������1�μ��� 650r/min
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_7;		//case 28:�����������ŷ������3�μ��٣��������������
			}
			LiftButton_Check();				
			break;

		case TEST_Win4_Up_Decelerate_7:		//case 28:�����������ŷ������2�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 550;  								//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(18);    					//�������ŷ������8�μ��� 150r/min												
						break;
					
					case 3:		//��������
						SERVO_FWDRun(18);    					//�������ŷ������8�μ��� 150r/min										
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_8;		//case 29:�����������ŷ������3�μ��٣��������������
			}
			LiftButton_Check();				
			break;

		case TEST_Win4_Up_Decelerate_8:		//case 29:�����������ŷ������2�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 				//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 500;  								//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(14);    					//�������ŷ������9�μ��� 125r/min												
						break;
					
					case 3:		//��������
						SERVO_FWDRun(14);    					//�������ŷ������9�μ��� 125r/min										
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_9;		//case 30:�����������ŷ������3�μ��٣��������������
			}
			LiftButton_Check();				
			break;


		case TEST_Win4_Up_Decelerate_9:		//case 30:�����������ŷ������3�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 		//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 450;  								//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(10);    					//�������ŷ������10�μ��� 100r/min												
						break;
					
					case 3:		//��������
						SERVO_FWDRun(10);    					//�������ŷ������10�μ��� 100r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Up_Decelerate_10;		//case 40:������������4�μ��٣��������������
			}
			LiftButton_Check();				
			break;

		case TEST_Win4_Up_Decelerate_10:		//case 30:�����������ŷ������3�μ��٣��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 		//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 400;  								//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_REVRun(6);    					//�������ŷ������11�μ��� 150r/min												
						break;
					
					case 3:		//��������
						SERVO_FWDRun(6);    					//�������ŷ������11�μ��� 150r/min										
						break;
					
					default:
						break;
				}
				Test.State = TEST_FEEDBACK_PULSE4;		//case 40:������������4�μ��٣��������������
			}
			LiftButton_Check();				
			break;
			
		case TEST_FEEDBACK_PULSE4:		//case 40:������������4�μ��٣��������������
			if(PulseCntComplete == 1)
			{//�����������趨λ�ã��������������
				PulseCntComplete = 0;							 //����������������־λ				
				SERVO_STOP(); 										 //�����������趨�߶ȣ����ֹͣ			
				Test.DlyTime = TIMER7_CNT + 100;	 //�����������趨�߶ȣ�ͣ1s����ҩ����ҩ
				Test.State = TEST_WINDOW4;			 	 //case 41: ��ҩ��4�������������ҩ��4����ʱʱ����		
			}			
			LiftButton_Check();
			break;
		
		case TEST_WINDOW4: //case 41: �����������ҩ��4: 2095mm
			if(Test.DlyTime == TIMER7_CNT)                                                                                                                                                                       
			{//����������ҩ��3�� ͣ1s,��ҩ��
				Dou_Opening();												 //��ҩ����ҩ
				Test.State 	 = TEST_DOU_OPEN_CHECK4;   //case 42: ��ҩ��4��ҩ������ʱʱ����						
			}
			LiftButton_Check();
      break;
			
		case TEST_DOU_OPEN_CHECK4:		 //case 42: ��ҩ��4��ҩ������ʱʱ����	
			if(DS_OPEN_Level == 1)				//��δ��⵽ҩ�����Ŵ�����ʱ
			{
				Dou_Stop();												//����⵽ҩ�����Ŵ�����ʱ�����ֹͣ����
				DouState = 1;											//ҩ��Ϊ��״̬
				Test.DlyTime = TIMER7_CNT + 200;	//��ҩ����ҩ�趨ʱ��2s
				Test.State = TEST_BUCKET_DLY4;		//case 43: ��ҩ��4��ҩ����ҩ��ʱʱ����						
			}
			LiftButton_Check();
			break;
			
		case TEST_BUCKET_DLY4: //case 43:��4��ҩ����ҩʱ���ж�
			if(Test.DlyTime == TIMER7_CNT)
			{//ҩ����ҩʱ��2s�����ر�ҩ��
				Dou_Closing();												//�ر�ҩ��
				Test.State = TEST_DOU_CLOSE_CHECK4;		//case 44: �ر�ҩ���������źż��									
			}	
			LiftButton_Check();			
			break;

		case TEST_DOU_CLOSE_CHECK4:		//case 44: ��ҩ��3���ر�ҩ���������źż��	
			if(DS_CLOSE_Level == 1)				//����⵽ҩ���رմ�����ʱ
			{
				Dou_Stop();																	//����⵽ҩ���رմ�����ʱ�����ֹͣ����
				DouState = 0;																//ҩ��Ϊ�ر�״̬
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 100;  										//���÷���������
				PulseCntComplete = 0;							 					//����������������־λ				
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(6);    					//�������½��ŷ������1�μ��� 50r/min									
						break;
					
					case 3:		//��������
						SERVO_REVRun(6);    					//�������½��ŷ������1�μ��� 50r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Accelerate_1;		//case 45:���������µ�1���ٽ׶Σ��жϷ���������
			}			
			LiftButton_Check();
			break;
			
		case TEST_Win4_Down_Accelerate_1:		//case 45:���������µ�1���ٽ׶Σ��жϷ���������
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
				Test.State = TEST_Win4_Down_Accelerate_2;		//case 46:�������½��ŷ������2�μ���,�������������
			}
			LiftButton_Check();
			break;
		
		case TEST_Win4_Down_Accelerate_2:		//case 46:�������½��ŷ������2�μ���,�������������
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
				Test.State = TEST_Win4_Down_Accelerate_3;		//case 47:�������½��ŷ������3�μ���,�������������
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Accelerate_3:		//case 47:�������½��ŷ������3�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//����������������־λ				
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
				Test.State = TEST_Win4_Down_Accelerate_4;		//case 48:�������ŷ����4���ٽ׶�,�������������
			}
			LiftButton_Check();
			break;
			
		case TEST_Win4_Down_Accelerate_4:		//case 47:�������½��ŷ������4�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 500;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(18);    					//�������½��ŷ������4�μ��� 350r/min									
						break;
					
					case 3:		//��������
						SERVO_REVRun(18);    					//�������½��ŷ������4�μ��� 350r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Accelerate_5;		//case 48:�������ŷ����5���ٽ׶�,�������������
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Accelerate_5:		//case 47:�������½��ŷ������3�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 600;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(22);    					//�������½��ŷ������4�μ��� 350r/min									
						break;
					
					case 3:		//��������
						SERVO_REVRun(22);    					//�������½��ŷ������4�μ��� 350r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Accelerate_6;		//case 48:�������ŷ����3���ٽ׶�,�������������
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Accelerate_6:		//case 47:�������½��ŷ������3�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 700;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(26);    					//�������½��ŷ������4�μ��� 350r/min									
						break;
					
					case 3:		//��������
						SERVO_REVRun(26);    					//�������½��ŷ������4�μ��� 350r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Accelerate_7;		//case 48:�������ŷ����3���ٽ׶�,�������������
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Accelerate_7:		//case 47:�������½��ŷ������3�μ���,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 800;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(30);    					//�������½��ŷ������4�μ��� 350r/min									
						break;
					
					case 3:		//��������
						SERVO_REVRun(30);    					//�������½��ŷ������4�μ��� 350r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Accelerate_8;		//case 48:�������ŷ����3���ٽ׶�,�������������
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Accelerate_8:		//case 48:�������ŷ����3���ٽ׶�,�������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 								//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = (46444 - 3000) - 8500;   //���÷���������	(������)    //2096mm / (0.045mm/p) = 46577p ����2096mm��Ҫ46577������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(32);    					//�������½��ŷ������4�μ��� 350r/min									
						break;
					
					case 3:		//��������
						SERVO_REVRun(32);    					//�������½��ŷ������4�μ��� 350r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Uniform;							//case 107:�������ŷ���������˶����������������
			}
			LiftButton_Check();
			break;
		
		case TEST_Win4_Down_Uniform:			//case 107:�������ŷ���������˶����������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 700;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(30);    					//�������½��ŷ������1�μ��� 750r/min							
						break;
					
					case 3:		//��������
						SERVO_REVRun(30);    					//�������½��ŷ������1�μ��� 750r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Decelerate_1;		//case 108:�������½��ŷ����1���ٽ׶Σ��������������
			}
			LiftButton_Check();
			break;
		
		case TEST_Win4_Down_Decelerate_1:		//case 108:�������½��ŷ����1���ٽ׶Σ��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;							 					//����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 650;  										//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(26);    					//�������½��ŷ������1�μ��� 750r/min							
						break;
					
					case 3:		//��������
						SERVO_REVRun(26);    					//�������½��ŷ������1�μ��� 750r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Decelerate_2;		//case 109:�������ŷ����2���ٽ׶Σ��������������
			}
			LiftButton_Check();
			break;
		
		case TEST_Win4_Down_Decelerate_2:		//case 109:�������ŷ����2���ٽ׶Σ��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;								  //����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 600;  							//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(22);    					//�������½��ŷ������1�μ��� 750r/min							
						break;
					
					case 3:		//��������
						SERVO_REVRun(22);    					//�������½��ŷ������1�μ��� 750r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Decelerate_3;		//case 110: �����������˶����պ������λ�ã��жϷ���������
			}
			LiftButton_Check();
			break;
	
		case TEST_Win4_Down_Decelerate_3:		//case 109:�������ŷ����2���ٽ׶Σ��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;								  //����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 550;  							//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(18);    					//�������½��ŷ������1�μ��� 750r/min							
						break;
					
					case 3:		//��������
						SERVO_REVRun(18);    					//�������½��ŷ������1�μ��� 750r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Decelerate_4;		//case 110: �����������˶����պ������λ�ã��жϷ���������
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Decelerate_4:		//case 109:�������ŷ����2���ٽ׶Σ��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;								  //����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 500;  							//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(14);    					//�������½��ŷ������1�μ��� 750r/min							
						break;
					
					case 3:		//��������
						SERVO_REVRun(14);    					//�������½��ŷ������1�μ��� 750r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Decelerate_5;		//case 110: �����������˶����պ������λ�ã��жϷ���������
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Decelerate_5:		//case 109:�������ŷ����2���ٽ׶Σ��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;								  //����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 450;  							//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(10);    					//�������½��ŷ������1�μ��� 750r/min							
						break;
					
					case 3:		//��������
						SERVO_REVRun(10);    					//�������½��ŷ������1�μ��� 750r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Decelerate_6;		//case 110: �����������˶����պ������λ�ã��жϷ���������
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Decelerate_6:		//case 109:�������ŷ����2���ٽ׶Σ��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;								  //����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 400;  							//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(6);    					//�������½��ŷ������1�μ��� 750r/min							
						break;
					
					case 3:		//��������
						SERVO_REVRun(6);    					//�������½��ŷ������1�μ��� 750r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_Win4_Down_Decelerate_7;		//case 110: �����������˶����պ������λ�ã��жϷ���������
			}
			LiftButton_Check();
			break;

		case TEST_Win4_Down_Decelerate_7:		//case 109:�������ŷ����2���ٽ׶Σ��������������
			if(PulseCntComplete == 1)
			{
				PulseCntComplete = 0;								  //����������������־λ				
				FeedbackNowNum = 0;						
				FeedbackHopeNum = 350;  							//���÷���������
				switch(Slave_ID)
				{
					case 2:  	//��������
						SERVO_FWDRun(3);    					//�������½��ŷ������1�μ��� 750r/min							
						break;
					
					case 3:		//��������
						SERVO_REVRun(3);    					//�������½��ŷ������1�μ��� 750r/min												
						break;
					
					default:
						break;
				}
				Test.State = TEST_3000_DOWN_CHECK4;		//case 110: �����������˶����պ������λ�ã��жϷ���������
			}
			LiftButton_Check();
			break;
			
		case TEST_3000_DOWN_CHECK4:		//case 110: ��ҩ��4���������������е���ȫ�����λ�ã���ⷴ��������
			if(PulseCntComplete == 1)
			{//�������պ���ȫ����ʱ����������������ɣ��ٵ����˶����趨����
				PulseCntComplete = 0;							 //����������������־λ				
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
				Test.State = TEST_LIFT_RST_CHECK4;	//case 111: ��ҩ��4����������λ�������źż��
			}
			LiftButton_Check();
			break;
			
		case TEST_LIFT_RST_CHECK4:		//case 111: ��ҩ��4����������λ�������źż��
			if(Bottom_Level == 1)
			{//��⵽��λ��������ֹͣ���
				delay_ms(10);	
				if(Bottom_Level == 1)
				{
					SERVO_STOP(); 											  //���ֹͣ	�������ص�ԭ������������΢С����
					Test.DlyTime = TIMER7_CNT + 15;				//150ms��ʱ
					Test.State = TEST_DISTANCE_RETURN4;		//case 112: ��ҩ��4��0����س�
				}
			}
			LiftButton_Check();
			break;
		
		case TEST_DISTANCE_RETURN4:			//case 112: ��ҩ��4��0����س�
			if(Test.DlyTime == TIMER7_CNT)
			{//��������λͣ150ms�����س�150ms
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
				Test.DlyTime = TIMER7_CNT + 1;	//��������λͣ50ms
				Test.State = TEST_RESET_DLY4;		//case 113: ��������λ�س���ʱʱ�估���
			}
			LiftButton_Check();
			break;

		case TEST_RESET_DLY4:			 //case 113:��������4�θ�λ����ʱ150ms
			if(Test.DlyTime == TIMER7_CNT)
			{//�������س�ʱ�䵽��ֹͣ���
				SERVO_STOP(); 											//���ֹͣ
				Test.DlyTime = TIMER7_CNT + 300;		//��������ɵ�4�ε�ҩ������ͣ��3s
				Test.State 	 = TEST_RESET_STOP4;    //case 114:��������ɵ�4�ε�ҩ������λ							
			}
			LiftButton_Check();
			break;
		
		case TEST_RESET_STOP4:		//case 114:��������λ���
			if(Test.DlyTime == TIMER7_CNT)
			{//�������س�ʱ�䵽��ֹͣ���
				Gate_Opening();											//��բ��				
				Test.State = TEST_LIFT_START;			//����״̬3���������ظ����Գ���ʼ 
			}
			LiftButton_Check();
			break;
			
		default:		//���ִ���״̬��ֹͣ���ת�����Żس�ʼ״̬
			SERVO_STOP(); 								//���ֹͣ
			TEST_LED = 1;									//���Ե���			
			Test.LEDEnable = DISABLE;			//ֹͣ��˸
			Test.State = 0;								//���������ж�״̬
			AllSystem_Init();     				//ʹ���л�����ʼ��
			break;
	}			
}


//////////////���԰���ɨ�����
void ButtonScan(void)			
{
	if(Slave_ID == 1)
	{//�ӻ���ַ = 1(���ʹ�)
		TransmitBelt_Test();		//����Ƥ������
	}
	else if((Slave_ID == 2)||(Slave_ID == 3))
	{//�ӻ���ַ = 2(��������) || �ӻ���ַ = 3(��������)
		Hoister_Test();				//���������Ƴ���
	}
	else
	{
	}
}






