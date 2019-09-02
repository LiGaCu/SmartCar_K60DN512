#include "common.h"
#include "QD.h"
#include "PID_Calculate.h"
#include "motor_forFun.h" 
#include "freecars.h"
#include "camera_test.h"
#include "servo_forFun.h" 

#define TurnAssistAngel_L SerLeftMax-30
#define TurnAssistAngel_R SerRightMin+30
float TurnAssistSpeed_L[2]={100,450};
float TurnAssistSpeed_R[2]={450,100};
float ObstacleOnL_LineSpeed[2]={250,50};
float ObstacleOnR_LineSpeed[2]={50,250};
float ObstacleOnL_CircleSpeed[2]={300,300};
float ObstacleOnR_CircleSpeed[2]={300,300};

float SpeedTarget__R[2]={450,100};//170��-80
float SpeedTarget__L[2]={100,450};
float SpeedTarget_ = 550;

#define SpeedPID_Kp_Obstacle_LineSpeed 75         //100
#define SpeedPID_Ki_Obstacle_LineSpeed 0.2 
#define SpeedPID_Kd_Obstacle_LineSpeed 0

#define SpeedPID_Kp_Obstacle_CircleSpeed 75
#define SpeedPID_Ki_Obstacle_CircleSpeed 0.2
#define SpeedPID_Kd_Obstacle_CircleSpeed 0

#define SpeedPID_Kp_TurnAssistSpeed 75              //55
#define SpeedPID_Ki_TurnAssistSpeed 0.8  
#define SpeedPID_Kd_TurnAssistSpeed 0  


#define SpeedPID_Kp_turn 75
#define SpeedPID_Ki_turn 0.8
#define SpeedPID_Kd_turn 0

#define SpeedPID_Kp_SpeedTarget_ 100                    //75
#define SpeedPID_Ki_normalspeed 0.8
#define SpeedPID_Kd_normalspeed 0

#define SpeedPID_Kp_lowspeed 75                        //55
#define SpeedPID_Ki_lowspeed 0.2
#define SpeedPID_Kd_lowspeed 0


int16 speed_right,speed_left;
float MotorPID_Speed_ave[2];
float SpeedPID_CarSpeed_L[4],SpeedPID_CarSpeed_R[4];
float SpeedMeasure[2];//�������ٶ�д��һ������ 
float SpeedTarget[2];//�ٶ�Ŀ��ֵ      

uint8 TurnInAdvanceDistance=48,TurnInAdvanceDistance2=39;       //67,56        //60,50
//uint8 brake_start_first=38;//50         //44    //33
//uint8 brake_start_second=28;//44        //39    //28
//uint8 brake_start_third=24;//39         //35    //24
//uint8 brake_start_forth=22;//35         //32    //22
//uint8 brake_start_fifth=19;//33         //31    //20

float brake_speed_first=180;            //180   //180
float brake_speed_second=250;           //200   //250
float brake_speed_third=350;            //350   //350
float brake_speed_forth=450;            //450   //450
float brake_speed_fifth=500;            //450   //500
float speedup_first = 280;
float speedup_second = 320;

float emergy_speed=500;
float acceleration_speed=350;
uint8 MadStopLock,AccelerationLock;

uint8 turn_inadvance_lock=0,turn_inadvance_lock2=0;
uint8 zero_lock1=0;
uint8 zero_lock2=0;
uint8 zero_lock3=0;
uint8 zero_lock4=0;
uint8 zero_lock5=0;
uint8 CleanIntegerFlag1=0;
uint8 CleanIntegerFlag2=0;
uint8 CleanIntegerFlag3=0;

int16 QD_RIGHT,QD_LEFT;
uint8 Turn_lock;//��ͼ���״ο����ű��λ�þ���ƫ������
uint8 RightSide_Go = 1,Surround_Go = 1;//Ϊ1��ƫ��,Ϊ0��ƫ��

uint8 ChooseSide_Buffer[18]={1,1,1,1,0,1,1,1,1,1,0,1,2,2,2,2,2,2};
uint8 ChooseSide_Num=0;
uint8 ChooseSide_Overflow=0;
uint16 TenSecondsRecordLength=0;
vuint16 TenSecondsRecordBegin=0;
uint8 distancelock,lastreal_y,lastreal_x;

int nobrakeNum=19,StopNum=1;
uint32 ProgrammeRunTime;
uint8 emergyflag,speedupflag;
uint8 IntelligentSteeringLock;

uint8 Complemented_Y,Complemented_X;
uint16 Nopointtime=12,Withpointtime,LastperiodWithpointtime;
uint8 RealNoSignal=1,NopointComplementWork;
uint8 Nopoint_Y_Complement[11],Nopoint_X_Complement[11];

