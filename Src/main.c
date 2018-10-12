/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include"HeadType.h"
#include "lcd.h"
#include "text.h"	
#include "GUI.h"
#include "FT6236.h"

int testTFT = 0;
int OldtestTFT = 100;
static u8 disbuf[4];
CRC_HandleTypeDef hcrc;
/* Private function prototypes -----------------------------------------------*/
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq);
void MX_CRC_Init(void);
void GUIDEMO_Main(void);
extern void MainTask_test(void);
void testLED(void);
void KeytestTFT(void);
void TPR_Dispaly_Coordinate(void);
int main(void)
{
  HAL_Init();
  Stm32_Clock_Init(336,8,2,7);
	MX_CRC_Init();
	LED_GPIO_Config();
	KEY_GPIO_Config();
	ROM_GT30L_Init();
  LCD_Init();
	TIM3_PWM_Init(2000,84-1);
	TIM_SetTIM3Compare1(1000);
	AT24CXX_Init();
	FT6236_Init();
	USART1_Config();
// 	GUI_Init();
	TIM2_Config();
	TIM4_Config();
// 	MainTask_test();
// 	GUIDEMO_Main();
// 	LCD_Clear(LGRAY);
  while (1)
  {
		dispose_key();
// 		TPR_Dispaly_Coordinate();
		if(Key_SetParamFlag == 0){
			Communication_Process();
// 			KeytestTFT();
		}else{
			dispose_menu();
		}
	}
}

/** System Clock Configuration
*/
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure; 
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
    
    __HAL_RCC_PWR_CLK_ENABLE(); //使能PWR时钟
    
    //下面这个设置用来设置调压器输出电压级别，以便在器件未以最大频率工作
    //时使性能与功耗实现平衡。
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//设置调压器输出电压级别1
    
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    //时钟源为HSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      //打开HSE
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;//打开PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;//PLL时钟源选择HSE
    RCC_OscInitStructure.PLL.PLLM=pllm; //主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
    RCC_OscInitStructure.PLL.PLLN=plln; //主PLL倍频系数(PLL倍频),取值范围:64~432.  
    RCC_OscInitStructure.PLL.PLLP=pllp; //系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
    RCC_OscInitStructure.PLL.PLLQ=pllq; //USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//初始化
	
    if(ret!=HAL_OK) while(1);
    
    //选中PLL作为系统时钟源并且配置HCLK,PCLK1和PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//设置系统时钟时钟源为PLL
    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;//AHB分频系数为1
    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV4; //APB1分频系数为4
    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV2; //APB2分频系数为2
    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_5);//同时设置FLASH延时周期为5WS，也就是6个CPU周期。
		
    if(ret!=HAL_OK) while(1);

	 //STM32F405x/407x/415x/417x Z版本的器件支持预取功能
	if (HAL_GetREVID() == 0x1001)
	{
		__HAL_FLASH_PREFETCH_BUFFER_ENABLE();  //使能flash预取
	}
}
void TPR_Dispaly_Coordinate(void)
{
	if(TPR_Structure.TouchKey)		//触摸有按下
		{
			if(TPR_Structure.TouchKey == TP_SHORT_Key){
         POINT_COLOR = WHITE;
				 TPR_Structure.TouchKey &= ~TP_SHORT_Key;
			}else if(TPR_Structure.TouchKey == TP_LONG_Key){
				POINT_COLOR = BLUE;
				TPR_Structure.TouchKey &= ~TP_LONG_Key;
			}else if(TPR_Structure.TouchKey == TP_LONGLONG_Key){
				POINT_COLOR = RED;
			}
			Touch_Contact_Time = 0;
			if(TPR_Structure.x[0] != 0x000){
				disbuf[1] = TPR_Structure.x[0]/100;
				disbuf[3] = TPR_Structure.x[0]%100;
				disbuf[0] = disbuf[1]/10 + '0';
				disbuf[1] = disbuf[1]%10 + '0';
				disbuf[2] = disbuf[3]/10 + '0';
				disbuf[3] = disbuf[3]%10 + '0';
				Show_Str(0,0,64*4,(u8*)disbuf,BACK_COLOR,POINT_COLOR,64,0);
			}
			if(TPR_Structure.y[0] != 0x0000){
				disbuf[1] = TPR_Structure.y[0]/100;
				disbuf[3] = TPR_Structure.y[0]%100;
				disbuf[0] = disbuf[1]/10 + '0';
				disbuf[1] = disbuf[1]%10 + '0';
				disbuf[2] = disbuf[3]/10 + '0';
				disbuf[3] = disbuf[3]%10 + '0';
				Show_Str(0,64,64*4,(u8*)disbuf,BACK_COLOR,POINT_COLOR,64,0);
			}
			delay_ms(1000);
		}

}


/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif



void MX_CRC_Init(void)
{
  hcrc.Instance = CRC;
  HAL_CRC_Init(&hcrc);
}

void HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc)
{

  if(hcrc->Instance==CRC)
  {
    __HAL_RCC_CRC_CLK_ENABLE();
  }
}

void HAL_CRC_MspDeInit(CRC_HandleTypeDef* hcrc)
{

  if(hcrc->Instance==CRC)
  {
    __HAL_RCC_CRC_CLK_DISABLE();
  }
} 

