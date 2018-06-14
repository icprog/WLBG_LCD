#include "stm32f10x.h"
#include "sys.h"
#include "declare.h"
#include "delay.h"
								    
//������ʼ������
void DialSwitch_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʼ��KEY0-->GPIOA.13,KEY1-->GPIOA.15  ��������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//ʹ��PA�˿�ʱ��

	//���뿪������˿�	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;    //PA11-CODER1, PA12-CODER2										
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;							//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);										//���ݲ�����ʼ��GPIOA
}

//���뿪�ش�����
//���ز��뿪��ֵ
//SLAVE ID��8λ
u8 DialSwitch_Scan_gewei(void)
{	 
	u8 id;
	
	id = ((Two&0x1)<<1) | (One&0x1);
	
	return id;
}

//����SLAVE ID
u8 SW_ID(void)
{
		unsigned char Adress_ID;

		Adress_ID = DialSwitch_Scan_gewei();
		return Adress_ID;
}
