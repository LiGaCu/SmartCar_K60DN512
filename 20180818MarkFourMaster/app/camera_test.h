
#ifndef CAMERA_TEST_H
#define CAMERA_TEST_H

#include "common.h"

#define COLUMN     180//ͼ����   ��Χ1-752     K60�ɼ���������188
#define ROW     90//ͼ��߶�	��Χ1-480

//����ͷ����ö��
typedef enum
{
    INIT = 0,               //����ͷ��ʼ������
    AUTO_EXP,               //�Զ��ع�����
    EXP_TIME,               //�ع�ʱ������
    FPS,                    //����ͷ֡������
    SET_COL,                //ͼ��������
    SET_ROW,                //ͼ��������
    LR_OFFSET,              //ͼ������ƫ������
    UD_OFFSET,              //ͼ������ƫ������
    GAIN,                   //ͼ��ƫ������
    CONFIG_FINISH,          //������λ����Ҫ����ռλ����
    
    SET_EXP_TIME = 0XF0,    //���������ع�ʱ������
    GET_STATUS,             //��ȡ����ͷ��������
    GET_VERSION,            //�̼��汾������
	
    SET_ADDR = 0XFE,        //�Ĵ�����ַ����
    SET_DATA                //�Ĵ�����������
}CMD;



void MT9V032_CFG_init(void);
void portb_isr(void);
void my_delay(long t);
void camera_init(void);
void Calculate_Middle_Point(void);
void mt9v032_cof_uart_interrupt(void);
void picture_send(void);
void Middle_Point_Configure( void );
void dis_bmp(uint16 high, uint16 width, uint8 *p,uint8 value);
void dis_bmp_half(uint16 high, uint16 width, uint8 *p,uint8 value);


extern uint8 real_pointx,real_pointy,NOpoint;
extern uint8 SetCenterX,setvalue,SCenter;
extern uint8 ph_lock1,ph_lock2;
#define DistortionLimit_L_Slope -2.2468
#define DistortionLimit_L_Intercept 108
#define DistortionLimit_R_Slope 2.8329
#define DistortionLimit_R_Intercept -369

extern const uint8 Distortion_140[90][2];
extern const uint8 Distortion_160[90][2];
extern const uint8 Distortion_180[90][2];
extern const uint8 Distortion_200[90][2];
extern const uint8 Distortion_120[90][2];
extern const uint8 Distortion_100[90][2];
extern const uint8 Distortion_40[90][2];
extern const uint8 Distortion_60[90][2];
extern const uint8 Distortion_80[90][2];

extern unsigned char Image1[ROW][COLUMN];
#endif