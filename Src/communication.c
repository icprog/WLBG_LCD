#include"HeadType.h"
#include "lcd.h"
#include "text.h"	
#include "GUI.h"

COMM_RecTemplate_Union_Type  MCU_Host_RecT;//MCU��Ϊ����ʱ�Ľṹ�����Ӧ�����
COMM_RecTemplate_Union_Type  Template_Save_buf[TEMPLATE_ALLOW_NUM];
COMM_RecControl_Union_Type   MCU_Host_RecC;
COMM_RecData_Union_Type      Default_data[TEMPLATE_ALLOW_NUM];
Communation_Rec_DataType     Display_data;
u8 Ddefault_Data_count;
u8 new_display_flag;
u8 multi_display_flag = 0;
u8 Ldata_Follow_flag = 0;
u8 Auto_Switch_flag = 0;
int Atuo_Switch_Time;
u8 new_display_num;
u8 Ldisplay_count;
u8 Current_Display_Page = 0;
u8 Loop_Display_Mode = 0;
static u8 slave_rec_state;
u8 slaveaddr = 0;
extern UART_HandleTypeDef huart1;
void Clear_Eeprom_Template_Data(void);
int Erase_Eeprom_Time = 0;
u8 Ldata_Svae_buf[15][1024];
u8 Ldata_Follow_Led[15];
//=============================================================================
//��������:Respond_Host_Comm
//���ܸ�Ҫ:��Ӧ��λ���ķ�����������������Ѿ��Ӵ���һ��������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
static void Respond_Host_Comm(void)
{
		u16 i;
		u16 crc;
	  u8 res;
		if(Usart1_Control_Data.rx_count < 8){
				return;
		}
		/**�������͵�ַ0��������ΪȺ��ָ���ָ��Ϊģ���Ĭ��ģ������ʱ���գ�Ϊ����ʱ������**/
		if(((Usart1_Control_Data.rxbuf[2] != 0)&&(Usart1_Control_Data.rxbuf[2] != slaveaddr))||((Usart1_Control_Data.rxbuf[6]*256 + Usart1_Control_Data.rxbuf[7]) != Usart1_Control_Data.rx_count - 12)){
				return ;
		}
		if((Usart1_Control_Data.rxbuf[2] == 0)&&(Usart1_Control_Data.rxbuf[3] == 'D')){
			  return ;
		}
		crc=CRC_GetCCITT(Usart1_Control_Data.rxbuf,Usart1_Control_Data.rx_count-4);
		if((Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-3]+\
			Usart1_Control_Data.rxbuf[Usart1_Control_Data.rx_count-4]*256 == crc)){
			if((Usart1_Control_Data.rxbuf[3] == 'T')&&(Usart1_Control_Data.rxbuf[4]< TEMPLATE_ALLOW_NUM)){
				if(Usart1_Control_Data.rxbuf[4] == Usart1_Control_Data.rxbuf[5]){//��ָ��
						Template_bag_compose(Usart1_Control_Data);
				}else{//����ָ��
					for(i = 0;i < Usart1_Control_Data.rx_count;i++){
	// 							MCU_Host_RecT.rectemplate_buf[i] = Usart1_Control_Data.rxbuf[i];
								Template_Save_buf[Usart1_Control_Data.rxbuf[5]].rectemplate_buf[i] = Usart1_Control_Data.rxbuf[i];
						}//�����ݸ��Ƹ�����ͨѶ�ṹ��,������ȷ���Ȼ�Ӧ��������¼ˢдOLED״̬λ	
						AT24CXX_Write(Usart1_Control_Data.rxbuf[5]*TEMPLATE_SECTION_SIZE + TEMPLATE_SAVE_ADDR,\
						(unsigned char *)Template_Save_buf[Usart1_Control_Data.rxbuf[5]].rectemplate_buf,TEMPLATE_SIZE);
						if((Usart1_Control_Data.rxbuf[4]-1) == Usart1_Control_Data.rxbuf[5]){
							AT24CXX_WriteOneByte(TEMPLATE_COUNT_ADDR,Template_Save_buf[Usart1_Control_Data.rxbuf[5]].rectemplate.template_no);
							AT24CXX_WriteOneByte(TEMPLATE_COUNT_ADDR + 1,Template_Save_buf[Usart1_Control_Data.rxbuf[5]].rectemplate.template_no);
						}
					}
			}else if((Usart1_Control_Data.rxbuf[3] == 'M')&&(Usart1_Control_Data.rxbuf[5]< TEMPLATE_ALLOW_NUM)){
					if(Usart1_Control_Data.rxbuf[4] == Usart1_Control_Data.rxbuf[5]){//��ָ��
						Defaultdata_bag_compose(Usart1_Control_Data);
					}else{//����ָ��
						AT24CXX_Write(Usart1_Control_Data.rxbuf[5]*(DATADISPLAY_SIZE+12) + DEFAULT_DATA_SAVE_ADDR,\
						(unsigned char *)Usart1_Control_Data.rxbuf,Usart1_Control_Data.rx_count);
						if((Usart1_Control_Data.rxbuf[4]-1) == Usart1_Control_Data.rxbuf[5]){
							AT24CXX_WriteOneByte(DEFAULT_DATA_COUNT_ADDR,Usart1_Control_Data.rxbuf[5]);
							AT24CXX_WriteOneByte(DEFAULT_DATA_COUNT_ADDR + 1,Usart1_Control_Data.rxbuf[5]);
						}
// 					for(i = 0;i < Usart1_Control_Data.rxbuf[6];i++){
// 						  Display_data.dis_buf[i] = Usart1_Control_Data.rxbuf[i + 7];
// 					}//�����ݸ��Ƹ�����ͨѶ�ṹ��,������ȷ���Ȼ�Ӧ��������¼ˢдOLED״̬λ	
// 					new_display_num = Usart1_Control_Data.rxbuf[5];
				}
			}else if((Usart1_Control_Data.rxbuf[3] == 'D')&&(Usart1_Control_Data.rxbuf[5] < TEMPLATE_ALLOW_NUM)){
					if(Usart1_Control_Data.rxbuf[4] == Usart1_Control_Data.rxbuf[5]){//��ָ��
						Ldisplay_count = Usart1_Control_Data.rxbuf[5];
						new_display_flag = 1;				
					}else{//����ָ��
						for(i = 0;i < Usart1_Control_Data.rxbuf[7];i++){
								Display_data.dis_buf[i] = Usart1_Control_Data.rxbuf[i + 8];
						}//�����ݸ��Ƹ�����ͨѶ�ṹ��,������ȷ���Ȼ�Ӧ��������¼ˢдOLED״̬λ	
						new_display_num = Usart1_Control_Data.rxbuf[5];
						new_display_flag = 1;
					}
				multi_display_flag = 0;
			}else if((Usart1_Control_Data.rxbuf[3] == 'L')){
				Ldisplay_count = Usart1_Control_Data.rxbuf[5];
				if(Usart1_Control_Data.rxbuf[4]){//��Ҫ��������
					for(i=0;i<Usart1_Control_Data.rx_count;i++){
						Ldata_Svae_buf[(Usart1_Control_Data.rxbuf[4]&0x0F)-1][i] = Usart1_Control_Data.rxbuf[i];
					}
					if((Usart1_Control_Data.rxbuf[4]&0x0F)-1 > 0){
						new_display_flag = 0; //����ָ��ڶ�����ֱ����ʾ����Ҫ�ͻ�������ʾ
						if((Usart1_Control_Data.rxbuf[4]&0x0F) ==((Usart1_Control_Data.rxbuf[4]>>4)&0x0F)){
							multi_display_flag = 1; //�����־���ͻ�����֮�����
						}else{
							multi_display_flag = 0;
						}
					}else{
						new_display_flag = 1;
					}
				}else{
					 new_display_flag = 1;
				}
			}else if((Usart1_Control_Data.rxbuf[3] == 'C')){

			}
			if(AdrrOK_Flag == 1){//������ַ�����ȷ���޸ĵ�ַ������ȷ�������Ļ��ʾ�豸OK������һ��ͨѶ��ȷ��Ϩ����Ļ��ʾ��λ����Ϣ
// 				LCD_Clear(BLACK);
				AdrrOK_Flag = 0;
			}
			slave_rec_state = 1;	//�ӻ�����������ȷ
			res = Execute_Host_Comm();  //ִ���궯���ٻظ�PC�������Ƚ��������ǿ��Ը�PC��ȷ״̬�Ĵ�		
			if(Usart1_Control_Data.rxbuf[2] == 0){//Ⱥ��ָ��ظ�
					return;
			}
			RE485_SEND;
			Usart1_Control_Data.tx_count = 0;	
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x01;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x58;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = Usart1_Control_Data.rxbuf[2];
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = Usart1_Control_Data.rxbuf[3];
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x00;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x02;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = res;	//���ݽ�����ȷ
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = Current_Display_Page;
			crc=CRC_GetCCITT(Usart1_Control_Data.txbuf,Usart1_Control_Data.tx_count);
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = (crc>>8)&0xFF; 
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = crc&0xFF;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X0D;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0X0A;
		}else{	//CRC����
			if(Usart1_Control_Data.rxbuf[2] == 0){//Ⱥ��ָ��ظ�
					return;
			}
			RE485_SEND;
			Usart1_Control_Data.tx_count = 0;	
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x01;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x58;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = Usart1_Control_Data.rxbuf[2];
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = Usart1_Control_Data.rxbuf[3];
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x00;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x02;
			Usart1_Control_Data.txbuf[Usart1_Control_Data.tx_count++] = 0x00;	//���ݽ��մ���
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
		Usart1_Control_Data.rx_aframe = 0;	//��պ�������ͨѶ������ͨѶ����
		Usart1_Control_Data.rx_count = 0;	
}