void QD_init(void)
{
   /////////////��//////////ʼ��FTM1
  static GPIO_InitTypeDef encoder_init_struct1;
  encoder_init_struct1.GPIO_PTx = PTA;
  encoder_init_struct1.GPIO_Pins = GPIO_Pin8|GPIO_Pin9;
  encoder_init_struct1.GPIO_Dir = DIR_INPUT;        //����
  encoder_init_struct1.GPIO_PinControl = INPUT_PULL_UP;
  LPLD_GPIO_Init(encoder_init_struct1);
  
  static FTM_InitTypeDef ftm_init_struct1;
  ftm_init_struct1.FTM_Ftmx = FTM1;              //ֻ��FTM1��FTM2���������빦��
  ftm_init_struct1.FTM_Mode = FTM_MODE_QD;       //�������빦��
  ftm_init_struct1.FTM_QdMode = QD_MODE_CNTDIR;    //�����ͷ������
  //��ʼ��FTM
  LPLD_FTM_Init(ftm_init_struct1);
  //ʹ��AB������ͨ��
  //PTA8���Ž�A�����롢PTA9���Ž�B������
  LPLD_FTM_QD_Enable(FTM1, PTA8, PTA9);  
  
  /////////////��//////////ʼ��FTM2
  static GPIO_InitTypeDef encoder_init_struct;
  encoder_init_struct.GPIO_PTx = PTB;
  encoder_init_struct.GPIO_Pins = GPIO_Pin18|GPIO_Pin19;
  encoder_init_struct.GPIO_Dir = DIR_INPUT;        //����
  encoder_init_struct.GPIO_PinControl = INPUT_PULL_UP;
  LPLD_GPIO_Init(encoder_init_struct);
  
  static FTM_InitTypeDef ftm_init_struct;
  ftm_init_struct.FTM_Ftmx = FTM2;              //ֻ��FTM1��FTM2���������빦��
  ftm_init_struct.FTM_Mode = FTM_MODE_QD;       //�������빦��
  ftm_init_struct.FTM_QdMode = QD_MODE_CNTDIR;    //�����ͷ������
  //��ʼ��FTM
  LPLD_FTM_Init(ftm_init_struct);
  //ʹ��AB������ͨ��
  //PTB18���Ž�A�����롢PTB19���Ž�B������
  LPLD_FTM_QD_Enable(FTM2, PTB18, PTB19);  


  
   LPLD_FTM_ClearCounter(FTM1);                //��ռ�����
   LPLD_FTM_ClearCounter(FTM2);                //��ռ�����
}

void pit0_init(void)
{
  static PIT_InitTypeDef pit0_init_struc; //5ms��ʱ�ж�
  pit0_init_struc.PIT_Pitx = PIT0;
  pit0_init_struc.PIT_PeriodMs =5;     
  pit0_init_struc.PIT_Isr = pit0_isr;   //�����жϺ���
  LPLD_PIT_Init(pit0_init_struc);  
  disable_irq(PIT0_IRQn);
  
}
void Obstacle_init(void)
{
  static GPIO_InitTypeDef Obstacle_struct;//���ϰ�
  Obstacle_struct.GPIO_PTx = PTE;    
  Obstacle_struct.GPIO_Pins = GPIO_Pin11|GPIO_Pin10|GPIO_Pin9|GPIO_Pin8|GPIO_Pin7|GPIO_Pin6;   
  Obstacle_struct.GPIO_Dir = DIR_INPUT;        //����
  Obstacle_struct.GPIO_PinControl = INPUT_PULL_DOWN;//�����ش����ж�
  LPLD_GPIO_Init(Obstacle_struct);
}

