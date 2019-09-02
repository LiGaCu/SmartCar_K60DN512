#include "common.h"
#include "botton_test.h"
#include "motor_forFun.h" 
#include "QD.h"
#include "oled.h"
#include "myflash.h"
#include "freecars.h"

void boma_init_gpio(void)
{
  static GPIO_InitTypeDef boma1_io_D_struct;
  boma1_io_D_struct.GPIO_PTx=PTD;
  boma1_io_D_struct.GPIO_Pins= GPIO_Pin0|GPIO_Pin1;
  boma1_io_D_struct.GPIO_Dir=DIR_INPUT;
  boma1_io_D_struct.GPIO_PinControl=INPUT_PULL_UP;
  LPLD_GPIO_Init(boma1_io_D_struct);
  
  static GPIO_InitTypeDef boma1_io_C_struct;
  boma1_io_C_struct.GPIO_PTx=PTC;
  boma1_io_C_struct.GPIO_Pins= GPIO_Pin18|GPIO_Pin19;
  boma1_io_C_struct.GPIO_Dir=DIR_INPUT;
  boma1_io_C_struct.GPIO_PinControl=INPUT_PULL_UP;
  LPLD_GPIO_Init(boma1_io_C_struct);
  
  static GPIO_InitTypeDef botton_gpio_init;
  botton_gpio_init.GPIO_PTx = PTA;
  botton_gpio_init.GPIO_Pins = GPIO_Pin11|GPIO_Pin12|GPIO_Pin13|GPIO_Pin14|GPIO_Pin15|GPIO_Pin16;
  botton_gpio_init.GPIO_Dir = DIR_INPUT;
  botton_gpio_init.GPIO_PinControl =INPUT_PULL_UP;
  LPLD_GPIO_Init(botton_gpio_init);
  
  static GPIO_InitTypeDef Beep;
  Beep.GPIO_PTx = PTE;
  Beep.GPIO_Pins = GPIO_Pin25;
  Beep.GPIO_Dir = DIR_OUTPUT;
  Beep.GPIO_PinControl = OUTPUT_DSH ;
  Beep.GPIO_Output = OUTPUT_L;
  LPLD_GPIO_Init(Beep);
}
void Key_delay()
{
	uint16 i, n;
	for (i = 0; i < 19000; i++)
	{
		for (n = 0; n < 50; n++)
		{
			asm("nop");
		}
	}
}
unsigned char KeyScan(void)
{
  if (PTA11_I == 0)
  {
    Key_delay();
    if (PTA11_I == 0)
    {
      return 1;
    }
  }
  else if (PTA12_I == 0)//6
  {
    Key_delay();
    if (PTA12_I == 0)
    {
      return 2;
    }
  }
  else if (PTA13_I == 0)//6
  {
    Key_delay();
    if (PTA13_I == 0)
    {
      return 3;
    }
  }
  
  else if (PTA14_I == 0)//1
  {
    Key_delay();
    if (PTA14_I == 0)
    {
      return 4;
    }
  }
  
  else if (PTA15_I == 0)
  {
    Key_delay();
    if (PTA15_I == 0)
    {
      return 5;
    }
  }
  
  else if (PTA16_I == 0)
  {
    Key_delay();
    if (PTA16_I == 0)
    {
      return 6;
    }
  }
  return 10;
}

/*---------------------------------------------------------------------
                             键盘发车
---------------------------------------------------------------------*/

void Move_Process(void)
{
    unsigned char temp = 10;//此初值表示没有按键按下
    temp = KeyScan();//扫描按键，所有按键都没被按下返回10
    if (temp == 4)//延时1S发车
    {
      LPLD_LPTMR_DelayMs(100);
      car_run=1;
      Nopointtime=12;
      ChooseSide_Num=0;
    }
    else if(temp == 6)
    {
      car_run=0;
    }  
}

/*-------------------------------------------------------------------------
                键盘输入过信标左右，并储存在FLASH中
20180322：编辑完成，未测试

--------------------------------------------------------------------------*/

int8 ChooseSide_Page=0,ChooseSide_Mouse=0,lasttimeSmart=0;

