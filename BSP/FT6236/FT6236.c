#include "FT6236.h"
#include"HeadType.h"


u32 Touch_Contact_Time = 0; 
extern void delay_nus(u32 nus);


void FT6236_INIT(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PC2 PC3 */
	GPIO_InitStruct.Pin=FT6236_SCL;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(FT6236_SCL_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.Pin=FT6236_SDA;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(FT6236_SDA_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.Pin=FT6236_RST;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(FT6236_RST_PORT,&GPIO_InitStruct);
	
	
	GPIO_InitStruct.Pin=GPIO_PIN_1;                //PB1
	GPIO_InitStruct.Mode=GPIO_MODE_IT_FALLING;      //�����ش���
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI1_IRQn,2,0);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);   
		
// 	FT6236_SCL_H;
// 	FT6236_SDA_H;
// 	FT6236_RST_H;
}

/*******************************************************************************
* �� �� ��         : I2C_SDA_OUT
* ��������		   : SDA�������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void FT6236_SDA_OUT()
{
//   GPIO_InitTypeDef GPIO_InitStruct;	
// 	__HAL_RCC_GPIOB_CLK_ENABLE();
// 	
// 	GPIO_InitStruct.Pin=FT6236_SDA;
//   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
// 	HAL_GPIO_Init(FT6236_SDA_PORT,&GPIO_InitStruct);
	GPIOB->MODER&=~(3<<(15*2));
	GPIOB->MODER|=1<<(15*2);
}

/*******************************************************************************
* �� �� ��         : I2C_SDA_IN
* ��������		   : SDA��������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void FT6236_SDA_IN(void)
{
// 	GPIO_InitTypeDef GPIO_InitStruct;	
// 	__HAL_RCC_GPIOB_CLK_ENABLE();
// 	
// 	GPIO_InitStruct.Pin = FT6236_SDA;
//   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
// 	GPIO_InitStruct.Pull = GPIO_PULLUP;
// 	HAL_GPIO_Init(FT6236_SDA_PORT, &GPIO_InitStruct);
	GPIOB->MODER&=~(3<<(15*2));
	GPIOB->MODER|=0<<(15*2);
}
/****************************************************
* �������� ��
* ��    �� ����Ƭ��������ʼ�ź�
* ��ڲ��� ����
* ���ز��� ����
* ע������ ��
*****************************************************/
void FT6236_Start(void)					
{
	FT6236_SDA_OUT();     		//sda�����
	FT6236_SDA_H;									
	FT6236_SCL_H;		//SCL��С�ߵ�ƽ����:0.6us
	delay_nus(5);		//��ʼ�źŵ���С����ʱ��:0.6us
	FT6236_SDA_L;		//SCL�ߵ�ƽ�ڼ䣬SDA��һ���½��ر�ʾ��ʼ�ź�
	delay_nus(6);		//��ʼ�źŵ���С����ʱ��:0.6us
	FT6236_SCL_L;		//��ס����,Ϊ����������ַ��׼��;
	delay_nus(2);		//SCL��С�͵�ƽ����:1.2us,��RETʵ��
}

/****************************************************
* �������� ��
* ��    �� ����Ƭ������ֹͣ�ź�
* ��ڲ��� ����
* ���ز��� ����
* ע������ ��
*****************************************************/
void FT6236_Stop(void)							
{
	FT6236_SDA_OUT();     		//sda�����	
	FT6236_SCL_L;
  FT6236_SDA_L;
	FT6236_SCL_H;		//SCL��С�ߵ�ƽ����:0.6us		
	delay_nus(6);		//ֹͣ�źŵ���С����ʱ��:0.6us
	FT6236_SDA_H;		//SCL�ߵ�ƽ�ڼ䣬SDA��һ�������ر�ʾֹͣ�ź�
	delay_nus(6);						
}

/****************************************************
* �������� ��
* ��    �� ����Ƭ������Ӧ���ź�
* ��ڲ��� ����
* ���ز��� ����
* ע������ ����Ƭ����1B���ݺ���һ��Ӧ���ź�
*****************************************************/
void FT6236_McuACK(void)							
{
	FT6236_SCL_L;	
	FT6236_SDA_OUT();     		//sda�����	
	FT6236_SDA_L;	
	delay_nus(2);																	
	FT6236_SCL_H;		//SCL��С�ߵ�ƽ����:0.6us
	delay_nus(5);
	FT6236_SCL_L;		//SCL��С�͵�ƽ����:1.2us
}

/****************************************************
* �������� ��
* ��    �� ����Ƭ�����ͷ�Ӧ���ź�
* ��ڲ��� ����
* ���ز��� ����
* ע������ ����Ƭ��������ֹͣǰ����һ����Ӧ���ź�
*****************************************************/
void FT6236_McuNACK(void)
{
	FT6236_SCL_L;	
	FT6236_SDA_OUT();     				//sda�����	
	FT6236_SDA_H;	
	delay_nus(2);																	
	FT6236_SCL_H;				//SCL��С�ߵ�ƽ����:0.6us
	delay_nus(5);
	FT6236_SCL_L;				//SCL��С�͵�ƽ����:1.2us
}

