#include "sys.h"
#include "delay.h"
#include "usart.h"	  
#include "stm32f10x_dma.h"
#include "declare.h"
#include "stm32f10x_rcc.h"
////////////////////////////////////////////////////////////////////////////////// 	 
UART volatile UART1;

//UART1ͨѶ����У�������  ���У��
u16 CRC16_Check1(u8 *bufdata, u8 length)			//����1�����
{
	u8 CRCHi = 0xff;
	u8 CRCLo = 0xff;
	int Index;
	u16 shift = 0xffff;
	while(length--)
	{
		Index = CRCLo ^ *(bufdata++);
		CRCLo = CRCHi ^ CRCHiTable[Index];
		CRCHi = CRCLoTable[Index];
	}
	shift = (CRCLo << 8) | CRCHi;
	return shift;
}

//u16 CRC16_Check2(u8 *bufdata, u8 length)			//����2��ʹ��ֱ�ӽ���ķ���
//{
//	u16 shift = 0xffff;
//	u8 i,j;
//	for(i=0;i<length;i++)
//	{
//		shift = shift ^ *(bufdata++);
//		for(j=0;j<8;j++)
//		{
//			for((shift & 0x0001) == 0)
//			{
//				shift = shift >> 1;
//				shift = shift ^ 0xA001;
//			}
//		}
//	}	
//	shift = (shift >> 8) ^ (shift << 8);
//	return shift;
//	
//}


