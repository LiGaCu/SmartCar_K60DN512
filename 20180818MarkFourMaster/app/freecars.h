#ifndef  _Freecars_H_
#define  _Freecars_H_  

//FreeCars上位机 串口数据显示框 接收通道数，按照上位机设置改变
#define UartDataNum 17//上位机数据通道数量，必须与上位机设置一样
#define UartRxBufferLen  100
#define UartRxDataLen 41//FreeCars上位机发送浮点数据MCU接收，不要改
#define UartRxCmdLen 7//FreeCars上位机接收命令数据长度，不要改
#define UartCmdNum  SerialPortRx.Buffer[SerialPortRx.Stack-3]//命令号
#define UartCmdData SerialPortRx.Buffer[SerialPortRx.Stack-2]//命令数据


typedef struct 
{
  int Stack;
  uint8 Data;
  uint8 PreData;
  uint8 Buffer[UartRxBufferLen];
  uint8 Enable;
  uint8 Check;
}SerialPortType;

void uart0_init(void);
void uart0_isr(void);//接收中断回调函数
//void uart88_isr(void);//wifi接收中断回调函数
void sendDataToScope(void);//Freecars示波器轮询法发送一帧数据
//void sendDataToScope_wifi(void);
void push(uint8 chanel,uint16 data);//向示波器某个通道缓冲区填充数据

void TenSecondsDataSend(void);
void TenSecondsDataSave(uint8 chanel,uint16 data);
void TenSecondsDataSaveOver(void);

//void picture_2_pc(void);
//void wifi_picture_2_pc(void);
//extern int Image_Mode_selection;
void sendCamImgToCamViewer(void);

extern uint8 car_run;
extern  uint16 time_servo;
extern float SpeedTarget[2];//速度目标值      

extern uint8 BIZHANGYUZHI;
#endif