/****************************************************
* �������� ��
* ��    �� ����Ƭ�����FT6236������Ӧ���ź�
* ��ڲ��� ����
* ���ز��� ��1������Ӧ��ʧ��
			 0������Ӧ��ɹ�
* ע������ ����Ƭ��д1����ַ/���ݺ���
			 ȫ�ֱ���RevAckF:�յ�FT6236Ӧ���źŵı�־λ,Ϊ0��ʾ�յ�
*****************************************************/
u8 FT6236_CheckAck(void)							
{
	u8 ucErrTime=0;
	FT6236_SDA_IN();      				//SDA����Ϊ����
	FT6236_SDA_H;
	FT6236_SCL_H;				//ʹSDA��������Ч;SCL��С�ߵ�ƽ����:0.6us
	delay_nus(3);
	while(FT6236_SDA_Read)
	{	
		ucErrTime++;
		if(ucErrTime>250)		//��Ӧ��
		{
			FT6236_Stop();	
			return 1;
		}
		delay_nus(2);
	}
	FT6236_SCL_L;
	return 0;
}

/****************************************************
* �������� ��
* ��    �� ����Ƭ����IIC���߷���1B�ĵ�ַ/����
* ��ڲ��� �������͵�1B��ַ/����
* ���ز��� ����
* ע������ ������һ�����������ݷ��͹���;������˳���ǴӸߵ���
*****************************************************/
void FT6236_WrOneByte(u8 dat)						
{
	u8 i;						
	FT6236_SDA_OUT();     				//sda�����	
	FT6236_SCL_L;				//����ʱ�ӿ�ʼ���ݴ���
	delay_nus(3);
	for(i = 8; i > 0; i--)		//8λ1B��ַ/���ݵĳ���
	{
		if(dat & 0x80) 		
			FT6236_SDA_H;		//����"1"		
		else
			FT6236_SDA_L;		//����"0"
		FT6236_SCL_H;			//ʹSDA�ϵ�������Ч
		delay_nus(2);			//SCL��С�ߵ�ƽ����:0.6us							
		FT6236_SCL_L;			//SCL��С�͵�ƽ����:1.2us
		delay_nus(2);
		dat <<= 1;				//������������1λ,Ϊ��λ����׼��	
	}		
}

/****************************************************
* �������� ��
* ��    �� ����Ƭ����IIC���߽���1B������
* ��ڲ��� ����
* ���ز��� ���յ���1B����
* ע������ ������һ�����������ݽ��չ���;�Ӹߵ��͵�˳���������
*****************************************************/
u8 FT6236_RdOneByte(void)						
{
	u8 i,dat = 0;				//��������λ���������ݴ浥Ԫ
	FT6236_SDA_IN();						//SDA����Ϊ����
	delay_nus(2);	
	FT6236_SDA_H;			//ʹ������,׼����ȡ����
	delay_nus(2);
	for(i = 8;i > 0;i--)
	{
		FT6236_SCL_L;
		delay_nus(2);
		FT6236_SCL_H;
		dat <<= 1;
		if(FT6236_SDA_Read)
			dat++;
		delay_nus(2);			//SCL��С�͵�ƽ����:1.2us
	}
	FT6236_SDA_H;		
	return(dat);				//����1B������
}

