#include"HeadType.h"

/*����1�ܽ���ӳ��*/
#define USART1REMAP 0
#define RXBUFFERSIZE  512
u8 aRxBuffer[RXBUFFERSIZE];
Usart_Type Usart1_Control_Data;
Usart_Type Usart2_Control_Data;
Usart_Type Usart3_Control_Data;
char Auto_Frame_Time1;
char Auto_Frame_Time2;
char Auto_Frame_Time3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART1)
  {

		__HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }else if(huart->Instance==USART2){
		__HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}else if(huart->Instance==USART3){
		__HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{

  if(huart->Instance==USART1)
  {
    __HAL_RCC_USART1_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOA,GPIO_PIN_9);
    HAL_GPIO_DeInit(GPIOA,GPIO_PIN_10);
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  }else if(huart->Instance==USART2){
    __HAL_RCC_USART2_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOA,GPIO_PIN_2);
    HAL_GPIO_DeInit(GPIOA,GPIO_PIN_3);
    HAL_NVIC_DisableIRQ(USART2_IRQn);
	} else if(huart->Instance==USART3){
    __HAL_RCC_USART3_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOB,GPIO_PIN_10);
    HAL_GPIO_DeInit(GPIOB,GPIO_PIN_11);
    HAL_NVIC_DisableIRQ(USART3_IRQn);
	}
}


static void MX_NVIC_USART1_UART_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{
   __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART1_CLK_ENABLE();
	
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 19200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  
	HAL_UART_Init(&huart1);
  MX_NVIC_USART1_UART_Init();
	HAL_UART_Receive_IT(&huart1, (u8 *)aRxBuffer, RXBUFFERSIZE);
}
static void MX_NVIC_USART2_UART_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 2);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/* USART1 init function */
static void MX_USART2_UART_Init(void)
{
   __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();
	
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 19200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  
	HAL_UART_Init(&huart2);
  MX_NVIC_USART2_UART_Init();
	HAL_UART_Receive_IT(&huart2, (u8 *)aRxBuffer, RXBUFFERSIZE);

}
static void MX_NVIC_USART3_UART_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART3_IRQn, 0, 3);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
}

/* USART1 init function */
static void MX_USART3_UART_Init(void)
{
   __HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_ENABLE();
	
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 19200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  
	HAL_UART_Init(&huart3);
  MX_NVIC_USART3_UART_Init();
	HAL_UART_Receive_IT(&huart3, (u8 *)aRxBuffer, RXBUFFERSIZE);

}

