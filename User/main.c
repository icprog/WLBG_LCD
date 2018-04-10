/**
  ******************************************************************************
  * @file    main.c 
  * @version V0.1
  * @date    2017.12.13
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 
#include"HeadType.h"
#include "lcd.h"
#include "text.h"	

extern void USART1_Config(u32 baudrate);
extern void KEY_GPIO_Config(void);
extern u8 Read_key(void);
/**
  * @brief  ������
  * @param  ��
  * @return ��
  */

int main(void)
{
	static u8 tests = 'P';
	delay_init();
//	USART1_Config(9600);
  TIM2_Config();
//	LED_GPIO_Config();
//  BEEP_GPIO_Config();
	ROM_GT30L_Init();
	LCD_Init();
	delay_ms(200);
	while(1){
		LCD_Clear(BLUE);
		Show_Str_Mid(0,0,"�����Ǻú���",16,320);
	  Show_Str(20,50,6*32,"�����Ǻú���",32,0);
		Show_Str2(100,100,&tests,24,0);
		delay_ms(1000);
		delay_ms(1000);
		LCD_Clear(RED);
		delay_ms(1000);

	}

}



