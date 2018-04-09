#include"HeadType.h"	

#if selectboard
#define	DEVICE1_KEY_IO					GPIO_Pin_3
#define	DEVICE1_KEY_PORT				GPIOA
#define	DEVICE1_KEY_RCC				  RCC_APB2Periph_GPIOA
#else
#define	DEVICE1_KEY_IO					GPIO_Pin_5
#define	DEVICE1_KEY_PORT				GPIOA
#define	DEVICE1_KEY_RCC				  RCC_APB2Periph_GPIOA
#endif 
#define READ_DEVICE1_KEY    		GPIO_ReadInputDataBit(DEVICE1_KEY_PORT,DEVICE1_KEY_IO) //���ص���һ���ֽڣ�������һ��λ



//=============================================================================
//��������: KEY_GPIO_Config
//���ܸ�Ҫ:LED����������
//��������:��
//��������:��
//ע��    :��
//=============================================================================
void KEY_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ʹ��GPIOC������ʱ��
	RCC_APB2PeriphClockCmd(DEVICE1_KEY_RCC,ENABLE);
	//ѡ��Ҫ�õ�GPIO����		
	GPIO_InitStructure.GPIO_Pin = DEVICE1_KEY_IO;
	///��������ģʽΪ�������ģʽ			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	//���������ٶ�Ϊ50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//���ÿ⺯������ʼ��GPIO
	GPIO_Init(DEVICE1_KEY_PORT, &GPIO_InitStructure);

}

u8 Read_key(void)
{
  return GPIO_ReadInputDataBit(DEVICE1_KEY_PORT,DEVICE1_KEY_IO);
}



