#ifndef _OLED_H_
#define _OLED_H_
#include "common.h"

void LED_Init(void);
void led_gpio_int(void);
void LED_Fill(unsigned char bmp_data);
void LED_Set_Pos(unsigned char x, unsigned char y);
//��ʾһ��6x8��׼ASCII�ַ�
void LED_P6x8Char(unsigned char x,unsigned char y,unsigned char ch);
//����
void LED_Fill(unsigned char bmp_data);
//����������д��һ��6x8��׼ASCII�ַ���
void LED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]); 
//��һ��char����ת����3λ��������ʾ 
void LED_PrintValueC(unsigned char x, unsigned char y, char data);
//������������һ��int����ת����5λ��������ʾ
void LED_PrintValueI(unsigned char x, unsigned char y, int data);
//��ʾunsigned int��
void LED_PrintValueFP(unsigned char x, unsigned char y, unsigned int data, unsigned char num);
//������������һ��float����ת������������5λ��С���ͷ��ŵ����ݲ�������ʾ
void LED_PrintValueF(unsigned char x, unsigned char y, float data, unsigned char num);
void LED_WrCmd(unsigned char cmd) ;
void LED_WrDat(unsigned char data);
//extern void LED_Init(void);
extern void display(void);
extern char mode;
extern char mode_pre;
#endif