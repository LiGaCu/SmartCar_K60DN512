#ifndef _OLED_H_
#define _OLED_H_
#include "common.h"

void LED_Init(void);
void led_gpio_int(void);
void LED_Fill(unsigned char bmp_data);
void LED_Set_Pos(unsigned char x, unsigned char y);
//显示一个6x8标准ASCII字符
void LED_P6x8Char(unsigned char x,unsigned char y,unsigned char ch);
//清屏
void LED_Fill(unsigned char bmp_data);
//功能描述：写入一组6x8标准ASCII字符串
void LED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]); 
//将一个char型数转换成3位数进行显示 
void LED_PrintValueC(unsigned char x, unsigned char y, char data);
//功能描述：将一个int型数转换成5位数进行显示
void LED_PrintValueI(unsigned char x, unsigned char y, int data);
//显示unsigned int型
void LED_PrintValueFP(unsigned char x, unsigned char y, unsigned int data, unsigned char num);
//功能描述：将一个float型数转换成整数部分5位带小数和符号的数据并进行显示
void LED_PrintValueF(unsigned char x, unsigned char y, float data, unsigned char num);
void LED_WrCmd(unsigned char cmd) ;
void LED_WrDat(unsigned char data);
//extern void LED_Init(void);
extern void display(void);
extern char mode;
extern char mode_pre;
#endif