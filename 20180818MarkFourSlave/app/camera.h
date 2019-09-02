
#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "common.h"

#define COLUMN 160       //��
#define ROW    100      //��

#define COLUMN_Two 160       //��
#define ROW_Two    100      //��
#define setvalue_Two 40//����������ֵ

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
void MT9V032_Two_CFG_init(void);
void portb_isr(void);
void portc_isr(void);

void my_delay(long t);
void camera_init(void);
void cameraTwo_init(void);
void mt9v032_cof_uart_interrupt(void);
void mt9v032_cof_uart_interrupt2(void);
void Noisefilter(void);
void Figure_Process(void);
void Figure_Process_Two(void);
void picture_send(void);
void picture_send_Two(void);


extern uint8 setvalue;
extern uint8 CircleFlag;

extern uint8 Barrier_x,Barrier_y,HaveBarrier;
extern int Validfiguresize;
extern double Variance;
extern uint8 Several_signal_y;

extern uint8 RealNopoint;
extern uint8 real_pointx,real_pointy;
extern uint32 PC;

//void Calculate_Middle_Point(void);
void dis_bmp(uint16 high, uint16 width, uint8 *p,uint8 value);
void dis_bmp_half(uint16 high, uint16 width, uint8 *p,uint8 value);

void display_BarrierLine_100_60(float slope,int intercept);
void display_Line_coordinate_y_100_60(float slope_L,float slope_R,int intercept_L,int intercept_R,uint8 coordinate_y);

void display_BarrierLine_80_50(float slope,int intercept);
void display_Line_coordinate_y_90_60(float slope_L,float slope_R,int intercept_L,int intercept_R,uint8 coordinate_y);

void display_Center_100_60(uint8 *p,uint8 value,uint8 Barrier_x,uint8 Barrier_y);
void dis_bmp_half_two(uint16 high, uint16 width, uint8 *p,uint8 value);
extern unsigned char Image2[ROW][COLUMN];
extern unsigned char ImageTwo2[ROW_Two][COLUMN_Two];
#endif
