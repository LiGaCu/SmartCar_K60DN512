
#include "common.h"
#include "freecars.h"
#include "PID_Calculate.h"
#include "QD.h"
#include "servo_forFun.h" 
#include "motor_forFun.h"
#include "camera_test.h"
#include "oled.h"
#include "botton_test.h"
#include "myflash.h"
   
uint8 model_switch;
uint8 BeginFlag=0;

void nvic_init()
{
  static NVIC_InitTypeDef nvic_init_struct0;
  nvic_init_struct0.NVIC_IRQChannel=PIT0_IRQn;//   10ms定时
  nvic_init_struct0.NVIC_IRQChannelGroupPriority=NVIC_PriorityGroup_2 ;//NVIC 中断分组
  nvic_init_struct0.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级
  nvic_init_struct0.NVIC_IRQChannelSubPriority=1;//NVIC响应式优先级
  LPLD_NVIC_Init(nvic_init_struct0);
  
  static NVIC_InitTypeDef nvic_init_struct1;
  nvic_init_struct1.NVIC_IRQChannel=PIT2_IRQn;//   20ms舵机拉高 定时
  nvic_init_struct1.NVIC_IRQChannelGroupPriority=NVIC_PriorityGroup_2 ;//NVIC 中断分组
  nvic_init_struct1.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级
  nvic_init_struct1.NVIC_IRQChannelSubPriority=1;//NVIC响应式优先级
  LPLD_NVIC_Init(nvic_init_struct1);
  
  static NVIC_InitTypeDef nvic_init_struct2;
  nvic_init_struct2.NVIC_IRQChannel=PIT1_IRQn;//   舵机拉低 定时
  nvic_init_struct2.NVIC_IRQChannelGroupPriority=NVIC_PriorityGroup_2 ;//NVIC 中断分组
  nvic_init_struct2.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级
  nvic_init_struct2.NVIC_IRQChannelSubPriority=1;//NVIC响应式优先级
  LPLD_NVIC_Init(nvic_init_struct2);

  static NVIC_InitTypeDef nvic_init_struct3;
  nvic_init_struct3.NVIC_IRQChannel=PORTB_IRQn;//      摄像头图像采集，优先级最高
  nvic_init_struct3.NVIC_IRQChannelGroupPriority=NVIC_PriorityGroup_2;//NVIC 中断分组
  nvic_init_struct3.NVIC_IRQChannelPreemptionPriority=0;//NVIC抢占式优先级
  nvic_init_struct3.NVIC_IRQChannelSubPriority=0;//NVIC响应式优先级
  LPLD_NVIC_Init(nvic_init_struct3);
  
  static NVIC_InitTypeDef nvic_init_struct4;
  nvic_init_struct4.NVIC_IRQChannel=UART0_RX_TX_IRQn ;//  通信中断
  nvic_init_struct4.NVIC_IRQChannelGroupPriority=NVIC_PriorityGroup_2 ;//NVIC 中断分组
  nvic_init_struct4.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级
  nvic_init_struct4.NVIC_IRQChannelSubPriority=1;//NVIC响应式优先级
  LPLD_NVIC_Init(nvic_init_struct4);
  
}

void main (void)
{
  DisableInterrupts;
  nvic_init();//中断优先级配置
  camera_init();
  uart0_init();
  Obstacle_init();
  pit0_init();//10ms定时中断
  QD_init();//编码器,FTM1 FTM2
  servo_pit_init();  //初始化舵机 
  motor_Init(); //电机 FTM0
  PID_Init();   //转向和速度PID初始化   
  enable_irq(PIT0_IRQn);//优先级配置后记得使能中断,定时中断 
  boma_init_gpio();
  LPLD_Flash_Init();
  parameter_flash();
  led_gpio_int();
  LED_Init();
  EnableInterrupts;
  while(1)
  {
    if( PTD0_I == 0 && PTD1_I == 0 && PTC18_I == 0 && PTC19_I == 0 )
      model_switch=0;            
    else if( PTD0_I == 1 && PTD1_I == 0 && PTC18_I == 0 && PTC19_I == 0 )
      model_switch=1;                                 
    else if( PTD0_I == 0 && PTD1_I == 1 && PTC18_I == 0 && PTC19_I == 0 )
      model_switch=2;                
    else if( PTD0_I == 0 && PTD1_I == 0 && PTC18_I == 1 && PTC19_I == 0 )
      model_switch=3;               
    else if( PTD0_I == 0 && PTD1_I == 0 && PTC18_I == 0 && PTC19_I == 1 )
      model_switch=4;             
    else if( PTD0_I == 0 && PTD1_I == 0 && PTC18_I == 1 && PTC19_I == 1 )
      model_switch=5;                       
    else if( PTD0_I == 1 && PTD1_I == 1 && PTC18_I == 1 && PTC19_I == 1 )
      model_switch=16;  //竞赛 
    
    if(model_switch==0)
    {
      Move_Process();
    }
    else if(model_switch==1) 
    {
      ChooseSide_Configure();
    }
    else if(model_switch==2) 
    {
      dis_bmp_half(ROW/2, COLUMN/2, Image1[0],setvalue);
      LED_PrintValueC(100 ,0,real_pointx);
      LED_PrintValueC(100 ,2,real_pointy);

      LED_PrintValueC(100 ,6,NOpoint);
    }
    else if(model_switch==3) 
    {
      picture_send();
    }
    else if(model_switch==4)
    {
      PID_Adjustment();
    }
    else if(model_switch==5)
    {
      Parameter_Adjustment();
    }
    else if(model_switch==16)
    {
      if(BeginFlag==0)
      {
        while(PTE7_I==0);
        
        if(!RealNoSignal||PTE6_I==0)
        {
          car_run=1;
          ChooseSide_Num=0;
          BeginFlag=1;
          TenSecondsRecordBegin=1;
          TenSecondsRecordLength=0;
        }
      }
    }
  } 
  
}