#include"HeadType.h"

#define LED_LIGHT_TIMEOUT  100			//N*5MS
u8 Gled_Num;
u8 Rled_Num;
u8 Gled_Light_Static;
u8 Rled_Light_Static;
//=============================================================================
//��������: LED_GPIO_Config
//���ܸ�Ҫ:LED����������
//��������:��
//��������:��
//ע��    :��
//=============================================================================
void LED_GPIO_Config(void)
{	
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  /*Configure GPIO pins : PC2 PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
 	GLED_OFF;
	RLED_OFF;
	Gled_Num = 1;
	Rled_Num = 0;
	RGB_GLED_OFF;
	RGB_RLED_OFF;
	RGB_BLED_OFF;
}

//=============================================================================
//��������:Gled_Flash
//���ܸ�Ҫ:��ɫLED��˸����
//��������:��
//��������:��
//ע��    :��
//=============================================================================
static void Gled_Flash(void )
{
	static u8 retry = 100; //led��˸ʱ����
	static u8 led_state=1;
	if(Gled_Num > 0){
		retry--;
		if(retry <=0){
			if(led_state){
			   GLED_ON;
			   led_state = 0;
			}else{
			   GLED_OFF;
//			   Gled_Num--;
			   led_state = 1;
			}
			retry = 100;
		}
	}	
}
//=============================================================================
//��������:Rled_Flash
//���ܸ�Ҫ:��ɫLED��˸����
//��������:��
//��������:��
//ע��    :��
//=============================================================================
static void Rled_Flash(void )
{
	static u8 retry = 100; //led��˸ʱ����
	static u8 led_state=1;
	if(Rled_Num > 0){
		retry--;
		if(retry <=0){
			if(led_state){
			   RLED_ON;
			   led_state = 0;
			}else{
			   RLED_OFF;
//			   Rled_Num--;
			   led_state = 1;
			}
			retry = 100;
		}
	}	
}
//=============================================================================
//��������:Led_Flash
//���ܸ�Ҫ:LED��˸����
//��������:��
//��������:��
//ע��    :�˺�����Ҫ��5MS��ʱ��2�ж��е���
//=============================================================================
void Led_Flash(void)
{
	static u16 gled_light_time = 0;
	static u16 rled_light_time = 0;
	Gled_Flash();
	Rled_Flash();
	if(Gled_Light_Static == 1){
		gled_light_time++;
		if(gled_light_time >= LED_LIGHT_TIMEOUT ){
			GLED_OFF;
			Gled_Light_Static = 0;
			gled_light_time = 0;
		}
	}
	if(Rled_Light_Static == 1){
		rled_light_time++;
		if(rled_light_time >= LED_LIGHT_TIMEOUT ){
			RLED_OFF;
			Rled_Light_Static = 0;
			rled_light_time = 0;
		}
	}
}





