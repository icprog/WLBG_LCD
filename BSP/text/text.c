#include "lcd.h"
#include "text.h"	
#include "string.h"												    											    
#include "stm32f1xx_hal.h"

unsigned char    FontBuf[128];//�ֿ⻺��
unsigned char    Font_Map[128];//�ֿ⻺��

					    
void ROM_GT30L_Init(void)
{ 	
	
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC2 PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA4 PA5 PA6 
                           PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

 void send_command_to_ZK( uchar dat )
{
	uchar i;
	for(i=0;i<8;i++ )
	{
		if(dat&0x80){
			ROM_IN_Set();
		}else{
			ROM_IN_Clr();
		}
			dat = dat<<1;
			ROM_SCK_Clr();
			ROM_SCK_Set();
	}
}
 
//-------------------------------------------------
 unsigned char ZK_read(void)
{
	unsigned char cnt_clk;
	unsigned char ret_data=0;

	for( cnt_clk = 0; cnt_clk < 8; cnt_clk++ )
	{
//		SDO_ZK =1; 
		ROM_SCK_Clr();  //�ֿ�ʱ������  
		ret_data=ret_data<<1;
		if(GPIO_PIN_SET == READ_ROM_OUT())
			{ret_data=ret_data+1; 
			}
		else
			{ret_data=ret_data+0; 
			}
		
		ROM_SCK_Set();; //�ֿ�ʱ������
	 	}
//	SDO_ZK =0;
	return(ret_data);
}

void CL_Mem(void)
{
  unsigned char  i;
	 
	for(i=0;i<128;i++ )
	{
      Font_Map[i]=0;
//	  FontBuf[i]=0;
	  }

}



//=================================================================
//��DataAddress�����ַ��������DataLen���ֽڵ����ݵ� pBuff�ĵ�ַ
//������ȡ
//=================================================================

void ZK_Read_1_n(unsigned char addrHigh,unsigned char addrMid,unsigned char addrLow,unsigned char *pBuff,uchar DataLen )
{
	unsigned char i;
 	ROM_CS_Clr();
//	CS = 1;
	ROM_SCK_Clr();
	send_command_to_ZK(0x03);
	send_command_to_ZK(addrHigh);
	send_command_to_ZK(addrMid);
	send_command_to_ZK(addrLow);
	for(i = 0; i < DataLen; i++ )
  		 *(pBuff+i)=ZK_read();
	ROM_CS_Set();
}

 
//code �ַ�ָ�뿪ʼ
//���ֿ��в��ҳ���ģ
//code �ַ����Ŀ�ʼ��ַ,GBK��
//mat  ���ݴ�ŵ�ַ (size/8+((size%8)?1:0))*(size) bytes��С	
//size:�����С
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{		    
	unsigned char qh,ql;			  
	u8 csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	unsigned char  AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ
	unsigned long  FontAddr=0; //�ֵ�ַ
	unsigned long  BaseAdd=0; //�ֿ����ַ	
	switch(size)
		{  // n������h���ָ߶ȣ�w���ֿ��ȣ� d���ּ�࣬c��ҳ��С
		case 12:  BaseAdd=0x0;			  break;  // 12*12  
		case 16:  BaseAdd=0x2C9D0;    break;   // 15*16  
	  case 24:  BaseAdd=0x68190;    break;   // 24*24  
	  case 32:  BaseAdd=0xEDF00;    break;   // 32*32  
 		}
	qh=*code;
	ql=*(++code);
	if((qh>0x00))
	{
		if(((qh>=0xA1) &&(qh<=0xA9))&&(ql>=0xA1))
		{						
			/*������壨GB2312�������� �ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1))*n+ BaseAdd; ������ȡ��ַ*/
		 	FontAddr = (qh- 0xA1)*94; 
			FontAddr += (ql-0xA1);
			FontAddr = (unsigned long)((FontAddr*csize)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			ZK_Read_1_n(AddrHigh,AddrMid,AddrLow,mat,csize);/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
		}
		else if(((qh>=0xB0) &&(qh<=0xF7))&&(ql>=0xA1))
		{						
			/*������壨GB2312�� �ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+846)*n+ BaseAdd; ������ȡ��ַ*/
			 
			FontAddr = (qh- 0xB0)*94; 
			FontAddr += (ql-0xA1)+846;
			FontAddr = (unsigned long)((FontAddr*csize)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			ZK_Read_1_n(AddrHigh,AddrMid,AddrLow,mat,csize);/*ȡһ�����ֵ����ݣ��浽"FontBuf[ ]"*/
		}
		
	} 		  
}  

//��ʾһ��ָ����С�ĺ���
//x,y :���ֵ�����
//font:����GBK��
//size:�����С
//mode:0,������ʾ,1,������ʾ	   
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode)
{
	u8 i,temp,t,t1;
	u16 y0=y;
	static u8 dzk[128];   
	static u8 csize;
	csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	for(i=0;i<128;i++ ){
      dzk[i]=0;
	}
	if(size!=12&&size!=16&&size!=24&&size!=32)return;	//��֧�ֵ�size
	Get_HzMat(font,dzk,size);	//�õ���Ӧ��С�ĵ������� 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//�õ���������                          
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if((mode==0)||(mode==2))LCD_Fast_DrawPoint(x,y,BACK_COLOR); 
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}  
}
//��ָ��λ�ÿ�ʼ��ʾһ���ַ���	    
//֧���Զ�����
//(x,y):��ʼ����
//width,height:����
//str  :�ַ���
//size :�����С
//mode:0,�ǵ��ӷ�ʽ;1,���ӷ�ʽ    	   		   
void Show_Str(u16 y,u16 x,u16 width,u8*str,u8 size,u8 mode)
{					
	u16 x0=x;
	u16 y0=y;	
	u16 height = size;
    u8 bHz=0;     //�ַ���������  	    				    				  	  
    while(*str!=0)//����δ����
    { 
        if(!bHz){
	        if(*str>0x80){
						bHz=1;//���� 
					}else{              //�ַ�     
              if(y>(y0+width-size/2)){				   
								x+=size;
								y=y0;	   
							}		
						if((x>(x0+height-size))&&(mode < 2))break;//Խ�緵��,mode>=2ʱ������һ����ʾ������Ի�����ʾ    
		        if(*str==13){//���з���     
		            x+=size;
								y=y0;
		            str++; 
		        }else {
//							LCD_ShowChar(y,x,*str,size,mode);//��Ч����д�� 
							Show_Ascchar(y,x,*str,size,mode);
						}
						str++; 
		        y+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
	        }
        }else{//����  
            bHz=0;//�к��ֿ�    
            if(y>(y0+width-size)){	    
							x+=size;
							y=y0;		  
						}
	        if((x>(x0+height-size))&&(mode<2))break;//Խ�緵��  						     
	        Show_Font(x,y,str,size,mode); //��ʾ�������,������ʾ 
	        str+=2; 
	        y+=size;//��һ������ƫ��	    
        }						 
    }   
}  			 		 
//��ָ�����ȵ��м���ʾ�ַ���
//����ַ����ȳ�����len,����Show_Str��ʾ
//len:ָ��Ҫ��ʾ�Ŀ���			  
void Show_Str_Mid(u16 y,u16 x,u8*str,u8 size,u16 len)
{
	u16 strlenth=0;
  strlenth=strlen((const char*)str);
	strlenth*=size/2;
	if(strlenth>len)Show_Str(y,x,lcddev.width,str,size,1);
	else
	{
			strlenth=(len-strlenth)/2;
	    Show_Str(strlenth+y,x,lcddev.width,str,size,1);
	}
}   

//code �ַ�ָ�뿪ʼ
//���ֿ��в��ҳ���ģ
//code �ַ����Ŀ�ʼ��ַ,GBK��
//mat  ���ݴ�ŵ�ַ (size/8+((size%8)?1:0))*(size) bytes��С	
//size:�����С
void Get_AscMat(unsigned char *code,unsigned char *mat,u8 size)
{		    
	unsigned char qh;			  
	u8 csize=(size/8+((size%8)?1:0))*(size/2);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	unsigned char  AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ
	unsigned long  FontAddr=0; //�ֵ�ַ
	unsigned long  BaseAdd=0; //�ֿ����ַ	
	switch(size)
		{  // n������h���ָ߶ȣ�w���ֿ��ȣ� d���ּ�࣬c��ҳ��С
		case 7:   BaseAdd=0x1DDF80;			  break;  // 5*7 
		case 8:   BaseAdd=0x1DE280;			  break;  // 7*8 
		case 12:  BaseAdd=0x1DBE00;			  break;  // 6*12  
		case 16:  BaseAdd=0x1DD780;   	 	break;   // 8*16  
	  case 24:  BaseAdd=0x1DFF00;	csize = 48;break;   // 12*24,size=24ʱ���������ֽڵĹ�ʽ������
	  case 32:  BaseAdd=0x1E5A50;    		break;   // 16*32  
 		}
		
	qh=*code;
	if((qh>0x00))
	{
		if((qh >= 0x20) &&(qh <= 0x7E))
		{				
			FontAddr = 	qh-0x20;
			FontAddr = (unsigned long)((FontAddr*csize)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			ZK_Read_1_n(AddrHigh,AddrMid,AddrLow,mat,csize);/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
		}	
	} 		  
}  
void Show_Ascchar(u16 y,u16 x,u8 Ascchar,u8 size,u8 mode)
{
	u8 i,temp,t,t1;
	u16 y0=y;
	static u8 dzk[128];   
	static u8 csize;
	if(size == 24){
		csize = 48;
	}else{
		csize=(size/8+((size%8)?1:0))*(size/2);//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	}
	for(i=0;i<128;i++ ){
      dzk[i]=0;
	}
	if(size!=12&&size!=16&&size!=24&&size!=32)return;	//��֧�ֵ�size
	Get_AscMat(&Ascchar,dzk,size);	//�õ���Ӧ��С�ĵ������� 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//�õ���������                          
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if((mode==0)||(mode==2))LCD_Fast_DrawPoint(x,y,BACK_COLOR); 
			temp<<=1;
			y++;
			if((y-y0)==size/2)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}  
}

//***************************************************************
//  ��ʾ ASCII 2015-11���²���ͨ��
//zk_num���壺���ֿ�ı�ʾ�� 1:5x7 ASCII,2:7x8 ASCII, 3:6x12 ASCII, 4:8x16 ASCII,	5: 12x24 ASCII,6:16x32 ASCII;
//   x: Start Column  ��ʼ��  
//   y: Start page   ��ʼҳ 0~7 
//*************************************************************** 
void Display_Asc( unsigned char zk_num,unsigned int x, unsigned int y )
{
//    
//    unsigned char i ,j ;
// 
//		
//	switch(zk_num)
//	{
//	    case '1':   
//				 {	 Set_Pos(x,y);
//					 for(i=0;i<5;i++)
//					 Write_Data(Font_Map[i]);
//		   			 }
//				 break;   //	  5x7 ASCII

//	 	case '2':   	
//				 { 	 Set_Pos(x,y);
//					 for(i=0;i<7;i++)
//					 Write_Data(Font_Map[i]);
//				    }  
//				break;   //	  7x8 ASCII

//	    case '3':   
//				{ 	 Set_Pos(x,y);
//					 for(i=0;i<6;i++)
//				     Write_Data(Font_Map[i]);
//				  	
//					 Set_Pos(x,y+1);
//					 for(i=6;i<12;i++)
//				     Write_Data(Font_Map[i]);
//			 	 	  }  
//				break;   //  6x12 ASCII

//	    case '4':  
//				{
//			 	 Set_Pos(x,y);
//				 for(i=0;i<8;i++)
//			     Write_Data(Font_Map[i]);
//			  
//			 	 Set_Pos(x,y+1);
//				 for(i=8;i<16;i++)
//			     Write_Data(Font_Map[i]);
//		 	 	  }   break;   //  8x16 ASCII

//	    case '5':  
//		    	 {
//			 	 Set_Pos(x,y);
//				 for(i=0;i<8;i++)
//			     Write_Data(Font_Map[i]);
//			  
//			 	 Set_Pos(x+8,y);
//				 for(i=8;i<12;i++)
//			     Write_Data(Font_Map[i]);
//		
//				 Set_Pos(x,y+1);
//				 for(i=12;i<20;i++)
//			     Write_Data(Font_Map[i]);
//			  
//			 	 Set_Pos(x+8,y+1);
//				 for(i=20;i<24;i++)
//			     Write_Data(Font_Map[i]);

//				 Set_Pos(x,y+2);
//				 for(i=24;i<32;i++)
//			     Write_Data(Font_Map[i]);
//			  
//			 	 Set_Pos(x+8,y+2);
//				 for(i=32;i<36;i++)
//			     Write_Data(Font_Map[i]);
//				 
// 	 	  }     break;   //  12x24 ASCII

//	    case '6':  	 
//				{
//				 for(j=0;j<4;j++)
//				 {
//				 Set_Pos(x,y+j);
//				 for(i=0+j*16;i<8+j*16;i++)
//			     Write_Data(Font_Map[i]);
//			  
//			 	 Set_Pos(x+8,y+j);
//				 for(i=8+j*16;i<16+j*16;i++)
//			     Write_Data(Font_Map[i]); 
//			      }
//		
//			 }    break;   //  16x32 ASCII


//			}
// 

}


//--------------------------------------------------------------------------------
// void lcdram_map( BYTE *getdate,BYTE *putdata,BYTE high,BYTE width,BYTE style )
// ����ת������  
//putdata ԭ���ֵ� (���ú���)
//getdate Ŀ�꺺�ֵ㣬high ԭ���ָ߶ȣ�width ԭ���ֿ��ȣ�
//------------------------------------------------------------------------------
void zk_map(  unsigned char *getdate, unsigned char *putdata, unsigned char high, unsigned char width)	 //byte
{
	unsigned short i,j,wbyte;			   //word
	unsigned char i_8;
	wbyte = (width+7)/8;
//	hbyte = (high+7)/8;

		for( i = 0; i < high; i++ )
		{
			for( j = 0; j < width; j++ )
			{
				i_8 = i/8;
				if((*(putdata+wbyte*i+j/8)&(0x80>>(j%8))) > 0)
					getdate[i_8*width+j] |= (0x01<<(i%8));
				else
					getdate[i_8*width+j] &= (~(0x01<<(i%8)));

			}
	
			 }
	}






















		  





