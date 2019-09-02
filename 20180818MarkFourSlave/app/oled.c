
//#include "camera.h"
#include "oled.h"
#include "common.h"
//OLED显示屏
//定义输出
#define LED_SCLK(x) LPLD_GPIO_Output_b(PTD, 15, x)// d0
#define LED_SDA(x)  LPLD_GPIO_Output_b(PTD, 12, x)//d1
#define LED_RST(x)  LPLD_GPIO_Output_b(PTD, 13, x)//复位低能电平
#define LED_DC(x)   LPLD_GPIO_Output_b(PTD, 14, x)//偏置常低
//OLED显示函数



void led_gpio_int(void)
{
  static GPIO_InitTypeDef led_Port_init;
  led_Port_init.GPIO_PTx = PTD;
  led_Port_init.GPIO_Dir = DIR_OUTPUT;
  led_Port_init.GPIO_Pins = GPIO_Pin15|GPIO_Pin14|GPIO_Pin13|GPIO_Pin12;
  led_Port_init.GPIO_Output  = OUTPUT_H ;
  LPLD_GPIO_Init(led_Port_init);
}


  
/*定义ASCII码表*/
const unsigned char F6x8[][6] =
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
	{ 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
	{ 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
	{ 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
	{ 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
	{ 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
	{ 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
	{ 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
	{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
	{ 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
	{ 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
	{ 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
	{ 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
	{ 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
	{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
	{ 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
	{ 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
	{ 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
	{ 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
	{ 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
	{ 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
	{ 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
	{ 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
	{ 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
	{ 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
	{ 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
	{ 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
	{ 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
	{ 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
	{ 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
	{ 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
	{ 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
	{ 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
	{ 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
	{ 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
	{ 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
	{ 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
	{ 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
	{ 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
	{ 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
	{ 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
	{ 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
	{ 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
	{ 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
	{ 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
	{ 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
	{ 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
	{ 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
	{ 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
	{ 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
	{ 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
	{ 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
	{ 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
	{ 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [ 91
	{ 0x00, 0x02, 0x04 ,0x08, 0x10, 0x20 },    // \92
	{ 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
	{ 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
	{ 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
	{ 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
	{ 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
	{ 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
	{ 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
	{ 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
	{ 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
	{ 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
	{ 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
	{ 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
	{ 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
	{ 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
	{ 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
	{ 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
	{ 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
	{ 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
	{ 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
	{ 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
	{ 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
	{ 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
	{ 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
	{ 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
	{ 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
	{ 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
	{ 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};

//写数据函数
void LED_WrDat(unsigned char data)   
{
	unsigned char i = 8;
	//LED_CS=0;
	LED_DC(1);
	LED_SCLK(0);  
	while (i--)
	{
		if (data & 0x80)
		{
			LED_SDA(1);
		}
		else
		{
			LED_SDA(0);
		}
		LED_SCLK(1); 
		asm("nop");     
		LED_SCLK(0);    
		data <<= 1;    
	}
}
//写入命令函数
void LED_WrCmd(unsigned char cmd) 
{
	unsigned char i = 8;
	//LED_CS = 0;
	LED_DC(0);
	LED_SCLK(0);
	while (i--)
	{
		if (cmd & 0x80)
		{
			LED_SDA(1);
		}
		else
		{
			LED_SDA(0);
		}
		LED_SCLK(1);
		asm("nop");           
		LED_SCLK(0);    
		cmd <<= 1;   
	} 	
	//LED_CS = 1;
}
//设置显示坐标位置
void LED_Set_Pos(unsigned char x, unsigned char y)
{ 
	LED_WrCmd(0xb0+y);
	LED_WrCmd(((x&0xf0)>>4)|0x10);
	LED_WrCmd((x&0x0f)|0x00); 
}
//清屏函数
void LED_Fill(unsigned char bmp_data)
{
	unsigned char y,x;
	
	for(y=0;y<8;y++)
	{
		LED_WrCmd(0xb0+y);   //0xb0+0~7表示页0~页7?
		LED_WrCmd(0x00);       //0x00+0~16表示将128列分成16组其地址在某组中的第几列
		LED_WrCmd(0x10);      //0x10+0~16表示将128列分成16组其地址所在第几组
		for(x=0;x<128;x++)
			LED_WrDat(bmp_data);
	}
}
//延时函数
void LED_DLY_ms(unsigned int ms)
{                         
  uint16 a;
  while(ms)
  {
    a=6675;
    while(a--);
    ms--;
  }
  return;
}
/*__________________________________
|  名称:                           |
|  作用:写数据等                   |
|_________________________________*/
void SetStartColumn(unsigned char d)
{
	LED_WrCmd(0x00+d%16);	// Set Lower Column Start Address for Page Addressing Mode
							// Default => 0x00
	LED_WrCmd(0x10+d/16);	// Set Higher Column Start Address for Page Addressing Mode
							// Default => 0x10
}
void SetAddressingMode(unsigned char d)
{
	LED_WrCmd(0x20);			// Set Memory Addressing Mode
	LED_WrCmd(d);			// Default => 0x02
							// 0x00 => Horizontal Addressing Mode
							// 0x01 => Vertical Addressing Mode
							// 0x02 => Page Addressing Mode
}

void SetColumnAddress(unsigned char a, unsigned char b)
{
	LED_WrCmd(0x21);			// Set Column Address
	LED_WrCmd(a);			// Default => 0x00 (Column Start Address)
	LED_WrCmd(b);			// Default => 0x7F (Column End Address)
}

void SetPageAddress(unsigned char a, unsigned char b)
{
	LED_WrCmd(0x22);			// Set Page Address
	LED_WrCmd(a);			// Default => 0x00 (Page Start Address)
	LED_WrCmd(b);			// Default => 0x07 (Page End Address)
}

void SetStartLine(unsigned char d)
{
	LED_WrCmd(0x40|d);		// Set Display Start Line
							// Default => 0x40 (0x00)
}

void SetContrastControl(unsigned char d)
{
	LED_WrCmd(0x81);			// Set Contrast Control
	LED_WrCmd(d);			// Default => 0x7F
}

void Set_Charge_Pump(unsigned char d)
{
	LED_WrCmd(0x8D);			// Set Charge Pump
	LED_WrCmd(0x10|d);		// Default => 0x10
							// 0x10 (0x00) => Disable Charge Pump
							// 0x14 (0x04) => Enable Charge Pump
}

void Set_Segment_Remap(unsigned char d)
{
	LED_WrCmd(0xA0|d);		// Set Segment Re-Map
							// Default => 0xA0
							// 0xA0 (0x00) => Column Address 0 Mapped to SEG0
							// 0xA1 (0x01) => Column Address 0 Mapped to SEG127
}

void Set_Entire_Display(unsigned char d)
{
	LED_WrCmd(0xA4|d);		// Set Entire Display On / Off
							// Default => 0xA4
							// 0xA4 (0x00) => Normal Display
							// 0xA5 (0x01) => Entire Display On
}

void Set_Inverse_Display(unsigned char d)
{
	LED_WrCmd(0xA6|d);		// Set Inverse Display On/Off
							// Default => 0xA6
							// 0xA6 (0x00) => Normal Display
							// 0xA7 (0x01) => Inverse Display On
}

void Set_Multiplex_Ratio(unsigned char d)
{
	LED_WrCmd(0xA8);			// Set Multiplex Ratio
	LED_WrCmd(d);			// Default => 0x3F (1/64 Duty)
}

void Set_Display_On_Off(unsigned char d)
{
	LED_WrCmd(0xAE|d);		// Set Display On/Off
							// Default => 0xAE
							// 0xAE (0x00) => Display Off
							// 0xAF (0x01) => Display On
}

void SetStartPage(unsigned char d)
{
	LED_WrCmd(0xB0|d);		// Set Page Start Address for Page Addressing Mode
							// Default => 0xB0 (0x00)
}

void Set_Common_Remap(unsigned char d)
{
	LED_WrCmd(0xC0|d);		// Set COM Output Scan Direction
							// Default => 0xC0
							// 0xC0 (0x00) => Scan from COM0 to 63
							// 0xC8 (0x08) => Scan from COM63 to 0
}

void Set_Display_Offset(unsigned char d)
{
	LED_WrCmd(0xD3);			// Set Display Offset
	LED_WrCmd(d);			// Default => 0x00
}

void Set_Display_Clock(unsigned char d)
{
	LED_WrCmd(0xD5);			// Set Display Clock Divide Ratio / Oscillator Frequency
	LED_WrCmd(d);			// Default => 0x80
							// D[3:0] => Display Clock Divider
							// D[7:4] => Oscillator Frequency
}

void Set_Precharge_Period(unsigned char d)
{
	LED_WrCmd(0xD9);			// Set Pre-Charge Period
	LED_WrCmd(d);			// Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
							// D[3:0] => Phase 1 Period in 1~15 Display Clocks
							// D[7:4] => Phase 2 Period in 1~15 Display Clocks
}

void Set_Common_Config(unsigned char d)
{
	LED_WrCmd(0xDA);			// Set COM Pins Hardware Configuration
	LED_WrCmd(0x02|d);		// Default => 0x12 (0x10)
							// Alternative COM Pin Configuration
							// Disable COM Left/Right Re-Map
}

void Set_VCOMH(unsigned char d)
{
	LED_WrCmd(0xDB);			// Set VCOMH Deselect Level
	LED_WrCmd(d);			// Default => 0x20 (0.77*VCC)
}

void Set_NOP(void)
{
	LED_WrCmd(0xE3);			// Command for No Operation
}

void LED_Init(void)        
{       
      // led_gpio_int();
       LED_DLY_ms(50);
        LED_DC(0);		
	LED_SDA(0);
        LED_SCLK(0);		
	LED_RST(0);
        LED_DLY_ms(50);
	LED_SCLK(1);		
	LED_RST(0);
//	for(i=0;i<100;i++)asm("nop");     //从上电到下面开始初始化要有足够的时间，即等待RC复位完毕	
        LED_DLY_ms(50);
	LED_RST(1);

	Set_Display_On_Off(0x00);		  // Display Off (0x00/0x01)
	Set_Display_Clock(0x80);		  // Set Clock as 100 Frames/Sec
	Set_Multiplex_Ratio(0x3F);	  // 1/64 Duty (0x0F~0x3F)
	Set_Display_Offset(0x00);		  // Shift Mapping RAM Counter (0x00~0x3F)
	SetStartLine(0x00);			  // Set Mapping RAM Display Start Line (0x00~0x3F)
	Set_Charge_Pump(0x04);		  // Enable Embedded DC/DC Converter (0x00/0x04)
	SetAddressingMode(0x02);	  // Set Page Addressing Mode (0x00/0x01/0x02)
	Set_Segment_Remap(0x01);	  // Set SEG/Column Mapping     0x00左右反置 0x01正常
	Set_Common_Remap(0x08);	  // Set COM/Row Scan Direction 0x00上下反置 0x08正常
	Set_Common_Config(0x10);	  // Set Sequential Configuration (0x00/0x10)
	SetContrastControl(0xCF); // Set SEG Output Current
	Set_Precharge_Period(0xF1);	  // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	Set_VCOMH(0x40);			  // Set VCOM Deselect Level
	Set_Entire_Display(0x00);		  // Disable Entire Display On (0x00/0x01)
	Set_Inverse_Display(0x00);	  // Disable Inverse Display On (0x00/0x01)  
	Set_Display_On_Off(0x01);		  // Display On (0x00/0x01)
	LED_Fill(0x00);                               //初始清屏
	LED_Set_Pos(0,0); 	
} 
//显示一个6x8标准ASCII字符
void LED_P6x8Char(unsigned char x,unsigned char y,unsigned char ch)
{
	 unsigned char c=0,i=0;     
	   
	c =ch-32;
	if(x>122)
	{
		x=0;
		y++;
	}
	LED_Set_Pos(x,y);    
	for(i=0;i<6;i++)
	{     
		LED_WrDat(F6x8[c][i]);  
	}
}

//功能描述：写入一组6x8标准ASCII字符串 
void LED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;      
	while (ch[j]!='\0')
	{    
		c =ch[j]-32;
		if(x>126)
		{
			x=0;
			y++;
		}
		LED_Set_Pos(x,y);    
		for(i=0;i<6;i++)
		{     
			LED_WrDat(F6x8[c][i]);  
		}
		x+=6;
		j++;
	}
}
//将一个char型数转换成3位数进行显示
void LED_PrintValueC(unsigned char x, unsigned char y, char data)
{
	unsigned char i,j,k;
	if(data<=0)
	{
		LED_P6x8Char(x,y,'-');
		data = - data;	
	}
	else
	{
		LED_P6x8Char(x,y,'+');
	}
	i = data/100;
	j = (data%100)/10;
	k = data%10;
	LED_P6x8Char(x+6,y,i+48);
	LED_P6x8Char(x+12,y,j+48);
	LED_P6x8Char(x+18,y,k+48);		
}
//功能描述：将一个int型数转换成5位数进行显示
void LED_PrintValueI(unsigned char x, unsigned char y, int data)
{
	unsigned char i,j,k,l,m;  
	if(data < 0)
	{
		LED_P6x8Char(x,y,'-');
		data = - data;	
	}
	else
	{
		LED_P6x8Char(x,y,'+');
	}
	
	l  = data/10000;
	m= (data%10000)/1000;
	i = (data%1000)/100;
	j = (data%100)/10;
	k = data%10;
	LED_P6x8Char(x+6,y,l+48);
	LED_P6x8Char(x+12,y,m+48);
	LED_P6x8Char(x+18,y,i+48);
	LED_P6x8Char(x+24,y,j+48);
	LED_P6x8Char(x+30,y,k+48);		
}
//显示unsigned int型
 void LED_PrintValueFP(unsigned char x, unsigned char y, unsigned int data, unsigned char num)
 {
 	unsigned char m,i,j,k;  	
 	//LED_P6x8Char(x, y, '.');
	m= data/1000;
	i = (data%1000)/100;
	j = (data%100)/10;
	k = data%10;
	switch(num)
	{
		case 1:  	LED_P6x8Char(x+6,y,k+48);
				break;
		case 2:  	LED_P6x8Char(x+6,y,j+48);
				LED_P6x8Char(x+12,y,k+48);
				break;
		case 3:	LED_P6x8Char(x+6,y,i+48);
				LED_P6x8Char(x+12,y,j+48);
				LED_P6x8Char(x+18,y,k+48);
				break;
		case 4: 	LED_P6x8Char(x+6,y,m+48);
				LED_P6x8Char(x+12,y,i+48);
				LED_P6x8Char(x+18,y,j+48);
				LED_P6x8Char(x+24,y,k+48);
				break;	
	}
 }
//功能描述：将一个float型数转换成整数部分5位带小数和符号的数据并进行显示
 void LED_PrintValueF(unsigned char x, unsigned char y, float data, unsigned char num)
 {
 	unsigned char l,m,i,j,k;  //万千百十个
 	unsigned char databiti = 6; //整数位数
        unsigned int tempdataui = 0;
  	int tempdataii = (int)data; //整数部分
 	long int tempdatalp = (long int)((data - (int)data)*10000); //取小数位后4位
 	
 	//整数部分显示
 	if(data < 0.0000001)  LED_P6x8Char(x, y,'-'); 
 	else LED_P6x8Char(x, y,'+');
	if(tempdataii < 0)tempdataii = - tempdataii;  //去掉整数部分负号
	tempdataui = tempdataii;
 	l  = tempdataui/10000;
	m= (tempdataui%10000)/1000;
	i = (tempdataui%1000)/100;
	j = (tempdataui%100)/10;
	k = tempdataui%10;
 	if (l != 0)  //五位
 	{
 		LED_P6x8Char(x+6,y,l+48);
 		LED_P6x8Char(x+12,y,m+48);
		LED_P6x8Char(x+18,y,i+48);
		LED_P6x8Char(x+24,y,j+48);
		LED_P6x8Char(x+30,y,k+48);
 	}
 	else if(m != 0) //四位
 	{
 		databiti = 5;
 		LED_P6x8Char(x+6,y,m+48);
 		LED_P6x8Char(x+12,y,i+48);
		LED_P6x8Char(x+18,y,j+48);
		LED_P6x8Char(x+24,y,k+48);
 	}
  	else if(i != 0) //三位
  	{
  		databiti = 4;
  	 	LED_P6x8Char(x+6,y,i+48);
 		LED_P6x8Char(x+12,y,j+48);
		LED_P6x8Char(x+18,y,k+48);
  	}
  	else if(j != 0) //两位
  	{
    		databiti = 3;	
  		LED_P6x8Char(x+6,y,j+48);
 		LED_P6x8Char(x+12,y,k+48);
  	}
	else 	
	{
		databiti = 2;
		LED_P6x8Char(x+6,y,k+48);
	}	
 	if(tempdatalp < 0)tempdatalp = - tempdatalp;	//去掉小数部分负号
	switch(num)
	{
		case 0: break;
		case 1:  LED_PrintValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp / 1000),num);break;
		case 2:  LED_PrintValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp / 100),num);break;
		case 3:  LED_PrintValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp / 10),num);break;
		case 4:  LED_PrintValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp),num);break;					
	}
 }
//void display(void)
//{     int16 Speed1=0;
//      int16 Speed2=0;
//     // Speed1=ch0_pulseacc;
//      //Speed2=ch1_pulseacc;
//      LED_P6x8Str(12,0,"Speed1:");
//      LED_PrintValueC(75,0,Speed1);
//      LED_P6x8Str(12,1,"Speed2:");
//      LED_PrintValueI(75,1,(Speed2));
//} 
//float *par[2][6]={{&Speed_g_pre,&car_angle_kp,&car_angle_kd,&Speed_kp,&Speed_kd,&Speed_ki},{&CONTROL_ROW,&direction_P,&direction_D,&epc,&edc,&ratio}};
//char press=1;
//char mode=0;
//char mode_pre;
//int  page=0;
//int show_fill;
//int cursor_line;
//float place;
//void key_delay(int ms);
/*void keycontrol(void)
{
 //模式1，参数调整 
  if(mode==1)
  {
   if(press==1)
   {
    show_fill=1;
    press=0;
    if(page==0)
    {
      LED_Fill(0x00);
      LED_P6x8Str(12,0,"Voltage:");
      LED_PrintValueF(75,0,Voltage,3);
      LED_P6x8Str(12,1,"Speed:");
      LED_PrintValueF(75,1,Speed_g_pre,0);
      LED_P6x8Str(12,2,"angle_p:");
      LED_PrintValueF(75,2,car_angle_kp,0);
      LED_P6x8Str(12,3,"angle_d:");
      LED_PrintValueF(75,3,car_angle_kd,1);
      LED_P6x8Str(12,4,"Speed_kp:");
      LED_PrintValueF(75,4,Speed_kp,1);
      LED_P6x8Str(12,5,"Speed_kd:");
      LED_PrintValueF(75,5,Speed_kd,2);
      LED_P6x8Str(12,6,"Speed_ki:");
      LED_PrintValueF(75,6,Speed_ki,4);
      //LED_P6x8Str(12,6,"quan:");
      //LED_PrintValueF(75,6,quan,2);
    }
    if(page==1)
    {
      LED_Fill(0x00);
      LED_P6x8Str(12,0,"Voltage:");
      LED_PrintValueF(75,0,Voltage,3);
      LED_P6x8Str(12,1,"Row:");
      LED_PrintValueI(75,1,CONTROL_ROW);
      LED_P6x8Str(12,2,"dir_P:");
      LED_PrintValueF(75,2,direction_P,0);
      LED_P6x8Str(12,3,"dir_D:");
      LED_PrintValueF(75,3,direction_D,3);
      LED_P6x8Str(12,4,"epc:");
      LED_PrintValueF(75,4,epc,3);
      LED_P6x8Str(12,5,"edc:");
      LED_PrintValueF(75,5,edc,3);
      LED_P6x8Str(12,6,"ratio:");
      LED_PrintValueF(75,6,ratio,2);
    }
      LED_P6x8Str(0,cursor_line,"_");
   }
    if(page==2)
    { 
      if(show_fill)
      { 
        LED_Fill(0x00);
        show_fill=0;
      }
      if(sampleover)
      {
        disable_irq(PORTB_IRQn);
        row_DMA_flag=0;
        sampleover=0;
        //解压图像
        img_extract();
        search();
        Centre_Calculate();
        Centre_Err_Calculate();
        //清PORTB中断标志
        LPLD_GPIO_ClearIntFlag(PORTB);      
        enable_irq(PORTB_IRQn);
      }
     //图像显示
      uint8 i,j,data;
      int temp,cnt;
      for(j=0;j<ROW;j++)
      {  
        if(line_storage_bool[j][M]) Image[j][(int)Mid_storage[j]+39]=0;
        if(line_storage_bool[j][L]) Image[j][(int)line_storage[j][L]+2]=0;
        if(line_storage_bool[j][R]) Image[j][(int)line_storage[j][R]-2]=0;
      }
      for(j=0;j<8;j++)
     { 
       LED_WrCmd(0xb0+j);
       LED_WrCmd(0x01);
       LED_WrCmd(0x10);
       for(i=0;i<80;i++)
       { 
         temp=0; 
         for(cnt=7;cnt>=0;cnt--)
         {  
           temp|=Image[j*8+cnt][i]/255; 
           temp=temp<<1;  
         }
         data=temp>>1;
         LED_WrDat(data);
       }
     }     
    }
    if(DN)
    {
      key_delay(20);
      if(DN)
      { 
        press=1;
        place=1;
        cursor_line++; 
        while(DN);
      }
    }        
    if(UP)
    {
      key_delay(20);
      if(UP)
      {
        press=1;
        place=1;
        cursor_line--; 
        while(UP);
      }     
    }
    if(cursor_line<0)
      cursor_line=7;
    if(cursor_line>7)
      cursor_line=0;
    if(cursor_line==0)
    {
     if(RT)
     {
       key_delay(20);
       if(RT)
       {
         press=1;
         page++;
         while(RT);
       }
     } 
      if(LT)
     {
       key_delay(20);
       if(LT)
       {
         press=1;
         page--;
         while(LT);
       }
     }
     if(page<0) page=2;
     if(page>2) page=0;
   }
   if(cursor_line)
   {
     if(ESC)
     {
       key_delay(20);
       if(ESC)
       {
         press=1;
         place*=10; 
         while(ESC);
       }
     }
     if(OK)
     {
       key_delay(20);
       if(OK)
       {
         press=1;
         place*=0.1; 
         while(OK);
       }
     }
     if(place>1000) place=1000;
     if(place<0.001) place=0.001;    
     if(RT)
     {
       key_delay(20);
       if(RT)
       {
         press=1;
         *par[page][cursor_line-1]=*par[page][cursor_line-1]+place;
         while(RT);
       }
     }
     if(LT)
     {
       key_delay(20);
       if(LT)
       {
         press=1;
         *par[page][cursor_line-1]=*par[page][cursor_line-1]-place;
         while(LT);
       }
     }
     
   }
  }
}
void key_delay(int ms)
{                         
    int a;
    while(ms)
    {
       a=1000;
       while(a--);
       ms--;
    }
    return;
}
*/