void pit0_isr(void)                                                             //3000 Us
{
//  LPLD_PIT_Time_Start  (PIT3);
  QD_LEFT = LPLD_FTM_GetCounter(FTM1);//�õ�16λ�޷�����
  QD_RIGHT= 65536-LPLD_FTM_GetCounter(FTM2);
  
  LPLD_FTM_ClearCounter(FTM1);                //��ռ�����
  LPLD_FTM_ClearCounter(FTM2);                //��ռ�����
  

  speed_right = (int16) (QD_RIGHT)*100*200/ CAR_PLUSE_ONE_METER ;//�ٶȼ���cm/s
  speed_left  = (int16) (QD_LEFT)*100*200/ CAR_PLUSE_ONE_METER ;//�ٶȼ���cm/s
  
  uint8 i;
  for (i = 3; i > 0; i--)
  {
     SpeedPID_CarSpeed_L[i] =  SpeedPID_CarSpeed_L[i - 1];
     SpeedPID_CarSpeed_R[i] =  SpeedPID_CarSpeed_R[i - 1];
  }
  SpeedPID_CarSpeed_L[0] = speed_left;
  SpeedPID_CarSpeed_R[0] = speed_right;
  
  MotorPID_Speed_ave[0] = 0.6*SpeedPID_CarSpeed_L[0] + 0.2*SpeedPID_CarSpeed_L[1] + 0.1*SpeedPID_CarSpeed_L[2]+ 0.1*SpeedPID_CarSpeed_L[3];
  MotorPID_Speed_ave[1] = 0.6*SpeedPID_CarSpeed_R[0] + 0.2*SpeedPID_CarSpeed_R[1] + 0.1*SpeedPID_CarSpeed_R[2]+ 0.1*SpeedPID_CarSpeed_R[3];//�ٶ��˲�
  
  Calculate_Middle_Point();//�ű��е����  

   
  /*-------------------------------10Hz����----------------------------------*/
  if(NOpoint)
  {
    Nopointtime++;
    if(NopointComplementWork)
    {
      if(Nopointtime==1) LastperiodWithpointtime=Withpointtime;
      if((LastperiodWithpointtime>2)&&(Nopoint_Y_Complement[12-LastperiodWithpointtime]!=0))
      {
        Complemented_Y=(Nopoint_Y_Complement[10]+Nopointtime*(Nopoint_Y_Complement[10]-Nopoint_Y_Complement[12-LastperiodWithpointtime])/(LastperiodWithpointtime-2));
        Complemented_X=(Nopoint_X_Complement[10]+Nopointtime*(Nopoint_X_Complement[10]-Nopoint_X_Complement[12-LastperiodWithpointtime])/(LastperiodWithpointtime-2));
        if(Complemented_Y>4&&Complemented_Y<75&&Complemented_X>1&&Complemented_X<180)//&&Complemented_Y>30&&Complemented_Y>(DistortionLimit_L_Slope*Complemented_X+DistortionLimit_L_Intercept)&&Complemented_Y>(DistortionLimit_R_Slope*Complemented_X+DistortionLimit_R_Intercept)
        {
//          if(Complemented_Y>(DistortionLimit_L_Slope*Complemented_X+DistortionLimit_L_Intercept)&&Complemented_Y>(DistortionLimit_R_Slope*Complemented_X+DistortionLimit_R_Intercept))
//          real_pointy=Complemented_Y;
//          else
//          real_pointy=lastreal_y;
          real_pointy=Complemented_Y;
          real_pointx=Complemented_X;
        }
        else
        {
          real_pointy=lastreal_y;
          real_pointx=lastreal_x;
        }
      }
      else
      {
        real_pointy=lastreal_y;
        real_pointx=lastreal_x;
      }
    }
    Withpointtime=0;
  }
  else
  {
    for(i=0;i<10;i++)
    {
      Nopoint_Y_Complement[i]=Nopoint_Y_Complement[i+1];
    }
    Nopoint_Y_Complement[10]=real_pointy;
    
    for(i=0;i<10;i++)
    {
      Nopoint_X_Complement[i]=Nopoint_X_Complement[i+1];
    }
    Nopoint_X_Complement[10]=real_pointx;

    NopointComplementWork=1;
    
    Withpointtime++;
    
    if(Withpointtime==1)
    {
      if(TenSecondsRecordBegin==1)
      {
        TenSecondsDataSave(7,( uint16 )Nopoint_Y_Complement[10]);
        TenSecondsDataSave(0,( uint16 )Nopoint_X_Complement[10]);
      }
      real_pointy=lastreal_y;
      real_pointx=lastreal_x;
      Nopoint_Y_Complement[10]=lastreal_y;
      Nopoint_X_Complement[10]=lastreal_x;
    }
    else if(Withpointtime==11&&(Nopoint_Y_Complement[9]!=0))
    {
      if(TenSecondsRecordBegin==1)
      {
        TenSecondsDataSave(7,( uint16 )Nopoint_Y_Complement[10]);
        TenSecondsDataSave(0,( uint16 )Nopoint_X_Complement[10]);
      }
      real_pointy=Nopoint_Y_Complement[9]+(Nopoint_Y_Complement[9]-Nopoint_Y_Complement[1])/9;
      real_pointx=Nopoint_X_Complement[9]+(Nopoint_X_Complement[9]-Nopoint_X_Complement[1])/9;
      Nopoint_Y_Complement[10]=Nopoint_Y_Complement[9]+(Nopoint_Y_Complement[9]-Nopoint_Y_Complement[1])/9;
      Nopoint_X_Complement[10]=Nopoint_X_Complement[9]+(Nopoint_X_Complement[9]-Nopoint_X_Complement[1])/9;
    }
    Nopointtime=0;
  }
  if(Withpointtime>11)
  {
    memset(Nopoint_Y_Complement,0,sizeof(Nopoint_Y_Complement));
    memset(Nopoint_X_Complement,0,sizeof(Nopoint_X_Complement));
    Withpointtime=0;
  }
  if(Nopointtime>11) 
  {
    RealNoSignal=1;
    NopointComplementWork=0;
    setvalue=40;
    memset(Nopoint_Y_Complement,0,sizeof(Nopoint_Y_Complement));
    memset(Nopoint_X_Complement,0,sizeof(Nopoint_X_Complement));
  }
  else 
  {
    if(Withpointtime!=1&&LastperiodWithpointtime!=1)
    {
      RealNoSignal=0;
      if(real_pointx>Distortion_200[real_pointy][0]&&real_pointx<Distortion_200[real_pointy][1])
      {
        setvalue=70;
      }
    }
  }
  if(Nopointtime>400)
  {
    Nopointtime=12;
  }
  /*------------------ת�򲿷�---------------------------------*/
  if( !RealNoSignal )  
  {
    if(Turn_lock == 0)//��ͼ���һ�ο����ű��ƫ�������趨
    {
      Turn_lock = 1;      //����ƫ������
      switch(ChooseSide_Num)
      {
        case 0:RightSide_Go=ChooseSide_Buffer[0];break;
        case 1:RightSide_Go=ChooseSide_Buffer[1];break;
        case 2:RightSide_Go=ChooseSide_Buffer[2];break;
        case 3:RightSide_Go=ChooseSide_Buffer[3];break;
        case 4:RightSide_Go=ChooseSide_Buffer[4];break;
        case 5:RightSide_Go=ChooseSide_Buffer[5];break;
        case 6:RightSide_Go=ChooseSide_Buffer[6];break;
        case 7:RightSide_Go=ChooseSide_Buffer[7];break;
        case 8:RightSide_Go=ChooseSide_Buffer[8];break;
        case 9:RightSide_Go=ChooseSide_Buffer[9];break;
        case 10:RightSide_Go=ChooseSide_Buffer[10];break;    
        case 11:RightSide_Go=ChooseSide_Buffer[11];break;
        case 12:RightSide_Go=ChooseSide_Buffer[12];break;
        case 13:RightSide_Go=ChooseSide_Buffer[13];break;
        case 14:RightSide_Go=ChooseSide_Buffer[14];break;
        case 15:RightSide_Go=ChooseSide_Buffer[15];break;
        case 16:RightSide_Go=ChooseSide_Buffer[16];break;
        case 17:RightSide_Go=ChooseSide_Buffer[17];break;//
        case 18:RightSide_Go=2;ChooseSide_Overflow=1;break; 
      }
      if(ChooseSide_Num==StopNum)
      {
        TenSecondsRecordBegin=0;car_run=0;
      }
      if(RightSide_Go==2||IntelligentSteeringLock==1)
      {
        if(real_pointx>SCenter) //�ű���ұ߽���
          RightSide_Go = 0;  //��ƫ��
        else
          RightSide_Go = 1; //��ƫ��
      }
    }
    
    Middle_Point_Configure();
    
    Turn_PID_Cal();//ת��PID����,�õ�Turn_PID.Output
      
    /*--------------------------------�ٶ�����--------------------------------*/
    if(!(real_pointx>Distortion_140[real_pointy][0]&&real_pointx<Distortion_140[real_pointy][1])) //����ִ��
    {
      if(Turn_PID.Output<TurnAssistAngel_L&&Turn_PID.Output>TurnAssistAngel_R)
      {
        if(PTE8_I==1&&PTE9_I==0)
        {
          time_servo=SerRightMin;
          SpeedTarget[0]=ObstacleOnL_LineSpeed[0];
          SpeedTarget[1]=ObstacleOnL_LineSpeed[1];
          SpeedPID.Kp[0]= SpeedPID_Kp_Obstacle_LineSpeed;
          SpeedPID.Kp[1]= SpeedPID_Kp_Obstacle_LineSpeed;
          SpeedPID.Ki[0]=SpeedPID_Ki_Obstacle_LineSpeed;
          SpeedPID.Ki[1]=SpeedPID_Ki_Obstacle_LineSpeed;
          SpeedPID.Kd[0]=SpeedPID_Kd_Obstacle_LineSpeed;
          SpeedPID.Kd[1]=SpeedPID_Kd_Obstacle_LineSpeed;
        }
        else if(PTE8_I==1&&PTE9_I==1)
        {
          time_servo=SerLeftMax;
          SpeedTarget[0]=ObstacleOnR_LineSpeed[0];
          SpeedTarget[1]=ObstacleOnR_LineSpeed[1];
          SpeedPID.Kp[0]= SpeedPID_Kp_Obstacle_LineSpeed;
          SpeedPID.Kp[1]= SpeedPID_Kp_Obstacle_LineSpeed;
          SpeedPID.Ki[0]=SpeedPID_Ki_Obstacle_LineSpeed;
          SpeedPID.Ki[1]=SpeedPID_Ki_Obstacle_LineSpeed;
          SpeedPID.Kd[0]=SpeedPID_Kd_Obstacle_LineSpeed;
          SpeedPID.Kd[1]=SpeedPID_Kd_Obstacle_LineSpeed;
        }
        else
        {
          time_servo=(uint32)Turn_PID.Output;
          SpeedTarget[0]=SpeedTarget_;
          SpeedTarget[1]=SpeedTarget_;
          SpeedPID.Kp[0]=SpeedPID_Kp_SpeedTarget_;
          SpeedPID.Kp[1]=SpeedPID_Kp_SpeedTarget_;
          SpeedPID.Ki[0]=SpeedPID_Ki_normalspeed;
          SpeedPID.Ki[1]=SpeedPID_Ki_normalspeed;
          SpeedPID.Kd[0]=SpeedPID_Kd_normalspeed;
          SpeedPID.Kd[1]=SpeedPID_Kd_normalspeed;
        }
      }
      else                                               //���ٸ���ר��ִ�з�Χ
      {
        if(PTE8_I==1&&PTE10_I==1)
        {
          time_servo=SerRightMin;
          SpeedTarget[0]=ObstacleOnL_LineSpeed[0];
          SpeedTarget[1]=ObstacleOnL_LineSpeed[1];
          SpeedPID.Kp[0]= SpeedPID_Kp_Obstacle_LineSpeed;
          SpeedPID.Kp[1]= SpeedPID_Kp_Obstacle_LineSpeed;
          SpeedPID.Ki[0]=SpeedPID_Ki_Obstacle_LineSpeed;
          SpeedPID.Ki[1]=SpeedPID_Ki_Obstacle_LineSpeed;
          SpeedPID.Kd[0]=SpeedPID_Kd_Obstacle_LineSpeed;
          SpeedPID.Kd[1]=SpeedPID_Kd_Obstacle_LineSpeed;
        }
        else if(PTE8_I==1&&PTE11_I==1)
        {
          time_servo=SerLeftMax;
          SpeedTarget[0]=ObstacleOnR_LineSpeed[0];
          SpeedTarget[1]=ObstacleOnR_LineSpeed[1];
          SpeedPID.Kp[0]= SpeedPID_Kp_Obstacle_LineSpeed;
          SpeedPID.Kp[1]= SpeedPID_Kp_Obstacle_LineSpeed;
          SpeedPID.Ki[0]=SpeedPID_Ki_Obstacle_LineSpeed;
          SpeedPID.Ki[1]=SpeedPID_Ki_Obstacle_LineSpeed;
          SpeedPID.Kd[0]=SpeedPID_Kd_Obstacle_LineSpeed;
          SpeedPID.Kd[1]=SpeedPID_Kd_Obstacle_LineSpeed;
        }
        else if(PTE8_I==0&&PTE10_I==1&&Turn_PID.Output>TurnAssistAngel_L)
        {
          time_servo=SerMid;
          SpeedTarget[0]=ObstacleOnL_CircleSpeed[0];
          SpeedTarget[1]=ObstacleOnL_CircleSpeed[1];
          SpeedPID.Kp[0]= SpeedPID_Kp_Obstacle_CircleSpeed;
          SpeedPID.Kp[1]= SpeedPID_Kp_Obstacle_CircleSpeed;
          SpeedPID.Ki[0]=SpeedPID_Ki_Obstacle_CircleSpeed;
          SpeedPID.Ki[1]=SpeedPID_Ki_Obstacle_CircleSpeed;
          SpeedPID.Kd[0]=SpeedPID_Kd_Obstacle_CircleSpeed;
          SpeedPID.Kd[1]=SpeedPID_Kd_Obstacle_CircleSpeed;
        }
        else if(PTE8_I==0&&PTE11_I==1&&Turn_PID.Output<TurnAssistAngel_R)
        {
          time_servo=SerMid;
          SpeedTarget[0]=ObstacleOnR_CircleSpeed[0];
          SpeedTarget[1]=ObstacleOnR_CircleSpeed[1];
          SpeedPID.Kp[0]= SpeedPID_Kp_Obstacle_CircleSpeed;
          SpeedPID.Kp[1]= SpeedPID_Kp_Obstacle_CircleSpeed;
          SpeedPID.Ki[0]=SpeedPID_Ki_Obstacle_CircleSpeed;
          SpeedPID.Ki[1]=SpeedPID_Ki_Obstacle_CircleSpeed;
          SpeedPID.Kd[0]=SpeedPID_Kd_Obstacle_CircleSpeed;
          SpeedPID.Kd[1]=SpeedPID_Kd_Obstacle_CircleSpeed;
        }
        else
        {
          time_servo=(uint32)Turn_PID.Output ;
          if(Turn_PID.Output>=TurnAssistAngel_L)
          {
            SpeedTarget[0]=TurnAssistSpeed_L[0];
            SpeedTarget[1]=TurnAssistSpeed_L[1];
            SpeedPID.Kp[0]= SpeedPID_Kp_TurnAssistSpeed;
            SpeedPID.Kp[1]= SpeedPID_Kp_TurnAssistSpeed;
            SpeedPID.Ki[0]=SpeedPID_Ki_TurnAssistSpeed;
            SpeedPID.Ki[1]=SpeedPID_Ki_TurnAssistSpeed;
            SpeedPID.Kd[0]=SpeedPID_Kd_TurnAssistSpeed;
            SpeedPID.Kd[1]=SpeedPID_Kd_TurnAssistSpeed;
          }
          else if(Turn_PID.Output<=TurnAssistAngel_R)
          {
            SpeedTarget[0]=TurnAssistSpeed_R[0];
            SpeedTarget[1]=TurnAssistSpeed_R[1];
            SpeedPID.Kp[0]= SpeedPID_Kp_TurnAssistSpeed;
            SpeedPID.Kp[1]= SpeedPID_Kp_TurnAssistSpeed;
            SpeedPID.Ki[0]=SpeedPID_Ki_TurnAssistSpeed;
            SpeedPID.Ki[1]=SpeedPID_Ki_TurnAssistSpeed;
            SpeedPID.Kd[0]=SpeedPID_Kd_TurnAssistSpeed;
            SpeedPID.Kd[1]=SpeedPID_Kd_TurnAssistSpeed;
          }
        }
      }
    }
    else //���ϲ�ִ�з�Χ��
    {
      time_servo=(uint32)Turn_PID.Output ;//������
      if(Turn_PID.Output>TurnAssistAngel_L)
      {
        SpeedTarget[0]=TurnAssistSpeed_L[0];
        SpeedTarget[1]=TurnAssistSpeed_L[1];
        SpeedPID.Kp[0]= SpeedPID_Kp_TurnAssistSpeed;
        SpeedPID.Kp[1]= SpeedPID_Kp_TurnAssistSpeed;
        SpeedPID.Ki[0]=SpeedPID_Ki_TurnAssistSpeed;
        SpeedPID.Ki[1]=SpeedPID_Ki_TurnAssistSpeed;
        SpeedPID.Kd[0]=SpeedPID_Kd_TurnAssistSpeed;
        SpeedPID.Kd[1]=SpeedPID_Kd_TurnAssistSpeed;
      }
      else if(Turn_PID.Output<TurnAssistAngel_R)
      {
        SpeedTarget[0]=TurnAssistSpeed_R[0];
        SpeedTarget[1]=TurnAssistSpeed_R[1];
        SpeedPID.Kp[0]= SpeedPID_Kp_TurnAssistSpeed;
        SpeedPID.Kp[1]= SpeedPID_Kp_TurnAssistSpeed;
        SpeedPID.Ki[0]=SpeedPID_Ki_TurnAssistSpeed;
        SpeedPID.Ki[1]=SpeedPID_Ki_TurnAssistSpeed;
        SpeedPID.Kd[0]=SpeedPID_Kd_TurnAssistSpeed;
        SpeedPID.Kd[1]=SpeedPID_Kd_TurnAssistSpeed;
      }
      else
      {
        SpeedTarget[0]=SpeedTarget_;
        SpeedTarget[1]=SpeedTarget_;
        SpeedPID.Kp[0]=SpeedPID_Kp_SpeedTarget_;
        SpeedPID.Kp[1]=SpeedPID_Kp_SpeedTarget_;
        SpeedPID.Ki[0]=SpeedPID_Ki_normalspeed;
        SpeedPID.Ki[1]=SpeedPID_Ki_normalspeed;
        SpeedPID.Kd[0]=SpeedPID_Kd_normalspeed;
        SpeedPID.Kd[1]=SpeedPID_Kd_normalspeed;
      }
    }
  }
  else if(RealNoSignal)//������ͼ�����Ѿ�����ʱת����� 
  {
      if(ChooseSide_Num==18) ChooseSide_Overflow=1;
      if(PTE8_I==1&&PTE10_I==1)
      {
        time_servo=SerRightMin;
        SpeedTarget[0]=ObstacleOnL_LineSpeed[0];
        SpeedTarget[1]=ObstacleOnL_LineSpeed[1];
        SpeedPID.Kp[0]= SpeedPID_Kp_Obstacle_LineSpeed;
        SpeedPID.Kp[1]= SpeedPID_Kp_Obstacle_LineSpeed;
        SpeedPID.Ki[0]=SpeedPID_Ki_Obstacle_LineSpeed;
        SpeedPID.Ki[1]=SpeedPID_Ki_Obstacle_LineSpeed;
        SpeedPID.Kd[0]=SpeedPID_Kd_Obstacle_LineSpeed;
        SpeedPID.Kd[1]=SpeedPID_Kd_Obstacle_LineSpeed;
      }
      else if(PTE8_I==1&&PTE11_I==1)
      {
        time_servo=SerLeftMax;
        SpeedTarget[0]=ObstacleOnR_LineSpeed[0];
        SpeedTarget[1]=ObstacleOnR_LineSpeed[1];
        SpeedPID.Kp[0]= SpeedPID_Kp_Obstacle_LineSpeed;
        SpeedPID.Kp[1]= SpeedPID_Kp_Obstacle_LineSpeed;
        SpeedPID.Ki[0]=SpeedPID_Ki_Obstacle_LineSpeed;
        SpeedPID.Ki[1]=SpeedPID_Ki_Obstacle_LineSpeed;
        SpeedPID.Kd[0]=SpeedPID_Kd_Obstacle_LineSpeed;
        SpeedPID.Kd[1]=SpeedPID_Kd_Obstacle_LineSpeed;
      }
      else if(PTE8_I==0&&PTE10_I==1)
      {
        if(((RightSide_Go == Surround_Go)&&IntelligentSteeringLock==0)||(IntelligentSteeringLock==1&&PTE6_I==1))
        {
          time_servo=SerMid;
          SpeedTarget[0]=ObstacleOnL_CircleSpeed[0];
          SpeedTarget[1]=ObstacleOnL_CircleSpeed[1];
          SpeedPID.Kp[0]= SpeedPID_Kp_Obstacle_CircleSpeed;
          SpeedPID.Kp[1]= SpeedPID_Kp_Obstacle_CircleSpeed;
          SpeedPID.Ki[0]=SpeedPID_Ki_Obstacle_CircleSpeed;
          SpeedPID.Ki[1]=SpeedPID_Ki_Obstacle_CircleSpeed;
          SpeedPID.Kd[0]=SpeedPID_Kd_Obstacle_CircleSpeed;
          SpeedPID.Kd[1]=SpeedPID_Kd_Obstacle_CircleSpeed;
        }
      }
      else if(PTE8_I==0&&PTE11_I==1)
      {
        if(((RightSide_Go != Surround_Go)&&IntelligentSteeringLock==0)||(IntelligentSteeringLock==1&&PTE6_I==0))
        {
          time_servo=SerMid;
          SpeedTarget[0]=ObstacleOnR_CircleSpeed[0];
          SpeedTarget[1]=ObstacleOnR_CircleSpeed[1];
          SpeedPID.Kp[0]= SpeedPID_Kp_Obstacle_CircleSpeed;
          SpeedPID.Kp[1]= SpeedPID_Kp_Obstacle_CircleSpeed;
          SpeedPID.Ki[0]=SpeedPID_Ki_Obstacle_CircleSpeed;
          SpeedPID.Ki[1]=SpeedPID_Ki_Obstacle_CircleSpeed;
          SpeedPID.Kd[0]=SpeedPID_Kd_Obstacle_CircleSpeed;
          SpeedPID.Kd[1]=SpeedPID_Kd_Obstacle_CircleSpeed;
        }
      }
      else 
      {
        if(((RightSide_Go != Surround_Go)&&IntelligentSteeringLock==0)||(IntelligentSteeringLock==1&&PTE6_I==0))
        {
          time_servo = (uint32)SerRightMin;
          SpeedTarget[0]=SpeedTarget__R[0];
          SpeedTarget[1]=SpeedTarget__R[1];
          SpeedPID.Kp[0]= SpeedPID_Kp_turn;
          SpeedPID.Kp[1]= SpeedPID_Kp_turn;
          SpeedPID.Ki[0]=SpeedPID_Ki_turn;
          SpeedPID.Ki[1]=SpeedPID_Ki_turn;
          SpeedPID.Kd[0]=SpeedPID_Kd_turn;
          SpeedPID.Kd[1]=SpeedPID_Kd_turn;
        }
        else  
        {
          time_servo = (uint32)SerLeftMax;
          SpeedTarget[0]=SpeedTarget__L[0];
          SpeedTarget[1]=SpeedTarget__L[1];
          SpeedPID.Kp[0]= SpeedPID_Kp_turn;
          SpeedPID.Kp[1]= SpeedPID_Kp_turn;
          SpeedPID.Ki[0]=SpeedPID_Ki_turn;
          SpeedPID.Ki[1]=SpeedPID_Ki_turn;
          SpeedPID.Kd[0]=SpeedPID_Kd_turn;
          SpeedPID.Kd[1]=SpeedPID_Kd_turn;
        }
      }
  }
  
  /*-----------------------�ж��ű���Ƿ��л�--------------------------*/
  if((real_pointx>Distortion_60[real_pointy][0]&&real_pointx<Distortion_60[real_pointy][1])&&(RealNoSignal==0))   distancelock=1;
  if((!(real_pointx>Distortion_100[real_pointy][0]&&real_pointx<Distortion_100[real_pointy][1])&&distancelock==1)||RealNoSignal)       //real_pointyΪ40ʱ������3��4��ű�ƣ������ﵽ40������ʱҲ��30
  {
    if((!(real_pointx>Distortion_100[real_pointy][0]&&real_pointx<Distortion_100[real_pointy][1])||RealNoSignal)&&distancelock==1) //������жϲ�Ϊ�ű���л�
    {
      if(ChooseSide_Overflow!=1)
      ChooseSide_Num++;
      
      distancelock=0;
      zero_lock1 = 0;
      zero_lock2 = 0;
      zero_lock3 = 0;
      zero_lock4 = 0;
      zero_lock5 = 0;
      emergyflag = 0;
      speedupflag=0;
      turn_inadvance_lock=0;
      turn_inadvance_lock2=0;
    }
    Turn_lock = 0;

    ph_lock1 = 0;
    ph_lock2 = 0;
  }

//  /*-------------�ж��Ƿ���Ͻ���----------------------------------*/
//  if(((NOpoint==0&&(real_pointy<ObstacleDistance||real_pointy<(DistortionLimit_L_Slope*real_pointx+DistortionLimit_L_Intercept)||real_pointy<(DistortionLimit_R_Slope*real_pointx+DistortionLimit_R_Intercept)))||NOpoint==1)&&(PTE8_I==1||PTE9_I==1||PTE10_I==1||PTE11_I==1))
//  {
//    Turn_lock = 0;
//    Obstacle_flag=1;
//  }
//  else Obstacle_flag=0;

  ///////////////////////////////////////////
  
  

  
  /*-----------------------����------------------------------------*/
  if(ChooseSide_Num!=nobrakeNum&&!RealNoSignal||(turn_inadvance_lock==1||turn_inadvance_lock2==1)&&!IntelligentSteeringLock)
  {
  if((real_pointy>TurnInAdvanceDistance&&real_pointx>Distortion_40[real_pointy][0]&&real_pointx<Distortion_40[real_pointy][1]&&real_pointy>(DistortionLimit_L_Slope*real_pointx+DistortionLimit_L_Intercept)&&real_pointy>(DistortionLimit_R_Slope*real_pointx+DistortionLimit_R_Intercept)||turn_inadvance_lock==1)&&!IntelligentSteeringLock) //��ǰ���
  {
      if(RightSide_Go != Surround_Go)  
        {
          time_servo = (uint32)SerRightMin;
          SpeedTarget[0]=SpeedTarget__R[0];
          SpeedTarget[1]=SpeedTarget__R[1];
          SpeedPID.Kp[0]= SpeedPID_Kp_turn;
          SpeedPID.Kp[1]= SpeedPID_Kp_turn;
          SpeedPID.Ki[0]=SpeedPID_Ki_turn;
          SpeedPID.Ki[1]=SpeedPID_Ki_turn;
          SpeedPID.Kd[0]=SpeedPID_Kd_turn;
          SpeedPID.Kd[1]=SpeedPID_Kd_turn;
        }
      else  
        {
          time_servo = (uint32)SerLeftMax;
          SpeedTarget[0]=SpeedTarget__L[0];
          SpeedTarget[1]=SpeedTarget__L[1];
          SpeedPID.Kp[0]= SpeedPID_Kp_turn;
          SpeedPID.Kp[1]= SpeedPID_Kp_turn;
          SpeedPID.Ki[0]=SpeedPID_Ki_turn;
          SpeedPID.Ki[1]=SpeedPID_Ki_turn;
          SpeedPID.Kd[0]=SpeedPID_Kd_turn;
          SpeedPID.Kd[1]=SpeedPID_Kd_turn;
        }
      if(turn_inadvance_lock==0)
      CleanIntegerFlag3=1;
      turn_inadvance_lock=1;
  }
  else if((real_pointy>TurnInAdvanceDistance2&&real_pointx>Distortion_60[real_pointy][0]&&real_pointx<Distortion_60[real_pointy][1]&&real_pointy>(DistortionLimit_L_Slope*real_pointx+DistortionLimit_L_Intercept)&&real_pointy>(DistortionLimit_R_Slope*real_pointx+DistortionLimit_R_Intercept) &&emergyflag||turn_inadvance_lock2==1)&&!IntelligentSteeringLock)//��ǰ���2��λ
  {
      if(RightSide_Go != Surround_Go)  
      {
        time_servo = (uint32)SerRightMin;
        SpeedTarget[0]=SpeedTarget__R[0];
        SpeedTarget[1]=SpeedTarget__R[1];
        SpeedPID.Kp[0]= SpeedPID_Kp_turn;
        SpeedPID.Kp[1]= SpeedPID_Kp_turn;
        SpeedPID.Ki[0]=SpeedPID_Ki_turn;
        SpeedPID.Ki[1]=SpeedPID_Ki_turn;
        SpeedPID.Kd[0]=SpeedPID_Kd_turn;
        SpeedPID.Kd[1]=SpeedPID_Kd_turn;
      }
      else  
      {
        time_servo = (uint32)SerLeftMax;
        SpeedTarget[0]=SpeedTarget__L[0];
        SpeedTarget[1]=SpeedTarget__L[1];
        SpeedPID.Kp[0]= SpeedPID_Kp_turn;
        SpeedPID.Kp[1]= SpeedPID_Kp_turn;
        SpeedPID.Ki[0]=SpeedPID_Ki_turn;
        SpeedPID.Ki[1]=SpeedPID_Ki_turn;
        SpeedPID.Kd[0]=SpeedPID_Kd_turn;
        SpeedPID.Kd[1]=SpeedPID_Kd_turn;
      }
      if(turn_inadvance_lock2==0)
      CleanIntegerFlag3=1;
      turn_inadvance_lock2=1;
  }
  else if(real_pointx>Distortion_60[real_pointy][0]&&real_pointx<Distortion_60[real_pointy][1]) //�����Ƽ���,�˶η��ں��棬�ɸ���ǰ���ٶ��趨//||zero_lock1==1
  {
    if(speedupflag==1&&AccelerationLock==1)
    {
      SpeedTarget[0]=speedup_first;  
      SpeedTarget[1]=speedup_first; 
    }
    else
    {
      SpeedTarget[0]=brake_speed_first;  
      SpeedTarget[1]=brake_speed_first; 
    }
      SpeedPID.Kp[0]= SpeedPID_Kp_lowspeed;
      SpeedPID.Kp[1]= SpeedPID_Kp_lowspeed;
      SpeedPID.Ki[0]= SpeedPID_Ki_lowspeed;
      SpeedPID.Ki[1]= SpeedPID_Ki_lowspeed;
      SpeedPID.Kd[0]= SpeedPID_Kd_lowspeed;
      SpeedPID.Kd[1]= SpeedPID_Kd_lowspeed;
      zero_lock1=1;
  }
  else if(real_pointx>Distortion_80[real_pointy][0]&&real_pointx<Distortion_80[real_pointy][1])//||zero_lock2==1
  {
    if(speedupflag==1&&AccelerationLock==1)
    {
      SpeedTarget[0]=speedup_second;  
      SpeedTarget[1]=speedup_second; 
    }
    else
    {
      SpeedTarget[0]=brake_speed_second;  
      SpeedTarget[1]=brake_speed_second; 
    }
      SpeedPID.Kp[0]= SpeedPID_Kp_lowspeed;
      SpeedPID.Kp[1]= SpeedPID_Kp_lowspeed;
      SpeedPID.Ki[0]= SpeedPID_Ki_lowspeed;
      SpeedPID.Ki[1]= SpeedPID_Ki_lowspeed;
      SpeedPID.Kd[0]= SpeedPID_Kd_lowspeed;
      SpeedPID.Kd[1]= SpeedPID_Kd_lowspeed;
      zero_lock2=1;
  }
  else if(real_pointx>Distortion_100[real_pointy][0]&&real_pointx<Distortion_100[real_pointy][1])//||zero_lock3==1
  {
      SpeedTarget[0]=brake_speed_third;  
      SpeedTarget[1]=brake_speed_third; 
      SpeedPID.Kp[0]= SpeedPID_Kp_lowspeed;
      SpeedPID.Kp[1]= SpeedPID_Kp_lowspeed;
      SpeedPID.Ki[0]= SpeedPID_Ki_lowspeed;
      SpeedPID.Ki[1]= SpeedPID_Ki_lowspeed;
      SpeedPID.Kd[0]= SpeedPID_Kd_lowspeed;
      SpeedPID.Kd[1]= SpeedPID_Kd_lowspeed;
      zero_lock3=1;
  }
  else if(real_pointx>Distortion_120[real_pointy][0]&&real_pointx<Distortion_120[real_pointy][1])
  {
      SpeedTarget[0]=brake_speed_forth;  
      SpeedTarget[1]=brake_speed_forth; 
      SpeedPID.Kp[0]= SpeedPID_Kp_lowspeed;
      SpeedPID.Kp[1]= SpeedPID_Kp_lowspeed;
      SpeedPID.Ki[0]= SpeedPID_Ki_lowspeed;
      SpeedPID.Ki[1]= SpeedPID_Ki_lowspeed;
      SpeedPID.Kd[0]= SpeedPID_Kd_lowspeed;
      SpeedPID.Kd[1]= SpeedPID_Kd_lowspeed;
      if(zero_lock5==0&&zero_lock4==0)
      {
        CleanIntegerFlag1=1;
        if((MotorPID_Speed_ave[0]+MotorPID_Speed_ave[1])>emergy_speed*2)
          emergyflag=1;
        if((MotorPID_Speed_ave[0]+MotorPID_Speed_ave[1])<acceleration_speed*2)
          speedupflag=1;
      }
      zero_lock4=1;
  }
  else if(real_pointx>Distortion_160[real_pointy][0]&&real_pointx<Distortion_160[real_pointy][1])
  {
      SpeedTarget[0]=brake_speed_fifth;  
      SpeedTarget[1]=brake_speed_fifth; 
      SpeedPID.Kp[0]= SpeedPID_Kp_lowspeed;
      SpeedPID.Kp[1]= SpeedPID_Kp_lowspeed;
      SpeedPID.Ki[0]= SpeedPID_Ki_lowspeed;
      SpeedPID.Ki[1]= SpeedPID_Ki_lowspeed;
      SpeedPID.Kd[0]= SpeedPID_Kd_lowspeed;
      SpeedPID.Kd[1]= SpeedPID_Kd_lowspeed;
      if(zero_lock5==0)
      {
        CleanIntegerFlag1=1;
        if((MotorPID_Speed_ave[0]+MotorPID_Speed_ave[1])>emergy_speed*2)
          emergyflag=1;
        if((MotorPID_Speed_ave[0]+MotorPID_Speed_ave[1])<acceleration_speed*2)
          speedupflag=1;
      }
      zero_lock5=1;
  }
  }
  /*----------------------------�����----------------------------------*/
  if(CleanIntegerFlag1==1||(CleanIntegerFlag2==1&&SpeedPID.Integer[0]>0&&SpeedPID.Integer[1]>0)||CleanIntegerFlag3==1)
  {
    CleanIntegerFlag1=0;
    CleanIntegerFlag2=0;
    CleanIntegerFlag3=0;
    SpeedPID.Integer[0]=0;
    SpeedPID.Integer[1]=0;
  }
 
  Jam_Restart();
  SpeedPID_Cal(SpeedTarget,MotorPID_Speed_ave[0],MotorPID_Speed_ave[1]);
  
  if(emergyflag==1&& MadStopLock==1)
  {
    SpeedPID.Output[0]=-8000;
    SpeedPID.Output[1]=-8000;
    PTE25_O=1;
    if(real_pointy<lastreal_y) emergyflag=0;
  }
  else PTE25_O=0;
  
  MotorContrl();
  
   if(TenSecondsRecordBegin==1)
   {
      TenSecondsDataSave(1,( uint16 )MotorPID_Speed_ave[0]);
      TenSecondsDataSave(2,( uint16 )MotorPID_Speed_ave[1]);
      TenSecondsDataSave(3,( uint16 )SpeedPID.Output[0]);
      TenSecondsDataSave(4,( uint16 )SpeedPID.Output[1]);
//      TenSecondsDataSave(1,( uint16 )RealNoSignal);
//      TenSecondsDataSave(2,( uint16 )Nopointtime);
//      TenSecondsDataSave(3,( uint16 )Withpointtime);
//      TenSecondsDataSave(4,( uint16 )ChooseSide_Num);
      TenSecondsDataSave(5,( uint16 )real_pointx);
      TenSecondsDataSave(6,( uint16 )real_pointy);
      if(!(Withpointtime==11&&(Nopoint_Y_Complement[9]!=0)&&(Nopoint_Y_Complement[10]<Nopoint_Y_Complement[9]))) 
      {
        TenSecondsDataSave(0,( uint16 )Nopoint_X_Complement[10]);
        TenSecondsDataSave(7,( uint16 )Nopoint_Y_Complement[10]);
      }
      TenSecondsDataSave(8,( uint16 )SpeedTarget[0]);
      TenSecondsDataSave(9,( uint16 )SpeedTarget[1]);
      TenSecondsDataSaveOver();
   }
   
  lastreal_y=real_pointy;
  lastreal_x=real_pointx;
//     ProgrammeRunTime=LPLD_PIT_Time_GetUs(PIT3);
//     LPLD_PIT_Close(PIT3);
}