#ifndef PID_CALCULATE_H
#define PID_CALCULATE_H
#include "common.h"

/*--�ٶȣУɣĽṹ������---*/
typedef struct
{
  float Kp[2];//PID��������
  float Ki[2];
  float Kd[2];
  /*--- �����һ��Ϊ���� �ڶ���Ϊ����---*/
  float SpeedNow[2];//��ǰ�ٶ�
  float SpeedSet[2];//�����ٶ�
  float Error[2][2];//1->2 ��ǰ���e(k),��һ��e(k-1)
  float Integer[2];//�����ܺ�ֵ�����ͣ�
  float Deriv[2];//΢����ֵ����ǰ���-��һ����
  float Output[2];//���
}SpeedPID_TypeDef;

//pid�ṹ������
typedef struct
{
    float P;
    float I;
    float D;
    float Desired;                  
    float Error;
    float PreError;
    float PrePreError;
    float Integer;                 //������             
    float iLimit;                  //������ֵ
    float Deriv;                   //΢����
    float Output;
 }PID_TypeDef;

extern SpeedPID_TypeDef SpeedPID;
extern PID_TypeDef Turn_PID;
extern float SpeedTarget[2];//�ٶ�Ŀ��ֵ   
void SpeedPID_Cal(float TargetSpeed[2],float MotorPID_Speed_ave_L,float MotorPID_Speed_ave_R);//Ŀ��ֵ�Ͳ���ֵ
void Turn_PID_Cal(void);
void PID_Init(void);
void SetSpeed_Configuration(void);
void Jam_Restart(void);
   
//extern uint16 NOpointtime,WITHpointtime;
//extern uint8 Jamlock;
#endif