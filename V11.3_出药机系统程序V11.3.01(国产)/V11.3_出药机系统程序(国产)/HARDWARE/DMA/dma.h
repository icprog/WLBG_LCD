#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//DMA ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/8
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
							    					    

void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u8 Dir,u32 ex_ph_addr,u32 buff,u16 length);//����DMA1_CHx

void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u8 Length);//ʹ��DMA1_CHx
		   
#endif




