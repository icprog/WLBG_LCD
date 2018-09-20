#include"HeadType.h"
#include "lcd.h"
#include "text.h"	
#include "GUI.h"

COMM_RecTemplate_Union_Type  MCU_Host_RecT;//MCU作为主机时的结构体接收应答变量
COMM_RecTemplate_Union_Type  Template_Save_buf[TEMPLATE_ALLOW_NUM];
COMM_RecControl_Union_Type   MCU_Host_RecC;
COMM_RecData_Union_Type      Default_data[TEMPLATE_ALLOW_NUM];
Communation_Rec_DataType     Display_data;
u8 Ddefault_Data_count;
u8 new_display_flag;
u8 new_display_num;
static u8 slave_rec_state;
u8 slaveaddr = 3;
extern UART_HandleTypeDef huart1;
//=============================================================================
//函数名称:Respond_Host_Comm
//功能概要:响应上位机的发出的数据命令，数据已经从串口一接收完整
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
static void Respond_Host_Comm(void)
{
		u8 i;
		u16 crc;
	  u8 res;
		if(Usart1_Control_Data.rx_count < 8){
				return;
		}
		if((Usart1_Control_Data.rxbuf[2] != slaveaddr)||(Usart1_Control_Data.rxbuf[6] != Usart1_Control_Data.rx_count - 11)){
				return ;
		}
		crc=CRC_GetCCITT(Usart1_Control_Data.rxbuf,Usart1_Control_Data.rx_count-4);
		if((Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-3]+\
			Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-4]*256 == crc)){
			if(((Usart1_Control_Data.rxbuf[3] == 'T')||((Usart1_Control_Data.rxbuf[3] == 'E')))&&(Usart1_Control_Data.rxbuf[4]< TEMPLATE_ALLOW_NUM)){
					for(i = 0;i < Usart1_Control_Data.rx_count;i++){
// 							MCU_Host_RecT.rectemplate_buf[i] = Usart1_Control_Data.rxbuf[i];
						  Template_Save_buf[Usart1_Control_Data.rxbuf[4]].rectemplate_buf[i] = Usart1_Control_Data.rxbuf[i];
					}//把数据复制给主机通讯结构体,数据正确，先回应主机，记录刷写OLED状态位	
					AT24CXX_Write(Usart1_Control_Data.rxbuf[4]*TEMPLATE_SECTION_SIZE + TEMPLATE_SAVE_ADDR,\
					(unsigned char *)Template_Save_buf[Usart1_Control_Data.rxbuf[4]].rectemplate_buf,TEMPLATE_SIZE);
					if(Usart1_Control_Data.rxbuf[3] == 'E'){
						AT24CXX_WriteOneByte(TEMPLATE_COUNT_ADDR,Template_Save_buf[Usart1_Control_Data.rxbuf[4]].rectemplate.template_no);
						AT24CXX_WriteOneByte(TEMPLATE_COUNT_ADDR + 1,Template_Save_buf[Usart1_Control_Data.rxbuf[4]].rectemplate.template_no);
					}
			}else if(((Usart1_Control_Data.rxbuf[3] == 'M')||((Usart1_Control_Data.rxbuf[3] == 'Q')))&&(Usart1_Control_Data.rxbuf[4]< TEMPLATE_ALLOW_NUM)){
					AT24CXX_Write(Usart1_Control_Data.rxbuf[4]*(DATADISPLAY_SIZE+11) + DEFAULT_DATA_SAVE_ADDR,\
					(unsigned char *)Usart1_Control_Data.rxbuf,Usart1_Control_Data.rx_count);
					if(Usart1_Control_Data.rxbuf[3] == 'Q'){
						AT24CXX_WriteOneByte(DEFAULT_DATA_COUNT_ADDR,Usart1_Control_Data.rxbuf[4]);
						AT24CXX_WriteOneByte(DEFAULT_DATA_COUNT_ADDR + 1,Usart1_Control_Data.rxbuf[4]);
					}
					for(i = 0;i < Usart1_Control_Data.rxbuf[6];i++){
						  Display_data.dis_buf[i] = Usart1_Control_Data.rxbuf[i + 7];
					}//把数据复制给主机通讯结构体,数据正确，先回应主机，记录刷写OLED状态位	
					new_display_num = Usart1_Control_Data.rxbuf[4];
					new_display_flag = 1;
			}else if((Usart1_Control_Data.rxbuf[3] == 'D')&&(Usart1_Control_Data.rxbuf[4] < DATADISPLAY_SIZE)){
					for(i = 0;i < Usart1_Control_Data.rxbuf[6];i++){
						  Display_data.dis_buf[i] = Usart1_Control_Data.rxbuf[i + 7];
					}//把数据复制给主机通讯结构体,数据正确，先回应主机，记录刷写OLED状态位	
					new_display_num = Usart1_Control_Data.rxbuf[4];
					new_display_flag = 1;
			}else if((Usart1_Control_Data.rxbuf[3] == 'C')){
					for(i = 0;i < Usart1_Control_Data.rx_count;i++){
							MCU_Host_RecC.rectemplate_buf[i] = Usart1_Control_Data.rxbuf[i];
					}
			}
			if(AdrrOK_Flag == 1){//开机地址检查正确和修改地址保存正确后点亮屏幕提示设备OK，当第一次通讯正确后熄灭屏幕显示上位机信息
// 				LCD_Clear(BLACK);
				AdrrOK_Flag = 0;
			}
			slave_rec_state = 1;	//从机接收数据正确
			res = Execute_Host_Comm();  //执行完动作再回复PC，这样比较慢，但是可以给PC正确状态的答复		
			RE485_SEND;
			Usart1_Control_Data.tx_count = 0;	
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x01;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x58;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_RecT.rectemplate.addr;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_RecT.rectemplate.funcode;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x00;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x02;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = res;	//数据接收正确
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x00;
			crc=CRC_GetCCITT(Usart1_Control_Data.txbuf,Usart1_Control_Data.tx_count);
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = (crc>>8)&0xFF; 
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = crc&0xFF;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X0D;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X0A;
		}else{	//CRC错误
			RE485_SEND;
			Usart1_Control_Data.tx_count = 0;	
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x01;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x58;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_RecT.rectemplate.addr;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = MCU_Host_RecT.rectemplate.funcode;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x00;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x02;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x00;	//数据接收错误
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x00;
			crc=CRC_GetCCITT(Usart1_Control_Data.txbuf,Usart1_Control_Data.tx_count);
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = (crc>>8)&0xFF; 
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = crc&0xFF;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X0D;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X0A;
		}
		Usart1_Control_Data.tx_index = 0;	
