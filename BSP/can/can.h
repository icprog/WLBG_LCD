#ifndef __CAN_H
#define __CAN_H



//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	0		//0,不使能;1,使能.

unsigned char CAN1_Mode_Init(unsigned long tsjw,unsigned long tbs2,unsigned long tbs1,unsigned short brp,unsigned long mode);//CAN初始化
unsigned char CAN1_Send_Msg(unsigned char* msg,unsigned char len);						//发送数据
unsigned char CAN1_Receive_Msg(unsigned char *buf);							//接收数据
#endif


