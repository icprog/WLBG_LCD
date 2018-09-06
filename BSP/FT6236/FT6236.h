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
// #define SDA_IN()  {GPIOG->CRL&=0X0FFFFFFF;GPIOG->CRL|=0X40000000;}	 //����ģʽ����������ģʽ
// #define SDA_OUT() {GPIOG->CRL&=0X0FFFFFFF;GPIOG->CRL|=0X10000000;}	 //ͨ���������������ٶ�50MHZ

#define TOUCH_SHORT_TIME 		    5
#define TOUCH_LONG_TIME			    300
#define TOUCH_LONGLONG_TIME			1000

#define TP_PRES_DOWN 0x80  //����������	
#define TP_COORD_UD  0x40  //����������±��
#define TP_SHORT_Key 0X01
#define TP_LONG_Key 0X11
#define TP_LONGLONG_Key 0XFF
//������������ݽṹ�嶨��
typedef struct			
{
	unsigned char TouchFlag;
	unsigned char TouchKey;
	unsigned char TouchSta;	//���������b7:����1/�ɿ�0; b6:0û�а�������/1�а�������;bit5:������bit4-bit0�����㰴����Ч��־����ЧΪ1���ֱ��Ӧ������5-1��
	unsigned short  x[5];		//֧��5�㴥������Ҫʹ��5������洢����������
	unsigned short y[5];
	
}TouchPointRefTypeDef;
extern TouchPointRefTypeDef TPR_Structure;

/*********************IO��������*********************/									

// #define FT6236_SDA_Read 	PGin(7)		//��������ģʽ	
// #define FT6236_INT 			PCin(6)		//��������ģʽ
							
//I2C��д����	
#define FT_CMD_WR 				0X70    	//д����
#define FT_CMD_RD 				0X71		//������
//FT6236 ���ּĴ������� 
#define FT_DEVIDE_MODE 			0x00   		//FT6236ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER       0x02		//����״̬�Ĵ���

#define FT_TP1_REG 				0X03	  	//��һ�����������ݵ�ַ
#define FT_TP2_REG 				0X09		//�ڶ������������ݵ�ַ
#define FT_TP3_REG 				0X0F		//���������������ݵ�ַ
#define FT_TP4_REG 				0X15		//���ĸ����������ݵ�ַ
#define FT_TP5_REG 				0X1B		//��������������ݵ�ַ  
 

#define	FT_ID_G_LIB_VERSION		0xA1		//�汾		
#define FT_ID_G_MODE 			0xA4   		//FT6236�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP			0x80   		//������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE	0x88   		//����״̬�������üĴ���  
#define Chip_Vendor_ID          0xA3        //оƬID(0x36)
#define ID_G_FT6236ID			0xA8		//0x11

unsigned char FT6236_WR_Reg(unsigned short reg,unsigned char  *buf,unsigned char  len);
void FT6236_RD_Reg(unsigned short reg,unsigned char  *buf,unsigned char  len);
void FT6236_Init(void);
void FT6236_Scan(void);

#endif



