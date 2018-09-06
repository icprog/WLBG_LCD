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
	GPIO_InitStruct.Mode=GPIO_MODE_IT_FALLING;      //上升沿触发
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI1_IRQn,2,0);       //抢占优先级为2，子优先级为0
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);   
		
// 	FT6236_SCL_H;
// 	FT6236_SDA_H;
// 	FT6236_RST_H;
}

/*******************************************************************************
* 函 数 名         : I2C_SDA_OUT
* 函数功能		   : SDA输出配置	   
* 输    入         : 无
* 输    出         : 无
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
* 函 数 名         : I2C_SDA_IN
* 函数功能		   : SDA输入配置	   
* 输    入         : 无
* 输    出         : 无
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
* 函数名称 ：
* 功    能 ：单片机发送起始信号
* 入口参数 ：无
* 返回参数 ：无
* 注意事项 ：
*****************************************************/
void FT6236_Start(void)					
{
	FT6236_SDA_OUT();     		//sda线输出
	FT6236_SDA_H;									
	FT6236_SCL_H;		//SCL最小高电平脉宽:0.6us
	delay_nus(5);		//起始信号的最小建立时间:0.6us
	FT6236_SDA_L;		//SCL高电平期间，SDA的一个下降沿表示起始信号
	delay_nus(6);		//起始信号的最小保持时间:0.6us
	FT6236_SCL_L;		//箝住总线,为发送器件地址做准备;
	delay_nus(2);		//SCL最小低电平脉宽:1.2us,由RET实现
}

/****************************************************
* 函数名称 ：
* 功    能 ：单片机发送停止信号
* 入口参数 ：无
* 返回参数 ：无
* 注意事项 ：
*****************************************************/
void FT6236_Stop(void)							
{
	FT6236_SDA_OUT();     		//sda线输出	
	FT6236_SCL_L;
  FT6236_SDA_L;
	FT6236_SCL_H;		//SCL最小高电平脉宽:0.6us		
	delay_nus(6);		//停止信号的最小建立时间:0.6us
	FT6236_SDA_H;		//SCL高电平期间，SDA的一个上升沿表示停止信号
	delay_nus(6);						
}

/****************************************************
* 函数名称 ：
* 功    能 ：单片机发送应答信号
* 入口参数 ：无
* 返回参数 ：无
* 注意事项 ：单片机读1B数据后发送一个应答信号
*****************************************************/
void FT6236_McuACK(void)							
{
	FT6236_SCL_L;	
	FT6236_SDA_OUT();     		//sda线输出	
	FT6236_SDA_L;	
	delay_nus(2);																	
	FT6236_SCL_H;		//SCL最小高电平脉宽:0.6us
	delay_nus(5);
	FT6236_SCL_L;		//SCL最小低电平脉宽:1.2us
}

/****************************************************
* 函数名称 ：
* 功    能 ：单片机发送非应答信号
* 入口参数 ：无
* 返回参数 ：无
* 注意事项 ：单片机读数据停止前发送一个非应答信号
*****************************************************/
void FT6236_McuNACK(void)
{
	FT6236_SCL_L;	
	FT6236_SDA_OUT();     				//sda线输出	
	FT6236_SDA_H;	
	delay_nus(2);																	
	FT6236_SCL_H;				//SCL最小高电平脉宽:0.6us
	delay_nus(5);
	FT6236_SCL_L;				//SCL最小低电平脉宽:1.2us
}

