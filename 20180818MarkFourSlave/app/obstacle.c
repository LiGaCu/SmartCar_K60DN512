
#include "obstacle.h"
#include "common.h"
GPIO_InitTypeDef obinterrupt0_struct;
GPIO_InitTypeDef obinterrupt1_struct;
GPIO_InitTypeDef obinterrupt2_struct;
GPIO_InitTypeDef obinterrupt3_struct;
GPIO_InitTypeDef obinterrupt4_struct;
GPIO_InitTypeDef obinterrupt5_struct;
GPIO_InitTypeDef Buzzer_struct;
//����������������E26-AA16  E27-AA17 E28-AA18 A4-AA19 A5-AA12 A7-AA13�ɴ����ж�
void ob_gpio_init(void)
{
  /*---------------���š�������-------*/
  obinterrupt0_struct.GPIO_PTx = PTE;    //�ϰ����ң�����ת�� 
  obinterrupt0_struct.GPIO_Pins = GPIO_Pin11;   
  obinterrupt0_struct.GPIO_Dir = DIR_OUTPUT;        //���
  obinterrupt0_struct.GPIO_Output=OUTPUT_L ;
  obinterrupt0_struct.GPIO_PinControl = OUTPUT_SR_SLOW;//INPUT_PULL_UP|IRQC_FA;     //�ڲ�����|�½����ж�
  LPLD_GPIO_Init(obinterrupt0_struct);
  
  obinterrupt1_struct.GPIO_PTx = PTE;    //�ϰ����ң�����ת�� 
  obinterrupt1_struct.GPIO_Pins = GPIO_Pin10;   
  obinterrupt1_struct.GPIO_Dir = DIR_OUTPUT;        //���
  obinterrupt1_struct.GPIO_Output=OUTPUT_L ;
  obinterrupt1_struct.GPIO_PinControl = OUTPUT_SR_SLOW;//INPUT_PULL_UP|IRQC_FA;     //�ڲ�����|�½����ж�
  LPLD_GPIO_Init(obinterrupt1_struct);
  /*---------------С��š�������-------*/
  obinterrupt2_struct.GPIO_PTx = PTE;    //�ϰ����ң�����ת�� 
  obinterrupt2_struct.GPIO_Pins = GPIO_Pin9;   
  obinterrupt2_struct.GPIO_Dir = DIR_OUTPUT;        //���
  obinterrupt2_struct.GPIO_Output=OUTPUT_L ;
  obinterrupt2_struct.GPIO_PinControl = OUTPUT_SR_SLOW;//INPUT_PULL_UP|IRQC_FA;     //�ڲ�����|�½����ж�
  LPLD_GPIO_Init(obinterrupt2_struct);
  
  obinterrupt3_struct.GPIO_PTx = PTE; //�ϰ���������ת��   
  obinterrupt3_struct.GPIO_Pins = GPIO_Pin8;   
  obinterrupt3_struct.GPIO_Dir = DIR_OUTPUT;        //���
  obinterrupt3_struct.GPIO_Output=OUTPUT_L ;
  obinterrupt3_struct.GPIO_PinControl = OUTPUT_SR_SLOW;//INPUT_PULL_UP|IRQC_FA;     //�ڲ�����|�½����ж�
  LPLD_GPIO_Init(obinterrupt3_struct);
  /*--------------�����ź�-------------*/
  obinterrupt4_struct.GPIO_PTx = PTE;    //�ϰ����ң�����ת�� 
  obinterrupt4_struct.GPIO_Pins = GPIO_Pin6;   
  obinterrupt4_struct.GPIO_Dir = DIR_OUTPUT;        //���
  obinterrupt4_struct.GPIO_Output=OUTPUT_L ;
  obinterrupt4_struct.GPIO_PinControl = OUTPUT_SR_SLOW;//INPUT_PULL_UP|IRQC_FA;     //�ڲ�����|�½����ж�
  LPLD_GPIO_Init(obinterrupt4_struct);
  
  obinterrupt5_struct.GPIO_PTx = PTE;    //�ϰ����ң�����ת�� 
  obinterrupt5_struct.GPIO_Pins = GPIO_Pin7;   
  obinterrupt5_struct.GPIO_Dir = DIR_OUTPUT;        //���
  obinterrupt5_struct.GPIO_Output=OUTPUT_L ;
  obinterrupt5_struct.GPIO_PinControl = OUTPUT_SR_SLOW;//INPUT_PULL_UP|IRQC_FA;     //�ڲ�����|�½����ж�
  LPLD_GPIO_Init(obinterrupt5_struct);
//  
//  obinterrupt3_struct.GPIO_PTx = PTE; //�ϰ���������ת��   
//  obinterrupt3_struct.GPIO_Pins = GPIO_Pin8;   
//  obinterrupt3_struct.GPIO_Dir = DIR_OUTPUT;        //���
//  obinterrupt3_struct.GPIO_Output=OUTPUT_L ;
//  obinterrupt3_struct.GPIO_PinControl = OUTPUT_SR_SLOW;//INPUT_PULL_UP|IRQC_FA;     //�ڲ�����|�½����ж�
//  LPLD_GPIO_Init(obinterrupt3_struct);
}