//x�Ǻ����꣬��128���㣬��д16���ַ���y�������꣬��64���㣬��д4���ַ�
static  u8 func01(u8 x,u8 y,u8 length,u8 *p)
{//��ַ����һ���ַ���ֻ��ʾ�����ַ�����ʾ�Ĳ��֣��������ֺ���
	u8 i,tempx,tempy,templength;
	u8 res;
	x=x*8;	//�����У�����ֻ����ʾ4��
	y=y*2; //�����У�����ֻ����ʾ8��
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
		return res; //������ַ��Χ����ʾ
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
	x=x*16;	//�����У�����ֻ����ʾ4��
	y=y*2; //�����У�����ֻ����ʾ8��
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
			templength++;	//������16���ַ�����ȥ�ĳ���Ҫ��2�ı���
		}
		tempy += 2;
		res = 1;
		return res; //������ַ��Χ����ʾ
	}
	for(i=0;i<(length - templength)/2;i++){
			/*������壨GB2312�������ھ���Ѷ�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
		/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
		/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
		if((text[i*2] == 0x00)&&(text[i*2+1] == 0x00)){

		}else{
// 			get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );/*ȡ32���ֽڵ����ݣ��浽"fontbuf[32]"*/
// 			display_graphic_16x16(y,x,fontbuf);/*��ʾ���ֵ�LCD�ϣ�yΪҳ��ַ��xΪ�е�ַ��fontbuf[]Ϊ����*/
	   Show_Str(2,2,32*4,text,BACK_COLOR,POINT_COLOR,32,1);
		}
		x+=16;
	}
	res = 0;
	return res;
}
//�����������ݱ���ʹ��A3+"�ַ�"����ģʽ
static  u8 func03(u8 x,u8 y,u8 length,u8 *text)
{
		uchar i,tempx,tempy,templength;
		uchar addrHigh,addrMid,addrLow ;
		uchar fontbuf[32];
	  u8 texttemp[16];
		ulong  fontaddr=0;
		u8 res;
		x=x*16;	//�����У�����ֻ����ʾ4��
	  y=y*2; //�����У�����ֻ����ʾ8��
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
				templength++;	//������16���ַ�����ȥ�ĳ���Ҫ��2�ı���
			}
			tempy += 2;
		res = 1;
		return res; //������ַ��Χ����ʾ
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
			/*������壨GB2312��15x16����ַ��ھ���Ѷ�ֿ�IC�еĵ�ַ�����¹�ʽ�����㣺*/
			/*Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
			/*���ڵ���8λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ*/