//��FT6236д��һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:д���ݳ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 FT6236_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	FT6236_Start();	 
	FT6236_WrOneByte(FT_CMD_WR);	//����д���� 	 
	FT6236_CheckAck(); 	 										  		   
	FT6236_WrOneByte(reg&0XFF);   	//���͵�8λ��ַ
	FT6236_CheckAck();  
	for(i=0;i<len;i++)
	{	   
    	FT6236_WrOneByte(buf[i]);  	//������
		ret=FT6236_CheckAck();
		if(ret)break;  
	}
    FT6236_Stop();					//����һ��ֹͣ����	    
	return ret; 
}
//��FT6236����һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:�����ݳ���			  
void FT6236_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	FT6236_Start();	
 	FT6236_WrOneByte(FT_CMD_WR);   	//����д���� 	 
	FT6236_CheckAck(); 	 										  		   
 	FT6236_WrOneByte(reg&0XFF);   	//���͵�8λ��ַ
	FT6236_CheckAck();  
 	FT6236_Start();  	 	   
	FT6236_WrOneByte(FT_CMD_RD);   	//���Ͷ�����		   
	FT6236_CheckAck();	  
	for(i=0;i<len - 1;i++)
	{	   
		*buf++ = FT6236_RdOneByte();		//����1B���ݵ��������ݻ�������
		FT6236_McuACK();					//����Ӧ��λ	  
	} 
	*buf = FT6236_RdOneByte();		//����1B���ݵ��������ݻ�������
	FT6236_McuNACK();						//n���ֽڶ���,���ͷ�Ӧ��λ
  FT6236_Stop();					//����һ��ֹͣ����	  
} 
/* 
**��������FT6236_Init
**�����������
**����ֵ����
**���ܣ���ʼ��FT6236����
*/  
void FT6236_Init(void)
{
	u8 temp; 
	static u8 temp1[2]; 
	FT6236_INIT();
// 	/**********************PC6���ж�INT*********************/
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				//PC6	
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	//���óɸ�������	
// 	GPIO_Init(GPIOC,&GPIO_InitStructure);					//��ʼ����ӦGPIOC	
// 	GPIO_SetBits(GPIOC,GPIO_Pin_6);	

	FT6236_RST_L;
	delay_nms(50);
	FT6236_RST_H;
	delay_nms(100);
	FT6236_SDA_H;
	FT6236_SCL_H;
	delay_nms(50);
	temp=0;
	FT6236_WR_Reg(FT_DEVIDE_MODE,&temp,1);	//������������ģʽ 
 	temp=22;								//������Чֵ��22��ԽСԽ����	
 	FT6236_WR_Reg(FT_ID_G_THGROUP,&temp,1);	//���ô�����Чֵ
 	temp=12;								//�������ڣ�����С��12�����14
 	FT6236_WR_Reg(FT_ID_G_PERIODACTIVE,&temp,1);
	delay_nms(20);	
	FT6236_RD_Reg(FT_ID_G_LIB_VERSION,temp1,2);
/******************************************************/
}
const u16 FT6236_TPX_TBL[5]=
{
	FT_TP1_REG,
	FT_TP2_REG,
	FT_TP3_REG,
	FT_TP4_REG,
	FT_TP5_REG
};
TouchPointRefTypeDef TPR_Structure; 
void FT6236_Scan(void)
{
	u8 i=0;
	u8 sta = 0;
	static u8 buf[4] = {0};  
	static u8 gest_id = 0;
	FT6236_RD_Reg(0x02,&sta,1);//��ȡ�������״̬  	   
 	if(sta & 0x0f)	//�ж��Ƿ��д����㰴�£�0x02�Ĵ����ĵ�4λ��ʾ��Ч�������
 	{
 		TPR_Structure.TouchSta = ~(0xFF << (sta & 0x0F));	//~(0xFF << (sta & 0x0F))����ĸ���ת��Ϊ�����㰴����Ч��־
		FT6236_RD_Reg(0x01,&gest_id,1);//��ȡ�������״̬ 
		for(i=0;i<5;i++){	                                //�ֱ��жϴ�����1-5�Ƿ񱻰���
				if(TPR_Structure.TouchSta & (1<<i)){			    //��ȡ����������,���������ȡ��Ӧ��������������
					FT6236_RD_Reg(FT6236_TPX_TBL[i],buf,4);	//��ȡXY����ֵ
					TPR_Structure.x[i]=((u16)(buf[0]&0X0F)<<8)+buf[1];
					TPR_Structure.y[i]=((u16)(buf[2]&0X0F)<<8)+buf[3];
					if((buf[0]&0XC0)!=0X80){
		 				TPR_Structure.x[i]=TPR_Structure.y[i]=0;//������contact�¼�������Ϊ��Ч	
						return;
					}
			 }
 		}
 		TPR_Structure.TouchSta |= TP_PRES_DOWN;     //�������±��
 	}	else{
 		if(TPR_Structure.TouchSta &TP_PRES_DOWN){ 	//֮ǰ�Ǳ����µ�
 			TPR_Structure.TouchSta &= ~0x80;        //�����ɿ����
			if((Touch_Contact_Time >= TOUCH_SHORT_TIME)&&(Touch_Contact_Time < TOUCH_LONG_TIME)){
				TPR_Structure.TouchKey |= TP_SHORT_Key;
			}else if((Touch_Contact_Time >= TOUCH_LONG_TIME)&&(Touch_Contact_Time < TOUCH_LONGLONG_TIME)){
				TPR_Structure.TouchKey |= TP_LONG_Key;
			}else{
				TPR_Structure.TouchKey = 0x00;
			}
			Touch_Contact_Time = 0;
			TPR_Structure.TouchFlag = 0;
 		}else{
 			TPR_Structure.x[0] = 0;
 			TPR_Structure.y[0] = 0;
 			TPR_Structure.TouchSta &=0xe0;	//�����������Ч���
			TPR_Structure.TouchKey = 0x00;
			TPR_Structure.TouchFlag = 0;
 		}
 	}
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	delay_nus(10);  
	switch(GPIO_Pin)
	{
			case GPIO_PIN_1:
			if(FT6236_INT_Read == 0)	{   	
				TPR_Structure.TouchSta |= TP_COORD_UD;				//���������и���
			}
			break;
	}
}


