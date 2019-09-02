#include "common.h"
#include "camera_test.h"
#include "math.h"
#include "QD.h"
#include "oled.h"

#define LPH_Slope -1.5619
#define LPH_Intercept 140
#define RPH_Slope 1.7406
#define RPH_Intercept -158

uint8 SetCenterX,SCenter=90;
uint8 ph_lock1,ph_lock2;

//需要配置到摄像头的数据
int16 MT9V032_CFG[CONFIG_FINISH][2]=
{
    {AUTO_EXP,          0},   //自动曝光设置      范围1-63 0为关闭 如果自动曝光开启  EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
                              //一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
    {EXP_TIME,          300}, //曝光时间          摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
    {FPS,               200},  //图像帧率          摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
    {SET_COL,           COLUMN}, //图像列数量        范围1-752     K60采集不允许超过188
    {SET_ROW,           ROW}, //图像行数量        范围1-480
    {LR_OFFSET,         0},   //图像左右偏移量    正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    {UD_OFFSET,         0},   //图像上下偏移量    正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    {GAIN,              50},  //图像增益          范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度

    
    {INIT,              0}    //摄像头开始初始化
};
//从摄像头内部获取到的配置数据
int16 GET_CFG[CONFIG_FINISH-1][2]=
{
    {AUTO_EXP,          0},   //自动曝光设置      
    {EXP_TIME,          0},   //曝光时间          
    {FPS,               0},   //图像帧率          
    {SET_COL,           0},   //图像列数量        
    {SET_ROW,           0},   //图像行数量        
    {LR_OFFSET,         0},   //图像左右偏移量    
    {UD_OFFSET,         0},   //图像上下偏移量    
    {GAIN,              0},   //图像增益          
};

unsigned char Image1[ROW][COLUMN];
unsigned char Image2[ROW][COLUMN];
unsigned char Image3[ROW][COLUMN];

uint8 end_flag=0; 
uint8 lock=0;
uint8 WhichBuffer = 1;
uint8 setvalue=50;//亮点亮度阈值
uint8 NOpoint=1,real_pointx,real_pointy;

uint8   receive[3];
uint8   receive_num = 0;
uint8   uart_receive_flag = 1;

