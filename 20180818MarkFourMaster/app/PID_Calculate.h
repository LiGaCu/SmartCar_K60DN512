#ifndef PID_CALCULATE_H
#define PID_CALCULATE_H
#include "common.h"

/*--速度ＰＩＤ结构体类型---*/
typedef struct
{
  float Kp[2];//PID三个参数
  float Ki[2];
  float Kd[2];
  /*--- 数组第一项为左轮 第二项为右轮---*/
  float SpeedNow[2];//当前速度
  float SpeedSet[2];//给定速度
  float Error[2][2];//1->2 当前误差e(k),上一次e(k-1)
  float Integer[2];//积分总和值（误差和）
  float Deriv[2];//微分项值（当前误差-上一次误差）
  float Output[2];//输出
}SpeedPID_TypeDef;

//pid结构体类型
typedef struct
{
    float P;
    float I;
    float D;
    float Desired;                  
    float Error;
    float PreError;
    float PrePreError;
    float Integer;                 //积分项             
    float iLimit;                  //积分限值
    float Deriv;                   //微分项
    float Output;
 }PID_TypeDef;

extern SpeedPID_TypeDef SpeedPID;
extern PID_TypeDef Turn_PID;
extern float SpeedTarget[2];//速度目标值   
void SpeedPID_Cal(float TargetSpeed[2],float MotorPID_Speed_ave_L,float MotorPID_Speed_ave_R);//目标值和测量值
void Turn_PID_Cal(void);
void PID_Init(void);
void SetSpeed_Configuration(void);
void Jam_Restart(void);
   
//extern uint16 NOpointtime,WITHpointtime;
//extern uint8 Jamlock;
#endif