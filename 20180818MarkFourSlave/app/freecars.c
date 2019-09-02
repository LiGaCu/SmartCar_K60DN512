#include "common.h"
#include "freecars.h"
#include "camera.h"
#include "timer.h"

uint8 uSendBuf[UartDataNum * 2] = {0};
uint8 FreeCarsDataNum = UartDataNum * 2; //它是通道数 UartDataNum*2(因为数据是Int16型的)
double UartData[9] = {0};


SerialPortType SerialPortRx;
//显示Freecars上位机修改数据


void uart0_init(void)
{
  static UART_InitTypeDef uart0_init_struct2;
  uart0_init_struct2.UART_Uartx = UART0; //UART0
  uart0_init_struct2.UART_BaudRate = 115200; 
  uart0_init_struct2.UART_RxPin = PTA1;  //接收引脚为PTA1
  uart0_init_struct2.UART_TxPin = PTA2;  //发送引脚为PTA2
  uart0_init_struct2.UART_RxIntEnable=TRUE;//使能接受中断
  uart0_init_struct2.UART_TxIntEnable=FALSE;//关闭发送中断
   uart0_init_struct2.UART_RxIsr=uart0_isr;
  LPLD_UART_EnableIrq(uart0_init_struct2);//中断使能
  LPLD_UART_Init(uart0_init_struct2); //重新初始化UART0
}

void UartDebug( void )
{
  /*
    push( 9, ( int16 )UartData[0] ); //将数据发送回去观察
    push( 10, ( int16 )UartData[1] ); //将数据发送回去观察
    push( 11, ( int16 )UartData[2] ); //将数据发送回去观察
    push( 12, ( int16 )UartData[3] ); //将数据发送回去观察
  */
 
   
//  if(UartData[0]!=0)
//       limit_slope_L = ( float )UartData[0];
//  
//  if(UartData[1]!=0)
//       limit_slope_R = ( float )( UartData[1] );
//  
//  if(UartData[2]!=0)
//       limit_intercept_L = (int )( UartData[2] );
//  
//  if(UartData[3]!=0)
//       limit_intercept_R = ( int8 )( UartData[3] );
//  
//  if(UartData[4]!=0)
//       limit_vertical = ( uint8 )( UartData[4] );
//  
//  if(UartData[5]!=0)
//       limit_range = ( uint8 )( UartData[5] );
  
//  if(UartData[6]!=0)
//      limit_horizontal = ( uint8 )( UartData[6] );
     
//  if(UartData[7]!=0)
//      SpeedTarget__R[1] = ( float)( UartData[7] );
//     
//  if(UartData[8]!=0)
//      brake_start=(uint8)(UartData[8]);
}
/*
命令接收函数
CmdNum：0~255号命令
DATA  ：0~255个命令所对应的命令字
*/
void UartCmd( uint8 CmdNum, uint8 Data )
{
    if( CmdNum == 2 && Data == 102 ) //page up
    {
    }
    if( CmdNum == 2 && Data == 103 ) //page down
    {
    }
}
/*轮询法发送一帧数据
消耗时间与数据长度有关
不可以放在中断里面周期性调用
消耗时间计算看帮助文档
*/
void sendDataToScope( void )
{
    uint8 i, sum = 0;
    //使用轮询的方式发送数据，当数据未发送，程序停在此处直到发送完成
    LPLD_UART_PutChar( UART0, ( int8 )251 );
    LPLD_UART_PutChar( UART0, ( int8 )109 );
    LPLD_UART_PutChar( UART0, ( int8 )37 );
    sum += ( 251 ); //全部数据加入校验
    sum += ( 109 );
    sum += ( 37 );
    for( i = 0; i < FreeCarsDataNum; i++ )
    {
        LPLD_UART_PutChar( UART0, uSendBuf[i] );
        sum += uSendBuf[i]; //全部数据加入校验
    }
    LPLD_UART_PutChar( UART0, sum );
}
/*
向示波器某个通道缓冲区填充数据
chanel：通道
data ：数据-32768~32767
push 是用来将数据填充到缓冲区里面，显然date是16位的，因此用两个8位数来储存。
*/
void push( uint8 chanel, uint16 data )
{
    uSendBuf[chanel * 2] = data / 256;
    uSendBuf[chanel * 2 + 1] = data % 256;
}