//UART1ָ����Ʒ��Ŵ���
void UART1_Ctrl_Check(void)
{
	u8 i;			//��������У���
	u16 CRC16;		//CRCУ���
	u8  *ptr;			//���������׵�ַ
	ptr = (u8 *) & DMA_SEND_BUFF[0];
	switch(DMA_REV_BUFF[1])
	{//���Ʒ��ż��
		case 6:		//��ʾд������
			//��������ͨ������  AD       06     ID[2][3]  Function[4][5]  CRC_L  CRC_H      
			//            		�ӻ���ַ д������ ��������    ��������       CRC16У����
			//���󷵻�ͨ������  AD       86     ID[2][3]  Reason[4][5]    CRC_L  CRC_H      
			//            		�ӻ���ַ ������   ��������    ����ԭ��       CRC16У����
			if(Reason)
			{
				DMA_SEND_BUFF[0] = DMA_REV_BUFF[0];
				DMA_SEND_BUFF[1] = 0x86;
				DMA_SEND_BUFF[2] = DMA_REV_BUFF[2];
				DMA_SEND_BUFF[3] = DMA_REV_BUFF[3];
				DMA_SEND_BUFF[4] = 0;
				DMA_SEND_BUFF[5] = Reason;
				CRC16 = CRC16_Check1(ptr,6);		//CRCУ���飬����1�����
				DMA_SEND_BUFF[6] = (CRC16 & 0xff00) >> 8;
				DMA_SEND_BUFF[7] = CRC16;
				MYDMA_Enable(DMA_UART1_TX,8);				
				RunFlag = 0;								//�������б�־λ��0���������� 1��������

//				DMA_Cmd(DMA_UART1_RX,DISABLE);		//�ر�DMA,��ֹ�����ڼ�������
//				DMA_UART1_RX -> CNDTR = 65535;		//��װ��
//				DMA_Cmd(DMA_UART1_RX,ENABLE);			//�����꣬�ؿ�DMA
			}
			else
			{
				for(i=0;i<UART1.Rev_Length;i++)
				{
					DMA_SEND_BUFF[i] = DMA_REV_BUFF[i];			
				}
				MYDMA_Enable(DMA_UART1_TX,8);
				RunFlag	= 1;					//�����������б�־λ
			  ID = ((DMA_REV_BUFF[2]<<8) | DMA_REV_BUFF[3]);     //��������
			  Distance = ((DMA_REV_BUFF[4]<<8) | (DMA_REV_BUFF[5])) & 0x0fff;   //�����������߶�
			  SpeedGread = (DMA_REV_BUFF[4] & 0xf0) >> 4;												//���ת��
				GateAction = DMA_REV_BUFF[5];					//բ�Ŷ���ָ��
				DouAction = DMA_REV_BUFF[5];					//ҩ������ָ��
			}
			break;
		
		case 3:		//��ѯ����״̬����汾��
			if(DMA_REV_BUFF[3] == 0x63)
			{//����4���ֽ�=0x63,���汾��
				DMA_SEND_BUFF[0] = DMA_REV_BUFF[0];								
				DMA_SEND_BUFF[1] = 3;
				DMA_SEND_BUFF[2] = DMA_REV_BUFF[2];
				DMA_SEND_BUFF[3] = DMA_REV_BUFF[3];
				DMA_SEND_BUFF[4] = 0x0B;						
				DMA_SEND_BUFF[5] = 1;						   		//�汾�ţ�V11.01 
				CRC16 = CRC16_Check1(ptr,6);		      //CRCУ���飬����1�����
				DMA_SEND_BUFF[6] = (CRC16 & 0xff00) >> 8;
				DMA_SEND_BUFF[7] = CRC16;
				MYDMA_Enable(DMA_UART1_TX,8);				
			}
			
			else
			{//��ѯ����״̬
				DMA_SEND_BUFF[0] = DMA_REV_BUFF[0];								
				DMA_SEND_BUFF[1] = 3;
				DMA_SEND_BUFF[2] = DMA_REV_BUFF[2];
				DMA_SEND_BUFF[3] = DMA_REV_BUFF[3];
				DMA_SEND_BUFF[4] = (MachaStatus << 4) | DouStatus;						
				DMA_SEND_BUFF[5] = (GateStatus << 4) | ServoStatus;						   		
				CRC16 = CRC16_Check1(ptr,6);		//CRCУ���飬����1�����
				DMA_SEND_BUFF[6] = (CRC16 & 0xff00) >> 8;
				DMA_SEND_BUFF[7] = CRC16;
				MYDMA_Enable(DMA_UART1_TX,8);				
				
			}
			
			break;
		
		default:	//�����ִ���
			DMA_Cmd(DMA_UART1_RX,DISABLE);		//�ر�DMA,��ֹ�����ڼ�������
			DMA_UART1_RX -> CNDTR = 65535;		//��װ��
			DMA_Cmd(DMA_UART1_RX,ENABLE);			//�����꣬�ؿ�DMA
			break;		
	}
	
}

	
//UART1ͨѶ�������
  void UART1_Com_Check(void)
{
	u16 CRC16;		//CRCУ���
	u8  *ptr;			//���������׵�ַ
	ptr = (u8 *)&DMA_REV_BUFF[0];
	
	if (UART1.State == 1)				//UART1.State��ͨѶ��Ӧ ��Ϊ�ӻ���ͨ�Ŵ����ʶ
	{//���ڽ�����λ����ͨѶ����,���֡ͷ��֡β
		if(DMA_REV_BUFF[0] == Slave_ID) 
		{//�����һ���ֽ� = ���뿪���趨ֵ��OK
				CRC16 = CRC16_Check1(ptr,UART1.Rev_Length-2);		//CRCУ���飬����1�����
				
//				CRC16 = CRC16_Check2(ptr,UART1.Rev_Length-2);  	//CRCУ���飬����2��ֱ�ӽ��㷨
			  
			if(CRC16 == ((DMA_REV_BUFF[6] << 8) | DMA_REV_BUFF[7]))
			{//CRCУ������ȷ
				UART1_Ctrl_Check();			//ͨѶ�����
			}
			else  //֡ͷ��֡β������
			{
				DMA_Cmd(DMA_UART1_RX, DISABLE);	//�ر�DMA,��ֹ�����ڼ�������	
				DMA_UART1_RX -> CNDTR = 65535;	//������д
				DMA_Cmd(DMA_UART1_RX,ENABLE);		//�����꣬�ؿ�DMA
			}
		}
		else
		{//ERROR
			DMA_Cmd(DMA_UART1_RX,DISABLE);		//�ر�DMA,��ֹ�����ڼ�������		
			DMA_UART1_RX -> CNDTR = 65535;		//������д
			DMA_Cmd(DMA_UART1_RX,ENABLE);			//�����꣬�ؿ�DMA
		}
		UART1.State = 0;
	}
	else  
	{//��ͨѶ
		
	}
	
}

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound)
{
    //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);	//ʹ��USART1��GPIOAʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
 	USART_DeInit(USART1);  //��λ����1
// 	USART_DeInit(USART2);  //��λ����2
	//------------------USART1-------------------------------//
 //USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
 
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10


	//------------------USART2-------------------------------//
 //USART2_TX   PA.2
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
//	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA2
// 
//	//USART2_RX	  PA.3
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
//	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA3


   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
//  USART_Init(USART2, &USART_InitStructure); //��ʼ������2
		 
//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
//  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   
  USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//�����ж� �����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
	USART_ClearFlag(USART1, USART_FLAG_TC);		    //�����һ���ֽ��޷���������

//  USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//�����ж� �����ж�
//  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
//	USART_ClearFlag(USART2, USART_FLAG_TC);		    //�����һ���ֽ��޷���������

}

//����1�жϷ������
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //�����ж�
	{		
		//���IDLE��ʶ
		Res = USART1->SR;
		Res = USART1->DR;
		Res = Res;
		UART1.State = 1;			//����ͨѶ�����־λ
		
		DMA_Cmd(DMA_UART1_RX, DISABLE);//�ر�DMA,��ֹ�������������
		UART1.Rev_Length = 65535 - DMA_GetCurrDataCounter(DMA_UART1_RX);
//		DMA_UART1_RX->CNDTR = 65535;//��װ��
//	  DMA_Cmd(DMA_UART1_RX, ENABLE);//������,�ؿ�DMA
  } 
	
	USART_ClearITPendingBit(USART1, USART_IT_IDLE);
} 

