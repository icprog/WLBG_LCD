#ifndef __CAN_H
#define __CAN_H



//CAN1����RX0�ж�ʹ��
#define CAN1_RX0_INT_ENABLE	0		//0,��ʹ��;1,ʹ��.

unsigned char CAN1_Mode_Init(unsigned long tsjw,unsigned long tbs2,unsigned long tbs1,unsigned short brp,unsigned long mode);//CAN��ʼ��
unsigned char CAN1_Send_Msg(unsigned char* msg,unsigned char len);						//��������
unsigned char CAN1_Receive_Msg(unsigned char *buf);							//��������
#endif