/*------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////

20180409：创建函数。
          FiveSecondsDataSend(void);
          FiveSecondsDataSave(uint8 chanel,uint16 data);
          FiveSecondsDataSaveOver(void);

///////////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------------*/
//uint16 TenSecondsRecordBuffer0[2000];
//uint16 TenSecondsRecordBuffer1[2000];
//uint16 TenSecondsRecordBuffer2[2000];
//uint16 TenSecondsRecordBuffer3[2000];
//uint16 TenSecondsRecordBuffer4[2000];
//uint16 TenSecondsRecordBuffer5[2000];
//uint16 TenSecondsRecordBuffer6[2000];
//uint16 TenSecondsRecordBuffer7[2000];
//uint16 TenSecondsRecordLength=0;
//uint16 TenSecondsRecordBegin=0;
//
//void TenSecondsDataSend(void)
//{
//  uint16 i;
//  for(i=0;i<TenSecondsRecordLength;i++)
//  {
//    push(0,TenSecondsRecordBuffer0[i]);
//    push(1,TenSecondsRecordBuffer1[i]);
//    push(2,TenSecondsRecordBuffer2[i]);
//    push(3,TenSecondsRecordBuffer3[i]);
//    push(4,TenSecondsRecordBuffer4[i]);
//    push(5,TenSecondsRecordBuffer5[i]);
//    push(6,TenSecondsRecordBuffer6[i]);
//    push(7,TenSecondsRecordBuffer7[i]);
//    sendDataToScope();     
//  }
//}
//
//void TenSecondsDataSave(uint8 chanel,uint16 data)
//{
//  switch(chanel)
//  {
//    case 0:TenSecondsRecordBuffer0[TenSecondsRecordLength]=data;break;
//    case 1:TenSecondsRecordBuffer1[TenSecondsRecordLength]=data;break;
//    case 2:TenSecondsRecordBuffer2[TenSecondsRecordLength]=data;break;
//    case 3:TenSecondsRecordBuffer3[TenSecondsRecordLength]=data;break;
//    case 4:TenSecondsRecordBuffer4[TenSecondsRecordLength]=data;break;
//    case 5:TenSecondsRecordBuffer5[TenSecondsRecordLength]=data;break;
//    case 6:TenSecondsRecordBuffer6[TenSecondsRecordLength]=data;break;
//    case 7:TenSecondsRecordBuffer7[TenSecondsRecordLength]=data;break;
//  }
//}
//
//void TenSecondsDataSaveOver(void)
//{
//  TenSecondsRecordLength++;
//  if(TenSecondsRecordLength>=2000)
//  {
//    TenSecondsRecordLength=1999;
//    TenSecondsRecordBegin=0;
//    LED_Fill(0);
//    LED_P6x8Str(6, 3,(unsigned char*)"RecorderOverflow!");
//    LED_P6x8Str(6, 5,(unsigned char*)"WaitingForSending");
//  }
//}

//接收中断回调函数
void uart0_isr( void )
{
    int i, b, d1;
    uint32 d;
    //进入接收中断函数
    if( ( UART0->S1 & UART_S1_RDRF_MASK ) && ( UART0->C2 & UART_C2_RIE_MASK ) )
    {
        SerialPortRx.Data = LPLD_UART_GetChar( UART0 );
        if( SerialPortRx.Stack < UartRxBufferLen )
        {
            SerialPortRx.Buffer[SerialPortRx.Stack++] = SerialPortRx.Data;
            if( SerialPortRx.Stack >= UartRxDataLen //UartRxDataLen个数为一帧
                    && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen]  == 0xff //校验字头
                    && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 1] == 0x55
                    && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 2] == 0xaa
                    && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 3] == 0x10 )
            {
                //double data 9个通道数据校验
                SerialPortRx.Check = 0;
                b = SerialPortRx.Stack - UartRxDataLen;//起始位
                for( i = b; i < SerialPortRx.Stack - 1; i++ ) //除校验位外的位进行校验
                {
                    SerialPortRx.Check += SerialPortRx.Buffer[i];//校验
                }
                if( SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1] )
                {
                    //校验成功，进行数据解算
                    for( i = 0; i < 9; i++ )
                    {
                        d = SerialPortRx.Buffer[b + i * 4 + 4] * 0x1000000L + SerialPortRx.Buffer[b + i * 4 + 5] * 0x10000L + SerialPortRx.Buffer[b + i * 4 + 6] * 0x100L + SerialPortRx.Buffer[b + i * 4 + 7];
                        if( d > 0x7FFFFFFF )
                        {
                            d1 = 0x7FFFFFFF - d;
                        }
                        else
                        {
                            d1 = d;
                        }
                        UartData[i] = d1;
                        UartData[i] /= 65536.0;
                    }
                    UartDebug();  //转去处理，把受到的数据付给变量
                }
                SerialPortRx.Stack = 0;
            }
            else if( SerialPortRx.Stack >= UartRxCmdLen //UartRxDataLen个数为一帧
                     && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen]  == 0xff
                     && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 1] == 0x55
                     && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 2] == 0xaa
                     && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 3] == 0x77 ) //cmd
            {
                SerialPortRx.Check = 0;
                b = SerialPortRx.Stack - UartRxCmdLen;//起始位
                for( i = b; i < SerialPortRx.Stack - 1; i++ ) //除校验位外的位进行校验
                {
                    SerialPortRx.Check += SerialPortRx.Buffer[i];//校验
                }
                if( SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1] )
                {
                    //校验成功
                    UartCmd( UartCmdNum, UartCmdData ); //处理接收到的命令，付给MCU命令变量
                }
                SerialPortRx.Stack = 0;
            }
        }
        else
        {
            SerialPortRx.Stack = 0;
        }
    }
}