/****************************************************
* 函数名称 ：
* 功    能 ：单片机检查FT6236送来的应答信号
* 入口参数 ：无
* 返回参数 ：1，接收应答失败
			 0，接收应答成功
* 注意事项 ：单片机写1个地址/数据后检查
			 全局变量RevAckF:收到FT6236应答信号的标志位,为0表示收到
*****************************************************/
u8 FT6236_CheckAck(void)							
{
	u8 ucErrTime=0;
	FT6236_SDA_IN();      				//SDA设置为输入
	FT6236_SDA_H;
	FT6236_SCL_H;				//使SDA上数据有效;SCL最小高电平脉宽:0.6us
	delay_nus(3);
	while(FT6236_SDA_Read)
	{	
		ucErrTime++;
		if(ucErrTime>250)		//无应答
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
* 函数名称 ：
* 功    能 ：单片机向IIC总线发送1B的地址/数据
* 入口参数 ：待发送的1B地址/数据
* 返回参数 ：无
* 注意事项 ：不是一个完整的数据发送过程;送数的顺序是从高到低
*****************************************************/
void FT6236_WrOneByte(u8 dat)						
{
	u8 i;						
	FT6236_SDA_OUT();     				//sda线输出	
	FT6236_SCL_L;				//拉低时钟开始数据传输
	delay_nus(3);
	for(i = 8; i > 0; i--)		//8位1B地址/数据的长度
	{
		if(dat & 0x80) 		
			FT6236_SDA_H;		//发送"1"		
		else
			FT6236_SDA_L;		//发送"0"
		FT6236_SCL_H;			//使SDA上的数据有效
		delay_nus(2);			//SCL最小高电平脉宽:0.6us							
		FT6236_SCL_L;			//SCL最小低电平脉宽:1.2us
		delay_nus(2);
		dat <<= 1;				//发送数据左移1位,为下位发送准备	
	}		
}

/****************************************************
* 函数名称 ：
* 功    能 ：单片机从IIC总线接收1B的数据
* 入口参数 ：无
* 返回参数 ：收到的1B数据
* 注意事项 ：不是一个完整的数据接收过程;从高到低的顺序接收数据
*****************************************************/
u8 FT6236_RdOneByte(void)						
{
	u8 i,dat = 0;				//接收数据位数和内容暂存单元
	FT6236_SDA_IN();						//SDA设置为输入
	delay_nus(2);	
	FT6236_SDA_H;			//使能上拉,准备读取数据
	delay_nus(2);
	for(i = 8;i > 0;i--)
	{
		FT6236_SCL_L;
		delay_nus(2);
		FT6236_SCL_H;
		dat <<= 1;
		if(FT6236_SDA_Read)
			dat++;
		delay_nus(2);			//SCL最小低电平脉宽:1.2us
	}
	FT6236_SDA_H;		
	return(dat);				//返回1B的数据
}

//向FT6236写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
u8 FT6236_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	FT6236_Start();	 
	FT6236_WrOneByte(FT_CMD_WR);	//发送写命令 	 
	FT6236_CheckAck(); 	 										  		   
	FT6236_WrOneByte(reg&0XFF);   	//发送低8位地址
	FT6236_CheckAck();  
	for(i=0;i<len;i++)
	{	   
    	FT6236_WrOneByte(buf[i]);  	//发数据
		ret=FT6236_CheckAck();
		if(ret)break;  
	}
    FT6236_Stop();					//产生一个停止条件	    
	return ret; 
}
//从FT6236读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度			  
void FT6236_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	FT6236_Start();	
 	FT6236_WrOneByte(FT_CMD_WR);   	//发送写命令 	 
	FT6236_CheckAck(); 	 										  		   
 	FT6236_WrOneByte(reg&0XFF);   	//发送低8位地址
	FT6236_CheckAck();  
 	FT6236_Start();  	 	   
	FT6236_WrOneByte(FT_CMD_RD);   	//发送读命令		   
	FT6236_CheckAck();	  
	for(i=0;i<len - 1;i++)
	{	   
		*buf++ = FT6236_RdOneByte();		//读入1B数据到接收数据缓冲区中
		FT6236_McuACK();					//发送应答位	  
	} 
	*buf = FT6236_RdOneByte();		//读入1B数据到接收数据缓冲区中
	FT6236_McuNACK();						//n个字节读完,发送非应答位
  FT6236_Stop();					//产生一个停止条件	  
} 
/* 
**函数名：FT6236_Init
**传入参数：无
**返回值：无
**功能：初始化FT6236引脚
*/  
void FT6236_Init(void)
{
	u8 temp; 
	static u8 temp1[2]; 
	FT6236_INIT();
// 	/**********************PC6，中断INT*********************/
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				//PC6	
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	//设置成浮空输入	
// 	GPIO_Init(GPIOC,&GPIO_InitStructure);					//初始化对应GPIOC	
// 	GPIO_SetBits(GPIOC,GPIO_Pin_6);	

	FT6236_RST_L;
	delay_nms(50);
	FT6236_RST_H;
	delay_nms(100);
	FT6236_SDA_H;
	FT6236_SCL_H;
	delay_nms(50);
	temp=0;
	FT6236_WR_Reg(FT_DEVIDE_MODE,&temp,1);	//进入正常操作模式 
 	temp=22;								//触摸有效值，22，越小越灵敏	
 	FT6236_WR_Reg(FT_ID_G_THGROUP,&temp,1);	//设置触摸有效值
 	temp=12;								//激活周期，不能小于12，最大14
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
	FT6236_RD_Reg(0x02,&sta,1);//读取触摸点的状态  	   
 	if(sta & 0x0f)	//判断是否有触摸点按下，0x02寄存器的低4位表示有效触点个数
 	{
 		TPR_Structure.TouchSta = ~(0xFF << (sta & 0x0F));	//~(0xFF << (sta & 0x0F))将点的个数转换为触摸点按下有效标志
		FT6236_RD_Reg(0x01,&gest_id,1);//读取触摸点的状态 
		for(i=0;i<5;i++){	                                //分别判断触摸点1-5是否被按下
				if(TPR_Structure.TouchSta & (1<<i)){			    //读取触摸点坐标,被按下则读取对应触摸点坐标数据
					FT6236_RD_Reg(FT6236_TPX_TBL[i],buf,4);	//读取XY坐标值
					TPR_Structure.x[i]=((u16)(buf[0]&0X0F)<<8)+buf[1];
					TPR_Structure.y[i]=((u16)(buf[2]&0X0F)<<8)+buf[3];
					if((buf[0]&0XC0)!=0X80){
		 				TPR_Structure.x[i]=TPR_Structure.y[i]=0;//必须是contact事件，才认为有效	
						return;
					}
			 }
 		}
 		TPR_Structure.TouchSta |= TP_PRES_DOWN;     //触摸按下标记
 	}	else{
 		if(TPR_Structure.TouchSta &TP_PRES_DOWN){ 	//之前是被按下的
 			TPR_Structure.TouchSta &= ~0x80;        //触摸松开标记
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
 			TPR_Structure.TouchSta &=0xe0;	//清除触摸点有效标记
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
				TPR_Structure.TouchSta |= TP_COORD_UD;				//触摸坐标有更新
			}
			break;
	}
}