//			text[i] = 0XA3;
//			text[i+1] = text[i+1] - 0x20 + 0xA0;
			if((texttemp[i] == 0x00)&&(texttemp[i+1] == 0x00)){
			
			}else{
				fontaddr = (texttemp[i]- 0xa1)*94; 
				fontaddr += (texttemp[i+1]-0xa1);
				fontaddr = (ulong)(fontaddr*32);
				
				addrHigh = (fontaddr&0xff0000)>>16;  /*��ַ�ĸ�8λ,��24λ*/
				addrMid = (fontaddr&0xff00)>>8;      /*��ַ����8λ,��24λ*/
				addrLow = fontaddr&0xff;	     /*��ַ�ĵ�8λ,��24λ*/
// 				get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );/*ȡ32���ֽڵ����ݣ��浽"fontbuf[32]"*/
// 				display_graphic_16x16(y,x,fontbuf);/*��ʾ���ֵ�LCD�ϣ�yΪҳ��ַ��xΪ�е�ַ��fontbuf[]Ϊ����*/
			}
			x+=16;
		}
	res = 0;
	return res;
}

//=============================================================================
//��������:Execute_Host_Comm
//���ܸ�Ҫ:ִ����λ������������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
u8  Execute_Host_Comm(void)
{
  u8 res,i,j;
	u8 word_size,display_size,display_mode;
	u16 start_x,start_y,b_color,f_color;
	static u16 Ldata_count,Ldata_addr;
	if(slave_rec_state == 1){//ִ���������͵�����
// 		switch(MCU_Host_RecT.rectemplate.funcode){
		switch(Usart1_Control_Data.rxbuf[3]){
			case 'T':
			case 'E':
			case 'M':
			case 'Q':
				res = 1;
			break;
			case 'D':
				if(Usart1_Control_Data.rxbuf[4] == Usart1_Control_Data.rxbuf[5]){//��ָ��
					Ldata_addr = 8;
					if(new_display_flag == 1){
						for(i=0;i<Ldisplay_count;i++){
								new_display_num = Usart1_Control_Data.rxbuf[Ldata_addr];
								start_x = Template_Save_buf[new_display_num].rectemplate.x_starH * 256 + Template_Save_buf[new_display_num].rectemplate.x_starL;
								start_y = Template_Save_buf[new_display_num].rectemplate.y_starH * 256 + Template_Save_buf[new_display_num].rectemplate.y_starL;
								b_color = Template_Save_buf[new_display_num].rectemplate.backcolorH * 256 + Template_Save_buf[new_display_num].rectemplate.backcolorL;
								f_color = Template_Save_buf[new_display_num].rectemplate.fontcolorH * 256 + Template_Save_buf[new_display_num].rectemplate.fontcolorL;
								word_size = Template_Save_buf[new_display_num].rectemplate.word_size;
								display_size = Template_Save_buf[new_display_num].rectemplate.display_size; //��ʾ�ֽڸ�����һ������ռ�����ֽ�
			// 					display_mode = Template_Save_buf[new_display_num].rectemplate.display_mode;
								display_mode = Usart1_Control_Data.rxbuf[Ldata_addr+3];
								Ldata_count = Usart1_Control_Data.rxbuf[2 + Ldata_addr];
								for(j = 0;j < Ldata_count;j++){
									Display_data.dis_buf[j] = Usart1_Control_Data.rxbuf[j + Ldata_addr + 3];
								}
								Ldata_addr = Ldata_addr + Ldata_count +3;
								/**Ϊ�˼���ͨѶ�ֽ�����ÿ��ָ�����������ȡ��ģ�壬�����Ϳ��Բ���Ҫÿ��ָ�������**/
								Show_Str(start_x,start_y,word_size/2 * display_size,(unsigned char*)&Display_data.dis_buf[1],b_color,f_color,word_size,display_mode);
							}
						new_display_flag = 0;
					}					
				}else{
					if(new_display_flag == 1){
						start_x = Template_Save_buf[new_display_num].rectemplate.x_starH * 256 + Template_Save_buf[new_display_num].rectemplate.x_starL;
						start_y = Template_Save_buf[new_display_num].rectemplate.y_starH * 256 + Template_Save_buf[new_display_num].rectemplate.y_starL;
						b_color = Template_Save_buf[new_display_num].rectemplate.backcolorH * 256 + Template_Save_buf[new_display_num].rectemplate.backcolorL;
						f_color = Template_Save_buf[new_display_num].rectemplate.fontcolorH * 256 + Template_Save_buf[new_display_num].rectemplate.fontcolorL;
						word_size = Template_Save_buf[new_display_num].rectemplate.word_size;
						display_size = Template_Save_buf[new_display_num].rectemplate.display_size; //��ʾ�ֽڸ�����һ������ռ�����ֽ�
	// 					display_mode = Template_Save_buf[new_display_num].rectemplate.display_mode;
						display_mode = Display_data.dis_buf[0];
						Show_Str(start_x,start_y,word_size/2 * display_size,(unsigned char*)&Display_data.dis_buf[1],b_color,f_color,word_size,display_mode);
						new_display_flag = 0;
					}
				}
				Current_Display_Page = 0;
				res = 1;
			break;
			case 'L':
				Ldata_addr = 8;
				if(new_display_flag == 1){
					for(i=0;i<Ldisplay_count;i++){
						  new_display_num = Usart1_Control_Data.rxbuf[Ldata_addr];					
							start_x = Template_Save_buf[new_display_num].rectemplate.x_starH * 256 + Template_Save_buf[new_display_num].rectemplate.x_starL;
							start_y = Template_Save_buf[new_display_num].rectemplate.y_starH * 256 + Template_Save_buf[new_display_num].rectemplate.y_starL;
							b_color = Template_Save_buf[new_display_num].rectemplate.backcolorH * 256 + Template_Save_buf[new_display_num].rectemplate.backcolorL;
							f_color = Template_Save_buf[new_display_num].rectemplate.fontcolorH * 256 + Template_Save_buf[new_display_num].rectemplate.fontcolorL;
							word_size = Template_Save_buf[new_display_num].rectemplate.word_size;
							display_size = Template_Save_buf[new_display_num].rectemplate.display_size; //��ʾ�ֽڸ�����һ������ռ�����ֽ�
		// 					display_mode = Template_Save_buf[new_display_num].rectemplate.display_mode;
							display_mode = Display_data.dis_buf[0];
						
						 	Ldata_count = Usart1_Control_Data.rxbuf[2 + Ldata_addr];
							for(j = 0;j < Ldata_count;j++){
								Display_data.dis_buf[j] = Usart1_Control_Data.rxbuf[j + Ldata_addr + 3];
							}	
						  Ldata_addr = Ldata_addr + Ldata_count +3;
						  Show_Str(start_x,start_y,word_size/2 * display_size,(unsigned char*)&Display_data.dis_buf[1],b_color,f_color,word_size,display_mode);
						}
					new_display_flag = 0;
					Current_Display_Page = 1;
				}
				res = 1;
			break;				
			case 'C':
					res = 1;
					switch(Usart1_Control_Data.rxbuf[4]){
							case 0x01:
									res = Template_Check_And_Load();
									LCD_Clear_Delay(BLACK);
									LCD_Clear(LGRAY);
									Display_Default_Data();
									multi_display_flag = 0;
									Current_Display_Page = 0;
							break;
							case 0x02:
								  LCD_Clear(BLACK);
									Clear_Eeprom_Template_Data();
							    LCD_Clear(BLACK);
									Template_Check_And_Load();
									multi_display_flag = 0;
									Current_Display_Page = 0;
							break;
							case 0x03:
									if(Usart1_Control_Data.rxbuf[5]&0x01){				
										RGB_RLED_ON;								
									}else{
										RGB_RLED_OFF;
									}
									if(Usart1_Control_Data.rxbuf[5]&0x02){
										RGB_GLED_ON;
									}else{
										RGB_GLED_OFF;
									}
									if(Usart1_Control_Data.rxbuf[5]&0x04){
										RGB_BLED_ON;
									}else{
										RGB_BLED_OFF;
									}
							break;
							case 0x04:
								  if(Usart1_Control_Data.rxbuf[5] == 0x00){
										Ldata_Follow_flag = 0;
									}else{
										Ldata_Follow_flag = 1;
										for(i=0;i< Usart1_Control_Data.rxbuf[7];i++){
												Ldata_Follow_Led[i] = Usart1_Control_Data.rxbuf[8+i];
										}
										if(Ldata_Follow_Led[0]&0x01){				
											RGB_RLED_ON;								
										}else{
											RGB_RLED_OFF;
										}
										if(Ldata_Follow_Led[0]&0x02){
											RGB_GLED_ON;
										}else{
											RGB_GLED_OFF;
										}
										if(Ldata_Follow_Led[0]&0x04){
											RGB_BLED_ON;
										}else{
											RGB_BLED_OFF;
										}
									}
							break;	
							case 0x05:
								  if(Usart1_Control_Data.rxbuf[5] == 0x00){
										Auto_Switch_flag = 0;
									}else{
										Auto_Switch_flag = 1;
										Atuo_Switch_Time = Usart1_Control_Data.rxbuf[8]*256 + Usart1_Control_Data.rxbuf[9];
									}
							break;
							case 0x06:
								  if(Usart1_Control_Data.rxbuf[5] == 0x00){
										Loop_Display_Mode = 0;
									}else{
										Loop_Display_Mode = 1;
									}
							break;
							case 0x10:
							break;									
							case 0x13:
								res = func03(MCU_Host_RecC.control.x,MCU_Host_RecC.control.y,(u8)(MCU_Host_RecC.control.datasizeL-2),&MCU_Host_RecC.control.recbuf[0]);
							break;
			 		    case 0x14:							
							res = func02(MCU_Host_RecC.control.x,MCU_Host_RecC.control.y,(u8)(MCU_Host_RecC.control.datasizeL-2),&MCU_Host_RecC.control.recbuf[0]);
							res = func01(MCU_Host_RecC.control.x,MCU_Host_RecC.control.y,(u8)(MCU_Host_RecC.control.datasizeL-2),&MCU_Host_RecC.control.recbuf[0]);
			// 		  display_bmp(MCU_Host_Rec.control.x,MCU_Host_Rec.control.y,MCU_Host_Rec.control.recbuf[0],MCU_Host_Rec.control.recbuf[1],bmp2);
						//display_bmp(MCU_Host_Rec.control.x,MCU_Host_Rec.control.y,54,16,bmp3);
			// 		  res = 1;
					    break;
						case 0x15:
									LCD_Clear(BLACK);   //clear all dots 
									AdrrOK_Flag = 1; //��λ�������൱�ڵ�ַ��־λ��ȷ��������Ļ��ʾ���´�ͨѶʱ����ʾ��Ӧ��Ϣ
									res = 1;
									break;
						case 0x16:res = 2;break;			
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
			crc_cal=CRC_GetCCITT(Template_Save_buf[i].rectemplate_buf,TEMPLATE_SIZE-4);	
				if((Template_Save_buf[i].rectemplate_buf[TEMPLATE_SIZE-3]+\
			    Template_Save_buf[i].rectemplate_buf[TEMPLATE_SIZE-4]*256 != crc_cal)){
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
			def_data_count = AT24CXX_ReadOneByte(DEFAULT_DATA_SAVE_ADDR + i*(DEFAULT_DATA_SAVE_SIZE) + 7) + 12;
			AT24CXX_Read(DEFAULT_DATA_SAVE_ADDR+i*(DEFAULT_DATA_SAVE_SIZE),(unsigned char *)Default_data[i].recdata_buf,def_data_count);
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
		display_size = Template_Save_buf[i].rectemplate.display_size; //��ʾ�ֽڸ�����һ������ռ�����ֽ�
// 		display_mode = Template_Save_buf[i].rectemplate.display_mode;
		display_mode = Default_data[i].recdata.dis_buf[0];
		Show_Str(start_x,start_y,word_size/2 * display_size,(unsigned char*)&Default_data[i].recdata.dis_buf[1],b_color,f_color,word_size,display_mode);
	}
}

void Clear_Eeprom_Template_Data(void)
{
	u16 i,j;
	static u8 display_erasetime[4];
	/**���Ĭ��ģ��**/
	Erase_Eeprom_Time = 0;
	Show_Str(32,32*2,8*32,"���ڲ���EEPROM",BACK_COLOR,POINT_COLOR,32,0);
	Show_Str(32,32*3,8*32,"�����ĵȴ�....",BACK_COLOR,POINT_COLOR,32,0);
  for(i=0;i<TEMPLATE_ALLOW_NUM;i++){
		for(j=0;j<TEMPLATE_SECTION_SIZE;j++){
			AT24CXX_WriteOneByte(TEMPLATE_SAVE_ADDR + TEMPLATE_SECTION_SIZE*i + j,0xFF);
		}
	}
	AT24CXX_WriteOneByte(TEMPLATE_COUNT_ADDR,0xFF);
	AT24CXX_WriteOneByte(TEMPLATE_COUNT_ADDR + 1,0xFF);
	/**���Ĭ��ģ������**/
  for(i=0;i<TEMPLATE_ALLOW_NUM;i++){
		for(j=0;j<DEFAULT_DATA_SAVE_SIZE;j++){
			AT24CXX_WriteOneByte(DEFAULT_DATA_SAVE_ADDR + DEFAULT_DATA_SAVE_SIZE*i + j,0xFF);
		}
	}
	AT24CXX_WriteOneByte(DEFAULT_DATA_COUNT_ADDR,0xFF);
	AT24CXX_WriteOneByte(DEFAULT_DATA_COUNT_ADDR + 1,0xFF);
	Erase_Eeprom_Time = Erase_Eeprom_Time/200;
	display_erasetime[0] = Erase_Eeprom_Time%100/10 + 0x30;
	display_erasetime[1] = Erase_Eeprom_Time%10 + 0x30;
	display_erasetime[2] = 'S';
	display_erasetime[3] = '\0';
	Show_Str(32,32*2,8*32,"EEPROM����OK  ",BACK_COLOR,POINT_COLOR,32,0);
	Show_Str(32,32*3,8*32,"��ʱ:         ",BACK_COLOR,POINT_COLOR,32,0);
	Show_Str(32 + 32*3,32*3,3*32,(u8 *)display_erasetime,BACK_COLOR,POINT_COLOR,32,0);
	delay_ms(1500);
}
u8 Template_Check_And_Load(void)
{
	  u8 checkres;
	  checkres = 0;
	
		if(True == Load_COMM_Template()){
// 			Show_Str(32+16,32*2,32*7,"ģ������ȷ����",BACK_COLOR,POINT_COLOR,32,0);
			if(True == Load_COMM_Default()){
// 					Show_Str(16,32*3,32*9,"Ĭ����������ȷ����",BACK_COLOR,POINT_COLOR,32,0);
					checkres = 1;
			}else{
					Show_Str(16,32*3,32*9,"��Ĭ������,������",BACK_COLOR,POINT_COLOR,32,0);
					delay_ms(1500);
			}
// 			LCD_Clear(LGRAY);
		}else{
			Show_Str(32,32*1,8*32,"�豸û�п���ģ��",BACK_COLOR,POINT_COLOR,32,0);
			Show_Str(16,32*2,9*32,"��������ģ���ʹ��",BACK_COLOR,POINT_COLOR,32,0);
			Show_Str(16,32*3,9*32,"������ģ����Ϊ64",BACK_COLOR,POINT_COLOR,32,0);
			Show_Str(32,32*4,8*32,"0<=ģ����<=63",BACK_COLOR,POINT_COLOR,32,0);
			delay_ms(1500);
			delay_ms(1500);
// 			LCD_Clear(BLACK);
		}
		
	return checkres;
}

void Template_bag_compose(Usart_Type usart)
{
	  u16 i,j,Taddr,crc16;
	  Usart_Type* pusart;
	  u8 tcount,crccount,templatesum = 0;
	  static COMM_RecTemplate_Union_Type templatedata;
		static u8 temp;
	  pusart = &usart;
		Taddr = 8;
	  if(pusart->rxbuf[4] != pusart->rxbuf[5]){
			return;
		}
		for(i = 0;i < 5;i++){
			templatedata.rectemplate_buf[i] = pusart->rxbuf[i];
		}	
		templatesum = pusart->rxbuf[4];		
		for(i=0;i< templatesum;i++){
			tcount = pusart->rxbuf[2 + Taddr];
			crccount = 5;
			for(j = 0;j < tcount+3;j++){
				temp = pusart->rxbuf[j + Taddr];
				templatedata.rectemplate_buf[crccount++] = temp;
			}
			Taddr = Taddr + tcount +3;
			if((templatesum - 1) == templatedata.rectemplate.template_no){
					AT24CXX_WriteOneByte(TEMPLATE_COUNT_ADDR,templatedata.rectemplate.template_no);
					AT24CXX_WriteOneByte(TEMPLATE_COUNT_ADDR + 1,templatedata.rectemplate.template_no);
			}
			crc16 = CRC_GetCCITT(templatedata.rectemplate_buf,crccount);
			templatedata.rectemplate_buf[crccount++] = (crc16>>8)&0xFF; 
			templatedata.rectemplate_buf[crccount++] = crc16&0xFF;
			templatedata.rectemplate_buf[crccount++] = 0X0D;
			templatedata.rectemplate_buf[crccount++] = 0X0A;
			AT24CXX_Write(templatedata.rectemplate.template_no*TEMPLATE_SECTION_SIZE + TEMPLATE_SAVE_ADDR,\
			(unsigned char *)templatedata.rectemplate_buf,TEMPLATE_SIZE);				
		}
}
void Defaultdata_bag_compose(Usart_Type usart)
{
	  u16 i,j,Daddr,crc16;
	  Usart_Type* pusart;
	  u8 dcount,crccount,defaultsum = 0;
	  static COMM_RecData_Union_Type defaultdata;
		static u8 temp;
	  pusart = &usart;
		Daddr = 8;
	  if(pusart->rxbuf[4] != pusart->rxbuf[5]){
			return;
		}
		for(i = 0;i < 5;i++){
			defaultdata.recdata_buf[i] = pusart->rxbuf[i];
		}	
		defaultsum = pusart->rxbuf[4];		
		for(i=0;i< defaultsum;i++){
			dcount = pusart->rxbuf[2 + Daddr];
			crccount = 5;
			for(j = 0;j < dcount+3;j++){
				temp = pusart->rxbuf[j + Daddr];
				defaultdata.recdata_buf[crccount++] = temp;
			}
			Daddr = Daddr + dcount +3;
			if((defaultsum - 1) == defaultdata.recdata.template_no){
					AT24CXX_WriteOneByte(DEFAULT_DATA_COUNT_ADDR,defaultdata.recdata.template_no);
					AT24CXX_WriteOneByte(DEFAULT_DATA_COUNT_ADDR + 1,defaultdata.recdata.template_no);
				}
			crc16 = CRC_GetCCITT(defaultdata.recdata_buf,crccount);
			defaultdata.recdata_buf[crccount++] = (crc16>>8)&0xFF; 
			defaultdata.recdata_buf[crccount++] = crc16&0xFF;
			defaultdata.recdata_buf[crccount++] = 0X0D;
			defaultdata.recdata_buf[crccount++] = 0X0A;
			AT24CXX_Write(defaultdata.recdata.template_no*(DATADISPLAY_SIZE+12) + DEFAULT_DATA_SAVE_ADDR,\
			(unsigned char *)defaultdata.recdata_buf,crccount);			
		}
}
void Display_Multi_Data(u8 loopmode)
{
	u8 i,j;
	u8 word_size,display_size,display_mode,display_sum;
	u16 start_x,start_y,b_color,f_color;
	static u16 data_count,data_addr,multi_num,multi_count = 1; 
	
	if(multi_display_flag == 0){
		return;
	}
	multi_num = ((Ldata_Svae_buf[0][4]>>4)&0x0F);//�ҵ���Ҫ��ʾ������
	if(multi_count < multi_num){
		if(Ldata_Follow_flag){
			if(Ldata_Follow_Led[multi_count]&0x01){				
				RGB_RLED_ON;								
			}else{
				RGB_RLED_OFF;
			}
			if(Ldata_Follow_Led[multi_count]&0x02){
				RGB_GLED_ON;
			}else{
				RGB_GLED_OFF;
			}
			if(Ldata_Follow_Led[multi_count]&0x04){
				RGB_BLED_ON;
			}else{
				RGB_BLED_OFF;
			}
		}
		multi_count++;
		Current_Display_Page = multi_count;
		data_addr = 8;
		display_sum = Ldata_Svae_buf[multi_count-1][5];
		for(i=0;i<display_sum;i++){
				new_display_num = Ldata_Svae_buf[multi_count-1][data_addr];					
				start_x = Template_Save_buf[new_display_num].rectemplate.x_starH * 256 + Template_Save_buf[new_display_num].rectemplate.x_starL;
				start_y = Template_Save_buf[new_display_num].rectemplate.y_starH * 256 + Template_Save_buf[new_display_num].rectemplate.y_starL;
				b_color = Template_Save_buf[new_display_num].rectemplate.backcolorH * 256 + Template_Save_buf[new_display_num].rectemplate.backcolorL;
				f_color = Template_Save_buf[new_display_num].rectemplate.fontcolorH * 256 + Template_Save_buf[new_display_num].rectemplate.fontcolorL;
				word_size = Template_Save_buf[new_display_num].rectemplate.word_size;
				display_size = Template_Save_buf[new_display_num].rectemplate.display_size; //��ʾ�ֽڸ�����һ������ռ�����ֽ�
// 					display_mode = Template_Save_buf[new_display_num].rectemplate.display_mode;
				display_mode = Ldata_Svae_buf[multi_count-1][data_addr+3];
			
				data_count = Ldata_Svae_buf[multi_count-1][2 + data_addr];
				for(j = 0;j < data_count;j++){
					Display_data.dis_buf[j] = Ldata_Svae_buf[multi_count-1][j + data_addr + 3];
				}	
				data_addr = data_addr + data_count +3;
				Show_Str(start_x,start_y,word_size/2 * display_size,(unsigned char*)&Display_data.dis_buf[1],b_color,f_color,word_size,display_mode);
			}
	}
	if(multi_count == multi_num){
		if(loopmode == 0){
			multi_display_flag = 0;
			multi_count = 1;
		}else{
			multi_display_flag = 1;
			multi_count = 0;		
		}
	}
}
void Communication_Process(void )
{
		if (1 == Usart1_Control_Data.rx_aframe){ 
			Respond_Host_Comm();
			Usart1_Control_Data.rx_count = 0;
			Usart1_Control_Data.rx_aframe = 0;
		}
}












