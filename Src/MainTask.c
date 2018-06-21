/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-03-30
  * ��    ��: emwin�������ܵ�ʵ��
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
#include "ff.h"
#include "lcd/bsp_lcd.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
#define Method3  

/* ˽�б��� ------------------------------------------------------------------*/
extern FATFS fs;												/* FatFs�ļ�ϵͳ���� */
extern FIL file;												/* �ļ����� */
extern FRESULT f_res;                   /* �ļ�������� */
extern UINT fnum;            					  /* �ļ��ɹ���д���� */

#define XBF_KAITI24_ADDR       0     //�ֿ�����SPI_FLash��ַ
uint8_t tempbuf[256]={0};            /* ��SD����ȡ���ݵĻ��� */
#define XBF_KAITI24_SIZE    (1600)   // kaiti24.xbf�ֿ��ļ���ռ��������(ÿ������Ϊ4096���ֽ�)

/* ��չ���� ------------------------------------------------------------------*/
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSong16;

/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: �Ի�����Ϣ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    "YSF1",             0,               0, 0,  320,480,0,0},
    { TEXT_CreateIndirect,      "Ӳʯ����",        GUI_ID_TEXT0,     0, 30, 80, 20, 0, 0x0, 0},
    { TEXT_CreateIndirect,      "Ӳʯ����",        GUI_ID_TEXT1,     0, 120, 80, 120, 0, 0x0, 0},
    { TEXT_CreateIndirect,      "Ӳʯ����",        GUI_ID_TEXT2,     0, 240, 240, 240, 0, 0x0, 0},
    { TEXT_CreateIndirect,      "Ӳʯ����",        GUI_ID_TEXT3,     0, 340, 320, 320, 0, 0x0, 0},
};

/**
  * ��������: �ػ�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void PaintDialog(WM_MESSAGE * pMsg)
{
//    WM_HWIN hWin = pMsg->hWin;
}

/**
  * ��������: �Ի����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void InitDialog(WM_MESSAGE * pMsg)
{
  WM_HWIN hWin = pMsg->hWin;

	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT0), GUI_BLUE);
  TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT0),&GUI_FontHZ12);
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT1), GUI_MAGENTA);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT1),&GUI_FontHZ16);
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT2), GUI_RED);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT2),&GUI_FontHZ24);
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT3), GUI_GREEN);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT3),&GUI_FontHZ32);
}

/**
  * ��������: �Ի���ص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialog(pMsg);
            break;
		
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
		
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
			
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
					
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
			
        default:
            WM_DefaultProc(pMsg);
    }
}

/**
  * ��������: ��SPIFlashд���ֿ��ļ�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void LoadFontLib(void)
{
  uint32_t j;
  uint32_t write_addr=0;

 /* ������ */
  LCD_BK_ON();	
	/* ���� */
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();

/* ��2�������ļ� ***************************************************************/
	f_res = f_open(&file, "0:GBK.bin", FA_OPEN_EXISTING | FA_READ );
	if (f_res !=  FR_OK)
	{
		printf("font.bin�ļ���ʧ��");
	}
  write_addr=XBF_KAITI24_ADDR;
  for(j=0;j<XBF_KAITI24_SIZE;j++)//��������
  {
    SPI_FLASH_SectorErase(write_addr+j*4096);
  }
  
/* ��3��������SD�����ֿ��ļ�font.bin��SPI FLASH **********************************/
  write_addr=XBF_KAITI24_ADDR;
  while(f_res == FR_OK) 
  {
   /* ��ȡһ�����������ݵ�tempbuf */
    f_res = f_read(&file, tempbuf, sizeof(tempbuf), &fnum);
    if(f_res!=FR_OK)break;			 //ִ�д���  
    /* д���ݵ�SPI FLASH */
    SPI_FLASH_BufferWrite(tempbuf, write_addr, 256);
    write_addr+=256;	
    j++;
    if(fnum !=256)break;    
  }
    f_close(&file);
      
	/* �ȴ�1���ʼ��ȥemWin������ */
	GUI_Delay(1000);
}

/**
  * ��������: GUI���ܺ���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void MainTask(void) 
{	
  /* ��ʼ�� */
  GUI_Init();

  LCD_BK_ON();

  /* ��ʼ��ʹ��GUI��ǰ���Ƚ��ֿ��SD���м��ص�SP FLASH���� �ú�������һ�μ��ɣ�֮����ע�͵�����Ϊ�ֿ���д��SPI-Flash */
  LoadFontLib();
  
  /* �����Ի��� */
  GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
  while(1)
	{	
		GUI_Delay(10);	
	}
  
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