void camera_init(void)
{
 static GPIO_InitTypeDef dmaReq_port_struct;
  dmaReq_port_struct.GPIO_PTx = PTB;
  dmaReq_port_struct.GPIO_Pins = GPIO_Pin22;
  dmaReq_port_struct.GPIO_Dir = DIR_INPUT;
  dmaReq_port_struct.GPIO_PinControl =IRQC_DMARI | INPUT_PULL_DOWN;//上升沿产生DMA请求,输入上拉
  LPLD_GPIO_Init(dmaReq_port_struct);
  
//  static GPIO_InitTypeDef href_port_struct;  //行中断接受端口
//  href_port_struct.GPIO_PTx = PTB;    
//  href_port_struct.GPIO_Pins = GPIO_Pin20;   
//  href_port_struct.GPIO_Dir = DIR_INPUT;        //输入
//  href_port_struct.GPIO_PinControl = IRQC_FA|INPUT_PULL_DOWN|INPUT_PF_EN;  //内部上拉|下降沿中断
//  href_port_struct.GPIO_Isr = portb_isr;        //中断函数
//  LPLD_GPIO_Init(href_port_struct);
//  LPLD_GPIO_EnableIrq(href_port_struct);
  
  static GPIO_InitTypeDef vsync_port_struct;  //场中断接受端口
  vsync_port_struct.GPIO_PTx = PTB;    
  vsync_port_struct.GPIO_Pins = GPIO_Pin23;   
  vsync_port_struct.GPIO_Dir = DIR_INPUT;        //输入
  vsync_port_struct.GPIO_PinControl = IRQC_FA|INPUT_PULL_DOWN|INPUT_PF_EN;    //内部下拉|下降沿中断
  vsync_port_struct.GPIO_Isr = portb_isr;        //中断函数
  LPLD_GPIO_Init(vsync_port_struct);
  LPLD_GPIO_EnableIrq(vsync_port_struct);
  
  //摄像头到单片机数据口初始化：PTC0~PTC7
  static GPIO_InitTypeDef data_port_struct;
  data_port_struct.GPIO_PTx = PTC;
  data_port_struct.GPIO_Dir = DIR_INPUT;
  data_port_struct.GPIO_Pins = GPIO_Pin0_7;
  data_port_struct.GPIO_PinControl = IRQC_DIS|INPUT_PULL_DIS;
  LPLD_GPIO_Init(data_port_struct);
  
  //DMA参数配置
  static DMA_InitTypeDef  dma_init_struct;
  dma_init_struct.DMA_CHx = DMA_CH0;    //CH0通道
  dma_init_struct.DMA_Req = PORTB_DMAREQ;     //PORTB22为请求源
  dma_init_struct.DMA_MajorLoopCnt = COLUMN*ROW;   //主循环计数值：行采集点数，宽度
  dma_init_struct.DMA_MinorByteCnt = 1; //次循环字节计数：每次传输1字节
  dma_init_struct.DMA_SourceAddr = (uint32)&PTC->PDIR; //源地址：PTC0-PTC7,Port Data Input Register
  dma_init_struct.DMA_DestAddr = (uint32)Image1;      //目的地址：存放图像的数组
  dma_init_struct.DMA_DestAddrOffset = 1;       //目的地址偏移：每次读入增加1
  dma_init_struct.DMA_AutoDisableReq = TRUE;    //自动禁用请求，使能后通道请求将在主循环结束后禁用
  LPLD_DMA_Init(dma_init_struct);//初始化但未使能
  
  static UART_InitTypeDef uart0_init_struct;
  uart0_init_struct.UART_Uartx = UART0; //UART0用于设置摄像头
  uart0_init_struct.UART_BaudRate = 9600;
  uart0_init_struct.UART_RxPin = PTB16;  //接收引脚为PTB16
  uart0_init_struct.UART_TxPin = PTB17;  //发送引脚为PTB17
  uart0_init_struct.UART_RxIntEnable=TRUE;//使能接受中断
  uart0_init_struct.UART_TxIntEnable=FALSE;//关闭发送中断
  uart0_init_struct.UART_RxIsr=mt9v032_cof_uart_interrupt;
  LPLD_UART_Init(uart0_init_struct); //初始化UART0
  LPLD_UART_EnableIrq(uart0_init_struct);//中断使能
  
  MT9V032_CFG_init();//MT9V032设置
}


