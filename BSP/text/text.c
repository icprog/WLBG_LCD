#include "lcd.h"
#include "text.h"	
#include "string.h"												    											    
 

unsigned char    FontBuf[128];//�ֿ⻺��
unsigned char    Font_Map[128];//�ֿ⻺��

					    
void ROM_GT30L_Init(void)
{ 	
 
 	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��A�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //��ʼ��
 	GPIO_SetBits(GPIOA,GPIO_Pin_1);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��A�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
 	GPIO_SetBits(GPIOC,GPIO_Pin_2|GPIO_Pin_3);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
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
		if( READ_ROM_OUT())
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
	unsigned char  h,w;// ��ͬ�����ֿ�ļ������
//	CL_Mem();
	switch(size)
		{  // n������h���ָ߶ȣ�w���ֿ�ȣ� d���ּ�࣬c��ҳ��С
		case 12:  BaseAdd=0x0;			h=12; w=12;  break;  // 12*12  
		case 16:  BaseAdd=0x2C9D0;  h=16; w=16;  break;   // 15*16  
	  case 24:  BaseAdd=0x68190;  h=24; w=24;  break;   // 24*24  
	  case 32:  BaseAdd=0xEDF00;  h=32; w=32;  break;   // 32*32  
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
//			zk_map(mat,FontBuf,h,w);//�ַ�ת��
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
//			zk_map(mat,FontBuf,h,w);//�ַ�ת��
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
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR); 
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
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode)
{					
	u16 x0=x;
	u16 y0=y;							  	  
    u8 bHz=0;     //�ַ���������  	    				    				  	  
    while(*str!=0)//����δ����
    { 
        if(!bHz)
        {
	        if(*str>0x80){
						bHz=1;//���� 
					}else{              //�ַ�     
              if(y>(y0+width-size/2)){				   
								x+=size;
								y=y0;	   
							}							    
		        if(x>(x0+height-size))break;//Խ�緵��      
		        if(*str==13){//���з���     
		            x+=size;
								y=y0;
		            str++; 
		        }  
		        else LCD_ShowChar(x,y,*str,size,mode);//��Ч����д�� 
						str++; 
		        y+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
	        }
        }else//���� 
        {     
            bHz=0;//�к��ֿ�    
            if(y>(y0+width-size)){	    
							x+=size;
							y=y0;		  
						}
	        if(x>(x0+height-size))break;//Խ�緵��  						     
	        Show_Font(x,y,str,size,mode); //��ʾ�������,������ʾ 
	        str+=2; 
	        y+=size;//��һ������ƫ��	    
        }						 
    }   
}  			 		 
//��ָ����ȵ��м���ʾ�ַ���
//����ַ����ȳ�����len,����Show_Str��ʾ
//len:ָ��Ҫ��ʾ�Ŀ��			  
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len)
{
	u16 strlenth=0;
   	strlenth=strlen((const char*)str);
	strlenth*=size/2;
	if(strlenth>len)Show_Str(x,y,lcddev.width,lcddev.height,str,size,1);
	else
	{
		strlenth=(len-strlenth)/2;
	    Show_Str(strlenth+x,y,lcddev.width,lcddev.height,str,size,1);
	}
}   





  

 
 //*************************************************************************
 //****	Display_GB2312_String���������ַ�����ʾ����	***//
 //zk_num���壺���ֿ�ı�ʾ�� 1:GB2312_12*12,2:GB2312_15*16, 3:GB2312_24*24, 4:GB2312_32*32,
 //x�������� y��������
 //text[]��Ҫ��ʾ�ĺ���
 //**************************************************************************
