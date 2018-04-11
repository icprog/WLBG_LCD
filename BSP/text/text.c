#include "lcd.h"
#include "text.h"	
#include "string.h"												    											    
#include "stm32f1xx_hal.h"

unsigned char    FontBuf[128];//字库缓存
unsigned char    Font_Map[128];//字库缓存

					    
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
		ROM_SCK_Clr();  //字库时钟拉低  
		ret_data=ret_data<<1;
		if(GPIO_PIN_SET == READ_ROM_OUT())
			{ret_data=ret_data+1; 
			}
		else
			{ret_data=ret_data+0; 
			}
		
		ROM_SCK_Set();; //字库时钟拉高
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
//从DataAddress这个地址连续读出DataLen个字节的数据到 pBuff的地址
//连续读取
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

 
//code 字符指针开始
//从字库中查找出字模
//code 字符串的开始地址,GBK码
//mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小	
//size:字体大小
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{		    
	unsigned char qh,ql;			  
	u8 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	 
	unsigned char  AddrHigh,AddrMid,AddrLow ; //字高、中、低地址
	unsigned long  FontAddr=0; //字地址
	unsigned long  BaseAdd=0; //字库基地址	
	switch(size)
		{  // n个数，h：字高度，w：字宽度， d：字间距，c：页大小
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
			/*国标简体（GB2312）汉字在 字库IC中的地址由以下公式来计算：*/
			/*Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1))*n+ BaseAdd; 分三部取地址*/
		 	FontAddr = (qh- 0xA1)*94; 
			FontAddr += (ql-0xA1);
			FontAddr = (unsigned long)((FontAddr*csize)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			AddrMid = (FontAddr&0xff00)>>8;      /*地址的中8位,共24位*/
			AddrLow = FontAddr&0xff;	     /*地址的低8位,共24位*/
			ZK_Read_1_n(AddrHigh,AddrMid,AddrLow,mat,csize);/*取一个汉字的数据，存到"FontBuf[]"*/
		}
		else if(((qh>=0xB0) &&(qh<=0xF7))&&(ql>=0xA1))
		{						
			/*国标简体（GB2312） 字库IC中的地址由以下公式来计算：*/
			/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+846)*n+ BaseAdd; 分三部取地址*/
			 
			FontAddr = (qh- 0xB0)*94; 
			FontAddr += (ql-0xA1)+846;
			FontAddr = (unsigned long)((FontAddr*csize)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			AddrMid = (FontAddr&0xff00)>>8;      /*地址的中8位,共24位*/
			AddrLow = FontAddr&0xff;	     /*地址的低8位,共24位*/
			ZK_Read_1_n(AddrHigh,AddrMid,AddrLow,mat,csize);/*取一个汉字的数据，存到"FontBuf[ ]"*/
		}
		
	} 		  
}  

