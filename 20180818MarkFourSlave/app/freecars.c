#include "common.h"
#include "freecars.h"
#include "camera.h"
#include "timer.h"

uint8 uSendBuf[UartDataNum * 2] = {0};
uint8 FreeCarsDataNum = UartDataNum * 2; //����ͨ���� UartDataNum*2(��Ϊ������Int16�͵�)
double UartData[9] = {0};


SerialPortType SerialPortRx;
//��ʾFreecars��λ���޸�����


void uart0_init(void)
{
  static UART_InitTypeDef uart0_init_struct2;
  uart0_init_struct2.UART_Uartx = UART0; //UART0
  uart0_init_struct2.UART_BaudRate = 115200; 
  uart0_init_struct2.UART_RxPin = PTA1;  //��������ΪPTA1
  uart0_init_struct2.UART_TxPin = PTA2;  //��������ΪPTA2
  uart0_init_struct2.UART_RxIntEnable=TRUE;//ʹ�ܽ����ж�
  uart0_init_struct2.UART_TxIntEnable=FALSE;//�رշ����ж�
   uart0_init_struct2.UART_RxIsr=uart0_isr;
  LPLD_UART_EnableIrq(uart0_init_struct2);//�ж�ʹ��
  LPLD_UART_Init(uart0_init_struct2); //���³�ʼ��UART0
}

void UartDebug( void )
{
  /*
    push( 9, ( int16 )UartData[0] ); //�����ݷ��ͻ�ȥ�۲�
    push( 10, ( int16 )UartData[1] ); //�����ݷ��ͻ�ȥ�۲�
    push( 11, ( int16 )UartData[2] ); //�����ݷ��ͻ�ȥ�۲�
    push( 12, ( int16 )UartData[3] ); //�����ݷ��ͻ�ȥ�۲�
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
������պ���
CmdNum��0~255������
DATA  ��0~255����������Ӧ��������
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
/*��ѯ������һ֡����
����ʱ�������ݳ����й�
�����Է����ж����������Ե���
����ʱ����㿴�����ĵ�
*/
void sendDataToScope( void )
{
    uint8 i, sum = 0;
    //ʹ����ѯ�ķ�ʽ�������ݣ�������δ���ͣ�����ͣ�ڴ˴�ֱ���������
    LPLD_UART_PutChar( UART0, ( int8 )251 );
    LPLD_UART_PutChar( UART0, ( int8 )109 );
    LPLD_UART_PutChar( UART0, ( int8 )37 );
    sum += ( 251 ); //ȫ�����ݼ���У��
    sum += ( 109 );
    sum += ( 37 );
    for( i = 0; i < FreeCarsDataNum; i++ )
    {
        LPLD_UART_PutChar( UART0, uSendBuf[i] );
        sum += uSendBuf[i]; //ȫ�����ݼ���У��
    }
    LPLD_UART_PutChar( UART0, sum );
}
/*
��ʾ����ĳ��ͨ���������������
chanel��ͨ��
data ������-32768~32767
push ��������������䵽���������棬��Ȼdate��16λ�ģ����������8λ�������档
*/
void push( uint8 chanel, uint16 data )
{
    uSendBuf[chanel * 2] = data / 256;
    uSendBuf[chanel * 2 + 1] = data % 256;
}

/*------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////

20180409������������
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

//�����жϻص�����
void uart0_isr( void )
{
    int i, b, d1;
    uint32 d;
    //��������жϺ���
    if( ( UART0->S1 & UART_S1_RDRF_MASK ) && ( UART0->C2 & UART_C2_RIE_MASK ) )
    {
        SerialPortRx.Data = LPLD_UART_GetChar( UART0 );
        if( SerialPortRx.Stack < UartRxBufferLen )
        {
            SerialPortRx.Buffer[SerialPortRx.Stack++] = SerialPortRx.Data;
            if( SerialPortRx.Stack >= UartRxDataLen //UartRxDataLen����Ϊһ֡
                    && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen]  == 0xff //У����ͷ
                    && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 1] == 0x55
                    && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 2] == 0xaa
                    && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 3] == 0x10 )
            {
                //double data 9��ͨ������У��
                SerialPortRx.Check = 0;
                b = SerialPortRx.Stack - UartRxDataLen;//��ʼλ
                for( i = b; i < SerialPortRx.Stack - 1; i++ ) //��У��λ���λ����У��
                {
                    SerialPortRx.Check += SerialPortRx.Buffer[i];//У��
                }
                if( SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1] )
                {
                    //У��ɹ����������ݽ���
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
                    UartDebug();  //תȥ�������ܵ������ݸ�������
                }
                SerialPortRx.Stack = 0;
            }
            else if( SerialPortRx.Stack >= UartRxCmdLen //UartRxDataLen����Ϊһ֡
                     && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen]  == 0xff
                     && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 1] == 0x55
                     && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 2] == 0xaa
                     && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 3] == 0x77 ) //cmd
            {
                SerialPortRx.Check = 0;
                b = SerialPortRx.Stack - UartRxCmdLen;//��ʼλ
                for( i = b; i < SerialPortRx.Stack - 1; i++ ) //��У��λ���λ����У��
                {
                    SerialPortRx.Check += SerialPortRx.Buffer[i];//У��
                }
                if( SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1] )
                {
                    //У��ɹ�
                    UartCmd( UartCmdNum, UartCmdData ); //������յ����������MCU�������
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

