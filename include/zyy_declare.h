#ifndef __ZYY_DECLARE_H__
#define __ZYY_DECLARE_H__
#include"HeadType.h"

//ASCII ת��������
extern unsigned char AsciiCode[];			//HEXתASCII
extern unsigned char HexCode[];				//ASCIIתHEX

//UART1 ���������ȶ���
#define Rxd_Length 8
#define Txd_Length 10

#define CRC_Hi	DMA_REV_BUFF[UART1.Rev_Length-2]
#define CRC_Lo 	DMA_REV_BUFF[UART1.Rev_Length-1]

//UART1
extern volatile unsigned char RS485_Delay;		//RS485 COM�� �л��ӳ�״̬ 0=���л� 1=��Ҫ��TX�л���RX

// #define  RS485_COM PAout(11)       
// #define  RS485_REN()	RS485_COM = 0	
// #define  RS485_TEN()	RS485_COM = 1
extern void UART1_Com_Check(void);//UART1ͨѶ�������


//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

////UARTͨѶ���ݽṹ����ʹ�õı��� 
//typedef struct
//{
//	u8 State;											//ͨѶ״̬��0=��ͨѶ���� 1=��ͨѶ������Ҫ����
//	u8 Rev_Length;								//ͨѶ���յ����ݳ���
//	u8 Send_Length;								//ͨѶ���͵����ݳ���
//}UART;
//extern UART volatile UART1;

//ָʾ�ƶ˿ڶ���
#define WORK_LED PAout(0)       //����ָʾ��
#define RFID_LED PAout(1)       //������ָʾ��

//RC500�˿ڶ���
#define RC500_NRD 	PAout(8)		//RC500������MF RC500�Ĵ�����������(D0~D7)ѡͨ
#define RC500_ALE 	PCout(6)		//RC500����ַ����ʹ�ܣ�Ϊ��ʱ��AD0~AD5����Ϊ�ڲ���ַ
#define RC500_RST 	PCout(7)		//RC500: ��λ�͵��磺��Ϊ��ʱ���ڲ�������رգ�����ֹͣ������˺��ⲿ�Ͽ����ùܽŵ��½��������ڲ���λ
#define RC500_NCS 	PCout(8)		//RC500��Ƭѡ��ѡ��ͼ���MF RC500��΢�������ӿ�
#define RC500_NWR 	PCout(9)		//RC500��д��MF RC500�Ĵ���д������(D0~D7)ѡͨ
#define RC500_IRQ		PCin(11)		//RC500���ж���������ж��¼�����
#define RC500_ADBUS_IN	(GPIOB -> IDR & 0XFF00) >> 8

#define Lock_Feedback PCin(0)	//����������źŶ���
#define BEEP PCout(4)						//������
#define FSD 64									//RC500 FIFO BUFFER SIZE

//RC500�������ݽṹ��������
typedef struct
{
	unsigned int port_init_error;		//�˿ڳ�ʼ�������ʶ 1=AD���߼��ʧ��
	unsigned int rc500_rst_faile;		//RS500����ʧ�ܱ�ʶ 1 = ����ʧ��
	unsigned char rc500_readcard;		//RC500����״̬��־ 0 = ����ʧ��, 1 = �����ɹ�, 2 = �п���ײ, 3 = ��Ч������û�п�, 4 = �ڹ涨ʱ����û��������� 
}RC500;
extern RC500 volatile RC500_STATE;

extern volatile u8 RC500_BUF[16];		//RC500���ݻ�����

//RC500 �Ĵ�������
//PAGE 0:�����״̬		
// #define	RegPage							0x00			//ѡ��Ĵ���ҳ
// #define RegCommand					0x01			//����(��ֹͣ)�����ִ��
// #define RegFIFOData					0x02			//64�ֽ�FIFO��������������
// #define RegPrimaryStatus		0x03			//�������ͷ������Լ�FIFO������״̬��־
// #define RegFIFOLength				0x04			//FIFO�л�����ֽ���
// #define RegSecondaryStatus 	0x05			//��ͬ��״̬��־
// #define RegInterruptEn		 	0x06			//ʹ�ܺͽ�ֹ�ж�����ͨ���Ŀ���λ
// #define RegInterruptRq			0x07			//�ж������־
// //PAGE 1:���ƺ�״̬
// #define RegControl					0x09			//��ͬ�Ŀ��Ʊ�־�����磺��ʱ�����ڵ�
// #define RegErrorFlag				0x0A			//��ʾ�ϴ�����ִ�д���״̬�Ĵ����־
#define RegCollpos					0x0B			//RF�ӿڼ�⵽�ĵ�һ����ͻλ��λ��
// #define RegTimerValue				0x0C			//��ʱ����ʵ��ֵ
// #define RegCRCResultLSB			0x0D			//CRCЭ�������Ĵ��������λ
// #define RegCRCResultMSB			0x0E			//CRCЭ�������Ĵ��������λ
// #define RegBitFraming				0x0F			//λ��ʽ֡�ĵ���
// //PAGE 2:�������ͱ��������� Transmitter and Coder Control
// #define RegTxControl				0x11			//����������TX1��TX2���߼�״̬����
// #define RegCwConductance		0x12			//ѡ������������TX1��TX2�ĵ絼��
// #define RegModConductance		0x13			//��ֵ����ı�
// #define RegCoderControl			0x14			//��ֵ����ı�
// #define RegModWidth					0x15			//ѡ���������Ŀ��
// #define RegModWidthSOF			0x16			//��ֵ����ı�
// #define RegTypeBFraming			0x17			//��ֵ����ı�
// //PAGE 3:�������ͽ������  Receiver and Decoder Control 
// #define RegRxControl1				0x19			//���ƽ�����״̬
// #define RegDecoderControl		0x1A			//���ƽ�����״̬
// #define RegBitPhase					0x1B			//ѡ�������ͽ�����ʱ��֮���λ��λ
// #define RegRxThreshold			0x1C			//ѡ��λ�������ķ�ֵ
// #define RegBPSKDemControl   0x1D 			//��ֵ����ı�
// #define RegRxControl2       0x1E 			//���ƽ�����״̬�Ͷ��������������Դ
// #define RegClockQControl    0x1F 			//����ʱ�Ӳ�������90�����Ƶ�Q�ŵ�ʱ��
// // PAGE 4:ʱ����ŵ�����   RF-Timing and Channel Redundancy 
// #define RegRxWait           	0x21 		//ѡ���ͺ󣬽���������ǰ��ʱ����
// #define RegChannelRedundancy  0x22 		//ѡ��RF�ŵ������������Լ������ͺ�ģʽ
// #define RegCRCPresetLSB       0x23 		//CRC�Ĵ���Ԥ��ֵ�ĵ��ֽ�
// #define RegCRCPresetMSB       0x24 		//CRC�Ĵ���Ԥ��ֵ�ĸ��ֽ�
// #define RegTimeSlotPeriod     0x25 		//��ֵ����ı�
// #define RegMfOutSelect        0x26 		//ѡ��������ܽ�MFOUT���ڲ��ź�
// #define RFU27                 0x27 		//��ֵ����ı�
// // PAGE 5:FIFO,��ʱ����IRQ����  FIFO, Timer and IRQ-Pin Configuration 
// #define RegFIFOLevel          0x29 		//����FIFO��������羯�����
// #define RegTimerClock         0x2A 		//ѡ��ʱ��ʱ�ӵķ�Ƶ��
// #define RegTimerControl       0x2B 		//ѡ��ʱ������ʼ��ֹͣ����
// #define RegTimerReload        0x2C 		//���嶨ʱ����Ԥװֵ
// #define RegIRqPinConfig       0x2D 		//����IRQ�ŵ����״̬
// #define RFU2E                 0x2E 		//��ֵ����ı�
// #define RFU2F                 0x2F 		//��ֵ����ı�
// // PAGE 6:      RFU 
// #define RFU31                 0x31 		//��������֮��
// #define RFU32                 0x32 		//��������֮��
// #define RFU33                 0x33 		//��������֮��
// #define RFU34                 0x34 		//��������֮��
// #define RFU35                 0x35 		//��������֮��
// #define RFU36                 0x36 		//��������֮��
// #define RFU37                 0x37 		//��������֮��
// // PAGE 7:���Կ���   Test Control 
// #define RFU39                 0x39   	//��������֮��
// #define RegTestAnaSelect      0x3A    //ѡ��ģ�����ģʽ
// #define RFU3B                 0x3B    //��ֵ����ı�
// #define RFU3C                 0x3C    //��ֵ����ı�
// #define RegTestDigiSelect     0x3D    //ѡ�����ֲ���ģʽ
// #define RFU3E                 0x3E    //��������֮��
// #define RFU3F		  						0x3F 		//��������֮��

// ///////////////////////////////////////////////////////////////////// 
// //RC500������ 
// ///////////////////////////////////////////////////////////////////// 
// #define PCD_IDLE         0x00   //�޶���:ȡ����ǰ����
// #define PCD_WRITEE2      0x01   //��FIFO������������ݲ�д���ڲ�EEPROM 
// #define PCD_READE2       0x03   //���ڲ�E2PROM�������ݲ��������FIFO������(�ܳײ��ܱ�����)
// #define PCD_LOADCONFIG   0x07   //��EEPROM�б���ļĴ������õ��뻺��  ��E2PROM��ȡ���ݲ���ʼ��MF RC500�Ĵ���
// #define PCD_LOADKEYE2    0x0B   //��EEPROM�б������Կ���뻺��
// #define PCD_AUTHENT1     0x0C   //��֤��Կ��һ�� 
// #define PCD_AUTHENT2     0x14   //��֤��Կ�ڶ���
// #define PCD_RECEIVE      0x16   //�������� (�ڽ�����ʵ������֮ǰ״̬�������Ĵ���RxWait���õ�ʱ���Ž����ȴ�)
// #define PCD_LOADKEY      0x19   //������Կ ��FIFO������������Կ�ֽڲ����������Կ������(��Կ������ָ���ĸ�ʽ׼��)
// #define PCD_TRANSMIT     0x1A   //�������� �����ݴ�FIFO���������͵���
// #define PCD_TRANSCEIVE   0x1E   //���Ͳ���������:�����ݴ�FIFO���͵������ڷ��ͺ��Զ����������� 
// 																// �ڽ�����ʵ������֮ǰ״̬�������Ĵ���RxWait���õ�ʱ���Ž����ȴ�
// 																// �������Ƿ��ͺͽ��յ����
// #define PCD_RESETPHASE   0x3F   //��λ ���и�λ�ͳ�ʼ���׶�(�������ͨ�����,ֻ��ͨ���ϵ��Ӳ����λ����)
// #define PCD_CALCCRC      0x12   //CRC���� (CRC�������ɴӼĴ���CRCResultLSB��CRCResultMSB�ж���)

// #define PICC_ANTICOLL1		0x93		// һ������ͻ
// 										// ����: optional parts of the card's serial number; 
// 										// ����:rest of) card'sserial number
// #define PICC_ANTICOLL2		0x95		// ��������ͻ
// #define PICC_ANTICOLL3		0x97		// ��������ͻ
// #define PICC_HALT					0x50		// ����(����: Dummy address; ����:None)

// #define MI_OK            0				 //״̬��ȷ��־λ

// #define MI_NOTAGERR      1			//��Ч������û�п�
// #define MI_CRCERR        2
// #define MI_EMPTY         3
// #define MI_AUTHERR       4
// #define MI_BITCOUNTERR   11		// û�п�Ƭ�Ļ������� MI_BITCOUNTERR������16-bit��TagType����
// #define MI_COLLERR       24		// �˴���ͻʱ���� MI_COLLERR��ֱ�ӽ��� ��������ͻ ��ʾ��ϢΪ Ѱ����ͻ

#define MI_RESETERR      25		//RC500��λʧ��
#define MI_INTERFACEERR  26		//RC500�˿ڳ�ʼ��ʧ��
#define MI_ACCESSTIMEOUT 27		//ErrorFlag û���κδ���ͨѶ��ʱ  �ڹ涨��ʱ����û���������

#define MI_COM_ERR       125		//ͨѶʧ��

//TIME
extern volatile unsigned int WORK_LED_STATUS;	//RC500��ʼ����־λ 0=��ʼ���ɹ� 1=��ʼ��ʧ��
extern volatile unsigned int TIMER7_CNT;  //��ʱ��7 ��ʱ10ms����  
extern volatile unsigned int BEEP_TIME;		//��������ʱ��
extern volatile unsigned char CARD_500ms;		//500msѰ����ʱ��
extern volatile unsigned char CARD_STATE;		//Ѱ��״̬ 0=�޿�, 1=�п�
extern volatile unsigned int CARD_VALUE_HOLD;	//Ѱ������ֵ
extern volatile unsigned int CARD_VALUE_TIME;	//�����ж�1S��Ѱ���Ŀ��Ƿ�Ϊͬһ�ſ�
extern volatile unsigned int CARD_TIME_1S;			//Ѱ�������1S��ʱ 0=��ʱʱ��û�е� 1=ʱ�䵽
extern volatile unsigned int cnt;

extern volatile unsigned int NO_CARD_Time;
extern volatile unsigned int NO_CARD_Cnt;

extern void TIM7_Int_Init(u16 arr,u16 psc);

//delay
extern void delay_us(u32 nus);		//nus:Ҫ��ʱ��us��
extern void delay_ms(u16 nms);		//nms:Ҫ��ʱ��ms��

extern void PORT_IO_Init(void);		//Ӳ���˿ڳ�ʼ��
extern void UART1_Cmd_Check(void);//UART1 ͨѶ�����ִ���

////////////RC500���Ƴ�������//////////////////
//RC500�Ĵ�����ʼ��
extern signed char RC500_CONFIG_INIT(void);	

// ��λ RC500
signed char PcdReset(void);   

//Ѱ������
extern void Find_Card(void);	

//ѰA������
extern u32 PcdRequest(unsigned  char req_code, volatile unsigned char *pTagType);

//����ײ����ȡ�����к�
extern char PbdAnticoll(unsigned char *pSnr);

///////////////RC500 AD���߶�д��������//////////////////////
extern u8 RC500_READ(u8 addr);							//MCU��RC500������
extern void RC500_WRITE(u8 addr, u8 data);	//MCU��RC500д����

//ʱ������ ����ʱ�������ⲿ8M��Ƶ��72M
extern void RCC_Config(void);






#endif