// 		USART_SendData(USART1,Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_index++]);
		HAL_UART_Transmit_IT(&huart1, Usart1_Control_Data.txbuf,Usart1_Control_Data.tx_count);
		Usart1_Control_Data.rx_aframe = 0;	//清空和主机的通讯，避免通讯错误
		Usart1_Control_Data.rx_count = 0;	
}

//x是横坐标，共128个点，可写16个字符，y是纵坐标，共64个点，可写4行字符
static  u8 func01(u8 x,u8 y,u8 length,u8 *p)
{//地址超过一行字符，只显示本行字符能显示的部分，超出部分忽略
	u8 i,tempx,tempy,templength;
	u8 res;
	x=x*8;	//处理行，文字只能显示4行
	y=y*2; //处理列，文字只能显示8列
	tempy = y;
	templength = 0;
	if(x+length*8 > 128)
	{
		tempx =length*8 + x -128 ;
		templength = tempx/8;
		if(tempx%8 != 0){
			templength++;
		}
		tempy += 2;
		res = 1;
		return res; //超出地址范围不显示
	}
	for(i=0;i<length - templength;i++)
	{
		if(*p == 0x00){

		}else{
// 			LCD_P8x16Showchar(x+i*8,y,*p);
			Show_Str(x+i*8,y,8,p,BACK_COLOR,RED,16,1);
		}
		p++;
	}
	res = 0;
	return res;
}