void RS485_start_send_byte(u16 send_count)
{
	Usart1_Control_Data.tx_index = 0;
	Usart1_Control_Data.tx_count = send_count;
	RS485_TEN();
	HAL_UART_Transmit_IT(&huart1, &Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index++], 1);
}
extern Menu_Option Menu;
void testLED(void)
{
		static int testLED = 0;
	 if(Menu == MENU_SETPARAM){
    return ;
	 }
			if(Key_ScanNum == 0x01){
				testLED++;
				if(testLED > 7){
					testLED = 0;
				}
				testTFT++;
				if(testTFT > 7){
					testTFT = 0;
				}
				Key_ScanNum = 0;
				TPR_Structure.TouchFlag = 0;
			}else if(Key_ScanNum == 0x11){
				testLED--;
				if(testLED < 0){
					testLED = 7;
				}	
				testTFT--;
				if(testTFT < 0){
					testTFT = 7;
				}	
				Key_ScanNum = 0;
				TPR_Structure.TouchFlag = 0;				
			}
			switch(testLED){
		  case 0:
					RGB_GLED_OFF;
					RGB_RLED_OFF;
					RGB_BLED_OFF;
				break;
			case 1:
					RGB_GLED_ON;
					RGB_RLED_OFF;
					RGB_BLED_OFF;
				break;				
			case 2:
					RGB_GLED_OFF;
					RGB_RLED_ON;
					RGB_BLED_OFF;
				break;
			case 3:
					RGB_GLED_OFF;
					RGB_RLED_OFF;
					RGB_BLED_ON;
				break;
			case 4:
					RGB_GLED_ON;
					RGB_RLED_ON;
					RGB_BLED_OFF;
				break;
			case 5:
					RGB_GLED_ON;
					RGB_RLED_OFF;
					RGB_BLED_ON;
				break;
			case 6:
					RGB_GLED_OFF;
					RGB_RLED_ON;
					RGB_BLED_ON;
				break;
			case 7:
					RGB_GLED_ON;
					RGB_RLED_ON;
					RGB_BLED_ON;
				break;
		}
}
void KeytestTFT(void)
{
	 if(Menu == MENU_SETPARAM){
    return ;
	 }
			if(testTFT == OldtestTFT){
						return;
			}
	   switch(testTFT){
					case 0:
					LCD_Clear(BLACK);
					Show_Str(0,80,80*4,"演示开始",BLACK,RED,80,1);
					break;
				case 1:
					LCD_Clear(BLACK);
					BACK_COLOR=WHITE;
					Show_Str(0,0,80*4,"南",BLACK,WHITE,80,1);
					Show_Str(80*1,0,80*4,"北",BLACK,WHITE,80,1);
					Show_Str(80*2,0,80*4,"56",BLACK,WHITE,80,1);
					Show_Str(80*3,0,80*4,"区",BLACK,WHITE,80,1);
					Show_Str(0,80,80*4," 住院部 ",BLACK,WHITE,80,0);
					Show_Str(0,160,80*4," 10268",BLACK,WHITE,80,1);
					Show_Str(80*3,160,80*4,"袋",BLACK,WHITE,80,1);
					break;
				case 2:
					LCD_Clear(RED);
					BACK_COLOR=BLACK;
					Show_Str(0,0,80*4,"东",BACK_COLOR,POINT_COLOR,80,1);
					Show_Str(80*1,0,80*4,"西",BACK_COLOR,BLUE,80,1);
					Show_Str(80*2,0,80*4,"12",BACK_COLOR,YELLOW,80,1);
					Show_Str(80*3,0,80*4,"区",BACK_COLOR,BROWN,80,1);
					Show_Str(80,80,80*4,"住院",BACK_COLOR,GREEN,80,0);
					Show_Str(0,160,80*4,"(2096)",BACK_COLOR,LBBLUE,80,1);
					Show_Str(80*3,160,80*4,"袋",BACK_COLOR,YELLOW,80,1);
					break;
				case 3:
					LCD_Clear(BLUE);
					Show_Str(0,0,80*4,"健麾集团",BACK_COLOR,POINT_COLOR,80,1);
					Show_Str(0,80,80*4,"韦乐海茨",BACK_COLOR,GREEN,80,0);
					Show_Str(0,160,80*4,"上海擅韬",BACK_COLOR,YELLOW,80,1);
					break;
				case 4:
					LCD_Clear(LIGHTBLUE);
					Show_Str(0,0+40,80*4,"背景演示",LIGHTBLUE,RED,80,1);
				  Show_Str(0,80+40,80*4," 浅蓝色 ",LIGHTBLUE,RED,80,1);
					break;
				case 5:
					LCD_Clear(YELLOW);
					Show_Str(0,0+40,80*4,"背景演示",YELLOW,RED,80,1);
				  Show_Str(0,80+40,80*4,"  黄色  ",YELLOW,RED,80,1);
					break;
				case 6:
					LCD_Clear(LBBLUE);
					Show_Str(0,0+40,80*4,"背景演示",LBBLUE,RED,80,1);
				  Show_Str(0,80+40,80*4,"浅棕蓝色",LBBLUE,RED,80,1);
					break;
				case 7:
					LCD_Clear(WHITE);
				  Show_Str(0,0+40,80*4,"演示结束",WHITE,RED,80,1);
				  Show_Str(0,80+40,80*4,"触摸循环",WHITE,RED,80,1);
					break;
				default: 
					testTFT = 0;
         break;
			}
			OldtestTFT = testTFT;
}


/******************* (C) COPYRIGHT 2015-2020 ????????? *****END OF FILE****/

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