//=============================================================================
//��������:Init_USART1
//���ܸ�Ҫ:USART1 ��ʼ��������ز���
//����˵��:��
//��������:��
//=============================================================================
static void Init_USART1(void)
{
    Usart1_Control_Data.tx_index = 0;
    Usart1_Control_Data.rx_index = 0;
    Usart1_Control_Data.tx_count = 0;
    Usart1_Control_Data.rx_count = 0;
    Usart1_Control_Data.rx_start = 0;
    Usart1_Control_Data.rx_aframe = 0;
    
}
//=============================================================================
//��������:Init_USART2
//���ܸ�Ҫ:USART2 ��ʼ��������ز���
//����˵��:��
//��������:��
//=============================================================================
static void Init_USART2(void)
{
    Usart2_Control_Data.tx_index = 0;
    Usart2_Control_Data.rx_index = 0;
    Usart2_Control_Data.tx_count = 0;
    Usart2_Control_Data.rx_count = 0;
    Usart2_Control_Data.rx_start = 0;
    Usart2_Control_Data.rx_aframe = 0;
    
}
//=============================================================================
//��������:Init_USART3
//���ܸ�Ҫ:USART3 ��ʼ��������ز���
//����˵��:��
//��������:��
//=============================================================================
static void Init_USART3(void)
{
    Usart3_Control_Data.tx_index = 0;
    Usart3_Control_Data.rx_index = 0;
    Usart3_Control_Data.tx_count = 0;
    Usart3_Control_Data.rx_count = 0;
    Usart3_Control_Data.rx_start = 0;
    Usart3_Control_Data.rx_aframe = 0;
    
}
//=============================================================================
//��������:USART1_Interrupts_Config
//���ܸ�Ҫ:USART1 �ж����ȼ�����
//����˵��:��
//��������:��
//=============================================================================
static void USART1_Interrupts_Config(void)
{
//     NVIC_InitTypeDef NVIC_InitStructure; 
//    
//     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
//    
//     NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//     NVIC_Init(&NVIC_InitStructure);
}
//=============================================================================
//��������:USART2_Interrupts_Config
//���ܸ�Ҫ:USART2 �ж����ȼ�����
//����˵��:��
//��������:��
//=============================================================================
static void USART2_Interrupts_Config(void)
{
//     NVIC_InitTypeDef NVIC_InitStructure; 
//    
//     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
//    
//     NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//     NVIC_Init(&NVIC_InitStructure);
}
//=============================================================================
//��������:USART3_Interrupts_Config
//���ܸ�Ҫ:USART3 �ж����ȼ�����
//����˵��:��
//��������:��
//=============================================================================
static void USART3_Interrupts_Config(void)
{
//     NVIC_InitTypeDef NVIC_InitStructure; 
//    
//     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
//    
//     NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//     NVIC_Init(&NVIC_InitStructure);
}
//=============================================================================
//��������:USART1_Config
//���ܸ�Ҫ:USART1 ��ʼ������,����ģʽ���á�57600 8-N-1
//����˵��:��
//��������:��
//=============================================================================
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART1_Interrupts_Config();
	Init_USART1();
	MX_USART1_UART_Init();
	
	  __HAL_RCC_GPIOA_CLK_ENABLE();
  /*Configure GPIO pins : PA11,RES485 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

 //=============================================================================
//��������:USART2_Config
//���ܸ�Ҫ:USART2 ��ʼ������,����ģʽ���á�57600 8-N-1
//����˵��:��
//��������:��
//=============================================================================
void USART2_Config(void)
{
	USART2_Interrupts_Config();
	Init_USART2();
	MX_USART2_UART_Init();
}
//=============================================================================
//��������:USART1_Config
//���ܸ�Ҫ:USART1 ��ʼ������,����ģʽ���á�57600 8-N-1
//����˵��:��
//��������:��
//=============================================================================


void USART3_Config(void )
{	
	USART3_Interrupts_Config();
	Init_USART3();
	MX_USART3_UART_Init();
}
//=============================================================================
//��������:fputc
//���ܸ�Ҫ:�ض���c�⺯��printf��USART
//����˵��:��
//��������:��
//ע��   :��printf����,��Ҫ��ѡUse Microlib
//=============================================================================

int fputc(int ch, FILE *f)
{
//��Printf���ݷ������� 
//  USART_SendData(USART1, (unsigned char) ch);
//  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
//  return (ch);
}
//=============================================================================
//��������:USART1_Putc
//���ܸ�Ҫ:��USART1_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��   :��
//=============================================================================
void USART1_Putc(unsigned char c)
{
//    USART_SendData(USART1, c);
//    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}

//=============================================================================
//��������:USART1_Putc
//���ܸ�Ҫ:��USART1_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART1_Puts(char * str)
{
//    while(*str)
//    {
//        USART_SendData(USART1, *str++);
//        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//    }
}
//=============================================================================
//��������:USART2_Putc
//���ܸ�Ҫ:��USART2_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��   :��
//=============================================================================
void USART2_Putc(unsigned char c)
{
//    USART_SendData(USART2, c);
//    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
}

//=============================================================================
//��������:USART2_Putc
//���ܸ�Ҫ:��USART2_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART2_Puts(char * str)
{
//    while(*str){
//        USART_SendData(USART2, *str++);
//        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
//    }
}
//=============================================================================
//��������:USART3_Putc
//���ܸ�Ҫ:��USART3_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��   :��
//=============================================================================
void USART3_Putc(unsigned char c)
{
//    USART_SendData(USART3, c);
//    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
}

//=============================================================================
//��������:USART3_Putc
//���ܸ�Ҫ:��USART3_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART3_Puts(char * str)
{
//    while(*str){
//        USART_SendData(USART3, *str++);
//        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
//    }
}
//=============================================================================
//��������:USART1_Do_Tx
//���ܸ�Ҫ:������һ�����ݷ��ͳ�ȥ��ͨ���жϷ���
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART1_Do_Tx(void )
{
    if (Usart1_Control_Data.tx_index < Usart1_Control_Data.tx_count) {
//		USART_SendData(USART1, Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index]);
		Usart1_Control_Data.tx_index++;
	}else{
       Usart1_Control_Data.tx_count = 0; 
       Usart1_Control_Data.tx_index = 0;
    }
}
//=============================================================================
//��������:USART2_Do_Tx
//���ܸ�Ҫ:�����ڶ������ݷ��ͳ�ȥ��ͨ���жϷ���
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART2_Do_Tx(void )
{
    if (Usart2_Control_Data.tx_index < Usart2_Control_Data.tx_count) {
//		USART_SendData(USART2, Usart2_Control_Data.txbuf[Usart2_Control_Data.tx_index]);
		Usart2_Control_Data.tx_index++;
	}else{
       Usart2_Control_Data.tx_count = 0; 
       Usart2_Control_Data.tx_index = 0;	   
    }
}
//=============================================================================
//��������:USART3_Do_Tx
//���ܸ�Ҫ:�����ڶ������ݷ��ͳ�ȥ��ͨ���жϷ���
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART3_Do_Tx(void )
{
    if (Usart3_Control_Data.tx_index < Usart3_Control_Data.tx_count) {
//		USART_SendData(USART3, Usart3_Control_Data.txbuf[Usart3_Control_Data.tx_index]);
		Usart3_Control_Data.tx_index++;
	}else{
       Usart3_Control_Data.tx_count = 0; 
       Usart3_Control_Data.tx_index = 0;	   
    }
}
//=============================================================================
//��������:USART1_Do_Rx
//���ܸ�Ҫ:������һ���յ�����
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART1_Do_Rx(u8 rxdata)
{       
		static u8 old_frame_end1,old_frame_end2;
    if (0 == Usart1_Control_Data.rx_aframe){
       if (0 == Usart1_Control_Data.rx_index){  //���յ�һ֡�ĵ�һ�����ݿ�����ʱ��3��ʱ���Զ���֡����
           Usart1_Control_Data.rx_start = 1;
           Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
       }else {
            if (Auto_Frame_Time1 <=0){   //ʱ�䳬ʱ�Զ���֡
               Usart1_Control_Data.rx_aframe = 1; 
               Usart1_Control_Data.rx_start = 0;
               Usart1_Control_Data.rx_count = Usart1_Control_Data.rx_index;
               Usart1_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
               return ;   //ֱ�ӷ��ز���������              
            }                    
        }
				Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1;
       	Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_index] = rxdata;
				Usart1_Control_Data.rx_index++;	
				if(Usart1_Control_Data.rx_index >= 2){		//��������֡β��֡
					old_frame_end1 = Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_index - 2];
					old_frame_end2 = Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_index - 1];
					if((old_frame_end1 == 0x0D)&&(old_frame_end2 == 0X09)){
							Usart1_Control_Data.rx_aframe = 1;  //�������ݳ����Զ���֡
							Usart1_Control_Data.rx_count = Usart1_Control_Data.rx_index;
						  Usart1_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
							Usart1_Control_Data.rx_start = 0;
							Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
					}
				}
        if (Usart1_Control_Data.rx_index > (RxBufMax - 1)){
            Usart1_Control_Data.rx_index = (RxBufMax - 1);
            Usart1_Control_Data.rx_aframe = 1;  //�������ݳ����Զ���֡
            Usart1_Control_Data.rx_count = Usart1_Control_Data.rx_index+1;
            Usart1_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
            Usart1_Control_Data.rx_start = 0;
            Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
        }   
    }else{  //���յ�һ֡���ݺ���봦����ſ��Լ���������һ֡����
       Usart1_Control_Data.rx_start = 0;
       Auto_Frame_Time1 = AUTO_FRAME_TIMEOUT1; 
        return;
    }           
}
//=============================================================================
//��������:USART2_Do_Rx
//���ܸ�Ҫ:�����ڶ����յ�����
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART2_Do_Rx(u8 rxdata)
{       
    if (0 == Usart2_Control_Data.rx_aframe){
       if (0 == Usart2_Control_Data.rx_index){  //���յ�һ֡�ĵ�һ�����ݿ�����ʱ��3��ʱ���Զ���֡����
           Usart2_Control_Data.rx_start = 1;
           Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2; 
       }else {
            if (Auto_Frame_Time2 <=0){   //ʱ�䳬ʱ�Զ���֡
               Usart2_Control_Data.rx_aframe = 1; 
               Usart2_Control_Data.rx_start = 0;
               Usart2_Control_Data.rx_count = Usart2_Control_Data.rx_index;
               Usart2_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
               return ;   //ֱ�ӷ��ز���������              
            }                    
        }
				Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2;
       	Usart2_Control_Data.rxbuf[Usart2_Control_Data.rx_index] = rxdata;
        Usart2_Control_Data.rx_index++;
        if (Usart2_Control_Data.rx_index > (RxBufMax - 1)){
            Usart2_Control_Data.rx_index = (RxBufMax - 1);
            Usart2_Control_Data.rx_aframe = 1;  //�������ݳ����Զ���֡
            Usart2_Control_Data.rx_count = Usart2_Control_Data.rx_index+1;
            Usart2_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
            Usart2_Control_Data.rx_start = 0;
            Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2; 
        }   
    }else{  //���յ�һ֡���ݺ���봦����ſ��Լ���������һ֡����
       Usart2_Control_Data.rx_start = 0;
       Auto_Frame_Time2 = AUTO_FRAME_TIMEOUT2; 
        return;
    }           
}
//=============================================================================
//��������:USART3_Do_Rx
//���ܸ�Ҫ:�����ڶ����յ�����
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART3_Do_Rx(u8 rxdata)
{       
    if (0 == Usart3_Control_Data.rx_aframe){
       if (0 == Usart3_Control_Data.rx_index){  //���յ�һ֡�ĵ�һ�����ݿ�����ʱ��3��ʱ���Զ���֡����
           Usart3_Control_Data.rx_start = 1;
           Auto_Frame_Time3 = AUTO_FRAME_TIMEOUT3; 
       }else {
            if (Auto_Frame_Time3 <=0){   //ʱ�䳬ʱ�Զ���֡
               Usart3_Control_Data.rx_aframe = 1; 
               Usart3_Control_Data.rx_start = 0;
               Usart3_Control_Data.rx_count = Usart3_Control_Data.rx_index;
               Usart3_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
               return ;   //ֱ�ӷ��ز���������              
            }                    
        }
				Auto_Frame_Time3 = AUTO_FRAME_TIMEOUT3;
       	Usart3_Control_Data.rxbuf[Usart3_Control_Data.rx_index] = rxdata;
        Usart3_Control_Data.rx_index++;
        if (Usart3_Control_Data.rx_index > (RxBufMax - 1)){
            Usart3_Control_Data.rx_index = (RxBufMax - 1);
            Usart3_Control_Data.rx_aframe = 1;  //�������ݳ����Զ���֡
            Usart3_Control_Data.rx_count = Usart3_Control_Data.rx_index+1;
            Usart3_Control_Data.rx_index = 0;   //�õ�һ֡���ݺ�ʱ����������
            Usart3_Control_Data.rx_start = 0;
            Auto_Frame_Time3 = AUTO_FRAME_TIMEOUT3; 
        }   
    }else{  //���յ�һ֡���ݺ���봦����ſ��Լ���������һ֡����
       Usart3_Control_Data.rx_start = 0;
       Auto_Frame_Time3 = AUTO_FRAME_TIMEOUT3; 
        return;
    }           
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

}