static  u8 func02(u8 x,u8 y,u8 length,u8 *text)
{
	uchar i,tempx,tempy,templength;
	u8 res;
	x=x*16;	//处理行，文字只能显示4行
	y=y*2; //处理列，文字只能显示8列
	tempy = y;
	templength = 0;
	if(x+length*8 > 128)
	{
		tempx =length*8 + x -128 ;
		templength = tempx/8;
		if(tempx%8 != 0){
			templength++;
		}
		if(templength %2 != 0){
			templength++;	//文字是16个字符，减去的长度要是2的倍数
		}
		tempy += 2;
		res = 1;
		return res; //超出地址范围不显示
	}
	for(i=0;i<(length - templength)/2;i++){
			/*国标简体（GB2312）汉字在晶联讯字库IC中的地址由以下公式来计算：*/
		/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
		/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
		if((text[i*2] == 0x00)&&(text[i*2+1] == 0x00)){

		}else{
// 			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );/*取32个字节的数据，存到"fontbuf[32]"*/
// 			display_graphic_16x16(y,x,fontbuf);/*显示汉字到LCD上，y为页地址，x为列地址，fontbuf[]为数据*/
	   Show_Str(2,2,32*4,text,BACK_COLOR,POINT_COLOR,32,1);
		}
		x+=16;
	}
	res = 0;
	return res;
}
//主机发送数据必须使用A3+"字符"处理模式
static  u8 func03(u8 x,u8 y,u8 length,u8 *text)
{
		uchar i,tempx,tempy,templength;
		uchar addrHigh,addrMid,addrLow ;
		uchar fontbuf[32];
	  u8 texttemp[16];
		ulong  fontaddr=0;
		u8 res;
		x=x*16;	//处理行，文字只能显示4行
	  y=y*2; //处理列，文字只能显示8列
		tempy = y;
		templength = 0;
		length = length*2;
		if(x+length*8 > 128)
		{
			tempx =length*8 + x -128 ;
			templength = tempx/8;
			if(tempx%8 != 0){
				templength++;
			}
			if(templength %2 != 0){
				templength++;	//文字是16个字符，减去的长度要是2的倍数
			}
			tempy += 2;
		res = 1;
		return res; //超出地址范围不显示
		}
		for(i=0;i<length;i++)
		{
			  if(text[i] == 0x00){
					texttemp[i*2] = 0x00;
					texttemp[i*2+1] = 0x00;
				}else{
					texttemp[i*2] = 0xA3;
					texttemp[i*2+1] = text[i]- 0x20 + 0xA0;
				}
		}
		for(i=0;i<(length - templength);i=i+2){
			/*国标简体（GB2312）15x16点的字符在晶联讯字库IC中的地址由以下公式来计算：*/
			/*Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
//			text[i] = 0XA3;
//			text[i+1] = text[i+1] - 0x20 + 0xA0;
			if((texttemp[i] == 0x00)&&(texttemp[i+1] == 0x00)){
			
			}else{
				fontaddr = (texttemp[i]- 0xa1)*94; 
				fontaddr += (texttemp[i+1]-0xa1);
				fontaddr = (ulong)(fontaddr*32);
				
				addrHigh = (fontaddr&0xff0000)>>16;  /*地址的高8位,共24位*/
				addrMid = (fontaddr&0xff00)>>8;      /*地址的中8位,共24位*/
				addrLow = fontaddr&0xff;	     /*地址的低8位,共24位*/
// 				get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );/*取32个字节的数据，存到"fontbuf[32]"*/
// 				display_graphic_16x16(y,x,fontbuf);/*显示汉字到LCD上，y为页地址，x为列地址，fontbuf[]为数据*/
			}
			x+=16;
		}
	res = 0;
	return res;
}
//=============================================================================
//函数名称:Execute_Host_Comm
//功能概要:执行上位机发出的命令
//参数说明:无
//函数返回:无
//注意    :无
//=============================================================================
u8  Execute_Host_Comm(void)
{
  u8 res;
	u8 word_size,display_size,display_mode;
	u16 start_x,start_y,b_color,f_color;
	if(slave_rec_state == 1){//执行主机发送的命令
// 		switch(MCU_Host_RecT.rectemplate.funcode){
		switch(Usart1_Control_Data.rxbuf[3]){
			case 'T':
			case 'E':
			case 'M':
			case 'Q':
				res = 1;
			break;
			case 'D':
				if(new_display_flag == 1){
					start_x = Template_Save_buf[new_display_num].rectemplate.x_starH * 256 + Template_Save_buf[new_display_num].rectemplate.x_starL;
					start_y = Template_Save_buf[new_display_num].rectemplate.y_starH * 256 + Template_Save_buf[new_display_num].rectemplate.y_starL;
					b_color = Template_Save_buf[new_display_num].rectemplate.backcolorH * 256 + Template_Save_buf[new_display_num].rectemplate.backcolorL;
					f_color = Template_Save_buf[new_display_num].rectemplate.fontcolorH * 256 + Template_Save_buf[new_display_num].rectemplate.fontcolorL;
					word_size = Template_Save_buf[new_display_num].rectemplate.word_size;
					display_size = Template_Save_buf[new_display_num].rectemplate.display_size; //显示字节个数，一个中文占两个字节
// 					display_mode = Template_Save_buf[new_display_num].rectemplate.display_mode;
					display_mode = Display_data.dis_buf[0];
					Show_Str(start_x,start_y,word_size/2 * display_size,"                     ",b_color,f_color,word_size,0);
					Show_Str(start_x,start_y,word_size/2 * display_size,(unsigned char*)&Display_data.dis_buf[1],b_color,f_color,word_size,display_mode);
					new_display_flag = 0;
				}
			break;
			case 'C':
					switch(MCU_Host_RecC.control.funcode){
							case 0x01:
							res = func01(MCU_Host_RecC.control.x,MCU_Host_RecC.control.y,(u8)(MCU_Host_RecC.control.datasizeL-2),&MCU_Host_RecC.control.recbuf[0]);
							break;
							case 0x02:
								res = func02(MCU_Host_RecC.control.x,MCU_Host_RecC.control.y,(u8)(MCU_Host_RecC.control.datasizeL-2),&MCU_Host_RecC.control.recbuf[0]);
							break;
							case 0x03:
								res = func03(MCU_Host_RecC.control.x,MCU_Host_RecC.control.y,(u8)(MCU_Host_RecC.control.datasizeL-2),&MCU_Host_RecC.control.recbuf[0]);
							break;
	// 		 		case 0x04:
			// 		  display_bmp(MCU_Host_Rec.control.x,MCU_Host_Rec.control.y,MCU_Host_Rec.control.recbuf[0],MCU_Host_Rec.control.recbuf[1],bmp2);
						//display_bmp(MCU_Host_Rec.control.x,MCU_Host_Rec.control.y,54,16,bmp3);
			// 		  res = 1;
			// 		break;
						case 0x06:
									LCD_Clear(BLACK);   //clear all dots 
									AdrrOK_Flag = 1; //上位机清屏相当于地址标志位正确，点亮屏幕提示，下次通讯时再显示响应信息
									res = 1;
									break;
						case 0x07:res = 2;break;			
						default: res = 2;break;
					}
					break;
			default: res = 2;break;
		}
		slave_rec_state = 0;
	}
	return res;
}
u8 Load_COMM_Template(void)
{
	u8 res,i,sum_count1,sum_count2;
	u16 crc_cal;
	sum_count1 = AT24CXX_ReadOneByte(TEMPLATE_COUNT_ADDR);
	sum_count2 = AT24CXX_ReadOneByte(TEMPLATE_COUNT_ADDR + 1);
	if((sum_count1 == sum_count2)&&(sum_count1 !=0)&&(sum_count1 < TEMPLATE_ALLOW_NUM)){
		for(i= 0;i<= sum_count1;i++){
			AT24CXX_Read(TEMPLATE_SAVE_ADDR+i*TEMPLATE_SECTION_SIZE,(unsigned char *)Template_Save_buf[i].rectemplate_buf,TEMPLATE_SIZE);
			crc_cal=CRC_GetCCITT(Template_Save_buf[Usart1_Control_Data.rxbuf[i]].rectemplate_buf,TEMPLATE_SIZE-4);	
				if((Template_Save_buf[Usart1_Control_Data.rxbuf[i]].rectemplate_buf[TEMPLATE_SIZE-3]+\
			    Template_Save_buf[Usart1_Control_Data.rxbuf[i]].rectemplate_buf[TEMPLATE_SIZE-4]*256 != crc_cal)){
					res = False;
					return res;
				}
		}
		res = True;
	}else{
		res = False;
	}
	return res;
}
u8 Load_COMM_Default(void)
{
	u8 res,i,sum_count1,sum_count2,def_data_count;
	u16 crc_cal;
	sum_count1 = AT24CXX_ReadOneByte(DEFAULT_DATA_COUNT_ADDR);
	sum_count2 = AT24CXX_ReadOneByte(DEFAULT_DATA_COUNT_ADDR + 1);
	if((sum_count1 == sum_count2)&&(sum_count1 !=0)&&(sum_count1 < TEMPLATE_ALLOW_NUM)){
		Ddefault_Data_count = sum_count1;
		for(i= 0;i<= sum_count1;i++){
			def_data_count = AT24CXX_ReadOneByte(DEFAULT_DATA_SAVE_ADDR + i*(DATADISPLAY_SIZE+11) + 6) + 11;
			AT24CXX_Read(DEFAULT_DATA_SAVE_ADDR+i*(DATADISPLAY_SIZE+11),(unsigned char *)Default_data[i].recdata_buf,def_data_count);
			crc_cal=CRC_GetCCITT(Default_data[i].recdata_buf,def_data_count-4);	
				if((Default_data[i].recdata_buf[def_data_count-3]+\
			    Default_data[i].recdata_buf[def_data_count-4]*256 != crc_cal)){
					res = False;
					return res;
				}
		}
		res = True;
	}else{
		res = False;
	}
	return res;
}

