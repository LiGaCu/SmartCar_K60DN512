
#include "obstacle.h"
#include "common.h"
GPIO_InitTypeDef obinterrupt0_struct;
GPIO_InitTypeDef obinterrupt1_struct;
GPIO_InitTypeDef obinterrupt2_struct;
GPIO_InitTypeDef obinterrupt3_struct;
GPIO_InitTypeDef obinterrupt4_struct;
GPIO_InitTypeDef obinterrupt5_struct;
GPIO_InitTypeDef Buzzer_struct;
//单纯主从引脚连接E26-AA16  E27-AA17 E28-AA18 A4-AA19 A5-AA12 A7-AA13可触发中断
void ob_gpio_init(void)
{
  /*---------------大打脚————-------*/
  obinterrupt0_struct.GPIO_PTx = PTE;    //障碍在右，向左转角 
  obinterrupt0_struct.GPIO_Pins = GPIO_Pin11;   
  obinterrupt0_struct.GPIO_Dir = DIR_OUTPUT;        //输出
  obinterrupt0_struct.GPIO_Output=OUTPUT_L ;
  obinterrupt0_struct.GPIO_PinControl = OUTPUT_SR_SLOW;//INPUT_PULL_UP|IRQC_FA;     //内部上拉|下降沿中断
  LPLD_GPIO_Init(obinterrupt0_struct);
  
  obinterrupt1_struct.GPIO_PTx = PTE;    //障碍在右，向左转角 
  obinterrupt1_struct.GPIO_Pins = GPIO_Pin10;   
  obinterrupt1_struct.GPIO_Dir = DIR_OUTPUT;        //输出
  obinterrupt1_struct.GPIO_Output=OUTPUT_L ;
  obinterrupt1_struct.GPIO_PinControl = OUTPUT_SR_SLOW;//INPUT_PULL_UP|IRQC_FA;     //内部上拉|下降沿中断
  LPLD_GPIO_Init(obinterrupt1_struct);
  /*---------------小打脚————-------*/
  obinterrupt2_struct.GPIO_PTx = PTE;    //障碍在右，向左转角 
  obinterrupt2_struct.GPIO_Pins = GPIO_Pin9;   
  obinterrupt2_struct.GPIO_Dir = DIR_OUTPUT;        //输出
  obinterrupt2_struct.GPIO_Output=OUTPUT_L ;
  obinterrupt2_struct.GPIO_PinControl = OUTPUT_SR_SLOW;//INPUT_PULL_UP|IRQC_FA;     //内部上拉|下降沿中断
  LPLD_GPIO_Init(obinterrupt2_struct);
  
  obinterrupt3_struct.GPIO_PTx = PTE; //障碍在左，向右转角   
  obinterrupt3_struct.GPIO_Pins = GPIO_Pin8;   
  obinterrupt3_struct.GPIO_Dir = DIR_OUTPUT;        //输出
  obinterrupt3_struct.GPIO_Output=OUTPUT_L ;
  obinterrupt3_struct.GPIO_PinControl = OUTPUT_SR_SLOW;//INPUT_PULL_UP|IRQC_FA;     //内部上拉|下降沿中断
  LPLD_GPIO_Init(obinterrupt3_struct);
  /*--------------有无信号-------------*/
  obinterrupt4_struct.GPIO_PTx = PTE;    //障碍在右，向左转角 
  obinterrupt4_struct.GPIO_Pins = GPIO_Pin6;   
  obinterrupt4_struct.GPIO_Dir = DIR_OUTPUT;        //输出
  obinterrupt4_struct.GPIO_Output=OUTPUT_L ;
  obinterrupt4_struct.GPIO_PinControl = OUTPUT_SR_SLOW;//INPUT_PULL_UP|IRQC_FA;     //内部上拉|下降沿中断
  LPLD_GPIO_Init(obinterrupt4_struct);
  
  obinterrupt5_struct.GPIO_PTx = PTE;    //障碍在右，向左转角 
  obinterrupt5_struct.GPIO_Pins = GPIO_Pin7;   
  obinterrupt5_struct.GPIO_Dir = DIR_OUTPUT;        //输出
  obinterrupt5_struct.GPIO_Output=OUTPUT_L ;
  obinterrupt5_struct.GPIO_PinControl = OUTPUT_SR_SLOW;//INPUT_PULL_UP|IRQC_FA;     //内部上拉|下降沿中断
  LPLD_GPIO_Init(obinterrupt5_struct);
//  
//  obinterrupt3_struct.GPIO_PTx = PTE; //障碍在左，向右转角   
//  obinterrupt3_struct.GPIO_Pins = GPIO_Pin8;   
//  obinterrupt3_struct.GPIO_Dir = DIR_OUTPUT;        //输出
//  obinterrupt3_struct.GPIO_Output=OUTPUT_L ;
//  obinterrupt3_struct.GPIO_PinControl = OUTPUT_SR_SLOW;//INPUT_PULL_UP|IRQC_FA;     //内部上拉|下降沿中断
//  LPLD_GPIO_Init(obinterrupt3_struct);
}
