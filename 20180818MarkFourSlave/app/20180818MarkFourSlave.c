/*
  信标组从单片机程序
  避障
  红外计数
 */
#include "common.h"
#include "camera.h"
#include "timer.h"
#include "obstacle.h" 
#include "oled.h"
#include "freecars.h"
#include "myflash.h"
#include "botton_test.h"
   
extern uint8 NOpoint;
extern uint8 real_pointx,real_pointy;

void nvic_init(void) ;
void main (void)
{
  DisableInterrupts;
  nvic_init();
  camera_init();
  cameraTwo_init(); 
  uart0_init();
  led_gpio_int();
  LED_Init();
  botton_init_gpio();
  ob_gpio_init();
  pit_init();
  LPLD_Flash_Init();
  parameter_flash();
//  enable_irq(PORTB_IRQn);
  enable_irq(PORTC_IRQn);
  enable_irq(PIT0_IRQn);
  EnableInterrupts;
  while(1)
  {
    setting_process();
//   dis_bmp(ROW,COLUMN,Image1[0],150);
//   display_BarrierLine_100_60(limit_slope_L,limit_intercept_L);
//   display_BarrierLine_100_60(limit_slope_R,limit_intercept_R);
//   display_Line_coordinate_y_100_60(limit_slope_L,limit_slope_R,limit_intercept_L,limit_intercept_R,limit_range);
//   display_Line_coordinate_y_100_60(limit_slope_L,limit_slope_R,limit_intercept_L,limit_intercept_R,limit_horizontal);
//   
//   display_BarrierLine_90_60(limit_slope_L,limit_intercept_L);
//   display_BarrierLine_90_60(limit_slope_R,limit_intercept_R);
//   display_Line_coordinate_y_90_60(limit_slope_L,limit_slope_R,limit_intercept_L,limit_intercept_R,limit_range);
//   display_Line_coordinate_y_90_60(limit_slope_L,limit_slope_R,limit_intercept_L,limit_intercept_R,limit_horizontal);
//  
//   dis_bmp_half(ROW/2, COLUMN/2, Image2[0],setvalue);
//   LED_PrintValueC(100 ,0,Barrier_x);
//   LED_PrintValueC(100 ,1,Barrier_y);
//   LED_PrintValueFP(100 ,2,Validfiguresize,3);
//   LED_PrintValueC(100 ,4,CircleFlag);
//   LED_PrintValueFP(100 ,3,Line_Left_Flag,1);
//   LED_PrintValueFP(100 ,4,Line_Right_Flag,1);
//   LED_PrintValueFP(100 ,5,Circle_Left_Flag,1);
//   LED_PrintValueFP(100 ,6,Circle_Right_Flag,1);

//   dis_bmp_half(ROW_Two/2, COLUMN_Two/2, ImageTwo2[0],setvalue_Two);
//   LED_PrintValueC(100 ,0,real_pointx);
//   LED_PrintValueC(100 ,1,real_pointy);
//   LED_PrintValueC(100 ,4,RealNopoint);
    
//   picture_send();
//   picture_send_Two();
  } 
}

void nvic_init(void)  
{
   static NVIC_InitTypeDef nvic_init_struct0;
  nvic_init_struct0.NVIC_IRQChannel=PIT0_IRQn;//中断IRQn_Type号   
  nvic_init_struct0.NVIC_IRQChannelGroupPriority=NVIC_PriorityGroup_2 ;//NVIC 中断分组
  nvic_init_struct0.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级
  nvic_init_struct0.NVIC_IRQChannelSubPriority=1;//NVIC响应式优先级
  LPLD_NVIC_Init(nvic_init_struct0);
  
  static NVIC_InitTypeDef nvic_init_struct1;
  nvic_init_struct1.NVIC_IRQChannel=PORTB_IRQn;//中断IRQn_Type号        摄像头图像
  nvic_init_struct1.NVIC_IRQChannelGroupPriority=NVIC_PriorityGroup_2;//NVIC 中断分组
  nvic_init_struct1.NVIC_IRQChannelPreemptionPriority=0;//NVIC抢占式优先级
  nvic_init_struct1.NVIC_IRQChannelSubPriority=0;//NVIC响应式优先级
  LPLD_NVIC_Init(nvic_init_struct1);
  
  static NVIC_InitTypeDef nvic_init_struct2;
  nvic_init_struct2.NVIC_IRQChannel=PORTC_IRQn;//中断IRQn_Type号        摄像头图像
  nvic_init_struct2.NVIC_IRQChannelGroupPriority=NVIC_PriorityGroup_2;//NVIC 中断分组
  nvic_init_struct2.NVIC_IRQChannelPreemptionPriority=0;//NVIC抢占式优先级
  nvic_init_struct2.NVIC_IRQChannelSubPriority=0;//NVIC响应式优先级
  LPLD_NVIC_Init(nvic_init_struct2);
  
}