void Display_Default_Data(void)
{
	u8 i,word_size,display_size,display_mode;
	u16 start_x,start_y,b_color,f_color;
	for(i=0;i<=Ddefault_Data_count;i++){
		start_x = Template_Save_buf[i].rectemplate.x_starH * 256 + Template_Save_buf[i].rectemplate.x_starL;
		start_y = Template_Save_buf[i].rectemplate.y_starH * 256 + Template_Save_buf[i].rectemplate.y_starL;
		b_color = Template_Save_buf[i].rectemplate.backcolorH * 256 + Template_Save_buf[i].rectemplate.backcolorL;
		f_color = Template_Save_buf[i].rectemplate.fontcolorH * 256 + Template_Save_buf[i].rectemplate.fontcolorL;
		word_size = Template_Save_buf[i].rectemplate.word_size;
		display_size = Template_Save_buf[i].rectemplate.display_size; //显示字节个数，一个中文占两个字节
// 		display_mode = Template_Save_buf[i].rectemplate.display_mode;
		display_mode = Default_data[i].recdata.dis_buf[0];
		Show_Str(start_x,start_y,word_size/2 * display_size,(unsigned char*)&Default_data[i].recdata.dis_buf[1],b_color,f_color,word_size,display_mode);
	}
}
void Communication_Process(void )
{
		if (1 == Usart1_Control_Data.rx_aframe){ 
			Respond_Host_Comm();
			Usart1_Control_Data.rx_count = 0;
			Usart1_Control_Data.rx_aframe = 0;
}
//		if(Key_ScanNum == 0x01){
//		func02(0,0,16,"韦乐海茨医药设备");
//			Key_ScanNum = 0;
//		}else if(Key_ScanNum == 0x11){
//			func02(0,1,16,"韦乐海茨医药设备");
//			Key_ScanNum = 0;
//		}else if(Key_ScanNum == 0xff){
//			func02(0,2,16,"韦乐海茨医药设备");
//			func02(0,3,16,"韦乐海茨医药设备");
//			Key_ScanNum = 0;
//		}
//		delay_ms(1000);
//		clear_screen();
//		delay_ms(1000);
}












