#include "timer.h"
#include "camera.h"
#include "obstacle.h"

uint8 Line_Left_Flag=0,Line_Right_Flag=0,Circle_Left_Flag=0,Circle_Right_Flag=0;

uint32 PC;
void pit_init( void )
{
    static PIT_InitTypeDef pit0_init_struct;
    pit0_init_struct.PIT_Pitx = PIT0;
    pit0_init_struct.PIT_PeriodMs = 10;    //计算时间会到2ms，
    pit0_init_struct.PIT_Isr = pit0_isr;   //设置中断函数
    LPLD_PIT_Init( pit0_init_struct );
    LPLD_PIT_EnableIrq( pit0_init_struct );
}   

void pit0_isr( void )   //1900 Us
{
//    LPLD_PIT_Time_Start  (PIT3);   
    Figure_Process();
    Figure_Process_Two();
 // Noisefilter();//勿和Figure_Process共用
    
    if(RealNopoint==1)  PTE6_O = 1;
    else        PTE6_O = 0;
    
    PTE7_O=1;   //告诉主机初始化完毕
    
    if(CircleFlag==0)
    {
      if(HaveBarrier&&Validfiguresize<limit_size_S&&Barrier_y>(S_limit_slope_L*Barrier_x+S_limit_intercept_L)&&Barrier_y>(S_limit_slope_R*Barrier_x+S_limit_intercept_R)&&Barrier_y>=limit_range)//有行驶范围内的障碍，且在左侧
      {
        PTE8_O = 1;
        if(Barrier_x<limit_vertical)//在触发障碍标志范围内，且在远处，图像为信标正常大小之内
        {
          if(Line_Right_Flag!=1)
          {
            PTE9_O = 0;
            Line_Left_Flag=1;
            Line_Right_Flag=0;
          }
        }
        else
        {
          if(Line_Left_Flag!=1)
          {
            PTE9_O =1;
            Line_Right_Flag=1;
            Line_Left_Flag=0;
          }
        }
        
        if(Barrier_y>S_limit_a_L*Barrier_x*Barrier_x+S_limit_b_L*Barrier_x+S_limit_c_L)
        {
          PTE10_O = 1;
          Circle_Left_Flag=1;
        }
        else
        {
          PTE10_O = 0;
          Circle_Left_Flag=0;
        }
        
        if(Barrier_y>S_limit_a_R*Barrier_x*Barrier_x+S_limit_b_R*Barrier_x+S_limit_c_R)
        {
          PTE11_O = 1;
          Circle_Right_Flag=1;
        }
        else
        {
          PTE11_O = 0;
          Circle_Right_Flag=0;
        }
      }
      else if(HaveBarrier&&Validfiguresize<limit_size_S&&Barrier_y>S_limit_a_L*Barrier_x*Barrier_x+S_limit_b_L*Barrier_x+S_limit_c_L)
      {
        PTE8_O = 0;
        PTE10_O = 1;
        PTE11_O = 0;
        Circle_Left_Flag=1;
        Circle_Right_Flag=0;
        Line_Left_Flag=0;
        Line_Right_Flag=0;
      }
      else if(HaveBarrier&&Validfiguresize<limit_size_S&&Barrier_y>S_limit_a_R*Barrier_x*Barrier_x+S_limit_b_R*Barrier_x+S_limit_c_R)
      {
        PTE8_O = 0;
        PTE10_O = 0;
        PTE11_O = 1;
        Circle_Left_Flag=0;
        Circle_Right_Flag=1;
        Line_Left_Flag=0;
        Line_Right_Flag=0;
      }
      else
      {
        PTE8_O = 0;
        PTE9_O = 0;
        PTE10_O = 0;
        PTE11_O = 0;
        Line_Left_Flag=0;
        Line_Right_Flag=0;
        Circle_Left_Flag=0;
        Circle_Right_Flag=0;
      }
    }
    else if(CircleFlag==1)
    {
      if(HaveBarrier&&Validfiguresize<limit_size_C&&Barrier_y>(C_limit_slope_L*Barrier_x+C_limit_intercept_L)&&Barrier_y>(C_limit_slope_R*Barrier_x+C_limit_intercept_R)&&Barrier_y>=limit_range)//有行驶范围内的障碍，且在左侧
      {
        PTE8_O = 1;
        if(Barrier_x<limit_vertical)//在触发障碍标志范围内，且在远处，图像为信标正常大小之内
        {
          if(Line_Right_Flag!=1)
          {
            PTE9_O = 0;
            Line_Left_Flag=1;
            Line_Right_Flag=0;
          }
        }
        else
        {
          if(Line_Left_Flag!=1)
          {
            PTE9_O =1;
            Line_Right_Flag=1;
            Line_Left_Flag=0;
          }
        }
        
        if(Barrier_y>C_limit_a_L*Barrier_x*Barrier_x+C_limit_b_L*Barrier_x+C_limit_c_L)
        {
          PTE10_O = 1;
          Circle_Left_Flag=1;
        }
        else
        {
          PTE10_O = 0;
          Circle_Left_Flag=0;
        }
        
        if(Barrier_y>C_limit_a_R*Barrier_x*Barrier_x+C_limit_b_R*Barrier_x+C_limit_c_R)
        {
          PTE11_O = 1;
          Circle_Right_Flag=1;
        }
        else
        {
          PTE11_O = 0;
          Circle_Right_Flag=0;
        }
      }
      else if(HaveBarrier&&Validfiguresize<limit_size_C&&Barrier_y>C_limit_a_L*Barrier_x*Barrier_x+C_limit_b_L*Barrier_x+C_limit_c_L)
      {
        PTE8_O = 0;
        PTE10_O = 1;
        PTE11_O = 0;
        Circle_Left_Flag=1;
        Circle_Right_Flag=0;
        Line_Left_Flag=0;
        Line_Right_Flag=0;
      }
      else if(HaveBarrier&&Validfiguresize<limit_size_C&&Barrier_y>C_limit_a_R*Barrier_x*Barrier_x+C_limit_b_R*Barrier_x+C_limit_c_R)
      {
        PTE8_O = 0;
        PTE10_O = 0;
        PTE11_O = 1;
        Circle_Left_Flag=0;
        Circle_Right_Flag=1;
        Line_Left_Flag=0;
        Line_Right_Flag=0;
      }
      else
      {
        PTE8_O = 0;
        PTE9_O = 0;
        PTE10_O = 0;
        PTE11_O = 0;
        Line_Left_Flag=0;
        Line_Right_Flag=0;
        Circle_Left_Flag=0;
        Circle_Right_Flag=0;
      }
    }
    else
    {
      PTE8_O = 0;
      PTE9_O = 0;
      PTE10_O = 0;
      PTE11_O = 0;
      Line_Left_Flag=0;
      Line_Right_Flag=0;
      Circle_Left_Flag=0;
      Circle_Right_Flag=0;
    }
    
//     PC=LPLD_PIT_Time_GetUs(PIT3);
//     LPLD_PIT_Close(PIT3);
}


//void Key_delay( void )
//{
//    uint16 i, n;
//    for( i = 0; i < 19000; i++ )
//    {
//        for( n = 0; n < 50; n++ )
//        {
//            asm( "nop" );
//        }
//    }
//}