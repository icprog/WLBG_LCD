#ifndef __ZYY_DEFINE_H__
#define __ZYY_DEFINE_H__

//HEXתASCII 0-F
unsigned char  AsciiCode[16] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x42,0x43,0x44,0x45,0x46};
//ASCIIתHEX 0x30-0x46
unsigned char  HexCode[71] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
								   0x08,0x09,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};

//UART1
volatile unsigned char RS485_Delay=0;		//RS485 COM�� �л��ӳ�״̬ 0=���л� 1=��Ҫ��TX�л���RX

//time
volatile unsigned int TIMER7_CNT = 0;  //��ʱ10ms����  
volatile unsigned int cnt = 0;
volatile unsigned char RC500_BUF[16]={0};			//rc500���ݻ�����

//DMA
volatile unsigned char DMA_SEND_BUFF[100] = {0};//����DMA���ͻ�����
volatile unsigned char DMA_REV_BUFF[100] 	= {0};//����DMA���ջ�����

volatile unsigned char Timer0Value;			   //TIMER0��ʱ����ļ������� ���ڵ�ŷ�500ms��ʱ���ʱ

//500msѰ����־λ
volatile unsigned int WORK_LED_STATUS=0;		//RC500��ʼ����־λ 0=��ʼ���ɹ� 1=��ʼ��ʧ��
volatile unsigned int NO_CARD_Time=0;
volatile unsigned int NO_CARD_Cnt=0;
volatile unsigned int BEEP_TIME = 0;				//��������ʱ��
volatile unsigned char CARD_500ms = 0;			//500msѰ����ʱ��
volatile unsigned char CARD_STATE = 0;			//Ѱ��״̬ 0=�޿�, 1=�п�
volatile unsigned int  CARD_VALUE_HOLD = 0;	//Ѱ������ֵ
volatile unsigned int CARD_VALUE_TIME = 0;	//�����ж�1S��Ѱ���Ŀ��Ƿ�Ϊͬһ�ſ�
volatile unsigned int CARD_TIME_1S = 0;			//Ѱ�������1S��ʱ 0=��ʱʱ��û�е� 1=ʱ�䵽
volatile unsigned char FST_READ = 0;				//�ϵ��һ��Ѱ�� 0=��һ��Ѱ�� 1=�ǵ�һ��Ѱ��

//
#endif
