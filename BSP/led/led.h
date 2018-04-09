#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"
#define ON 	0
#define OFF 1

//��\�����з����﷨Ҫ�����ϸ������ĺ��治���пո�ע�͵�һ�С����
#define LED1(a)	if(a)	\
				GPIO_SetBits(GPIOA, GPIO_Pin_0);\
				else	\
				GPIO_ResetBits(GPIOA, GPIO_Pin_0)

#define LED2(a)	if(a)	\
				GPIO_SetBits(GPIOA, GPIO_Pin_1);\
				else	\
				GPIO_ResetBits(GPIOA, GPIO_Pin_1)
				
// ֱ�Ӳ����Ĵ����ķ�������IO
#define digitalHigh(p,i) {p->BSRR = i;}			// ����Ϊ�ߵ�ƽ
#define digitalLow(p,i) {p->BRR = i;}			// ����Ϊ�͵�ƽ
#define digitalToggle(p,i) {p->ODR ^= i;}		// ����Ϊ��ת��ƽ

#define LED1_TOGGLE digitalToggle(GPIOA,GPIO_Pin_0)
#define LED1_OFF 	digitalHigh(GPIOA,GPIO_Pin_0)
#define LED1_ON 	digitalLow(GPIOA,GPIO_Pin_0)

#define LED2_TOGGLE digitalToggle(GPIOCA,GPIO_Pin_1)
#define LED2_OFF 	digitalHigh(GPIOA,GPIO_Pin_1)
#define LED2_ON 	digitalLow(GPIOA,GPIO_Pin_1)

				
#define       GLED_OFF   	GPIO_SetBits(GPIOA, GPIO_Pin_0)
#define       GLED_ON  		GPIO_ResetBits(GPIOA, GPIO_Pin_0)
#define       RLED_OFF   	GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define       RLED_ON 	 	GPIO_ResetBits(GPIOA, GPIO_Pin_1)

extern u8 Gled_Num;
extern u8 Rled_Num;
extern u8 Gled_Light_Static;
extern u8 Rled_Light_Static;
void LED_GPIO_Config(void);
void Led_Flash(void);

#endif