void Display_GB2312_String(unsigned char zk_num,unsigned int  x,unsigned int  y, unsigned char  text[])
{
   
	unsigned char  i= 0;
	unsigned char  AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ
	
	unsigned long  FontAddr=0; //�ֵ�ַ
	unsigned long  BaseAdd=0; //�ֿ����ַ	
	unsigned char  n,h,w,d,p;// ��ͬ�����ֿ�ļ������
 	CL_Mem(); //�建�����飬����ᵼ��12*12��ʾ�쳣
	switch(zk_num)
		{  // n������h���ָ߶ȣ�w���ֿ�ȣ� d���ּ�࣬c��ҳ��С
		case '1':  BaseAdd=0x0;     n=24;  h=12; w=12; d=12; p=2;  break;  // 12*12  
		case '2':  BaseAdd=0x2C9D0; n=32;  h=16; w=16; d=16; p=2;  break;   // 15*16  
	    case '3':  BaseAdd=0x68190; n=72;  h=24; w=24; d=24; p=3;  break;   // 24*24  
	    case '4':  BaseAdd=0xEDF00; n=128; h=32; w=32; d=32; p=4;  break;   // 32*32  
 		}
		
	while((text[i]>0x00))
	{
	    if(x>(128-d))
	      {  y=y+d; 
		     x=0; 
			 	 }//�������

		if(((text[i]>=0xA1) &&(text[i]<=0xA9))&&(text[i+1]>=0xA1))
		{						
			/*������壨GB2312�������� �ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1))*n+ BaseAdd; ������ȡ��ַ*/
		 	FontAddr = (text[i]- 0xA1)*94; 
			FontAddr += (text[i+1]-0xA1);
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			ZK_Read_1_n(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
				zk_map(Font_Map,FontBuf,h,w);//�ַ�ת��
			Display_GB2312(zk_num,x,y);//��ʾһ�����ֵ�OLED��/ 
					
			x=x+d; //��һ��������
			i+=2;  //�¸��ַ�
		}
		else if(((text[i]>=0xB0) &&(text[i]<=0xF7))&&(text[i+1]>=0xA1))
		{						
			/*������壨GB2312�� �ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+846)*n+ BaseAdd; ������ȡ��ַ*/
			 
			FontAddr = (text[i]- 0xB0)*94; 
			FontAddr += (text[i+1]-0xA1)+846;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			ZK_Read_1_n(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[ ]"*/
				zk_map(Font_Map,FontBuf,h,w);//�ַ�ת��
			Display_GB2312(zk_num,x,y);//��ʾһ�����ֵ�OLED��/
	 		x=x+d; //��һ��������
			i+=2;  //�¸��ַ�

		}
		
	}
	
}

//***************************************************************
//  ��ʾ12*12 15*16 24*24 32*32 GB2312������ 2015-11���²���ͨ��
//   zk_num������ ���е�����  GB_1212 1616 2424 3232 
//   x: ��ʼ�� 
//   y: ��ʼҳ 0~7
//*************************************************************** 
void Display_GB2312( unsigned char zk_num,unsigned int x, unsigned int y )
{
//	unsigned char i,j;
 
		
	switch(zk_num)
	{
		case '1':  

			break;  // 12*12  

		case '2': 

	 		 break;     // 15*16 

	    case '3':  

		    break;      // 24*24  

	    case '4':  
	
			  break;     // 32*32  
			}
}





  //*************************************************************************
 //****	Display_GBasc_String����������չ�ַ��ַ�����ʾ����	***//
 //zk_num���壺 �ֿ�ı�ʾ�� 1: 6X12 �������չ�ַ�,2:8X16 �������չ�ַ�,3:8X16 �������չ�����ַ�, 4:12X24 �������չ�ַ�, 5:16X32 �������չ�ַ�
 //x�������� y��������
 //text[]��Ҫ��ʾ���ַ�
 //**************************************************************************
void Display_GBasc_String(unsigned char zk_num,unsigned int x, unsigned int y, unsigned char  text[])
{
   
	unsigned char  i= 0;
	unsigned short  FontCode  ; // �ַ�����
	unsigned char  AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ

	unsigned long  FontAddr=0; //�ֵ�ַ
	unsigned long  BaseAdd=0; //�ֿ����ַ	
	unsigned char  n= 0, d=0;// ��ͬ�����ֿ�ļ������
	
	switch(zk_num)
	{
		case '1':  BaseAdd=0x1DBE0C; n=12 ;d=8;  break;	 //	   6X12 �������չ�ַ�
		case '2':  BaseAdd=0x1DD790; n=16;d=8;  break;	 //8X16 �������չ�ַ�
		case '3':  BaseAdd=0x1F2880; n=16;d=8;  break;	 //8X16 �������չ�����ַ�
	    case '4':  BaseAdd=0x1DFF30; n=48; d=12; break;	 //12X24 �������չ�ַ�
	    case '5':  BaseAdd=0x1E5A90; n=64;d=16; break;	 // 16X32 �������չ�ַ�
			}
		
	while((text[i]>0x00))
	{	
	     if(x>(256-d))
	      {  y=y+d; 
		     x=0; 
			 	 }//�������

 	     FontCode =	0xAA00|(text[i]|0x80);	//ascii���λΪ1
		if((FontCode>=0xAAA1) &&(FontCode<=0xAAFE))
		{						
		    FontAddr = 	FontCode-0xAAA1;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			ZK_Read_1_n(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
			Display_GBasc(zk_num,x,y);/*��ʾһ���ַ���OLED�� */
			i+=1;
			x+=d;//��һ��������
		 
		}
		else
		
		  if((FontCode>=0xABA1) &&(FontCode<=0xABC0))	 
		{						
		    FontAddr = 	(FontCode-0xABA1)+95;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			ZK_Read_1_n(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
			Display_GBasc(zk_num,x,y);/*��ʾһ���ַ���OLED�� */
			i+=1;
			x+=d;//��һ��������
		 
		}
		 else
			
			if((FontCode>=0xACA1) &&(FontCode<=0xACDF))		// 8X16 �������չ�����ַ�
		{						
		    FontAddr = 	FontCode-0xACA1;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			AddrHigh = (FontAddr&0xff0000)>>16;  //��ַ�ĸ�8λ,��24λ/
			AddrMid = (FontAddr&0xff00)>>8;      //��ַ����8λ,��24λ/
			AddrLow = FontAddr&0xff;	     //��ַ�ĵ�8λ,��24λ/
			ZK_Read_1_n(AddrHigh,AddrMid,AddrLow,FontBuf,n );//ȡһ�����ֵ����ݣ��浽"FontBuf[]"/
			Display_GBasc(zk_num,x,y);//��ʾһ���ַ���OLED�� /
			i+=1;
			x+=d;//��һ��������
		 
		}


	}
	
}
 
//***************************************************************
//  ��ʾ6*12 8*16 12*24 16*32 GB2312����ASCII 2015-11���²���ͨ��
//zk_num���壺 �ֿ�ı�ʾ�� 1: 6X12 �������չ�ַ�,2:8X16 �������չ�ַ�,3:8X16 �������չ�����ַ�, 4:12X24 �������չ�ַ�, 5:16X32 �������չ�ַ�
//   x: Start Column  ��ʼ�� ��Χ 0~��256-16��
//   y: Start Row   ��ʼ�� 0~63 
//*************************************************************** 
void Display_GBasc( unsigned char zk_num,unsigned int x, unsigned int y )
{
    //extern unsigned char  FontBuf[128];//�ֿ⻺��	
//	unsigned char j ,x0,y0,n;
//	unsigned int  x1=x/4; 
		
//	switch(zk_num)
//	{
//		case '1':  x0=1;y0=11; n=12;  break;
//		case '2':  x0=1;y0=15; n=16;  break;
//		case '3':  x0=1;y0=15; n=16;  break;
//	    case '4':  x0=3;y0=23; n=48;  break;
//	    case '5':  x0=3;y0=31; n=64;  break;
//			}
//		
//	Set_Column_Address(Shift+x1,Shift+x1+x0); // ���������꣬shiftΪ��ƫ������1322������ 
//	Set_Row_Address(y,y+y0); //����������
//	Set_Write_RAM();	 //	д�Դ�
//	 
//	for(j=0;j<n;j++)
//	{
//		 con_4_byte(FontBuf[j]);//����ת������������������SSD1322
//	}

}
 







 //*************************************************************************
 //****	Display_Asc_String  ASCII�ַ�����ʾ����	***//
 //zk_num���壺���ֿ�ı�ʾ�� 1:5x7 ASCII,2:7x8 ASCII, 3:6x12 ASCII, 4:8x16 ASCII,	5: 12x24 ASCII,6:16x32 ASCII;
 //x�������� y��������
 //text[]��Ҫ��ʾ��ASCII
 //**************************************************************************
void Display_Asc_String(unsigned char zk_num,unsigned int x, unsigned int y, unsigned char  text[])
{
   
	unsigned char  i= 0;
	unsigned char  AddrHigh,AddrMid,AddrLow ; //�ָߡ��С��͵�ַ

	unsigned long  FontAddr=0; //�ֵ�ַ
	unsigned long  BaseAdd=0; //�ֿ����ַ	
      unsigned char  n,h,w,d,c;// ��ͬ�����ֿ�ļ������
	CL_Mem(); //�建������ ,����ᵼ��6*12��ʾ�쳣
	switch(zk_num)
	{	// n������h���ָ߶ȣ�w���ֿ�ȣ� d���ּ�࣬c��ҳ��С
		case '1':  BaseAdd=0x1DDF80; n=8;  h=7;  w=5;  d=6 ; c=1;  break;	 //	  5x7 ASCII
		case '2':  BaseAdd=0x1DE280; n=8;  h=8;  w=7;  d=8;  c=1;  break;	 //   7x8 ASCII
		case '3':  BaseAdd=0x1DBE00; n=12; h=12; w=6;  d=6;  c=2;  break;	 //  6x12 ASCII
	    case '4':  BaseAdd=0x1DD780; n=16; h=16; w=8;  d=8;  c=2;  break;	 //  8x16 ASCII	
	    case '5':  BaseAdd=0x1DFF00; n=48; h=24; w=12; d=12; c=3;  break;	 //  12x24 ASCII
	 	case '6':  BaseAdd=0x1E5A50; n=64; h=32; w=16; d=16; c=4;  break;	 //  16x32 ASCII

			}
		
	while((text[i]>0x00))
	{	
		 if(x>(128-d))
	      {  y=y+c; 
		     x=0; 
			 	 }//�������


	   		if((text[i] >= 0x20) &&(text[i] <= 0x7E))
		{						
		    FontAddr = 	text[i]-0x20;
			FontAddr = (unsigned long)((FontAddr*n)+BaseAdd);
			
			AddrHigh = (FontAddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
			AddrMid = (FontAddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
			AddrLow = FontAddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
			ZK_Read_1_n(AddrHigh,AddrMid,AddrLow,FontBuf,n );/*ȡһ�����ֵ����ݣ��浽"FontBuf[]"*/
		 zk_map(Font_Map,FontBuf, h, w);	//���ݱ任
			Display_Asc(zk_num,x,y);/*��ʾһ��ascii��OLED�� */
            i+=1;
			x+=d;//��һ��������
		 
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
//getdate Ŀ�꺺�ֵ㣬high ԭ���ָ߶ȣ�width ԭ���ֿ�ȣ�
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






















		  






