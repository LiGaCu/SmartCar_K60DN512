#include "common.h"
#include "PID_Calculate.h"
#include "camera_test.h"
#include "servo_forFun.h" 
#include "motor_forFun.h" 
#include "QD.h"
#include "Fuzzy.h"


float error_limit,integer_limit;
SpeedPID_TypeDef SpeedPID;
PID_TypeDef Turn_PID;


float kp_crease;
float Turn_PID_P_Am = 1.2;
float New_Turn_PID_P;

//uint16 NOpointtime,WITHpointtime;
//uint8 Jamlock=0,ReStarttime=0;

#define MaxOutput_I 8000    //积分项限幅值
#define MaxOutput 9500          //pwm最大输出

void SpeedPID_Cal(float TargetSpeed[2],float MotorPID_Speed_ave_L,float MotorPID_Speed_ave_R)//目标值和测量值
{ 
//  SpeedPID.Kp=2.45*p;//PID三个参数
//  SpeedPID.Ki=3.5*p;
//  SpeedPID.Kd=1.25*p;
  
  SpeedPID.Error[0][0]=TargetSpeed[0]-MotorPID_Speed_ave_L;//当前误差
  SpeedPID.Error[1][0]=TargetSpeed[1]-MotorPID_Speed_ave_R;
  
  if(SpeedPID.Error[0][0]>error_limit*TargetSpeed[0])
    SpeedPID.Integer[0]=integer_limit*SpeedPID.Integer[0];
  if(SpeedPID.Error[1][0]>error_limit*TargetSpeed[1])
    SpeedPID.Integer[1]=integer_limit*SpeedPID.Integer[1];
  
  SpeedPID.Integer[0]+=SpeedPID.Error[0][0];//积分项和
  SpeedPID.Integer[1]+=SpeedPID.Error[1][0];
  
//  if(SpeedPID.Error[0][0]<-120&&SpeedPID.Integer[0]>3000)
//  {//减速清积份
//    SpeedPID.Integer[0]*=0.2;
//  }
//  else if(SpeedPID.Error[0][0]<-100&&SpeedPID.Integer[0]>3000)
//  {
//    SpeedPID.Integer[0]*=0.4;
//  }
//  else if(SpeedPID.Error[0][0]<-80&&SpeedPID.Integer[0]>3000)
//  {
//    SpeedPID.Integer[0]*=0.6;
//  }
//  else if(SpeedPID.Error[0][0]<-50&&SpeedPID.Integer[0]>3000)
//  {
//    SpeedPID.Integer[0]*=0.8;
//  }
//  else if(SpeedPID.Error[0][0]<-30&&SpeedPID.Integer[0]>3000)
//  {
//    SpeedPID.Integer[0]*=0.85;
//  }
//  
//  if(SpeedPID.Error[1][0]<-120&&SpeedPID.Integer[1]>3000)
//  {//减速清积份
//    SpeedPID.Integer[1]*=0.2;
//  }
//  else if(SpeedPID.Error[1][0]<-100&&SpeedPID.Integer[1]>3000)
//  {
//    SpeedPID.Integer[1]*=0.4;
//  }
//  else if(SpeedPID.Error[1][0]<-80&&SpeedPID.Integer[1]>3000)
//  {
//    SpeedPID.Integer[1]*=0.6;
//  }
//  else if(SpeedPID.Error[1][0]<-50&&SpeedPID.Integer[1]>3000)
//  {
//    SpeedPID.Integer[1]*=0.8;
//  }
//  else if(SpeedPID.Error[1][0]<-30&&SpeedPID.Integer[1]>3000)
//  {
//    SpeedPID.Integer[1]*=0.85;
//  }
  
  SpeedPID.Deriv[0]=SpeedPID.Error[0][0]-SpeedPID.Error[0][1];//微分项
  SpeedPID.Deriv[1]=SpeedPID.Error[1][0]-SpeedPID.Error[1][1];
  
  /*----------积分分限幅--------------*/
  SpeedPID.Integer[0]=(SpeedPID.Integer[0]>MaxOutput_I)?MaxOutput_I:SpeedPID.Integer[0];
  SpeedPID.Integer[1]=(SpeedPID.Integer[1]>MaxOutput_I)?MaxOutput_I:SpeedPID.Integer[1];
  
  SpeedPID.Integer[0]=(SpeedPID.Integer[0]<1000-MaxOutput_I)?1000-MaxOutput_I:SpeedPID.Integer[0];
  SpeedPID.Integer[1]=(SpeedPID.Integer[1]<1000-MaxOutput_I)?1000-MaxOutput_I:SpeedPID.Integer[1];
  /*---------------------------------*/
  
  SpeedPID.Output[0]=SpeedPID.Kp[0]*SpeedPID.Error[0][0]+SpeedPID.Ki[0]*SpeedPID.Integer[0]+SpeedPID.Kd[0]*SpeedPID.Deriv[0];//输出值
  SpeedPID.Output[1]=SpeedPID.Kp[1]*SpeedPID.Error[1][0]+SpeedPID.Ki[1]*SpeedPID.Integer[1]+SpeedPID.Kd[1]*SpeedPID.Deriv[1];//输出值  
  
  SpeedPID.Output[0]=(SpeedPID.Output[0]>MaxOutput)?MaxOutput:SpeedPID.Output[0];
  SpeedPID.Output[1]=(SpeedPID.Output[1]>MaxOutput)?MaxOutput:SpeedPID.Output[1];
  
  SpeedPID.Output[0]=(SpeedPID.Output[0]<-MaxOutput)?-MaxOutput:SpeedPID.Output[0];
  SpeedPID.Output[1]=(SpeedPID.Output[1]<-MaxOutput)?-MaxOutput:SpeedPID.Output[1];
  
  SpeedPID.Error[0][1]=SpeedPID.Error[0][0];
  SpeedPID.Error[1][1]=SpeedPID.Error[1][0];
}