void picture_send(void)
{
  int i,j;
    if(end_flag)
    {
      disable_irq(PORTB_IRQn); 
      LPLD_UART_PutChar(UART0,0X01);
      LPLD_UART_PutChar(UART0,0XFE);
      for(i=0;i<ROW;i++)
        for(j=0;j<COLUMN;j++)
            LPLD_UART_PutChar(UART0,Image1[i][j]);
      LPLD_UART_PutChar(UART0,0XFE);
      LPLD_UART_PutChar(UART0,0X01);
      enable_irq(PORTB_IRQn);
      end_flag=0;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取摄像头内部配置信息
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				调用该函数前请先初始化uart0
//-------------------------------------------------------------------------------------------------------------------
void get_config(void)
{
	uint16 temp, i;
    uint8  send_buffer[4];
    
    for(i=0; i<CONFIG_FINISH-1; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = GET_STATUS;
        temp = GET_CFG[i][0];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;
        
        LPLD_UART_PutChar(UART0,send_buffer[0]);
        LPLD_UART_PutChar(UART0,send_buffer[1]);
        LPLD_UART_PutChar(UART0,send_buffer[2]);
        LPLD_UART_PutChar(UART0,send_buffer[3]);
        
        //等待接受回传数据
        while(!uart_receive_flag);
        uart_receive_flag = 0;
        
        GET_CFG[i][1] = receive[1]<<8 | receive[2];
        
    }
}
void MT9V032_CFG_init(void)
{       //设置参数    具体请参看使用手册
    uint16 temp, i,j;
    uint8  send_buffer[4];
    my_delay(9999999);//延时以保证摄像头上面的51完成上电初始化
    my_delay(9999999);//延时以保证摄像头上面的51完成上电初始化
   //开始配置摄像头并重新初始化
    for(i=0; i<CONFIG_FINISH; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = MT9V032_CFG[i][0];
        temp = MT9V032_CFG[i][1];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;
        for(j=0;j<4;j++)
        {
          LPLD_UART_PutChar(UART0,send_buffer[j]);
          LPLD_LPTMR_DelayMs(2);
        }
    }    
//    while(!uart_receive_flag);
//    uart_receive_flag = 0;
//    while((0xff != receive[1]) || (0xff != receive[2]));
//    
//     //获取配置便于查看配置是否正确
//    get_config();
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头串口中断函数
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       该函数在ISR文件 串口3中断程序被调用
//-------------------------------------------------------------------------------------------------------------------
void mt9v032_cof_uart_interrupt(void)
{
    receive[receive_num]=LPLD_UART_GetChar(UART0);
    receive_num++;
    
    if(1==receive_num && 0XA5!=receive[0])  receive_num = 0;
    if(3 == receive_num)
    {
        receive_num = 0;
        uart_receive_flag = 1;
    }
        
}

void portb_isr()
{  
   if(LPLD_GPIO_IsPinxExt(PORTB, GPIO_Pin23)) // 场中断         
  {     
    LPLD_GPIO_ClearIntFlag(PORTB);
    LPLD_DMA_DisableReq(DMA_CH0);//采集结束，屏蔽DMA请求源
    if(WhichBuffer==1)
      WhichBuffer=2;  //切换传图，图1图2依次传输
    else if(WhichBuffer==2)
      WhichBuffer=3;
    else if(WhichBuffer==3)
      WhichBuffer=1;
    
    
    if(WhichBuffer==1&&lock==1)                          //将要传图1且图1没有在处理，或者将要传图2且图2没有在处理
    {
      LPLD_DMA_LoadDstAddr(DMA_CH0,(Image2[0]));//DMA启动标志，采集开始
      end_flag=1;
    }
    else if(WhichBuffer==2&&lock==2)   
      LPLD_DMA_LoadDstAddr(DMA_CH0,(Image3[0]));//DMA启动标志，采集开始
    else if(WhichBuffer==3&&lock==3) 
      LPLD_DMA_LoadDstAddr(DMA_CH0,(Image1[0]));//DMA启动标志，采集开始
    else
    {
        if(WhichBuffer==1)
        LPLD_DMA_LoadDstAddr(DMA_CH0,(Image1[0]));//加载或更换目的地址
        else if(WhichBuffer==2)
        {
        LPLD_DMA_LoadDstAddr(DMA_CH0,(Image2[0]));//加载或更换目的地址
        end_flag=1;
        }
        else if(WhichBuffer==3)
        LPLD_DMA_LoadDstAddr(DMA_CH0,(Image3[0]));//加载或更换目的地址
    }    
    LPLD_DMA_EnableReq(DMA_CH0);//初始化中设定主循环结束屏蔽请求源，需重新使能请求源
  }
}
void my_delay(long t)
{
    while(t--);
}

/*------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

20180319        同从机图像处理方式，需计算程序运行时间
20180319:未完结 只计算Y的中点，然后存储其对应行
20180320:编辑完成，未测试是否有BUG
20180320:初步测试，未发现BUG，运行时间2900Us。X、Y值基本稳定，存在1、2格的变动。
         主要时间消耗在获取mini和minj上，单BufferByte变换耗时2500Us。
20180628:限定畸变，15行之后取值，15到20行只扫描宽度左右减20之间的图像，20行以后全扫描。
20180711:限定视野范围，删除20180628所做修改。数据见matlab，采用查表方式完成起始值与结束值设定。

///////////////////////////////////////////////////////////////////////////////
------------------------------------------------------------------------------*/

const uint8 j_BeginAndEnd[90][2]=
{
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{60,110},{55,116},{49,123},{45,126},//{67,103},
  {41,130},{38,134},{34,138},{30,141},{28,144},{26,146},{23,149},{21,151},{20,153},{18,156},
  {16,158},{14,159},{12,161},{10,163},{9 ,165},{8 ,167},{6 ,168},{5 ,169},{3 ,171},{1 ,172},
  {1 ,174},{1 ,175},{1 ,175},{1 ,177},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180}
};

uint8 j_begin,j_end;

void Calculate_Middle_Point(void)
{
  register uint8 i,j,BufferByte=0,value=setvalue;
  uint8 pointy;
  uint8 InvalidBuffer=1;
  uint8 maxi=0,mini=ROW,maxj=0,minj=0;
  uint8 Done_J=0;
  
  if(WhichBuffer == 2)//正在传图2
  {
    lock=1;//锁图1，处理图1
    for(i=0;i<ROW;i++)
    {
      j_begin=j_BeginAndEnd[i][0];
      j_end=j_BeginAndEnd[i][1];
      BufferByte=0;
      for(j=j_begin;j<j_end;j++)     //Mr.Li的备注      ？？？若是有干扰点出现在信标灯所在行，下面的行minj和maxj就会全错 （貌似程序逻辑假设无干扰点）
      {
         BufferByte=BufferByte<<1;
         BufferByte|=(Image1[i][j]>=value?1:0);
         BufferByte&=3;
         if(BufferByte==3)
         {
           if(i<=mini)
             mini=i;
           if(i>=maxi)
             maxi=i;
           InvalidBuffer=0;
           break;
         }
      }
    }
    if(InvalidBuffer==1)
    {
      NOpoint=1;
    }
    else
    {
      BufferByte=0;Done_J=0;
      pointy=(mini+maxi)/2;
      j_begin=j_BeginAndEnd[pointy][0];
      j_end=j_BeginAndEnd[pointy][1];
      for(j=j_begin;j<j_end;j++)
      {
        BufferByte=BufferByte<<1;
        BufferByte|=(Image1[pointy][j]>=value?1:0);
        BufferByte&=7;
        if(j<j_end-1)
        {
          switch(BufferByte)
          {
            case 3:  if(Done_J==0)
                      {
                        minj=j-2;
                        Done_J=1;
                      }
                      break;
            case 6: maxj=j-1;
                      break;
          }
        }
        else
        {
          switch(BufferByte)
          {
            case 3:  if(Done_J==0)
                      {
                        minj=j-2;
                        maxj=j;
                        Done_J=1;
                      }
                      break;
            case 6: maxj=j-1;
                      break;
            case 7: maxj=j;
                      break;
          }
        }
      }
      if(Done_J==0)
      {
        NOpoint=1;
      }
      else
      {
        real_pointy=pointy;
        real_pointx=(maxj+minj)/2;
        NOpoint=0;
      }
    }
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  else if(WhichBuffer == 3)//正在传图1
  {
    lock=2;//锁图2，处理图2
    for(i=0;i<ROW;i++)
    {
      BufferByte=0;
      j_begin=j_BeginAndEnd[i][0];
      j_end=j_BeginAndEnd[i][1];
      for(j=j_begin;j<j_end;j++)     //Mr.Li的备注      ？？？若是有干扰点出现在信标灯所在行，下面的行minj和maxj就会全错 （貌似程序逻辑假设无干扰点）
      {
         BufferByte=BufferByte<<1;
         BufferByte|=(Image2[i][j]>=value?1:0);
         BufferByte&=3;
         if(BufferByte==3)
         {
           if(i<=mini)
             mini=i;
           if(i>=maxi)
             maxi=i;
           InvalidBuffer=0;
           break;
         }
      }
    }
    if(InvalidBuffer==1)
    {
      NOpoint=1;
    }
    else
    {
      BufferByte=0;Done_J=0;
      pointy=(mini+maxi)/2;
      j_begin=j_BeginAndEnd[pointy][0];
      j_end=j_BeginAndEnd[pointy][1];
      for(j=j_begin;j<j_end;j++)
      {
        BufferByte=BufferByte<<1;
        BufferByte|=(Image2[pointy][j]>=value?1:0);
        BufferByte&=7;
        if(j<j_end-1)
        {
          switch(BufferByte)
          {
            case 3:  if(Done_J==0)
                      {
                        minj=j-2;
                        Done_J=1;
                      }
                      break;
            case 6: maxj=j-1;
                      break;
          }
        }
        else
        {
          switch(BufferByte)
          {
            case 3:  if(Done_J==0)
                      {
                        minj=j-2;
                        maxj=j;
                        Done_J=1;
                      }
                      break;
            case 6: maxj=j-1;
                      break;
            case 7: maxj=j;
                      break;
          }
        }
      }
      if(Done_J==0)
      {
        NOpoint=1;
      }
      else
      {
        real_pointy=pointy;
        real_pointx=(maxj+minj)/2;
        NOpoint=0;
      }
    }
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////
    else if(WhichBuffer == 1)//正在传图1
    {
      lock=3;//锁图2，处理图2
      for(i=0;i<ROW;i++)
      {
        BufferByte=0;
        j_begin=j_BeginAndEnd[i][0];
        j_end=j_BeginAndEnd[i][1];
        for(j=j_begin;j<j_end;j++)     //Mr.Li的备注      ？？？若是有干扰点出现在信标灯所在行，下面的行minj和maxj就会全错 （貌似程序逻辑假设无干扰点）
        {
           BufferByte=BufferByte<<1;
           BufferByte|=(Image3[i][j]>=value?1:0);
           BufferByte&=3;
           if(BufferByte==3)
           {
             if(i<=mini)
               mini=i;
             if(i>=maxi)
               maxi=i;
             InvalidBuffer=0;
             break;
           }
        }
      }
      if(InvalidBuffer==1)
      {
        NOpoint=1;
      }
      else
      {
        BufferByte=0;Done_J=0;
        pointy=(mini+maxi)/2;
        j_begin=j_BeginAndEnd[pointy][0];
        j_end=j_BeginAndEnd[pointy][1];
        for(j=j_begin;j<j_end;j++)
        {
          BufferByte=BufferByte<<1;
          BufferByte|=(Image3[pointy][j]>=value?1:0);
          BufferByte&=7;
          if(j<j_end-1)
          {
            switch(BufferByte)
            {
              case 3:  if(Done_J==0)
                        {
                          minj=j-2;
                          Done_J=1;
                        }
                        break;
              case 6: maxj=j-1;
                        break;
            }
          }
          else
          {
            switch(BufferByte)
            {
              case 3:  if(Done_J==0)
                        {
                          minj=j-2;
                          maxj=j;
                          Done_J=1;
                        }
                        break;
              case 6: maxj=j-1;
                        break;
              case 7: maxj=j;
                        break;
            }
          }
        }
        if(Done_J==0)
        {
          NOpoint=1;
        }
        else
        {
          real_pointy=pointy;
          real_pointx=(maxj+minj)/2;
          NOpoint=0;
        }
      }
    }
  }

/*-------------------靠右靠左中点校正---------------*/
void Middle_Point_Configure( void )  
{
  
    if((speed_right>400 && ph_lock2!=1)|| ph_lock1==1)
    {
      ph_lock1=1;         //偏航锁定
      if(real_pointy>(DistortionLimit_L_Slope*real_pointx+DistortionLimit_L_Intercept)&&real_pointy>(DistortionLimit_R_Slope*real_pointx+DistortionLimit_R_Intercept))  //RightSide_Go判断左右
        if (RightSide_Go == 0)  SetCenterX = (uint8) ((real_pointy-RPH_Intercept)/RPH_Slope); // 82
        else  SetCenterX = (uint8) ((real_pointy-LPH_Intercept)/LPH_Slope);
                  
      else SetCenterX=SCenter;
    }
    else if((speed_right<=400 &&  ph_lock1!=1) || ph_lock2==1)
    {
      ph_lock2=1;        //偏航锁定
      if(real_pointy>(DistortionLimit_L_Slope*real_pointx+DistortionLimit_L_Intercept)&&real_pointy>(DistortionLimit_R_Slope*real_pointx+DistortionLimit_R_Intercept))  //RightSide_Go判断左右
        if (RightSide_Go == 0)  SetCenterX = (uint8) ((real_pointy-RPH_Intercept)/RPH_Slope); // 82
        else  SetCenterX = (uint8) ((real_pointy-LPH_Intercept)/LPH_Slope);
                  
      else SetCenterX=SCenter;
    }
    else
    {
      if(real_pointy>(DistortionLimit_L_Slope*real_pointx+DistortionLimit_L_Intercept)&&real_pointy>(DistortionLimit_R_Slope*real_pointx+DistortionLimit_R_Intercept))  //RightSide_Go判断左右
        if (RightSide_Go == 0)  SetCenterX = (uint8) ((real_pointy-RPH_Intercept)/RPH_Slope); // 82
        else  SetCenterX = (uint8) ((real_pointy-LPH_Intercept)/LPH_Slope);
                  
      else SetCenterX=SCenter;
    }

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示图像
//  @param      high  		图像高度
//  @param      width 		图像宽度
//  @param      *p    		图像地址（数组）
//  @param      value 		二值化阀值
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void dis_bmp(uint16 high, uint16 width, uint8 *p,uint8 value)
{
    int16 i,j;
    int16 temp,temp1;
    uint8 dat;
    
    
    temp1 = high%8;
    if(temp1 == 0) temp = high/8;
    else           temp = high/8+1;
    
 
    for(i=0; i<temp; i++)
    {
        LED_Set_Pos(0,i);
        for(j=0; j<width; j++)
        {
            dat = 0;
            if( i<(temp-1) || !temp1 || temp1>=1)dat |= (*(p+i*8*width+j+width*0) > value? 1: 0)<<0;
            if( i<(temp-1) || !temp1 || temp1>=2)dat |= (*(p+i*8*width+j+width*1) > value? 1: 0)<<1;
            if( i<(temp-1) || !temp1 || temp1>=3)dat |= (*(p+i*8*width+j+width*2) > value? 1: 0)<<2;
            if( i<(temp-1) || !temp1 || temp1>=4)dat |= (*(p+i*8*width+j+width*3) > value? 1: 0)<<3;
            if( i<(temp-1) || !temp1 || temp1>=5)dat |= (*(p+i*8*width+j+width*4) > value? 1: 0)<<4;
            if( i<(temp-1) || !temp1 || temp1>=6)dat |= (*(p+i*8*width+j+width*5) > value? 1: 0)<<5;
            if( i<(temp-1) || !temp1 || temp1>=7)dat |= (*(p+i*8*width+j+width*6) > value? 1: 0)<<6;
            if( i<(temp-1) || !temp1 || temp1>=8)dat |= (*(p+i*8*width+j+width*7) > value? 1: 0)<<7;
           
            LED_WrDat(dat);
        }
    }
}

//void dis_bmp_half(uint16 high, uint16 width, uint8 *p,uint8 value)
//{
//    int16 i,j;
//    int16 temp,temp1;
//    uint8 dat;
//    
//    
//    temp1 = high%8;
//    if(temp1 == 0) temp = high/8;
//    else           temp = high/8+1;
//    
// 
//    for(i=0; i<temp; i++)
//    {
//        LED_Set_Pos(0,i);
//        for(j=0; j<width; j++)
//        {
//            dat = 0;
//            if( i<(temp-1) || !temp1 || temp1>=1)dat |= (*(p+i*8*width*2*2+j*2+width*0*2*2) > value? 1: 0)<<0;
//            if( i<(temp-1) || !temp1 || temp1>=2)dat |= (*(p+i*8*width*2*2+j*2+width*1*2*2) > value? 1: 0)<<1;
//            if( i<(temp-1) || !temp1 || temp1>=3)dat |= (*(p+i*8*width*2*2+j*2+width*2*2*2) > value? 1: 0)<<2;
//            if( i<(temp-1) || !temp1 || temp1>=4)dat |= (*(p+i*8*width*2*2+j*2+width*3*2*2) > value? 1: 0)<<3;
//            if( i<(temp-1) || !temp1 || temp1>=5)dat |= (*(p+i*8*width*2*2+j*2+width*4*2*2) > value? 1: 0)<<4;
//            if( i<(temp-1) || !temp1 || temp1>=6)dat |= (*(p+i*8*width*2*2+j*2+width*5*2*2) > value? 1: 0)<<5;
//            if( i<(temp-1) || !temp1 || temp1>=7)dat |= (*(p+i*8*width*2*2+j*2+width*6*2*2) > value? 1: 0)<<6;
//            if( i<(temp-1) || !temp1 || temp1>=8)dat |= (*(p+i*8*width*2*2+j*2+width*7*2*2) > value? 1: 0)<<7;
//           
//            LED_WrDat(dat);
//        }
//    }
//}

void dis_bmp_half(uint16 high, uint16 width, uint8 *p,uint8 value)
{
    int16 i,j;
    int16 temp,temp1;
    uint8 dat;
    
    
    temp1 = high%8;
    if(temp1 == 0) temp = high/8;
    else           temp = high/8+1;
    
 
    for(i=0; i<temp; i++)
    {
        LED_Set_Pos(0,i);
        for(j=0; j<width; j++)
        {
            dat = 0;
            if( i<(temp-1) || !temp1 || temp1>=1)
            {
              if(((2*j)>=j_BeginAndEnd[i*8*2+0*2][0])&&((2*j)<=j_BeginAndEnd[i*8*2+0*2][1]))
              {
                  dat |= (*(p+i*8*width*2*2+j*2+width*0*2*2) > value? 1: 0)<<0;
              }
              else
                dat &= 1<<0;
            }
            if( i<(temp-1) || !temp1 || temp1>=2)
            {
              if(((2*j)>=j_BeginAndEnd[i*8*2+1*2][0])&&((2*j)<=j_BeginAndEnd[i*8*2+1*2][1]))
              {
                  dat |= (*(p+i*8*width*2*2+j*2+width*1*2*2) > value? 1: 0)<<1;;
              }
              else
                dat &= 1<<1;
            }
            if( i<(temp-1) || !temp1 || temp1>=3)
            {
              if(((2*j)>=j_BeginAndEnd[i*8*2+2*2][0])&&((2*j)<=j_BeginAndEnd[i*8*2+2*2][1]))
              {
                  dat |= (*(p+i*8*width*2*2+j*2+width*2*2*2) > value? 1: 0)<<2;
              }
              else
                dat &= 1<<2;
            }
            if( i<(temp-1) || !temp1 || temp1>=4)
            {
              if(((2*j)>=j_BeginAndEnd[i*8*2+3*2][0])&&((2*j)<=j_BeginAndEnd[i*8*2+3*2][1]))
              {
                  dat |= (*(p+i*8*width*2*2+j*2+width*3*2*2) > value? 1: 0)<<3;
              }
              else
                dat &= 1<<3;
            }
            if( i<(temp-1) || !temp1 || temp1>=5)
            {
              if(((2*j)>=j_BeginAndEnd[i*8*2+4*2][0])&&((2*j)<=j_BeginAndEnd[i*8*2+4*2][1]))
              {
                  dat |= (*(p+i*8*width*2*2+j*2+width*4*2*2) > value? 1: 0)<<4;
              }
              else
                dat &= 1<<4;
            }
            if( i<(temp-1) || !temp1 || temp1>=6)
            {
              if(((2*j)>=j_BeginAndEnd[i*8*2+5*2][0])&&((2*j)<=j_BeginAndEnd[i*8*2+5*2][1]))
              {
                  dat |= (*(p+i*8*width*2*2+j*2+width*5*2*2) > value? 1: 0)<<5;
              }
              else
                dat &= 1<<5;
            }
            if( i<(temp-1) || !temp1 || temp1>=7)
            {
              if(((2*j)>=j_BeginAndEnd[i*8*2+6*2][0])&&((2*j)<=j_BeginAndEnd[i*8*2+6*2][1]))
              {
                  dat |= (*(p+i*8*width*2*2+j*2+width*6*2*2) > value? 1: 0)<<6;
              }
              else
                dat &= 1<<6;
            }
            if( i<(temp-1) || !temp1 || temp1>=8)
            {
              if(((2*j)>=j_BeginAndEnd[i*8*2+7*2][0])&&((2*j)<=j_BeginAndEnd[i*8*2+7*2][1]))
              {
                  dat |= (*(p+i*8*width*2*2+j*2+width*7*2*2) > value? 1: 0)<<7;
              }
              else
                dat &= 1<<7;
            }
           
            LED_WrDat(dat);
        }
    }
}

const uint8 Distortion_40[90][2]=
{
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{58,106},{56,106},
  {56,112},{52,117},{50,120},{47,123},{44,126},{41,128},{38,130},{37,131},{35,134},{32,135},
  {31,138},{30,140},{29,142},{27,143},{26,145},{25,146},{24,148},{23,150},{21,151},{20,152},
  {19,154},{18,154},{17,156},{16,157},{15,158},{14,158},{14,159},{13,160},{12,161},{12,162},
  {11,162},{9 ,163},{8 ,165},{7 ,166},{5 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
};

const uint8 Distortion_60[90][2]=
{
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{63,98 },{63,98 },{59,101},{55,107},{52,112},
  {49,116},{45,119},{43,123},{40,126},{37,129},{35,132},{33,135},{31,137},{29,139},{27,141},
  {25,143},{24,145},{23,146},{21,149},{20,150},{18,152},{16,153},{16,154},{15,155},{13,157},
  {12,158},{11,159},{10,160},{9 ,162},{8 ,163},{7 ,164},{7 ,165},{6 ,165},{6 ,166},{5 ,167},
  {5 ,168},{1 ,168},{1 ,169},{1 ,170},{1 ,170},{1 ,171},{1 ,171},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
};

const uint8 Distortion_80[90][2]=
{
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{63,101},{63,101},
  {58,109},{55,113},{52,118},{47,122},{44,126},{41,129},{38,132},{36,135},{33,138},{31,140},
  {28,142},{26,145},{24,147},{22,148},{21,150},{19,152},{17,153},{16,155},{15,156},{13,158},
  {12,159},{11,160},{10,161},{9 ,163},{7 ,163},{6 ,164},{5 ,166},{5 ,167},{1 ,168},{1 ,169},
  {1 ,170},{1 ,170},{1 ,171},{1 ,172},{1 ,173},{1 ,174},{1 ,174},{1 ,175},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
};

const uint8 Distortion_100[90][2]=
{
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },
  {0 ,0  },{0 ,0  },{73,97 },{73,97 },{61,104},{57,110},{52,115},{48,118},{45,122},{40,126},
  {37,129},{35,133},{33,136},{31,139},{29,141},{27,143},{25,146},{23,147},{21,149},{18,151},
  {17,153},{16,155},{14,156},{13,158},{11,160},{10,161},{9 ,162},{8 ,164},{7 ,165},{6 ,166},
  {5 ,167},{4 ,169},{3 ,169},{1 ,170},{1 ,171},{1 ,172},{1 ,173},{1 ,174},{1 ,175},{1 ,176},
  {1 ,176},{1 ,176},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
};

const uint8 Distortion_120[90][2]=
{
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{74,97 },
  {74,97 },{64,106},{58,112},{52,117},{47,122},{43,126},{41,130},{38,133},{34,135},{32,138},
  {29,140},{27,143},{25,145},{23,148},{21,149},{19,151},{18,154},{16,155},{14,157},{13,158},
  {11,160},{10,162},{8 ,163},{7 ,165},{6 ,166},{5 ,167},{4 ,168},{3 ,169},{1 ,170},{1 ,171},
  {1 ,172},{1 ,173},{1 ,174},{1 ,176},{1 ,176},{1 ,177},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
};

const uint8 Distortion_140[90][2]=
{
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{67,105},{67,105},{60,112},
  {54,117},{50,122},{47,125},{43,129},{40,132},{37,135},{34,138},{32,141},{30,143},{27,146},
  {25,148},{23,150},{21,152},{19,154},{17,155},{15,157},{14,159},{13,160},{11,162},{9 ,163},
  {8 ,164},{7 ,165},{6 ,167},{5 ,168},{3 ,169},{2 ,170},{1 ,171},{1 ,172},{1 ,173},{1 ,174},
  {1 ,175},{1 ,176},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
};

const uint8 Distortion_160[90][2]=
{
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{73,95 },{73,95 },{65,106},{57,113},{53,118},
  {48,123},{44,127},{40,130},{37,134},{34,137},{32,139},{29,142},{27,145},{24,147},{22,150},
  {20,151},{18,153},{16,155},{15,157},{13,159},{12,160},{10,162},{9 ,163},{8 ,164},{7 ,166},
  {5 ,167},{4 ,169},{3 ,170},{1 ,172},{1 ,172},{1 ,173},{1 ,175},{1 ,176},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
};

const uint8 Distortion_180[90][2]=
{
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{66,105},{66,105},{58,112},{54,117},{50,121},{46,126},
  {43,129},{39,132},{36,136},{33,139},{31,141},{29,144},{26,146},{24,148},{22,150},{19,152},
  {18,154},{16,156},{14,158},{13,160},{11,161},{10,163},{9 ,164},{7 ,166},{6 ,167},{4 ,169},
  {3 ,170},{1 ,171},{1 ,172},{1 ,174},{1 ,175},{1 ,176},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
};

const uint8 Distortion_200[90][2]=
{
  {0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },{0 ,0  },
  {0 ,0  },{0 ,0  },{75,98 },{75,98 },{62,109},{57,115},{53,119},{48,124},{45,129},{41,132},
  {38,135},{35,138},{32,141},{30,142},{27,145},{25,148},{23,150},{21,152},{20,154},{18,156},
  {16,158},{14,159},{13,161},{11,162},{9 ,164},{8 ,165},{7 ,167},{5 ,168},{4 ,170},{1 ,171},
  {1 ,172},{1 ,173},{1 ,174},{1 ,175},{1 ,176},{1 ,177},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
  {1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},{1 ,180},
};