#include "common.h"
#include "motor_forFun.h" 
#include "PID_Calculate.h"

uint8 car_run=0;

void motor_Init(void)
{
  static  FTM_InitTypeDef FTM_Init;
  FTM_Init.FTM_Ftmx=FTM0;
  FTM_Init.FTM_Mode=FTM_MODE_PWM;
  FTM_Init.FTM_PwmFreq=10000;
 
  FTM_Init.FTM_PwmDeadtimeVal=8;//电机频率20K 周期50us  取百分之5的死区 5*0.3/50=5%
  FTM_Init.FTM_PwmDeadtimeCfg=DEADTIME_CH45|DEADTIME_CH67;
  FTM_Init.FTM_PwmDeadtimeDiv=DEADTIME_DIV16;//总线50M/16=3M  周期0.3us
  
  LPLD_FTM_Init(FTM_Init);          //使能PWM
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch4,0,PTD4,ALIGN_LEFT);  //左  反转   
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch5,0,PTD5,ALIGN_LEFT);  // 左 正转
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch6,0,PTD6,ALIGN_LEFT);  //右  正转   
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch7,0,PTD7,ALIGN_LEFT);  // 右  反转
  
  LPLD_FTM_PWM_ChangeDuty(FTM0,FTM_Ch4,0);//未启动，输出0
  LPLD_FTM_PWM_ChangeDuty(FTM0,FTM_Ch5,0);
  LPLD_FTM_PWM_ChangeDuty(FTM0,FTM_Ch6,0);
  LPLD_FTM_PWM_ChangeDuty(FTM0,FTM_Ch7,0);
}

void MotorContrl(void)
{

  if(car_run==1)
  {
  if(SpeedPID.Output[0]>=0)
    MotorLeftForward((uint32)SpeedPID.Output[0]);
  if(SpeedPID.Output[0]<0)
    MotorLeftBackward((uint32)-SpeedPID.Output[0]);
  if(SpeedPID.Output[1]>=0)
    MotorRightForward((uint32)SpeedPID.Output[1]);
  if(SpeedPID.Output[1]<0)
    MotorRightBackward((uint32)-SpeedPID.Output[1]);
  }
  else
    CarStop();
}


void MotorLeftForward( uint16 x )
{
    LPLD_FTM_PWM_ChangeDuty( FTM0, FTM_Ch4, 0 );
    LPLD_FTM_PWM_ChangeDuty( FTM0, FTM_Ch5, ( x ) ) ; 
}
void MotorLeftBackward( uint16 x )
{
    LPLD_FTM_PWM_ChangeDuty( FTM0, FTM_Ch4, ( x ) ); 
    LPLD_FTM_PWM_ChangeDuty( FTM0, FTM_Ch5, 0 );
}
void MotorRightForward( uint16 x )
{
    LPLD_FTM_PWM_ChangeDuty( FTM0, FTM_Ch7, 0 );
    LPLD_FTM_PWM_ChangeDuty( FTM0, FTM_Ch6, ( x ) ); 
}
void MotorRightBackward( uint16 x )
{
    LPLD_FTM_PWM_ChangeDuty( FTM0, FTM_Ch7, ( x ) ); 
    LPLD_FTM_PWM_ChangeDuty( FTM0, FTM_Ch6, 0 );
}
void CarStop( void )
{
    LPLD_FTM_PWM_ChangeDuty( FTM0, FTM_Ch7, 0 );
    LPLD_FTM_PWM_ChangeDuty( FTM0, FTM_Ch5, 0 );
    LPLD_FTM_PWM_ChangeDuty( FTM0, FTM_Ch6, 0 );
    LPLD_FTM_PWM_ChangeDuty( FTM0, FTM_Ch4, 0 );
}