void Turn_PID_Cal(void)
{ 
  Turn_PID.Error=SetCenterX-real_pointx;  //设定值 减 测量值
  Turn_PID.Deriv=Turn_PID.Error-Turn_PID.PreError; 
  kp_crease = Turn_PID_P_Am * Fuzzy(Turn_PID.Error,Turn_PID.Deriv) / 100.0f;
  New_Turn_PID_P = Turn_PID.P + kp_crease;
  Turn_PID.Output=New_Turn_PID_P*(Turn_PID.Error)+Turn_PID.Deriv*Turn_PID.D+SerMid;//Turn_PID
  
//  if(Turn_PID.Error>=15||Turn_PID.Error<=-15)
//    Turn_PID.Output=Turn_close_PID.P*(Turn_PID.Error)+Turn_PID.Deriv*Turn_close_PID.D+SerMid;//Turn_close_PID
//  else
//    Turn_PID.Output=Turn_PID.P*(Turn_PID.Error)+Turn_PID.Deriv*Turn_PID.D+SerMid;//Turn_PID
  
  if (Turn_PID.Output>SerLeftMax)//转角限幅
    Turn_PID.Output=SerLeftMax;
  else if (Turn_PID.Output<SerRightMin)
    Turn_PID.Output=SerRightMin;
  
  Turn_PID.PreError=Turn_PID.Error;
}

void PID_Init(void)
{  
  Turn_PID.P=2.5;    //6     
  Turn_PID.I=0;
  Turn_PID.D=10.2;       //5 
  Turn_PID.iLimit=0;
  
 /////////////////////////////////////////////////////////// 
  memset(SpeedPID.Kp,0,sizeof(SpeedPID.Kp));
  memset(SpeedPID.Ki,0,sizeof(SpeedPID.Ki));
  memset(SpeedPID.Kd,0,sizeof(SpeedPID.Kd));
  /*--- 数组第一项为左轮 第二项为右轮---*/
  memset(SpeedPID.Error,0,sizeof(SpeedPID.Error));//1->2 当前误差e(k),上一次e(k-1)
  memset(SpeedPID.Integer,0,sizeof(SpeedPID.Integer));//积分总和值（误差和）
  memset(SpeedPID.Deriv,0,sizeof(SpeedPID.Deriv));//微分项值（当前误差-上一次误差）
  memset(SpeedPID.Output,0,sizeof(SpeedPID.Output));//输出
  
  integer_limit=0.7;
  error_limit=0.97;
  
//  SpeedTarget[0]=200;
//  SpeedTarget[1]=200;
} 
  

uint8 Jamtimetest,JamBeginTime,RestartBegin,RestartDirection;

