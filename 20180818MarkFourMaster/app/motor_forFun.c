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
 
  FTM_Init.FTM_PwmDeadtimeVal=8;//���Ƶ��20K ����50us  ȡ�ٷ�֮5������ 5*0.3/50=5%
  FTM_Init.FTM_PwmDeadtimeCfg=DEADTIME_CH45|DEADTIME_CH67;
  FTM_Init.FTM_PwmDeadtimeDiv=DEADTIME_DIV16;//����50M/16=3M  ����0.3us
  
  LPLD_FTM_Init(FTM_Init);          //ʹ��PWM
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch4,0,PTD4,ALIGN_LEFT);  //��  ��ת   
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch5,0,PTD5,ALIGN_LEFT);  // �� ��ת
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch6,0,PTD6,ALIGN_LEFT);  //��  ��ת   
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch7,0,PTD7,ALIGN_LEFT);  // ��  ��ת
  
  LPLD_FTM_PWM_ChangeDuty(FTM0,FTM_Ch4,0);//δ���������0
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
