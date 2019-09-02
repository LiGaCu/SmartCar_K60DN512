
#ifndef CAMERA_TEST_H
#define CAMERA_TEST_H

#include "common.h"

#define COLUMN     180//图像宽度   范围1-752     K60采集不允许超过188
#define ROW     90//图像高度	范围1-480

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