void Jam_Restart(void)
{  
  if(car_run==1)
  {
    if(speed_right<100&&speed_left<100&&RestartBegin==0)
      Jamtimetest++;
    else Jamtimetest=0;
    if(Jamtimetest>=100)
    {
      RestartBegin=1;
      Jamtimetest=0;
      if(time_servo>SerMid)
        RestartDirection=1;
      else RestartDirection=0;
    }
    if(RestartBegin)
    {
      JamBeginTime++;
      if(JamBeginTime<100)
      {
        if(RestartDirection==1)
        {
          time_servo=SerMid;
          SpeedTarget[0]=-300;
          SpeedTarget[1]=-300;
          SpeedPID.Kp[0]= 55;
          SpeedPID.Kp[1]= 55;
          SpeedPID.Ki[0]=0.7;
          SpeedPID.Ki[1]=0.7;
        }
        else
        {
          time_servo=SerMid;
          SpeedTarget[0]=-300;
          SpeedTarget[1]=-300;
          SpeedPID.Kp[0]= 55;
          SpeedPID.Kp[1]= 55;
          SpeedPID.Ki[0]=0.7;
          SpeedPID.Ki[1]=0.7;
        }
      }
      else
      {
        JamBeginTime=0;
        RestartBegin=0;
      }
    }
  }
}



  /*---------------------判断小车是否卡死方式1---------------------------
  if(car_run==1)
  {
    uint8 j=0;
    if(!NOpoint)
    {
      WITHpointtime++;
      NOpointtime=0;
      Jamflag_y=0;                                                                              //Jamflag_x=0;
      ChangeImage_y[ChangeImage_y_Num]=(real_pointy-lastpointy)*(real_pointy-lastpointy);       //ChangeImage_x[i++]=(real_pointx-lastpointx)*(real_pointx-lastpointx);
      ChangeImage_y_Num++;
      lastpointy=real_pointy;                                                                   // lastpointx=real_pointx;
      if(ChangeImage_y_Num>=50) ChangeImage_y_Num=0;                                            //if(i>=25) i=0;
      for(j=0;j<50;j++)
      {
      Jamflag_y=Jamflag_y+ChangeImage_y[j];                                                     //      Jamflag_x=Jamflag_x+ChangeImage_x[j];
      }
    }
    else 
    {
//     Jamflag_y=226;
      WITHpointtime=0;
      NOpointtime++;
    }
    if((Jamflag_y<=50&&WITHpointtime>=100)||(NOpointtime>=200)||Jamlock)                        //    if((Jamflag_y<=225&&Jamflag_x<=225)||(NOpointtime>=200)||Jamlock)
    {
      ReStarttime++;
      NOpointtime=0;
      WITHpointtime=0;
      Jamlock=1;
      time_servo=SerRightMin;
      SpeedPID.Kp[0] = 150;
      SpeedPID.Kp[1] = 136;  
      SpeedPID.Ki[0] = 0.7;
      SpeedPID.Ki[1] = 0.7;
      
      SpeedTarget[0]=-125;
      SpeedTarget[1]=-100;
      if(ReStarttime>50)
      {
        ReStarttime=0;
        Jamlock=0;
        Turn_lock = 0;
      }
    }
  }
  ----------------------------------------------------------------------*/
  
/*-------------------------------------------------------------------
void Jam_Restart(void)
{  

  if(car_run==1)
  {
    if(Framesimplify)
    {
      uint8 j=0,WITHpointJam=0;
      uint16 Sumpre=0,Sumlast=0;

      if(!NOpoint)
      {
        WITHpointtime++;
        NOpointtime=0;                                                                            
        Sumpoint_y=0;
        SquareError_y=0;
        for(j=0;j<24;j++)
        {
        ChangeImage_y[j]=ChangeImage_y[j+1];
        }
        ChangeImage_y[24]=real_pointy;
        for(j=0;j<25;j++)
        {
        Sumpoint_y=Sumpoint_y+ChangeImage_y[j];                                                     
        }
        Average_y=Sumpoint_y/25;
        for(j=0;j<25;j++)
        {
        SquareError_y=SquareError_y+(ChangeImage_y[j]-Average_y)*(ChangeImage_y[j]-Average_y);         
        } 
        if(SquareError_y<50)
        {
          Sumpre=0;Sumlast=0;
          for(j=0;j<5;j++)
          Sumpre=Sumpre+ChangeImage_y[j];
          for(j=20;j<25;j++)
          Sumlast=Sumlast+ChangeImage_y[j];
          if((Sumlast-Sumpre)/5<6)
             WITHpointJam=1;
        }
        else WITHpointJam=0;
      }
      else 
      {
        WITHpointtime=0;
        NOpointtime++;
      }
      if((WITHpointJam&&WITHpointtime>=50)||(NOpointtime>=200)||Jamlock)                       
      {
        ReStarttime++;
        NOpointtime=0;
        WITHpointtime=0;
        Jamlock=1;
        time_servo=SerRightMin;
        SpeedPID.Kp[0] = 150;
        SpeedPID.Kp[1] = 136;  
        SpeedPID.Ki[0] = 0.7;
        SpeedPID.Ki[1] = 0.7;
        
        SpeedTarget[0]=-125;
        SpeedTarget[1]=-100;
        if(ReStarttime>50)
        {
          ReStarttime=0;
          Jamlock=0;
          Turn_lock = 0;
        }
      }
      Framesimplify=0;
    }
    else Framesimplify=1;
  }
}
---------------------------------------------------------------*/