#include "common.h"
#include "servo_forFun.h" 
uint16 time_servo=SerMid;

void servo_pit_init(void)
{
  static GPIO_InitTypeDef servo_GPIO;  //������ų�ʼ��
  servo_GPIO.GPIO_PTx = PTD;
  servo_GPIO.GPIO_Pins = GPIO_Pin12;
  servo_GPIO.GPIO_Dir =DIR_OUTPUT;
  servo_GPIO.GPIO_Output=OUTPUT_L;
  LPLD_GPIO_Init(servo_GPIO);
  
  
  static PIT_InitTypeDef pit1_init_struct; //PIT1��ʱ�ж�  �������͵�ƽ
  pit1_init_struct.PIT_Pitx = PIT1;
  pit1_init_struct.PIT_PeriodUs   =1000000;     //
  pit1_init_struct.PIT_Isr = pit1_isr;   //�����жϺ���
  LPLD_PIT_Init(pit1_init_struct);  
  LPLD_PIT_EnableIrq(pit1_init_struct);
  PIT->CHANNEL[1].TCTRL &= ~PIT_TCTRL_TEN_MASK;//ֹͣ��ʱ
  
  
  static PIT_InitTypeDef pit2_init_struc_SERVO; //20ms��ʱ�ж�  50HZ�������ߵ�ƽ
  pit2_init_struc_SERVO.PIT_Pitx = PIT2;
  pit2_init_struc_SERVO.PIT_PeriodMs =20;     //
  pit2_init_struc_SERVO.PIT_Isr = servo_control;   //�����жϺ���
  LPLD_PIT_Init(pit2_init_struc_SERVO);  
  LPLD_PIT_EnableIrq(pit2_init_struc_SERVO);
  
}

void servo_control(void)  //PIT2���жϺ��� ���ߵ�ƽ
{
  if (time_servo>SerLeftMax)//ת���޷�
    time_servo=SerLeftMax;
  else if (time_servo<SerRightMin)
    time_servo=SerRightMin;
  
  PIT->CHANNEL[1].LDVAL = (time_servo)*(g_bus_clock / 1000000) - 1; //��������PIT1���ж�����
  PTD12_O = 1;                                  //���ߵ�ƽ
  PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;//��ʼ��ʱ 
}

void pit1_isr(void)  //PIT1���жϺ��� ���͵�ƽ
{
    PTD12_O = 0;
    PIT->CHANNEL[1].TCTRL &= ~PIT_TCTRL_TEN_MASK;//ֹͣ��ʱ
}


