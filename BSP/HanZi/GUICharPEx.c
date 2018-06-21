/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-03-30
  * ��    ��: ������ʾ��ʽ-�������Сȫ�ֿ����ɺ�ʹ�÷�����GB2312���룩
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F4Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "MainTask.h"
#include "text.h"	
#include "GT60L16M2K4.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* �������ݻ���, ������ڵ��ڵ�����ģ��Ҫ�Ĵ洢�ռ�*/ 
#define BYTES_PER_FONT      512 

/* ˽�б��� ------------------------------------------------------------------*/
static U8 GUI_FontDataBuf[BYTES_PER_FONT];

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ��ȡ��������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static void GUI_GetDataFromMemory(const GUI_FONT_PROP GUI_UNI_PTR *pProp, U16P c) 
{ 
  U16 BytesPerFont; 
  U32 oft = 0, BaseAdd; 
	U8 hzcode_high,hzcode_low;
	
	char *font = (char *)pProp->paCharInfo->pData; 

	/* ÿ����ģ�������ֽ��� */
    BytesPerFont = GUI_pContext->pAFont->YSize * pProp->paCharInfo->BytesPerLine; 
    if (BytesPerFont > BYTES_PER_FONT){
			BytesPerFont = BYTES_PER_FONT;
		}
		oft = oft;
	/* Ӣ���ַ���ַƫ�Ƽ��� */ 
    if (c < 0x80){ 
			if(strncmp("A12", font, 3) == 0){     /* 6*12 ASCII�ַ� */
// 				BaseAdd = 0x00000000;
				get_font(GUI_FontDataBuf,0x02,0x00,c,6,12,12);
			}else if(strncmp("A16", font, 3) == 0){ /* 8*16 ASCII�ַ� */
// 				BaseAdd = 0x00000C00;
				get_font(GUI_FontDataBuf,0x02,0x00,c,8,16,16);
			}else if(strncmp("A24", font, 3) == 0){ /* 12*24 ASCII�ַ� */
// 				BaseAdd = 0x00001C00;
				get_font(GUI_FontDataBuf,0x01,0x00,c,12,24,24);
			}else if(strncmp("A32", font, 3) == 0){ /* 24*48 ASCII�ַ� */
// 				BaseAdd = 0x00004C00;
				get_font(GUI_FontDataBuf,0x04,0x00,c,16,32,32);
			}
			
			oft = c* BytesPerFont + BaseAdd; 
    } 
	/* ���ֺ�ȫ���ַ���ƫ�Ƶ�ַ���� */
    else                                                                           
    { 
		hzcode_high = c & 0xFF;
		hzcode_low = c>> 8;
		if(strncmp("H12", font, 3) == 0)      /* 12*12 �ַ� */
		{
// 			BaseAdd = 0x00008C00;
			get_font(GUI_FontDataBuf,0x05,hzcode_high,hzcode_low,12,12,12);
		}
		else if(strncmp("H16", font, 3) == 0)  /* 16*16 �ַ� */
		{
// 			BaseAdd = 0x00095060;
			get_font(GUI_FontDataBuf,0x05,hzcode_high,hzcode_low,16,16,16);
		}
		else if(strncmp("H24", font, 3) == 0)  /* 24*24 �ַ� */
		{
// 			BaseAdd = 0x001500E0;
		}
		else if(strncmp("H32", font, 3) == 0)  /* 32*32 �ַ� */
		{
// 			BaseAdd = 0x002F4E00;
			get_font(GUI_FontDataBuf,0x05,hzcode_high,hzcode_low,32,32,32);
		}
		
		/* ���ݺ�������ļ��㹫ʽ������ʼ��ַ */
// 		code2 = c >> 8;
// 		code1 = c & 0xFF;
		
		/* �����ַ������ǰ�˳��洢�ģ��ȴ洢����λ�����ţ���Ȼ���ǵ�λ��λ�ţ����������õ���С�˸�ʽ��
		   һ�����������ֽڣ���ȡ��16λ�����������෴��16λ�����ĸ�λ��λ�ţ���λ�����š�
		*/
// 		oft = ((code1 - 0x81) * 190 + (code2- 0x40) - (code2 / 128))* BytesPerFont + BaseAdd;
    }
// 	printf("oft=%x\n",oft);
	/* ��ȡ�������� */
// 	SPI_FLASH_BufferRead(GUI_FontDataBuf, oft, BytesPerFont);
} 


/**
  * ��������: ��ʾ�ַ�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void GUIUSER_DispChar(U16P c)  
{ 
    int BytesPerLine; 
    GUI_DRAWMODE DrawMode = GUI_pContext->TextMode; 
    const GUI_FONT_PROP GUI_UNI_PTR *pProp = GUI_pContext->pAFont->p.pProp; 
	
    /* ȷ����ʾ���ַ��Ƿ�����Ч��Χ�� */  
    for (; pProp; pProp = pProp->pNext)                                          
    { 
        if ((c >= pProp->First) && (c <= pProp->Last))break; 
    } 
	/* �ַ���Ч��������ʾ */
    if (pProp) 
    { 
        GUI_DRAWMODE OldDrawMode;
        const GUI_CHARINFO GUI_UNI_PTR * pCharInfo = pProp->paCharInfo;
        GUI_GetDataFromMemory(pProp, c);
        BytesPerLine = pCharInfo->BytesPerLine;                
        OldDrawMode  = LCD_SetDrawMode(DrawMode);
        LCD_DrawBitmap(GUI_pContext->DispPosX, GUI_pContext->DispPosY,
                       pCharInfo->XSize, GUI_pContext->pAFont->YSize,
                       GUI_pContext->pAFont->XMag, GUI_pContext->pAFont->YMag,
                       1,    
                       BytesPerLine,
                       &GUI_FontDataBuf[0],
                       &LCD_BKCOLORINDEX
                       );
		
        /* ��䱳�� */
        if (GUI_pContext->pAFont->YDist > GUI_pContext->pAFont->YSize) 
        {
            int YMag = GUI_pContext->pAFont->YMag;
            int YDist = GUI_pContext->pAFont->YDist * YMag;
            int YSize = GUI_pContext->pAFont->YSize * YMag;
            if (DrawMode != LCD_DRAWMODE_TRANS) 
            {
                LCD_COLOR OldColor = GUI_GetColor();
                GUI_SetColor(GUI_GetBkColor());
                LCD_FillRect(GUI_pContext->DispPosX, GUI_pContext->DispPosY + YSize, 
                             GUI_pContext->DispPosX + pCharInfo->XSize, 
                             GUI_pContext->DispPosY + YDist);
                GUI_SetColor(OldColor);
            }
        }
		
        LCD_SetDrawMode(OldDrawMode);
//      if (!GUI_MoveRTL)
        GUI_pContext->DispPosX += pCharInfo->XDist * GUI_pContext->pAFont->XMag;
    }
} 

/**
  * ��������: ��ȡ�ַ���X����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
int GUIUSER_GetCharDistX(U16P c, int * pSizeX) 
{
    const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUI_pContext->pAFont->p.pProp;  
    for (; pProp; pProp = pProp->pNext)                                         
    {
        if ((c >= pProp->First) && (c <= pProp->Last))break;
    }
    return (pProp) ? (pProp->paCharInfo)->XSize * GUI_pContext->pAFont->XMag : 0;
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
