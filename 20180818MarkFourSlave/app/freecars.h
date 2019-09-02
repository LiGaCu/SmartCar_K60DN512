#ifndef  _Freecars_H_
#define  _Freecars_H_  

//FreeCars��λ�� ����������ʾ�� ����ͨ������������λ�����øı�
#define UartDataNum 17//��λ������ͨ����������������λ������һ��
#define UartRxBufferLen  100
#define UartRxDataLen 41//FreeCars��λ�����͸�������MCU���գ���Ҫ��
#define UartRxCmdLen 7//FreeCars��λ�������������ݳ��ȣ���Ҫ��
#define UartCmdNum  SerialPortRx.Buffer[SerialPortRx.Stack-3]//�����
#define UartCmdData SerialPortRx.Buffer[SerialPortRx.Stack-2]//��������



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
void uart0_isr(void);//�����жϻص�����
void sendDataToScope(void);//Freecarsʾ������ѯ������һ֡����
void push(uint8 chanel,uint16 data);//��ʾ����ĳ��ͨ���������������

void TenSecondsDataSend(void);
void TenSecondsDataSave(uint8 chanel,uint16 data);
void TenSecondsDataSaveOver(void);
void LED_Fill(unsigned char bmp_data);
void LED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
#endif