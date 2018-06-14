#include "dma.h"
#include "delay.h"
#include "stm32f10x_dma.h"
#include "declare.h"


DMA_InitTypeDef DMA_InitStructure;

//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//Dir:���䷽��  0=���ڴ淢�͵����� 1=�����跢�͵��ڴ�
//ex_ph_addr:�����ַ
//buff:�洢����ַ
//length:���ݴ����� 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u8 Dir,u32 ex_ph_addr,u32 buff,u16 length)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
  DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = ex_ph_addr;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = buff;  //DMA�ڴ����ַ
	if(Dir)
	{
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴����跢�͵��ڴ�
	}
	else
	{
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	}
	DMA_InitStructure.DMA_BufferSize = length;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
	  	
} 

//����һ��DMA����
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u8 Length)
{ 
//	RS485_TEN();
	DMA_Cmd(DMA_UART1_TX, DISABLE );  //�ر�USART1 TX DMA1 ��ָʾ��ͨ��      
 	DMA_SetCurrDataCounter(DMA_UART1_TX,Length);//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_UART1_TX, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
}	  

 
//DMA �жϳ�ʼ��
//��������������DMA���ڴ�����ݴ��䵽���ڷ��ʹﵽ���ٴ����Ŀ��
void DMA_INIT(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
//---------------DMA1 - USART1  
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
  DMA_ITConfig(DMA_UART1_TX,DMA_IT_TC,ENABLE);	
  DMA_ITConfig(DMA_UART1_RX,DMA_IT_TC,ENABLE);	
	 /* Enable USART1 DMA TX request */   
	 USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  
	 /* Enable USART1 DMA RX request */    
	 USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); 

	DMA_Cmd(DMA_UART1_TX,DISABLE);						  //ʧЧDMA����
	DMA_Cmd(DMA_UART1_RX,ENABLE);								//ʹ��DMA����

//---------------DMA1 - USART2  
//  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�0
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

//  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
//	
//  DMA_ITConfig(DMA_UART2_TX,DMA_IT_TC,ENABLE);	
//  DMA_ITConfig(DMA_UART2_RX,DMA_IT_TC,ENABLE);	
//	 /* Enable USART1 DMA TX request */   
//	 USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);  
//	 /* Enable USART1 DMA RX request */    
//	 USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE); 

//	DMA_Cmd(DMA_UART2_TX,DISABLE);						  //ʧЧDMA����
//	DMA_Cmd(DMA_UART2_RX,ENABLE);								//ʹ��DMA����

}

//DMA1_URAT1_TX IRQ
void DMA1_Channel4_IRQHandler()
{
  if(DMA_GetITStatus(DMA1_IT_TC4))
  {
		DMA_Cmd(DMA_UART1_TX, DISABLE);//�ر�DMA����ʹ��
	  DMA_Cmd(DMA_UART1_RX, DISABLE);//�ر�DMA,��ֹ�������������
		DMA_UART1_RX->CNDTR = 65535;//��װ��
	  DMA_Cmd(DMA_UART1_RX, ENABLE);//������,�ؿ�DMA
//		RS485_Delay = 1;							//ʹ��TX�л���ʶ
		DMA_ClearITPendingBit(DMA1_IT_TC4);
  }
}

//DMA1_URAT1_RX IRQ
void DMA1_Channel5_IRQHandler()
{
  if(DMA_GetITStatus(DMA1_IT_TC5))
  {
	  DMA_Cmd(DMA_UART1_RX, DISABLE);//�ر�DMA,��ֹ�������������
	  DMA_UART1_RX->CNDTR = 65535;//��װ��
	  DMA_Cmd(DMA_UART1_RX, ENABLE);//������,�ؿ�DMA
	  DMA_ClearITPendingBit(DMA1_IT_TC5);
  }
		
}

////DMA1_URAT2_TX IRQ
//void DMA1_Channe17_IRQHandler()
//{
//  if(DMA_GetITStatus(DMA1_IT_TC4))
//  {
//		DMA_Cmd(DMA_UART2_TX, DISABLE);//�ر�DMA����ʹ��
//	  DMA_Cmd(DMA_UART2_RX, DISABLE);//�ر�DMA,��ֹ�������������
//		DMA_UART2_RX->CNDTR = 65535;//��װ��
//	  DMA_Cmd(DMA_UART2_RX, ENABLE);//������,�ؿ�DMA

//		DMA_ClearITPendingBit(DMA1_IT_TC4);
//  }
//}

////DMA1_URAT2_RX IRQ
//void DMA1_Channe16_IRQHandler()
//{
//  if(DMA_GetITStatus(DMA1_IT_TC5))
//  {
//	  DMA_Cmd(DMA_UART2_RX, DISABLE);//�ر�DMA,��ֹ�������������
//	  DMA_UART2_RX->CNDTR = 65535;//��װ��
//	  DMA_Cmd(DMA_UART2_RX, ENABLE);//������,�ؿ�DMA
//	  DMA_ClearITPendingBit(DMA1_IT_TC5);
//  }
//		
//}

