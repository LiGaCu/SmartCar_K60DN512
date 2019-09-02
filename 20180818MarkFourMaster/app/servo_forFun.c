#include "common.h"
#include "servo_forFun.h" 
uint16 time_servo=SerMid;

void servo_pit_init(void)
{
  static GPIO_InitTypeDef servo_GPIO;  //舵机引脚初始化
  servo_GPIO.GPIO_PTx = PTD;
  servo_GPIO.GPIO_Pins = GPIO_Pin12;
  servo_GPIO.GPIO_Dir =DIR_OUTPUT;
  servo_GPIO.GPIO_Output=OUTPUT_L;
  LPLD_GPIO_Init(servo_GPIO);
  
  
  static PIT_InitTypeDef pit1_init_struct; //PIT1定时中断  负责拉低电平
  pit1_init_struct.PIT_Pitx = PIT1;
  pit1_init_struct.PIT_PeriodUs   =1000000;     //
  pit1_init_struct.PIT_Isr = pit1_isr;   //设置中断函数
  LPLD_PIT_Init(pit1_init_struct);  
  LPLD_PIT_EnableIrq(pit1_init_struct);
  PIT->CHANNEL[1].TCTRL &= ~PIT_TCTRL_TEN_MASK;//停止计时
  
  
  static PIT_InitTypeDef pit2_init_struc_SERVO; //20ms定时中断  50HZ负责拉高电平
  pit2_init_struc_SERVO.PIT_Pitx = PIT2;
  pit2_init_struc_SERVO.PIT_PeriodMs =20;     //
  pit2_init_struc_SERVO.PIT_Isr = servo_control;   //设置中断函数
  LPLD_PIT_Init(pit2_init_struc_SERVO);  
  LPLD_PIT_EnableIrq(pit2_init_struc_SERVO);
  
}

void servo_control(void)  //PIT2的中断函数 拉高电平
{
  if (time_servo>SerLeftMax)//转角限幅
    time_servo=SerLeftMax;
  else if (time_servo<SerRightMin)
    time_servo=SerRightMin;
  
  PIT->CHANNEL[1].LDVAL = (time_servo)*(g_bus_clock / 1000000) - 1; //重新设置PIT1的中断周期
  PTD12_O = 1;                                  //拉高电平
  PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;//开始计时 
}

void pit1_isr(void)  //PIT1的中断函数 拉低电平
{
    PTD12_O = 0;
    PIT->CHANNEL[1].TCTRL &= ~PIT_TCTRL_TEN_MASK;//停止计时
}


