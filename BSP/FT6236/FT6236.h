#ifndef __FT6236_H_
#define __FT6236_H_
#include "stm32f4xx_hal.h"

#define FT6236_SCL       	GPIO_PIN_14 
#define FT6236_SDA       	GPIO_PIN_15
#define FT6236_RST       	GPIO_PIN_13
#define FT6236_SCL_PORT      	GPIOB
#define FT6236_SDA_PORT     	GPIOB
#define FT6236_RST_PORT     	GPIOB

#define FT6236_SCL_H 			HAL_GPIO_WritePin(FT6236_SCL_PORT,FT6236_SCL,GPIO_PIN_SET)
#define FT6236_SCL_L 			HAL_GPIO_WritePin(FT6236_SCL_PORT,FT6236_SCL,GPIO_PIN_RESET)

#define FT6236_SDA_H 			HAL_GPIO_WritePin(FT6236_SDA_PORT,FT6236_SDA,GPIO_PIN_SET)
#define FT6236_SDA_L 			HAL_GPIO_WritePin(FT6236_SDA_PORT,FT6236_SDA,GPIO_PIN_RESET)

#define FT6236_RST_H			HAL_GPIO_WritePin(FT6236_RST_PORT,FT6236_RST,GPIO_PIN_SET)
#define FT6236_RST_L			HAL_GPIO_WritePin(FT6236_RST_PORT,FT6236_RST,GPIO_PIN_RESET)

#define FT6236_SDA_Read 	HAL_GPIO_ReadPin(FT6236_SDA_PORT,FT6236_SDA)
#define FT6236_INT_Read 	HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
// #define SDA_IN()  {GPIOG->CRL&=0X0FFFFFFF;GPIOG->CRL|=0X40000000;}	 //输入模式，浮空输入模式
// #define SDA_OUT() {GPIOG->CRL&=0X0FFFFFFF;GPIOG->CRL|=0X10000000;}	 //通用推挽输出，输出速度50MHZ

#define TOUCH_SHORT_TIME 		    5
#define TOUCH_LONG_TIME			    300
#define TOUCH_LONGLONG_TIME			1000

#define TP_PRES_DOWN 0x80  //触屏被按下	
#define TP_COORD_UD  0x40  //触摸坐标更新标记
#define TP_SHORT_Key 0X01
#define TP_LONG_Key 0X11
#define TP_LONGLONG_Key 0XFF
//触摸点相关数据结构体定义
typedef struct			
{
	unsigned char TouchFlag;
	unsigned char TouchKey;
	unsigned char TouchSta;	//触摸情况，b7:按下1/松开0; b6:0没有按键按下/1有按键按下;bit5:保留；bit4-bit0触摸点按下有效标志，有效为1，分别对应触摸点5-1；
	unsigned short  x[5];		//支持5点触摸，需要使用5组坐标存储触摸点数据
	unsigned short y[5];
	
}TouchPointRefTypeDef;
extern TouchPointRefTypeDef TPR_Structure;

/*********************IO操作函数*********************/									

// #define FT6236_SDA_Read 	PGin(7)		//用在输入模式	
// #define FT6236_INT 			PCin(6)		//用在输入模式
							
//I2C读写命令	
#define FT_CMD_WR 				0X70    	//写命令
#define FT_CMD_RD 				0X71		//读命令
//FT6236 部分寄存器定义 
#define FT_DEVIDE_MODE 			0x00   		//FT6236模式控制寄存器
#define FT_REG_NUM_FINGER       0x02		//触摸状态寄存器

#define FT_TP1_REG 				0X03	  	//第一个触摸点数据地址
#define FT_TP2_REG 				0X09		//第二个触摸点数据地址
#define FT_TP3_REG 				0X0F		//第三个触摸点数据地址
#define FT_TP4_REG 				0X15		//第四个触摸点数据地址
#define FT_TP5_REG 				0X1B		//第五个触摸点数据地址  
 

#define	FT_ID_G_LIB_VERSION		0xA1		//版本		
#define FT_ID_G_MODE 			0xA4   		//FT6236中断模式控制寄存器
#define FT_ID_G_THGROUP			0x80   		//触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE	0x88   		//激活状态周期设置寄存器  
#define Chip_Vendor_ID          0xA3        //芯片ID(0x36)
#define ID_G_FT6236ID			0xA8		//0x11

unsigned char FT6236_WR_Reg(unsigned short reg,unsigned char  *buf,unsigned char  len);
void FT6236_RD_Reg(unsigned short reg,unsigned char  *buf,unsigned char  len);
void FT6236_Init(void);
void FT6236_Scan(void);

#endif