void ChooseSide_Configure(void)
{
  unsigned char temp=10;
  if(IntelligentSteeringLock==1)
  {
    LED_P6x8Str(6, 0,(unsigned char*)"SmartSteering");
    LED_P6x8Str(6, 2,(unsigned char*)"NoBrakeNum");
    LED_P6x8Str(6, 4,(unsigned char*)"StopNumber");
    LED_PrintValueFP(102,0,IntelligentSteeringLock,1);
    LED_PrintValueFP(96,2,nobrakeNum,2);
    LED_PrintValueFP(96,4,StopNum,2);
  }
  else
  {
    if(ChooseSide_Page==0)
    {
      LED_P6x8Str(6, 0,(unsigned char*)"Smart");
      LED_P6x8Str(6, 2,(unsigned char*)"NB");
      LED_P6x8Str(6, 4,(unsigned char*)"STOP");
      LED_P6x8Str(6, 6,(unsigned char*)"Sur?");
      LED_PrintValueFP(48,0,IntelligentSteeringLock,1);
      LED_PrintValueFP(42,2,nobrakeNum,2);
      LED_PrintValueFP(42,4,StopNum,2);
      LED_PrintValueFP(48,6,(int16)Surround_Go,1);

      LED_P6x8Str(66, 0,(unsigned char*)"Num");
      LED_P6x8Str(90, 0,(unsigned char*)"Side");
      LED_P6x8Str(72, 1,(unsigned char*)"00");
      LED_P6x8Str(72, 2,(unsigned char*)"01");
      LED_P6x8Str(72, 3,(unsigned char*)"02");
      LED_P6x8Str(72, 4,(unsigned char*)"03");
      LED_P6x8Str(72, 5,(unsigned char*)"04");
      LED_P6x8Str(72, 6,(unsigned char*)"05");
      LED_P6x8Str(72, 7,(unsigned char*)"06");
      
      LED_PrintValueFP(102,1,ChooseSide_Buffer[0],1);
      LED_PrintValueFP(102,2,ChooseSide_Buffer[1],1);
      LED_PrintValueFP(102,3,ChooseSide_Buffer[2],1);
      LED_PrintValueFP(102,4,ChooseSide_Buffer[3],1);
      LED_PrintValueFP(102,5,ChooseSide_Buffer[4],1);
      LED_PrintValueFP(102,6,ChooseSide_Buffer[5],1);
      LED_PrintValueFP(102,7,ChooseSide_Buffer[6],1);
    }
    else
    {
      LED_P6x8Str(6, 0,(unsigned char*)"Num");
      LED_P6x8Str(30, 0,(unsigned char*)"Side");
      LED_P6x8Str(66, 0,(unsigned char*)"Num");
      LED_P6x8Str(90, 0,(unsigned char*)"Side");
      
      LED_P6x8Str(12, 1,(unsigned char*)"07");
      LED_P6x8Str(12, 2,(unsigned char*)"08");
      LED_P6x8Str(12, 3,(unsigned char*)"09");
      LED_P6x8Str(12, 4,(unsigned char*)"10");
      LED_P6x8Str(12, 5,(unsigned char*)"11");
      LED_P6x8Str(12, 6,(unsigned char*)"12");
      LED_P6x8Str(12, 7,(unsigned char*)"13");   
      LED_P6x8Str(72, 1,(unsigned char*)"14");
      LED_P6x8Str(72, 2,(unsigned char*)"15");
      LED_P6x8Str(72, 3,(unsigned char*)"16");
      LED_P6x8Str(72, 4,(unsigned char*)"17");

      LED_PrintValueFP(42,1,ChooseSide_Buffer[7],1);
      LED_PrintValueFP(42,2,ChooseSide_Buffer[8],1);
      LED_PrintValueFP(42,3,ChooseSide_Buffer[9],1);
      LED_PrintValueFP(42,4,ChooseSide_Buffer[10],1);
      LED_PrintValueFP(42,5,ChooseSide_Buffer[11],1);
      LED_PrintValueFP(42,6,ChooseSide_Buffer[12],1);
      LED_PrintValueFP(42,7,ChooseSide_Buffer[13],1);
      LED_PrintValueFP(102,1,ChooseSide_Buffer[14],1);
      LED_PrintValueFP(102,2,ChooseSide_Buffer[15],1);
      LED_PrintValueFP(102,3,ChooseSide_Buffer[16],1);
      LED_PrintValueFP(102,4,ChooseSide_Buffer[17],1);

    }
  }
  
  if(IntelligentSteeringLock==1)
  {
    if(ChooseSide_Mouse==0)
    {
      LED_P6x8Char(0,ChooseSide_Mouse*2,'*');
      LED_P6x8Char(0,(ChooseSide_Mouse+1)*2,' ');
    }
    else if(ChooseSide_Mouse==2)
    {
      LED_P6x8Char(0,ChooseSide_Mouse*2,'*');
      LED_P6x8Char(0,(ChooseSide_Mouse-1)*2,' ');
    }
    else
    {
      LED_P6x8Char(0,ChooseSide_Mouse*2,'*');
      LED_P6x8Char(0,(ChooseSide_Mouse-1)*2,' ');
      LED_P6x8Char(0,(ChooseSide_Mouse+1)*2,' ');
    }
  }
  else
  {
    if(ChooseSide_Page==0)
    {
      if(ChooseSide_Mouse==0)
      {
        LED_P6x8Char(66*(ChooseSide_Mouse>3?1:0),(ChooseSide_Mouse>3?ChooseSide_Mouse:2*ChooseSide_Mouse)-(ChooseSide_Mouse>3?3:0),'*');
        LED_P6x8Char(66*(ChooseSide_Mouse+1>3?1:0),(ChooseSide_Mouse+1>3?ChooseSide_Mouse+1:2*(ChooseSide_Mouse+1))-(ChooseSide_Mouse+1>3?3:0),' ');
      }
      else if(ChooseSide_Mouse==10)
      {
        LED_P6x8Char(66*(ChooseSide_Mouse>3?1:0),(ChooseSide_Mouse>3?ChooseSide_Mouse:2*ChooseSide_Mouse)-(ChooseSide_Mouse>3?3:0),'*');
        LED_P6x8Char(66*(ChooseSide_Mouse-1>3?1:0),(ChooseSide_Mouse-1>3?ChooseSide_Mouse-1:2*(ChooseSide_Mouse-1))-(ChooseSide_Mouse-1>3?3:0),' ');
      }
      else
      {
        LED_P6x8Char(66*(ChooseSide_Mouse>3?1:0),(ChooseSide_Mouse>3?ChooseSide_Mouse:2*ChooseSide_Mouse)-(ChooseSide_Mouse>3?3:0),'*');
        LED_P6x8Char(66*(ChooseSide_Mouse-1>3?1:0),(ChooseSide_Mouse-1>3?ChooseSide_Mouse-1:2*(ChooseSide_Mouse-1))-(ChooseSide_Mouse-1>3?3:0),' ');
        LED_P6x8Char(66*(ChooseSide_Mouse+1>3?1:0),(ChooseSide_Mouse+1>3?ChooseSide_Mouse+1:2*(ChooseSide_Mouse+1))-(ChooseSide_Mouse+1>3?3:0),' ');
      }
    }
    else
    {
      if(ChooseSide_Mouse==0)
      {
        LED_P6x8Char(6+60*(ChooseSide_Mouse/7),ChooseSide_Mouse%7+1,'*');
        LED_P6x8Char(6+60*((ChooseSide_Mouse+1)/7),(ChooseSide_Mouse+1)%7+1,' ');
      }
      else if(ChooseSide_Mouse==10)
      {
        LED_P6x8Char(6+60*(ChooseSide_Mouse/7),ChooseSide_Mouse%7+1,'*');
        LED_P6x8Char(6+60*((ChooseSide_Mouse-1)/7),(ChooseSide_Mouse-1)%7+1,' ');
      }
      else
      {
        LED_P6x8Char(6+60*(ChooseSide_Mouse/7),ChooseSide_Mouse%7+1,'*');
        LED_P6x8Char(6+60*((ChooseSide_Mouse-1)/7),(ChooseSide_Mouse-1)%7+1,' ');
        LED_P6x8Char(6+60*((ChooseSide_Mouse+1)/7),(ChooseSide_Mouse+1)%7+1,' ');
      }
    }
  }

  temp=KeyScan();
  
  if(temp==4&&ChooseSide_Mouse==10&&ChooseSide_Page==0)
    LED_Fill(0);
  else if(temp==3&&ChooseSide_Mouse==0&&ChooseSide_Page==1)
    LED_Fill(0);
  if(lasttimeSmart!=IntelligentSteeringLock)  
  { 
    ChooseSide_Mouse=0;
    ChooseSide_Page=0;
    LED_Fill(0);
  }
  lasttimeSmart=IntelligentSteeringLock;
  
  switch(temp)
  {
    case 1:flash_config();
            LED_Fill(0);
            LED_P6x8Str(42, 3,(unsigned char*)"Done!");
            LPLD_LPTMR_DelayMs(1000);
            LED_Fill(0);
            break;
    case 4:ChooseSide_Mouse++;
            if(IntelligentSteeringLock==1)
            {
              if(ChooseSide_Mouse>2)
                ChooseSide_Mouse=2;
            }
            else if(ChooseSide_Page==0&&ChooseSide_Mouse==11)
            {
              ChooseSide_Mouse=0;
              ChooseSide_Page=1;
            }
            else if(ChooseSide_Page==1&&ChooseSide_Mouse>10)
              ChooseSide_Mouse=10;
            Key_delay();
            break;
    case 3:ChooseSide_Mouse--;
            if(IntelligentSteeringLock==1)
            {
              if(ChooseSide_Mouse<0)
                ChooseSide_Mouse=0;
            }
            else if(ChooseSide_Page==0&&ChooseSide_Mouse<0)
              ChooseSide_Mouse=0;
            else if(ChooseSide_Page==1&&ChooseSide_Mouse==-1)
            {
              ChooseSide_Mouse=10;
              ChooseSide_Page=0;
            }
            Key_delay();
            break;
    case 5:if(IntelligentSteeringLock==1)
            {
              if(ChooseSide_Mouse==0)
              {
                IntelligentSteeringLock=1;
              }
              else if(ChooseSide_Mouse==1)
              {
                if(nobrakeNum<19)
                  nobrakeNum++;
              }
              else
              {
                if(StopNum<19)
                  StopNum++;
              }
            }
            else if(ChooseSide_Page==0)
            {
              if(ChooseSide_Mouse==0)
              {
                IntelligentSteeringLock=1;
              }
              else if(ChooseSide_Mouse==1)
              {
                if(nobrakeNum<19)
                  nobrakeNum++;
              }
              else if(ChooseSide_Mouse==2)
              {
                if(StopNum<19)
                  StopNum++;
              }
              else if(ChooseSide_Mouse==3)
              {
                Surround_Go=1;
              }
              else
              {
                if(ChooseSide_Buffer[ChooseSide_Mouse-4]<2)
                  ChooseSide_Buffer[ChooseSide_Mouse-4]++;
              }
            }
            else
            {
              if(ChooseSide_Buffer[ChooseSide_Mouse+7]<2)
                ChooseSide_Buffer[ChooseSide_Mouse+7]++;
            }
            Key_delay();
            break;
    case 6:if(IntelligentSteeringLock==1)
            {
              if(ChooseSide_Mouse==0)
              {
                IntelligentSteeringLock=0;
              }
              else if(ChooseSide_Mouse==1)
              {
                if(nobrakeNum>0)
                  nobrakeNum--;
              }
              else
              {
                if(StopNum>0)
                  StopNum--;
              }
            }
            else if(ChooseSide_Page==0)
            {
              if(ChooseSide_Mouse==0)
              {
                IntelligentSteeringLock=0;
              }
              else if(ChooseSide_Mouse==1)
              {
                if(nobrakeNum>0)
                  nobrakeNum--;
              }
              else if(ChooseSide_Mouse==2)
              {
                if(StopNum>0)
                  StopNum--;
              }
              else if(ChooseSide_Mouse==3)
              {
                Surround_Go=0;
              }
              else
              {
                if(ChooseSide_Buffer[ChooseSide_Mouse-4]>0)
                  ChooseSide_Buffer[ChooseSide_Mouse-4]--;
              }
            }
            else
            {
              if(ChooseSide_Buffer[ChooseSide_Mouse+7]>0)
                ChooseSide_Buffer[ChooseSide_Mouse+7]--;
            }
            Key_delay();
            break;
  }
}

