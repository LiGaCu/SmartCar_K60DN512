
#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "common.h"

#define COLUMN 160       //列
#define ROW    100      //行

#define COLUMN_Two 160       //列
#define ROW_Two    100      //行
#define setvalue_Two 40//亮点亮度阈值

//摄像头命令枚举
typedef enum
{
    INIT = 0,               //摄像头初始化命令
    AUTO_EXP,               //自动曝光命令
    EXP_TIME,               //曝光时间命令
    FPS,                    //摄像头帧率命令
    SET_COL,                //图像列命令
    SET_ROW,                //图像行命令
    LR_OFFSET,              //图像左右偏移命令
    UD_OFFSET,              //图像上下偏移命令
    GAIN,                   //图像偏移命令
    CONFIG_FINISH,          //非命令位，主要用来占位计数
    
    SET_EXP_TIME = 0XF0,    //单独设置曝光时间命令
    GET_STATUS,             //获取摄像头配置命令
    GET_VERSION,            //固件版本号命令
	
    SET_ADDR = 0XFE,        //寄存器地址命令
    SET_DATA                //寄存器数据命令
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