//显示一个指定大小的汉字
//x,y :汉字的坐标
//font:汉字GBK码
//size:字体大小
//mode:0,正常显示,1,叠加显示	   
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode)
{
	u8 i,temp,t,t1;
	u16 y0=y;
	static u8 dzk[128];   
	static u8 csize;
	csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数
	for(i=0;i<128;i++ ){
      dzk[i]=0;
	}
	if(size!=12&&size!=16&&size!=24&&size!=32)return;	//不支持的size
	Get_HzMat(font,dzk,size);	//得到相应大小的点阵数据 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//得到点阵数据                          
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
//在指定位置开始显示一个字符串	    
//支持自动换行
//(x,y):起始坐标
//width,height:区域
//str  :字符串
//size :字体大小
//mode:0,非叠加方式;1,叠加方式    	   		   
void Show_Str(u16 y,u16 x,u16 width,u8*str,u8 size,u8 mode)
{					
	u16 x0=x;
	u16 y0=y;	
	u16 height = size;
    u8 bHz=0;     //字符或者中文  	    				    				  	  
    while(*str!=0)//数据未结束
    { 
        if(!bHz){
	        if(*str>0x80){
						bHz=1;//中文 
					}else{              //字符     
              if(y>(y0+width-size/2)){				   
								x+=size;
								y=y0;	   
							}		
						if((x>(x0+height-size))&&(mode < 2))break;//越界返回,mode>=2时，文字一行显示不完可以换行显示    
		        if(*str==13){//换行符号     
		            x+=size;
								y=y0;
		            str++; 
		        }else {
//							LCD_ShowChar(y,x,*str,size,mode);//有效部分写入 
							Show_Ascchar(y,x,*str,size,mode);
						}
						str++; 
		        y+=size/2; //字符,为全字的一半 
	        }
        }else{//中文  
            bHz=0;//有汉字库    
            if(y>(y0+width-size)){	    
							x+=size;
							y=y0;		  
						}
	        if((x>(x0+height-size))&&(mode<2))break;//越界返回  						     
	        Show_Font(x,y,str,size,mode); //显示这个汉字,空心显示 
	        str+=2; 
	        y+=size;//下一个汉字偏移	    
        }						 
    }   
}  			 		 
//在指定宽度的中间显示字符串
//如果字符长度超过了len,则用Show_Str显示
//len:指定要显示的宽度			  
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

//code 字符指针开始
//从字库中查找出字模
//code 字符串的开始地址,GBK码
//mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小	
//size:字体大小
void Get_AscMat(unsigned char *code,unsigned char *mat,u8 size)
{		    
	unsigned char qh;			  
	u8 csize=(size/8+((size%8)?1:0))*(size/2);//得到字体一个字符对应点阵集所占的字节数	 
	unsigned char  AddrHigh,AddrMid,AddrLow ; //字高、中、低地址
	unsigned long  FontAddr=0; //字地址
	unsigned long  BaseAdd=0; //字库基地址	
	switch(size)
		{  // n个数，h：字高度，w：字宽度， d：字间距，c：页大小
		case 7:   BaseAdd=0x1DDF80;			  break;  // 5*7 
		case 8:   BaseAdd=0x1DE280;			  break;  // 7*8 
		case 12:  BaseAdd=0x1DBE00;			  break;  // 6*12  
		case 16:  BaseAdd=0x1DD780;   	 	break;   // 8*16  
	  case 24:  BaseAdd=0x1DFF00;	csize = 48;break;   // 12*24,size=24时，上面算字节的公式不适用
	  case 32:  BaseAdd=0x1E5A50;    		break;   // 16*32  
 		}
		
	qh=*code;
	if((qh>0x00))
	{
		if((qh >= 0x20) &&(qh <= 0x7E))
		{				
			FontAddr = 	qh-0x20;
			FontAddr = (unsigned long)((FontAddr*csize)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*地址的高8位,共24位*/
			AddrMid = (FontAddr&0xff00)>>8;      /*地址的中8位,共24位*/
			AddrLow = FontAddr&0xff;	     /*地址的低8位,共24位*/
			ZK_Read_1_n(AddrHigh,AddrMid,AddrLow,mat,csize);/*取一个汉字的数据，存到"FontBuf[]"*/
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
		csize=(size/8+((size%8)?1:0))*(size/2);//得到字体一个字符对应点阵集所占的字节数
	}
	for(i=0;i<128;i++ ){
      dzk[i]=0;
	}
	if(size!=12&&size!=16&&size!=24&&size!=32)return;	//不支持的size
	Get_AscMat(&Ascchar,dzk,size);	//得到相应大小的点阵数据 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//得到点阵数据                          
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
//  显示 ASCII 2015-11晶奥测试通过
//zk_num定义：汉字库的标示符 1:5x7 ASCII,2:7x8 ASCII, 3:6x12 ASCII, 4:8x16 ASCII,	5: 12x24 ASCII,6:16x32 ASCII;
//   x: Start Column  开始列  
//   y: Start page   开始页 0~7 
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
// 汉字转换程序  
//putdata 原汉字点 (横置横排)
//getdate 目标汉字点，high 原汉字高度，width 原汉字宽度，
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






















		  