/*-----------------------------------------------------------------------------
                            PID参数5秒录制回看
20180409：创建函数。
------------------------------------------------------------------------------*/
void PID_Adjustment(void)
{
    unsigned char temp = 10;//此初值表示没有按键按下
    temp = KeyScan();//扫描按键，所有按键都没被按下返回10
    if (temp == 4)//延时1S发车
    {
      LED_Fill(0);
      LED_P6x8Str(30, 3,(unsigned char*)"Counting...");
      LPLD_LPTMR_DelayMs(1000);
      LED_Fill(0);
      LED_P6x8Str(30, 3,(unsigned char*)"Recording...");
      car_run=1;
      Nopointtime=12;
      ChooseSide_Num=0;
      TenSecondsRecordBegin=1;
      TenSecondsRecordLength=0;
    }
    else if(temp == 6)
    {
      car_run=0;
      TenSecondsRecordBegin=0;
    }
    else if(temp==3)
    {
      LED_Fill(0);
      LED_P6x8Str(30, 3,(unsigned char*)"DataSending...");
      TenSecondsDataSend();
      LED_Fill(0);
      LED_P6x8Str(30, 3,(unsigned char*)"Finished!");
    }
}

/*------------------------------------------------------------------------------
                               交车后调整参数
20180505：创建函数。
------------------------------------------------------------------------------*/
int8 ParameterMouse=0,lastAccState,lastMadstate;
void Parameter_Adjustment(void)
{
  if(lastAccState!=MadStopLock||lastMadstate!=AccelerationLock)
    LED_Fill(0);
  lastAccState=MadStopLock;
  lastMadstate=AccelerationLock;
  if(IntelligentSteeringLock==1)
  {
    LED_P6x8Str(6, 0,(unsigned char*)"Speed");
    LED_P6x8Str(6, 1,(unsigned char*)"MadStopLock");
    LED_PrintValueFP(102,0,(int16)SpeedTarget_,3);
    LED_PrintValueFP(102,1,(int16)MadStopLock,1);
    if(MadStopLock==1)
    {
      LED_P6x8Str(6, 2,(unsigned char*)"EmergySpeed");
      LED_P6x8Str(6, 3,(unsigned char*)"AccSLock");
      LED_PrintValueFP(102,2,(int16)emergy_speed,3);
      LED_PrintValueFP(102,3,(int16)AccelerationLock,1);
      if(AccelerationLock==1)
      {
        LED_P6x8Str(6, 4,(unsigned char*)"AccTestSpeed");
        LED_PrintValueFP(102,4,(int16)acceleration_speed,3);
      }
      
    }
    else
    {
      LED_P6x8Str(6, 2,(unsigned char*)"AccSLock");
      LED_PrintValueFP(102,2,(int16)AccelerationLock,1);
      if(AccelerationLock==1)
      {
        LED_P6x8Str(6, 3,(unsigned char*)"AccTestSpeed");
        LED_PrintValueFP(102,3,(int16)acceleration_speed,3);
      }    
    }
  }
  else
  {
    LED_P6x8Str(6, 0,(unsigned char*)"Speed");
    LED_P6x8Str(6, 1,(unsigned char*)"TurnDistance");
    LED_P6x8Str(6, 2,(unsigned char*)"TurnDistance2");
    LED_P6x8Str(6, 3,(unsigned char*)"MadStopLock");
    LED_PrintValueFP(102,0,(int16)SpeedTarget_,3);
    LED_PrintValueFP(102,1,TurnInAdvanceDistance,2);
    LED_PrintValueFP(102,2,TurnInAdvanceDistance2,2);
    LED_PrintValueFP(102,3,(int16)MadStopLock,1);
    if(MadStopLock==1)
    {
      LED_P6x8Str(6, 4,(unsigned char*)"EmergySpeed");
      LED_P6x8Str(6, 5,(unsigned char*)"AccSLock");
      LED_PrintValueFP(102,4,(int16)emergy_speed,3);
      LED_PrintValueFP(102,5,(int16)AccelerationLock,1);
      if(AccelerationLock==1)
      {
        LED_P6x8Str(6, 6,(unsigned char*)"AccTestSpeed");
        LED_PrintValueFP(102,6,(int16)acceleration_speed,3);
      }
      
    }
    else
    {
      LED_P6x8Str(6, 4,(unsigned char*)"AccSLock");
      LED_PrintValueFP(102,4,(int16)AccelerationLock,1);
      if(AccelerationLock==1)
      {
        LED_P6x8Str(6, 5,(unsigned char*)"AccTestSpeed");
        LED_PrintValueFP(102,5,(int16)acceleration_speed,3);
      }    
    }
  }
  
  if(IntelligentSteeringLock==1)
  {
    if(MadStopLock==1)
    {
      if(AccelerationLock==1)
      {
        if(ParameterMouse==0)
        {
          LED_P6x8Char(0,ParameterMouse,'*');
          LED_P6x8Char(0,ParameterMouse+1,' ');
        }
        else if(ParameterMouse==4)
        {
          LED_P6x8Char(0,ParameterMouse-1,' ');
          LED_P6x8Char(0,ParameterMouse,'*');
        }
        else
        {
          LED_P6x8Char(0,ParameterMouse,'*');
          LED_P6x8Char(0,ParameterMouse-1,' ');
          LED_P6x8Char(0,ParameterMouse+1,' ');
        }
      }
      else
      {
        if(ParameterMouse==0)
        {
          LED_P6x8Char(0,ParameterMouse,'*');
          LED_P6x8Char(0,ParameterMouse+1,' ');
        }
        else if(ParameterMouse==3)
        {
          LED_P6x8Char(0,ParameterMouse-1,' ');
          LED_P6x8Char(0,ParameterMouse,'*');
        }
        else
        {
          LED_P6x8Char(0,ParameterMouse,'*');
          LED_P6x8Char(0,ParameterMouse-1,' ');
          LED_P6x8Char(0,ParameterMouse+1,' ');
        }
      }
    }
    else
    {
      if(AccelerationLock==1)
      {
        if(ParameterMouse==0)
        {
          LED_P6x8Char(0,ParameterMouse,'*');
          LED_P6x8Char(0,ParameterMouse+1,' ');
        }
        else if(ParameterMouse==3)
        {
          LED_P6x8Char(0,ParameterMouse-1,' ');
          LED_P6x8Char(0,ParameterMouse,'*');
        }
        else
        {
          LED_P6x8Char(0,ParameterMouse,'*');
          LED_P6x8Char(0,ParameterMouse-1,' ');
          LED_P6x8Char(0,ParameterMouse+1,' ');
        }
      }
      else
      {
        if(ParameterMouse==0)
        {
          LED_P6x8Char(0,ParameterMouse,'*');
          LED_P6x8Char(0,ParameterMouse+1,' ');
        }
        else if(ParameterMouse==2)
        {
          LED_P6x8Char(0,ParameterMouse-1,' ');
          LED_P6x8Char(0,ParameterMouse,'*');
        }
        else
        {
          LED_P6x8Char(0,ParameterMouse,'*');
          LED_P6x8Char(0,ParameterMouse-1,' ');
          LED_P6x8Char(0,ParameterMouse+1,' ');
        }
      }
    }
  }
  else
  {
    if(MadStopLock==1)
    {
      if(AccelerationLock==1)
      {
        if(ParameterMouse==0)
        {
          LED_P6x8Char(0,ParameterMouse,'*');
          LED_P6x8Char(0,ParameterMouse+1,' ');
        }
        else if(ParameterMouse==6)
        {
          LED_P6x8Char(0,ParameterMouse-1,' ');
          LED_P6x8Char(0,ParameterMouse,'*');
        }
        else
        {
          LED_P6x8Char(0,ParameterMouse,'*');
          LED_P6x8Char(0,ParameterMouse-1,' ');
          LED_P6x8Char(0,ParameterMouse+1,' ');
        }
      }
      else
      {
        if(ParameterMouse==0)
        {
          LED_P6x8Char(0,ParameterMouse,'*');
          LED_P6x8Char(0,ParameterMouse+1,' ');
        }
        else if(ParameterMouse==5)
        {
          LED_P6x8Char(0,ParameterMouse-1,' ');
          LED_P6x8Char(0,ParameterMouse,'*');
        }
        else
        {
          LED_P6x8Char(0,ParameterMouse,'*');
          LED_P6x8Char(0,ParameterMouse-1,' ');
          LED_P6x8Char(0,ParameterMouse+1,' ');
        }
      }
    }
    else
    {
      if(AccelerationLock==1)
      {
        if(ParameterMouse==0)
        {
          LED_P6x8Char(0,ParameterMouse,'*');
          LED_P6x8Char(0,ParameterMouse+1,' ');
        }
        else if(ParameterMouse==5)
        {
          LED_P6x8Char(0,ParameterMouse-1,' ');
          LED_P6x8Char(0,ParameterMouse,'*');
        }
        else
        {
          LED_P6x8Char(0,ParameterMouse,'*');
          LED_P6x8Char(0,ParameterMouse-1,' ');
          LED_P6x8Char(0,ParameterMouse+1,' ');
        }
      }
      else
      {
        if(ParameterMouse==0)
        {
          LED_P6x8Char(0,ParameterMouse,'*');
          LED_P6x8Char(0,ParameterMouse+1,' ');
        }
        else if(ParameterMouse==4)
        {
          LED_P6x8Char(0,ParameterMouse-1,' ');
          LED_P6x8Char(0,ParameterMouse,'*');
        }
        else
        {
          LED_P6x8Char(0,ParameterMouse,'*');
          LED_P6x8Char(0,ParameterMouse-1,' ');
          LED_P6x8Char(0,ParameterMouse+1,' ');
        }
      }
    }
  }
  
    unsigned char temp = 10;//此初值表示没有按键按下
    temp = KeyScan();//扫描按键，所有按键都没被按下返回10
    
    switch(temp)
    {
      case 1:flash_config();
              LED_Fill(0);
              LED_P6x8Str(42, 3,(unsigned char*)"Done!");
              LPLD_LPTMR_DelayMs(1000);
              LED_Fill(0);
              break;
      case 4:if(IntelligentSteeringLock==1)
              {
                if(ParameterMouse<(AccelerationLock+MadStopLock+2))
                  ParameterMouse++;
              }
              else
              {
                if(ParameterMouse<(AccelerationLock+MadStopLock+4))
                  ParameterMouse++;
              }
              Key_delay();
              break;
      case 3:if(ParameterMouse>0)
                ParameterMouse--;
              Key_delay();
              break;
      case 5:if(IntelligentSteeringLock==1)
              {
                if(MadStopLock==1)
                {
                  if(AccelerationLock==1)
                  {
                    if(ParameterMouse==0)
                    {
                      SpeedTarget_+=10;
                      if(SpeedTarget_>600)
                        SpeedTarget_=600;
                    }
                    else if(ParameterMouse==1)
                    {
                      MadStopLock = 1;
                    }
                    else if(ParameterMouse==2)
                    {
                      emergy_speed+=10;
                      if(emergy_speed>620)
                        emergy_speed=620;
                    }
                    else if(ParameterMouse==3)
                    {
                      AccelerationLock = 1;
                    }
                    else if(ParameterMouse==4)
                    {
                      acceleration_speed+=10;
                      if(acceleration_speed>550)
                        acceleration_speed=550;
                    }
                  }
                  else
                  {
                    if(ParameterMouse==0)
                    {
                      SpeedTarget_+=10;
                      if(SpeedTarget_>600)
                        SpeedTarget_=600;
                    }
                    else if(ParameterMouse==1)
                    {
                      MadStopLock = 1;
                    }
                    else if(ParameterMouse==2)
                    {
                      emergy_speed+=10;
                      if(emergy_speed>620)
                        emergy_speed=620;
                    }
                    else if(ParameterMouse==3)
                    {
                      AccelerationLock = 1;
                    }
                  }
                }
                else
                {
                  if(AccelerationLock==1)
                  {
                    if(ParameterMouse==0)
                    {
                      SpeedTarget_+=10;
                      if(SpeedTarget_>600)
                        SpeedTarget_=600;
                    }
                    else if(ParameterMouse==1)
                    {
                      MadStopLock = 1;
                    }
                    else if(ParameterMouse==2)
                    {
                      AccelerationLock = 1;
                    }
                    else if(ParameterMouse==3)
                    {
                      acceleration_speed+=10;
                      if(acceleration_speed>550)
                        acceleration_speed=550;
                    }
                  }
                  else
                  {
                    if(ParameterMouse==0)
                    {
                      SpeedTarget_+=10;
                      if(SpeedTarget_>600)
                        SpeedTarget_=600;
                    }
                    else if(ParameterMouse==1)
                    {
                      MadStopLock = 1;
                    }
                    else if(ParameterMouse==2)
                    {
                      AccelerationLock = 1;
                    }
                  }
                }
              }
             else
             {
               if(MadStopLock==1)
                {
                  if(AccelerationLock==1)
                  {
                    if(ParameterMouse==0)
                    {
                      SpeedTarget_+=10;
                      if(SpeedTarget_>600)
                        SpeedTarget_=600;
                    }
                    else if(ParameterMouse==1)
                    {
                      TurnInAdvanceDistance++;
                      if(TurnInAdvanceDistance>75)
                        TurnInAdvanceDistance=75;
                    }
                    else if(ParameterMouse==2)
                    {
                      TurnInAdvanceDistance2++;
                      if(TurnInAdvanceDistance2>75)
                        TurnInAdvanceDistance2=75;
                    }
                    else if(ParameterMouse==3)
                    {
                      MadStopLock = 1;
                    }
                    else if(ParameterMouse==4)
                    {
                      emergy_speed+=10;
                      if(emergy_speed>620)
                        emergy_speed=620;
                    }
                    else if(ParameterMouse==5)
                    {
                      AccelerationLock = 1;
                    }
                    else if(ParameterMouse==6)
                    {
                      acceleration_speed+=10;
                      if(acceleration_speed>550)
                        acceleration_speed=550;
                    }
                  }
                  else
                  {
                    if(ParameterMouse==0)
                    {
                      SpeedTarget_+=10;
                      if(SpeedTarget_>600)
                        SpeedTarget_=600;
                    }
                    else if(ParameterMouse==1)
                    {
                      TurnInAdvanceDistance++;
                      if(TurnInAdvanceDistance>75)
                        TurnInAdvanceDistance=75;
                    }
                    else if(ParameterMouse==2)
                    {
                      TurnInAdvanceDistance2++;
                      if(TurnInAdvanceDistance2>75)
                        TurnInAdvanceDistance2=75;
                    }
                    else if(ParameterMouse==3)
                    {
                      MadStopLock = 1;
                    }
                    else if(ParameterMouse==4)
                    {
                      emergy_speed+=10;
                      if(emergy_speed>620)
                        emergy_speed=620;
                    }
                    else if(ParameterMouse==5)
                    {
                      AccelerationLock = 1;
                    }
                  }
                }
                else
                {
                  if(AccelerationLock==1)
                  {
                    if(ParameterMouse==0)
                    {
                      SpeedTarget_+=10;
                      if(SpeedTarget_>600)
                        SpeedTarget_=600;
                    }
                    else if(ParameterMouse==1)
                    {
                      TurnInAdvanceDistance++;
                      if(TurnInAdvanceDistance>75)
                        TurnInAdvanceDistance=75;
                    }
                    else if(ParameterMouse==2)
                    {
                      TurnInAdvanceDistance2++;
                      if(TurnInAdvanceDistance2>75)
                        TurnInAdvanceDistance2=75;
                    }
                    else if(ParameterMouse==3)
                    {
                      MadStopLock = 1;
                    }
                    else if(ParameterMouse==4)
                    {
                      AccelerationLock = 1;
                    }
                    else if(ParameterMouse==5)
                    {
                      acceleration_speed+=10;
                      if(acceleration_speed>550)
                        acceleration_speed=550;
                    }
                  }
                  else
                  {
                    if(ParameterMouse==0)
                    {
                      SpeedTarget_+=10;
                      if(SpeedTarget_>600)
                        SpeedTarget_=600;
                    }
                    else if(ParameterMouse==1)
                    {
                      TurnInAdvanceDistance++;
                      if(TurnInAdvanceDistance>75)
                        TurnInAdvanceDistance=75;
                    }
                    else if(ParameterMouse==2)
                    {
                      TurnInAdvanceDistance2++;
                      if(TurnInAdvanceDistance2>75)
                        TurnInAdvanceDistance2=75;
                    }
                    else if(ParameterMouse==3)
                    {
                      MadStopLock = 1;
                    }
                    else if(ParameterMouse==4)
                    {
                      AccelerationLock = 1;
                    }
                  }
                }
             }
              Key_delay();
              break;
      case 6:if(IntelligentSteeringLock==1)
              {
                if(MadStopLock==1)
                {
                  if(AccelerationLock==1)
                  {
                    if(ParameterMouse==0)
                    {
                      SpeedTarget_-=10;
                      if(SpeedTarget_<300)
                        SpeedTarget_=300;
                    }
                    else if(ParameterMouse==1)
                    {
                      MadStopLock = 0;
                    }
                    else if(ParameterMouse==2)
                    {
                      emergy_speed-=10;
                      if(emergy_speed<400)
                        emergy_speed=400;
                    }
                    else if(ParameterMouse==3)
                    {
                      AccelerationLock = 0;
                    }
                    else if(ParameterMouse==4)
                    {
                      acceleration_speed-=10;
                      if(acceleration_speed<180)
                        acceleration_speed=180;
                    }
                  }
                  else
                  {
                    if(ParameterMouse==0)
                    {
                      SpeedTarget_-=10;
                      if(SpeedTarget_<300)
                        SpeedTarget_=300;
                    }
                    else if(ParameterMouse==1)
                    {
                      MadStopLock = 0;
                    }
                    else if(ParameterMouse==2)
                    {
                      emergy_speed-=10;
                      if(emergy_speed<400)
                        emergy_speed=400;
                    }
                    else if(ParameterMouse==3)
                    {
                      AccelerationLock = 0;
                    }
                  }
                }
                else
                {
                  if(AccelerationLock==1)
                  {
                    if(ParameterMouse==0)
                    {
                      SpeedTarget_-=10;
                      if(SpeedTarget_<300)
                        SpeedTarget_=300;
                    }
                    else if(ParameterMouse==1)
                    {
                      MadStopLock = 0;
                    }
                    else if(ParameterMouse==2)
                    {
                      AccelerationLock = 0;
                    }
                    else if(ParameterMouse==3)
                    {
                      acceleration_speed-=10;
                      if(acceleration_speed<180)
                        acceleration_speed=180;
                    }
                  }
                  else
                  {
                    if(ParameterMouse==0)
                    {
                      SpeedTarget_-=10;
                      if(SpeedTarget_<300)
                        SpeedTarget_=300;
                    }
                    else if(ParameterMouse==1)
                    {
                      MadStopLock = 0;
                    }
                    else if(ParameterMouse==2)
                    {
                      AccelerationLock = 0;
                    }
                  }
                }
              }
             else
             {
               if(MadStopLock==1)
                {
                  if(AccelerationLock==1)
                  {
                    if(ParameterMouse==0)
                    {
                      SpeedTarget_-=10;
                      if(SpeedTarget_<300)
                        SpeedTarget_=300;
                    }
                    else if(ParameterMouse==1)
                    {
                      TurnInAdvanceDistance--;
                      if(TurnInAdvanceDistance<45)
                        TurnInAdvanceDistance=45;
                    }
                    else if(ParameterMouse==2)
                    {
                      TurnInAdvanceDistance2--;
                      if(TurnInAdvanceDistance2<40)
                        TurnInAdvanceDistance2=40;
                    }
                    else if(ParameterMouse==3)
                    {
                      MadStopLock = 0;
                    }
                    else if(ParameterMouse==4)
                    {
                      emergy_speed-=10;
                      if(emergy_speed<400)
                        emergy_speed=400;
                    }
                    else if(ParameterMouse==5)
                    {
                      AccelerationLock = 0;
                    }
                    else if(ParameterMouse==6)
                    {
                      acceleration_speed-=10;
                      if(acceleration_speed<180)
                        acceleration_speed=180;
                    }
                  }
                  else
                  {
                    if(ParameterMouse==0)
                    {
                      SpeedTarget_-=10;
                      if(SpeedTarget_<300)
                        SpeedTarget_=300;
                    }
                    else if(ParameterMouse==1)
                    {
                      TurnInAdvanceDistance--;
                      if(TurnInAdvanceDistance<45)
                        TurnInAdvanceDistance=45;
                    }
                    else if(ParameterMouse==2)
                    {
                      TurnInAdvanceDistance2--;
                      if(TurnInAdvanceDistance2<40)
                        TurnInAdvanceDistance2=40;
                    }
                    else if(ParameterMouse==3)
                    {
                      MadStopLock = 0;
                    }
                    else if(ParameterMouse==4)
                    {
                      emergy_speed-=10;
                      if(emergy_speed<400)
                        emergy_speed=400;
                    }
                    else if(ParameterMouse==5)
                    {
                      AccelerationLock = 0;
                    }
                  }
                }
                else
                {
                  if(AccelerationLock==1)
                  {
                    if(ParameterMouse==0)
                    {
                      SpeedTarget_-=10;
                      if(SpeedTarget_<300)
                        SpeedTarget_=300;
                    }
                    else if(ParameterMouse==1)
                    {
                      TurnInAdvanceDistance--;
                      if(TurnInAdvanceDistance<45)
                        TurnInAdvanceDistance=45;
                    }
                    else if(ParameterMouse==2)
                    {
                      TurnInAdvanceDistance2--;
                      if(TurnInAdvanceDistance2<40)
                        TurnInAdvanceDistance2=40;
                    }
                    else if(ParameterMouse==3)
                    {
                      MadStopLock = 0;
                    }
                    else if(ParameterMouse==4)
                    {
                      AccelerationLock = 0;
                    }
                    else if(ParameterMouse==5)
                    {
                      acceleration_speed-=10;
                      if(acceleration_speed<180)
                        acceleration_speed=180;
                    }
                  }
                  else
                  {
                    if(ParameterMouse==0)
                    {
                      SpeedTarget_-=10;
                      if(SpeedTarget_<300)
                        SpeedTarget_=300;
                    }
                    else if(ParameterMouse==1)
                    {
                      TurnInAdvanceDistance--;
                      if(TurnInAdvanceDistance<45)
                        TurnInAdvanceDistance=45;
                    }
                    else if(ParameterMouse==2)
                    {
                      TurnInAdvanceDistance2--;
                      if(TurnInAdvanceDistance2<40)
                        TurnInAdvanceDistance2=40;
                    }
                    else if(ParameterMouse==3)
                    {
                      MadStopLock = 0;
                    }
                    else if(ParameterMouse==4)
                    {
                      AccelerationLock = 0;
                    }
                  }
                }
             }
              Key_delay();
              break;
    }
}