#include "camera.h"
#include "common.h"
#include "math.h"
#include "oled.h"

uint8 setvalue=180;//����������ֵ

uint8   receive[3];
uint8   receive_num = 0;
uint8   uart_receive_flag = 1;

unsigned char Image1[ROW][COLUMN];
unsigned char Image2[ROW][COLUMN];
unsigned char Image3[ROW][COLUMN];

unsigned char ImageTwo1[ROW_Two][COLUMN_Two];
unsigned char ImageTwo2[ROW_Two][COLUMN_Two];
unsigned char ImageTwo3[ROW_Two][COLUMN_Two];

uint8 end_flag=0; 
uint8 end_flag_Two=0;
uint8 start_flag=0;
uint8 lock=0;
uint8 lockTwo=0;
uint8 WhichBuffer = 1;
uint8 WhichBufferTwo = 1;

uint8 j_begin,j_end;

//��Ҫ���õ�����ͷ������
int16 MT9V032_CFG[CONFIG_FINISH][2]=
{
    {AUTO_EXP,          0},   //�Զ��ع�����      ��Χ1-63 0Ϊ�ر� ����Զ��ع⿪��  EXP_TIME�������õ����ݽ����Ϊ����ع�ʱ�䣬Ҳ�����Զ��ع�ʱ�������
                              //һ������ǲ���Ҫ����������ܣ���Ϊ�������ع���һ�㶼�ȽϾ��ȣ�����������߷ǳ������ȵ�������Գ������ø�ֵ������ͼ���ȶ���
    {EXP_TIME,          300}, //�ع�ʱ��          ����ͷ�յ�����Զ����������ع�ʱ�䣬������ù���������Ϊ�������������ع�ֵ
    {FPS,               150},  //ͼ��֡��          ����ͷ�յ�����Զ���������FPS���������������Ϊ������������FPS
    {SET_COL,           COLUMN}, //ͼ��������        ��Χ1-752     K60�ɼ���������188
    {SET_ROW,           ROW}, //ͼ��������        ��Χ1-480
    {LR_OFFSET,         0},   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ188 376 752ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
    {UD_OFFSET,         0},   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ120 240 480ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
    {GAIN,              32},  //ͼ������          ��Χ16-64     ����������ع�ʱ��̶�������¸ı�ͼ�������̶�

    
    {INIT,              0}    //����ͷ��ʼ��ʼ��
};

//��Ҫ���õ�����ͷ������
int16 MT9V032_Two_CFG[CONFIG_FINISH][2]=
{
    {AUTO_EXP,          0},   //�Զ��ع�����      ��Χ1-63 0Ϊ�ر� ����Զ��ع⿪��  EXP_TIME�������õ����ݽ����Ϊ����ع�ʱ�䣬Ҳ�����Զ��ع�ʱ�������
                              //һ������ǲ���Ҫ����������ܣ���Ϊ�������ع���һ�㶼�ȽϾ��ȣ�����������߷ǳ������ȵ�������Գ������ø�ֵ������ͼ���ȶ���
    {EXP_TIME,          300}, //�ع�ʱ��          ����ͷ�յ�����Զ����������ع�ʱ�䣬������ù���������Ϊ�������������ع�ֵ
    {FPS,               150},  //ͼ��֡��          ����ͷ�յ�����Զ���������FPS���������������Ϊ������������FPS
    {SET_COL,           COLUMN_Two}, //ͼ��������        ��Χ1-752     K60�ɼ���������188
    {SET_ROW,           ROW_Two}, //ͼ��������        ��Χ1-480
    {LR_OFFSET,         0},   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ188 376 752ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
    {UD_OFFSET,         0},   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ120 240 480ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
    {GAIN,              32},  //ͼ������          ��Χ16-64     ����������ع�ʱ��̶�������¸ı�ͼ�������̶�

    
    {INIT,              0}    //����ͷ��ʼ��ʼ��
};

//������ͷ�ڲ���ȡ������������
int16 GET_CFG[CONFIG_FINISH-1][2]=
{
    {AUTO_EXP,          0},   //�Զ��ع�����      
    {EXP_TIME,          0},   //�ع�ʱ��          
    {FPS,               0},   //ͼ��֡��          
    {SET_COL,           0},   //ͼ��������        
    {SET_ROW,           0},   //ͼ��������        
    {LR_OFFSET,         0},   //ͼ������ƫ����    
    {UD_OFFSET,         0},   //ͼ������ƫ����    
    {GAIN,              0},   //ͼ������          
};

void camera_init(void)
{
 static GPIO_InitTypeDef dmaReq_port_struct;
  dmaReq_port_struct.GPIO_PTx = PTB;
  dmaReq_port_struct.GPIO_Pins = GPIO_Pin22;
  dmaReq_port_struct.GPIO_Dir = DIR_INPUT;
  dmaReq_port_struct.GPIO_PinControl =IRQC_DMARI | INPUT_PULL_DOWN;//�����ز���DMA����,��������
  LPLD_GPIO_Init(dmaReq_port_struct);
  
  static GPIO_InitTypeDef vsync_port_struct;  //���жϽ��ܶ˿�
  vsync_port_struct.GPIO_PTx = PTB;    
  vsync_port_struct.GPIO_Pins = GPIO_Pin23;   
  vsync_port_struct.GPIO_Dir = DIR_INPUT;        //����
  vsync_port_struct.GPIO_PinControl = IRQC_FA|INPUT_PULL_DOWN|INPUT_PF_EN;    //�ڲ�����|�½����ж�
  vsync_port_struct.GPIO_Isr = portb_isr;        //�жϺ���
  LPLD_GPIO_Init(vsync_port_struct);
  LPLD_GPIO_EnableIrq(vsync_port_struct);
  
  //����ͷ����Ƭ�����ݿڳ�ʼ����PTC0~PTC7
  static GPIO_InitTypeDef data_port_struct;
  data_port_struct.GPIO_PTx = PTC;
  data_port_struct.GPIO_Dir = DIR_INPUT;
  data_port_struct.GPIO_Pins = GPIO_Pin0_7;
  data_port_struct.GPIO_PinControl = IRQC_DIS|INPUT_PULL_DIS;
  LPLD_GPIO_Init(data_port_struct);
  
  //DMA��������
  static DMA_InitTypeDef  dma_init_struct;
  dma_init_struct.DMA_CHx = DMA_CH0;    //CH0ͨ��
  dma_init_struct.DMA_Req = PORTB_DMAREQ;     //PORTB22Ϊ����Դ
  dma_init_struct.DMA_MajorLoopCnt = COLUMN*ROW;   //��ѭ������ֵ���вɼ����������
  dma_init_struct.DMA_MinorByteCnt = 1; //��ѭ���ֽڼ�����ÿ�δ���1�ֽ�
  dma_init_struct.DMA_SourceAddr = (uint32)&PTC->PDIR; //Դ��ַ��PTC0-PTC7,Port Data Input Register
  dma_init_struct.DMA_DestAddr = (uint32)Image1;      //Ŀ�ĵ�ַ�����ͼ�������
  dma_init_struct.DMA_DestAddrOffset = 1;       //Ŀ�ĵ�ַƫ�ƣ�ÿ�ζ�������1
  dma_init_struct.DMA_AutoDisableReq = TRUE;    //�Զ���������ʹ�ܺ�ͨ����������ѭ�����������
  LPLD_DMA_Init(dma_init_struct);//��ʼ����δʹ��
  
  static UART_InitTypeDef uart0_init_struct;
  uart0_init_struct.UART_Uartx = UART0; //UART0������������ͷ
  uart0_init_struct.UART_BaudRate = 9600;
  uart0_init_struct.UART_RxPin = PTB16;  //��������ΪPTB16
  uart0_init_struct.UART_TxPin = PTB17;  //��������ΪPTB17
  uart0_init_struct.UART_RxIntEnable=TRUE;//ʹ�ܽ����ж�
  uart0_init_struct.UART_TxIntEnable=FALSE;//�رշ����ж�
  uart0_init_struct.UART_RxIsr=mt9v032_cof_uart_interrupt;
  LPLD_UART_Init(uart0_init_struct); //��ʼ��UART0
  LPLD_UART_EnableIrq(uart0_init_struct);//�ж�ʹ��
  
  MT9V032_CFG_init();//MT9V032����
}

void cameraTwo_init(void)
{
 static GPIO_InitTypeDef dmaReq_port_d_struct;
  dmaReq_port_d_struct.GPIO_PTx = PTD;
  dmaReq_port_d_struct.GPIO_Pins = GPIO_Pin0;
  dmaReq_port_d_struct.GPIO_Dir = DIR_INPUT;
  dmaReq_port_d_struct.GPIO_PinControl =IRQC_DMARI | INPUT_PULL_DOWN;//�����ز���DMA����,��������
  LPLD_GPIO_Init(dmaReq_port_d_struct);
  
  static GPIO_InitTypeDef vsync_port_c_struct;  //���жϽ��ܶ˿�
  vsync_port_c_struct.GPIO_PTx = PTC;    
  vsync_port_c_struct.GPIO_Pins = GPIO_Pin16;   
  vsync_port_c_struct.GPIO_Dir = DIR_INPUT;        //����
  vsync_port_c_struct.GPIO_PinControl = IRQC_FA|INPUT_PULL_DOWN|INPUT_PF_EN;    //�ڲ�����|�½����ж�
  vsync_port_c_struct.GPIO_Isr = portc_isr;        //�жϺ���
  LPLD_GPIO_Init(vsync_port_c_struct);
  LPLD_GPIO_EnableIrq(vsync_port_c_struct);
  
  //����ͷ����Ƭ�����ݿڳ�ʼ����PTC0~PTC7
  static GPIO_InitTypeDef data_port_c_struct;
  data_port_c_struct.GPIO_PTx = PTC;
  data_port_c_struct.GPIO_Dir = DIR_INPUT;
  data_port_c_struct.GPIO_Pins = GPIO_Pin8_15;
  data_port_c_struct.GPIO_PinControl = IRQC_DIS|INPUT_PULL_DIS;
  LPLD_GPIO_Init(data_port_c_struct);
  
  //DMA��������
  static DMA_InitTypeDef  dma_init_c_struct;
  dma_init_c_struct.DMA_CHx = DMA_CH1;    //CH0ͨ��
  dma_init_c_struct.DMA_Req = PORTD_DMAREQ;     //PORTB22Ϊ����Դ
  dma_init_c_struct.DMA_MajorLoopCnt = COLUMN_Two*ROW_Two;   //��ѭ������ֵ���вɼ����������
  dma_init_c_struct.DMA_MinorByteCnt = 1; //��ѭ���ֽڼ�����ÿ�δ���1�ֽ�
  dma_init_c_struct.DMA_SourceAddr = (uint32)&PTC->PDIR+1; //Դ��ַ��PTC0-PTC7,Port Data Input Register
  dma_init_c_struct.DMA_DestAddr = (uint32)ImageTwo1;      //Ŀ�ĵ�ַ�����ͼ�������
  dma_init_c_struct.DMA_DestAddrOffset = 1;       //Ŀ�ĵ�ַƫ�ƣ�ÿ�ζ�������1
  dma_init_c_struct.DMA_AutoDisableReq = TRUE;    //�Զ���������ʹ�ܺ�ͨ����������ѭ�����������
  LPLD_DMA_Init(dma_init_c_struct);//��ʼ����δʹ��
  
  static UART_InitTypeDef uart0_init_struct3;
  uart0_init_struct3.UART_Uartx = UART1; //UART0������������ͷ
  uart0_init_struct3.UART_BaudRate = 9600;
  uart0_init_struct3.UART_RxPin = PTE1;  //��������ΪPTB16
  uart0_init_struct3.UART_TxPin = PTE0;  //��������ΪPTB17
  uart0_init_struct3.UART_RxIntEnable=TRUE;//ʹ�ܽ����ж�
  uart0_init_struct3.UART_TxIntEnable=FALSE;//�رշ����ж�
  uart0_init_struct3.UART_RxIsr=mt9v032_cof_uart_interrupt2;
  LPLD_UART_Init(uart0_init_struct3); //��ʼ��UART0
  LPLD_UART_EnableIrq(uart0_init_struct3);//�ж�ʹ��
  
  MT9V032_Two_CFG_init();//MT9V032����
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

void picture_send_Two(void)
{
  int i,j;
    if(end_flag_Two)
    {
      disable_irq(PORTC_IRQn); 
      LPLD_UART_PutChar(UART0,0X01);
      LPLD_UART_PutChar(UART0,0XFE);
      for(i=0;i<ROW_Two;i++)
        for(j=0;j<COLUMN_Two;j++)
            LPLD_UART_PutChar(UART0,ImageTwo1[i][j]);
      LPLD_UART_PutChar(UART0,0XFE);
      LPLD_UART_PutChar(UART0,0X01);
      enable_irq(PORTC_IRQn);
      end_flag_Two=0;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����ͷ�ڲ�������Ϣ
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				���øú���ǰ���ȳ�ʼ��uart0
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
        
        //�ȴ����ܻش�����
        while(!uart_receive_flag);
        uart_receive_flag = 0;
        
        GET_CFG[i][1] = receive[1]<<8 | receive[2];
        
    }
}
void MT9V032_CFG_init(void)
{       //���ò���    ������ο�ʹ���ֲ�
    uint16 temp, i,j;
    uint8  send_buffer[4];
    my_delay(9999999);//��ʱ�Ա�֤����ͷ�����51����ϵ��ʼ��
    my_delay(9999999);//��ʱ�Ա�֤����ͷ�����51����ϵ��ʼ��
   //��ʼ��������ͷ�����³�ʼ��
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
//     //��ȡ���ñ��ڲ鿴�����Ƿ���ȷ
//    get_config();
}

void MT9V032_Two_CFG_init(void)
{       //���ò���    ������ο�ʹ���ֲ�
    uint16 temp, i,j;
    uint8  send_buffer[4];
    my_delay(9999999);//��ʱ�Ա�֤����ͷ�����51����ϵ��ʼ��
    my_delay(9999999);//��ʱ�Ա�֤����ͷ�����51����ϵ��ʼ��
   //��ʼ��������ͷ�����³�ʼ��
    for(i=0; i<CONFIG_FINISH; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = MT9V032_Two_CFG[i][0];
        temp = MT9V032_Two_CFG[i][1];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;
        for(j=0;j<4;j++)
        {
          LPLD_UART_PutChar(UART1,send_buffer[j]);
          LPLD_LPTMR_DelayMs(2);
        }
    }    
//    while(!uart_receive_flag);
//    uart_receive_flag = 0;
//    while((0xff != receive[1]) || (0xff != receive[2]));
//    
//     //��ȡ���ñ��ڲ鿴�����Ƿ���ȷ
//    get_config();
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032����ͷ�����жϺ���
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       �ú�����ISR�ļ� ����3�жϳ��򱻵���
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

void mt9v032_cof_uart_interrupt2(void)
{
    receive[receive_num]=LPLD_UART_GetChar(UART1);
    receive_num++;
    
    if(1==receive_num && 0XA5!=receive[0])  receive_num = 0;
    if(3 == receive_num)
    {
        receive_num = 0;
        uart_receive_flag = 1;
    }
        
}

void my_delay(long t)
{
    while(t--);
}





uint8 portb_enter_times,portc_enter_times;


void portb_isr()
{ 
  portb_enter_times++;
  if(portb_enter_times==1)
  {
     if(LPLD_GPIO_IsPinxExt(PORTB, GPIO_Pin23)) // ���ж�         
    { 
      LPLD_GPIO_ClearIntFlag(PORTB);
//      LPLD_DMA_DisableReq(DMA_CH0);//�ɼ�����������DMA����Դ
   
      LPLD_DMA_EnableReq(DMA_CH0);//��ʼ�����趨��ѭ��������������Դ��������ʹ������Դ
    }
  }
  else
  {
    LPLD_GPIO_ClearIntFlag(PORTB);
    LPLD_DMA_DisableReq(DMA_CH0);//�ɼ�����������DMA����Դ
    LPLD_DMA_DisableReq(DMA_CH1);//�ɼ�����������DMA����Դ
      if(WhichBuffer==1)
        WhichBuffer=2;  //�л���ͼ��ͼ1ͼ2���δ���
      else if(WhichBuffer==2)
        WhichBuffer=3;
      else if(WhichBuffer==3)
        WhichBuffer=1;
      
      
      if(WhichBuffer==1&&lock==1)                          //��Ҫ��ͼ1��ͼ1û���ڴ������߽�Ҫ��ͼ2��ͼ2û���ڴ���
      {
        LPLD_DMA_LoadDstAddr(DMA_CH0,(Image2[0]));//DMA������־���ɼ���ʼ
        end_flag=1;
      }
      else if(WhichBuffer==2&&lock==2)   
        LPLD_DMA_LoadDstAddr(DMA_CH0,(Image3[0]));//DMA������־���ɼ���ʼ
      else if(WhichBuffer==3&&lock==3) 
        LPLD_DMA_LoadDstAddr(DMA_CH0,(Image1[0]));//DMA������־���ɼ���ʼ
      else
      {
          if(WhichBuffer==1)
          LPLD_DMA_LoadDstAddr(DMA_CH0,(Image1[0]));//���ػ����Ŀ�ĵ�ַ
          else if(WhichBuffer==2)
          {
          LPLD_DMA_LoadDstAddr(DMA_CH0,(Image2[0]));//���ػ����Ŀ�ĵ�ַ
          end_flag=1;
          }
          else if(WhichBuffer==3)
          LPLD_DMA_LoadDstAddr(DMA_CH0,(Image3[0]));//���ػ����Ŀ�ĵ�ַ
      } 
    portb_enter_times=0;
    disable_irq(PORTB_IRQn);
    enable_irq(PORTC_IRQn);
    portc_enter_times=0;
  }
}

void portc_isr()
{
  portc_enter_times++;
  if(portc_enter_times==1)
  {
     if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin16)) // ���ж�         
    { 
      LPLD_GPIO_ClearIntFlag(PORTC);
//      LPLD_DMA_DisableReq(DMA_CH1);//�ɼ�����������DMA����Դ
    
      LPLD_DMA_EnableReq(DMA_CH1);//��ʼ�����趨��ѭ��������������Դ��������ʹ������Դ
    }
  }
  else
  {
    LPLD_GPIO_ClearIntFlag(PORTC);
    LPLD_DMA_DisableReq(DMA_CH1);//�ɼ�����������DMA����Դ
    LPLD_DMA_DisableReq(DMA_CH0);//�ɼ�����������DMA����Դ
    if(WhichBufferTwo==1)
      WhichBufferTwo=2;  //�л���ͼ��ͼ1ͼ2���δ���
    else if(WhichBufferTwo==2)
      WhichBufferTwo=3;
    else if(WhichBufferTwo==3)
      WhichBufferTwo=1;
    
    
    if(WhichBufferTwo==1&&lockTwo==1)                          //��Ҫ��ͼ1��ͼ1û���ڴ������߽�Ҫ��ͼ2��ͼ2û���ڴ���
    {
      LPLD_DMA_LoadDstAddr(DMA_CH1,(ImageTwo2[0]));//DMA������־���ɼ���ʼ
      end_flag_Two=1;
    }
    else if(WhichBufferTwo==2&&lockTwo==2)   
      LPLD_DMA_LoadDstAddr(DMA_CH1,(ImageTwo3[0]));//DMA������־���ɼ���ʼ
    else if(WhichBufferTwo==3&&lockTwo==3) 
      LPLD_DMA_LoadDstAddr(DMA_CH1,(ImageTwo1[0]));//DMA������־���ɼ���ʼ
    else
    {
        if(WhichBufferTwo==1)
        LPLD_DMA_LoadDstAddr(DMA_CH1,(ImageTwo1[0]));//���ػ����Ŀ�ĵ�ַ
        else if(WhichBufferTwo==2)
        {
        LPLD_DMA_LoadDstAddr(DMA_CH1,(ImageTwo2[0]));//���ػ����Ŀ�ĵ�ַ
        end_flag_Two=1;
        }
        else if(WhichBufferTwo==3)
        LPLD_DMA_LoadDstAddr(DMA_CH1,(ImageTwo3[0]));//���ػ����Ŀ�ĵ�ַ
    }
    portb_enter_times=0;
    disable_irq(PORTC_IRQn);
    enable_irq(PORTB_IRQn);
    portc_enter_times=0;
  }
}


//------------------------------------------------------------------------------
//
//��ͼ������ȡ������Ϣ����
//�����Ϣ:Barrier_x,Barrier_y,HaveBarrier��Validfiguresize
//
//------------------------------------------------------------------------------
#define validfixelrange_slope_L -1.2000
#define validfixelrange_slope_R 0.9333
#define validfixelrange_intercept_L 45
#define validfixelrange_intercept_R -106
#define i_begin_validfixel 15
#define ChangeWidth 20  //ChangeWidth��Ҫ����i_begin_validfixel
#define NearYellowSizeLimit 100
#define VarianceLimit 100


uint8 line_x[50]={0},line_y[50]={0},line_min_x[50]={0},line_max_x[50]={0};
uint8 Barrier_x,Barrier_y,HaveBarrier=0,CircleFlag;
uint8 longest_x=0,smallest_y=ROW,largest_y=0;
int Validfiguresize;double Variance;uint8 Several_signal_y;
void Figure_Process(void)
{
 register uint16 BufferByte=0;
 register uint8 i,j,value=setvalue;
 int figuresize=0;
 float Average_line_x=0;double SquareSum_line_x=0;
 int Sum_line_x=0,Valid_Sum_x=0,Valid_Sum_y=0;
 uint8 linenum=0,Valid_Sum_Num=0;
 Several_signal_y=0;
 
 if(WhichBuffer == 2)//���ڴ�ͼ2
  {
     lock=1;//��ͼ1������ͼ1
     for (i=0;i<ROW;i++)
     {
       BufferByte=0;
       if(i<i_begin_validfixel)
       {
         j_begin=(uint8)((i-validfixelrange_intercept_L)/validfixelrange_slope_L);
         j_end=(uint8)((i-validfixelrange_intercept_R)/validfixelrange_slope_R);
         for (j=j_begin;j<j_end;j++)
         {
           BufferByte=BufferByte<<1;
           BufferByte|=(Image1[i][j]>=value?1:0);
           BufferByte&=127;
           if(j<j_end-1)
           {
             switch(BufferByte)
             {
               case 63:  line_min_x[linenum]=j-2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������,��49��������ʮ��λ�ù̶�Ԥ��0
                         figuresize+=6;
                         break;
               case 127: figuresize++;
                         break;
               case 126: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
           else
           {
             switch(BufferByte)
             {
               case 63:  line_min_x[linenum]=j-2;
                         line_max_x[linenum]=j;
                         line_x[linenum]=(line_max_x[linenum]+line_min_x[linenum])/2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������
                         figuresize+=6;
                         break;
               case 127: figuresize++;
                         line_max_x[linenum-1]=j;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
               case 126: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
         }
       }
       else if(i<ChangeWidth)
       {
         for (j=1;j<COLUMN;j++)
         {
           BufferByte=BufferByte<<1;
           BufferByte|=(Image1[i][j]>=value?1:0);
           BufferByte&=127;
           if(j<COLUMN-1)
           {
             switch(BufferByte)
             {
               case 63:  line_min_x[linenum]=j-2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������,��49��������ʮ��λ�ù̶�Ԥ��0
                         figuresize+=6;
                         break;
               case 127: figuresize++;
                         break;
               case 126: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
           else
           {
             switch(BufferByte)
             {
               case 63:  line_min_x[linenum]=j-2;
                         line_max_x[linenum]=j;
                         line_x[linenum]=(line_max_x[linenum]+line_min_x[linenum])/2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������
                         figuresize+=6;
                         break;
               case 127: figuresize++;
                         line_max_x[linenum-1]=j;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
               case 126: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
         }
       }
       else                                             //�����ڻ���������ʱ���޷�ͨ�������б�����
       {
         for (j=1;j<COLUMN;j++)
         {
           BufferByte=BufferByte<<1;
           BufferByte|=(Image1[i][j]>=value?1:0);
           BufferByte&=8191;
           if(j<COLUMN-1)
           {
             switch(BufferByte)
             {
               case 4095:  line_min_x[linenum]=j-2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������,��49��������ʮ��λ�ù̶�Ԥ��0
                         figuresize+=6;
                         break;
               case 8191: figuresize++;
                         break;
               case 8190: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
           else
           {
             switch(BufferByte)
             {
               case 4095:  line_min_x[linenum]=j-2;
                         line_max_x[linenum]=j;
                         line_x[linenum]=(line_max_x[linenum]+line_min_x[linenum])/2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������
                         figuresize+=6;
                         break;
               case 8191: figuresize++;
                         line_max_x[linenum-1]=j;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
               case 8190: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
         }
       }//�д������
     }//һ��ͼ�������
  }

  else if(WhichBuffer == 3)//���ڴ�ͼ1
   {
     lock=2;//��ͼ2������ͼ2
     for (i=0;i<ROW;i++)
     {
       BufferByte=0;
       if(i<i_begin_validfixel)
       {
         j_begin=(uint8)((i-validfixelrange_intercept_L)/validfixelrange_slope_L);
         j_end=(uint8)((i-validfixelrange_intercept_R)/validfixelrange_slope_R);
         for (j=j_begin;j<j_end;j++)
         {
           BufferByte=BufferByte<<1;
           BufferByte|=(Image2[i][j]>=value?1:0);
           BufferByte&=127;
           if(j<j_end-1)
           {
             switch(BufferByte)
             {
               case 63:  line_min_x[linenum]=j-2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������,��49��������ʮ��λ�ù̶�Ԥ��0
                         figuresize+=6;
                         break;
               case 127: figuresize++;
                         break;
               case 126: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
           else
           {
             switch(BufferByte)
             {
               case 63:  line_min_x[linenum]=j-2;
                         line_max_x[linenum]=j;
                         line_x[linenum]=(line_max_x[linenum]+line_min_x[linenum])/2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������
                         figuresize+=6;
                         break;
               case 127: figuresize++;
                         line_max_x[linenum-1]=j;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
               case 126: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
         }
       }
       else if(i<ChangeWidth)
       {
         for (j=1;j<COLUMN;j++)
         {
           BufferByte=BufferByte<<1;
           BufferByte|=(Image2[i][j]>=value?1:0);
           BufferByte&=127;
           if(j<COLUMN-1)
           {
             switch(BufferByte)
             {
               case 63:  line_min_x[linenum]=j-2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������,��49��������ʮ��λ�ù̶�Ԥ��0
                         figuresize+=6;
                         break;
               case 127: figuresize++;
                         break;
               case 126: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
           else
           {
             switch(BufferByte)
             {
               case 63:  line_min_x[linenum]=j-2;
                         line_max_x[linenum]=j;
                         line_x[linenum]=(line_max_x[linenum]+line_min_x[linenum])/2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������
                         figuresize+=6;
                         break;
               case 127: figuresize++;
                         line_max_x[linenum-1]=j;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
               case 126: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
         }
       }
       else                                             //�����ڻ���������ʱ���޷�ͨ�������б�����
       {
         for (j=1;j<COLUMN;j++)
         {
           BufferByte=BufferByte<<1;
           BufferByte|=(Image2[i][j]>=value?1:0);
           BufferByte&=8191;
           if(j<COLUMN-1)
           {
             switch(BufferByte)
             {
               case 4095:  line_min_x[linenum]=j-2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������,��49��������ʮ��λ�ù̶�Ԥ��0
                         figuresize+=6;
                         break;
               case 8191: figuresize++;
                         break;
               case 8190: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
           else
           {
             switch(BufferByte)
             {
               case 4095:  line_min_x[linenum]=j-2;
                         line_max_x[linenum]=j;
                         line_x[linenum]=(line_max_x[linenum]+line_min_x[linenum])/2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������
                         figuresize+=6;
                         break;
               case 8191: figuresize++;
                         line_max_x[linenum-1]=j;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
               case 8190: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
         }
       }//�д������
     }//һ��ͼ�������
   }
 else if(WhichBuffer == 1)//���ڴ�ͼ1
   {
     lock=3;//��ͼ2������ͼ2
     for (i=0;i<ROW;i++)
     {
       BufferByte=0;
       if(i<i_begin_validfixel)
       {
         j_begin=(uint8)((i-validfixelrange_intercept_L)/validfixelrange_slope_L);
         j_end=(uint8)((i-validfixelrange_intercept_R)/validfixelrange_slope_R);
         for (j=j_begin;j<j_end;j++)
         {
           BufferByte=BufferByte<<1;
           BufferByte|=(Image3[i][j]>=value?1:0);
           BufferByte&=127;
           if(j<j_end-1)
           {
             switch(BufferByte)
             {
               case 63:  line_min_x[linenum]=j-2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������,��49��������ʮ��λ�ù̶�Ԥ��0
                         figuresize+=6;
                         break;
               case 127: figuresize++;
                         break;
               case 126: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
           else
           {
             switch(BufferByte)
             {
               case 63:  line_min_x[linenum]=j-2;
                         line_max_x[linenum]=j;
                         line_x[linenum]=(line_max_x[linenum]+line_min_x[linenum])/2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������
                         figuresize+=6;
                         break;
               case 127: figuresize++;
                         line_max_x[linenum-1]=j;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
               case 126: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
         }
       }
       else if(i<ChangeWidth)
       {
         for (j=1;j<COLUMN;j++)
         {
           BufferByte=BufferByte<<1;
           BufferByte|=(Image3[i][j]>=value?1:0);
           BufferByte&=127;
           if(j<COLUMN-1)
           {
             switch(BufferByte)
             {
               case 63:  line_min_x[linenum]=j-2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������,��49��������ʮ��λ�ù̶�Ԥ��0
                         figuresize+=6;
                         break;
               case 127: figuresize++;
                         break;
               case 126: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
           else
           {
             switch(BufferByte)
             {
               case 63:  line_min_x[linenum]=j-2;
                         line_max_x[linenum]=j;
                         line_x[linenum]=(line_max_x[linenum]+line_min_x[linenum])/2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������
                         figuresize+=6;
                         break;
               case 127: figuresize++;
                         line_max_x[linenum-1]=j;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
               case 126: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
         }
       }
       else                                             //�����ڻ���������ʱ���޷�ͨ�������б�����
       {
         for (j=1;j<COLUMN;j++)
         {
           BufferByte=BufferByte<<1;
           BufferByte|=(Image3[i][j]>=value?1:0);
           BufferByte&=8191;
           if(j<COLUMN-1)
           {
             switch(BufferByte)
             {
               case 4095:  line_min_x[linenum]=j-2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������,��49��������ʮ��λ�ù̶�Ԥ��0
                         figuresize+=6;
                         break;
               case 8191: figuresize++;
                         break;
               case 8190: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
           else
           {
             switch(BufferByte)
             {
               case 4095:  line_min_x[linenum]=j-2;
                         line_max_x[linenum]=j;
                         line_x[linenum]=(line_max_x[linenum]+line_min_x[linenum])/2;
                         line_y[linenum]=i;
                         linenum++;
                         if(linenum>49) linenum=1;//��ֹ�������
                         figuresize+=6;
                         break;
               case 8191: figuresize++;
                         line_max_x[linenum-1]=j;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
               case 8190: line_max_x[linenum-1]=j-1;
                         line_x[linenum-1]=(line_max_x[linenum-1]+line_min_x[linenum-1])/2;
                         break;
             }
           }
         }
       }//�д������
     }//һ��ͼ�������
   }
     
      if(figuresize>=30)
     {
       i=0;
       while(line_x[i]!=0)//ֻ���������ݵ�line_x���ݶ�
       {
         Sum_line_x+=line_x[i];
         i++;
       }
       Average_line_x=Sum_line_x/i;
       i=0;
       while(line_x[i]!=0)//ֻ���������ݵ�line_x���ݶ�
       {
         SquareSum_line_x=SquareSum_line_x+(line_x[i]-Average_line_x)*(line_x[i]-Average_line_x);
         i++;
       }
       Variance=SquareSum_line_x;                               //Mr.Li �������ݷ���
       SquareSum_line_x=SquareSum_line_x/i;                     //�����׼��
       i=0;j=0;
       while(line_x[i]!=0)
       {
         if((line_x[i]-Average_line_x)*(line_x[i]-Average_line_x)>9*SquareSum_line_x)
         {
           Variance-=(line_x[i]-Average_line_x)*(line_x[i]-Average_line_x);     //�����·���
           line_x[i]=0;
           line_y[i]=0;
           j++;
           figuresize-=(line_max_x[i]-line_min_x[i]);
         }
         i++;
       }
       Variance=Variance/(i-j);                                  //�����±�׼��
       if(figuresize>=20&&Variance<VarianceLimit)       //Mr.Li  ��������X������ɢ���
       {
         Valid_Sum_Num=i-j;
         longest_x=0,smallest_y=ROW,largest_y=0;
         for(i=0;i<50;i++)
         {
         Valid_Sum_x+=line_x[i];
         Valid_Sum_y+=line_y[i];
         if(line_x[i]!=0)
         {
           if((line_max_x[i]-line_min_x[i])>longest_x)
             longest_x=line_max_x[i]-line_min_x[i];
           if((line_y[i]<smallest_y))
             smallest_y=line_y[i];
           if(line_y[i]>largest_y)
             largest_y=line_y[i];
         }
         if((line_y[i+1]-line_y[i])>10&&line_y[i+1]!=0&&line_y[i]!=0)
           Several_signal_y=1;
         }
         Validfiguresize=figuresize;
         if(Validfiguresize<NearYellowSizeLimit&&(Valid_Sum_y/Valid_Sum_Num)>ChangeWidth||Several_signal_y==1||(largest_y-smallest_y)>3*longest_x||4*(largest_y-smallest_y)<longest_x||(largest_y-smallest_y)>60)//Validfiguresize<NearYellowSizeLimit&&(Valid_Sum_y/Valid_Sum_Num)>ChangeWidth||
         {
           HaveBarrier=0;
           CircleFlag=2;
         }
         else
         {
            Barrier_y=Valid_Sum_y/Valid_Sum_Num;
            Barrier_x=Valid_Sum_x/Valid_Sum_Num;
            HaveBarrier=1;
            if((largest_y-smallest_y)<longest_x)
              CircleFlag=1;
            else
              CircleFlag=0;
         }
       }
       else 
       {
         HaveBarrier=0;
         CircleFlag=2;
       }
       
     }
     else 
     {
       HaveBarrier=0;
       CircleFlag=2;
     }
     
     memset(line_min_x,0,50*sizeof(uint8));
     memset(line_max_x,0,50*sizeof(uint8));
     memset(line_x,0,50*sizeof(uint8));
     memset(line_y,0,50*sizeof(uint8));
     
     
}


//------------------------------------------------------------------------------
//
//�ӵ���������ͷ���ټ�������ź�
//����ʱ�䣺2018��8��7��
//
//------------------------------------------------------------------------------
const uint8 j_BeginAndEnd[100][2]=
{
  {0 ,0  },{0 ,0  },{64,95 },{56,103},{50,109},{45,113},{42,117},{38,121},
  {35,125},{32,128},{30,130},{27,133},{24,136},{22,138},{19,140},{17,142},{15,144},{13,146},
  {11,148},{10,149},{8 ,151},{6 ,153},{5 ,154},{4 ,156},{3 ,157},{1 ,158},{1 ,160},{1 ,160},
  {1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},
  {1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},
  {1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},
  {1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},
  {1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},
  {1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},
  {1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160},{1 ,160}
};

uint8 NOpoint,Nopointtime=12,RealNopoint=1;
uint8 real_pointx,real_pointy;

void Figure_Process_Two(void)
{
  register uint8 i,j,BufferByte=0;
  uint8 pointy;
  uint8 InvalidBuffer=1;
  uint8 maxi=0,mini=ROW_Two,maxj=0,minj=0;
  uint8 Done_J=0;
  
  if(WhichBufferTwo == 2)//���ڴ�ͼ2
  {
    lockTwo=1;//��ͼ1������ͼ1
    for(i=0;i<ROW_Two;i++)
    {
      j_begin=j_BeginAndEnd[i][0];
      j_end=j_BeginAndEnd[i][1];
      BufferByte=0;
      for(j=j_begin;j<j_end;j++)     //Mr.Li�ı�ע      �����������и��ŵ�������ű�������У��������minj��maxj�ͻ�ȫ�� ��ò�Ƴ����߼������޸��ŵ㣩
      {
         BufferByte=BufferByte<<1;
         BufferByte|=(ImageTwo1[i][j]>=setvalue_Two?1:0);
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
        BufferByte|=(ImageTwo1[pointy][j]>=setvalue_Two?1:0);
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
  else if(WhichBufferTwo == 3)//���ڴ�ͼ1
  {
    lockTwo=2;//��ͼ2������ͼ2
    for(i=0;i<ROW_Two;i++)
    {
      BufferByte=0;
      j_begin=j_BeginAndEnd[i][0];
      j_end=j_BeginAndEnd[i][1];
      for(j=j_begin;j<j_end;j++)     //Mr.Li�ı�ע      �����������и��ŵ�������ű�������У��������minj��maxj�ͻ�ȫ�� ��ò�Ƴ����߼������޸��ŵ㣩
      {
         BufferByte=BufferByte<<1;
         BufferByte|=(ImageTwo2[i][j]>=setvalue_Two?1:0);
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
        BufferByte|=(ImageTwo2[pointy][j]>=setvalue_Two?1:0);
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
    else if(WhichBufferTwo == 1)//���ڴ�ͼ1
    {
      lockTwo=3;//��ͼ2������ͼ2
      for(i=0;i<ROW_Two;i++)
      {
        BufferByte=0;
        j_begin=j_BeginAndEnd[i][0];
        j_end=j_BeginAndEnd[i][1];
        for(j=j_begin;j<j_end;j++)     //Mr.Li�ı�ע      �����������и��ŵ�������ű�������У��������minj��maxj�ͻ�ȫ�� ��ò�Ƴ����߼������޸��ŵ㣩
        {
           BufferByte=BufferByte<<1;
           BufferByte|=(ImageTwo3[i][j]>=setvalue_Two?1:0);
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
          BufferByte|=(ImageTwo3[pointy][j]>=setvalue_Two?1:0);
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
  //////////////////////////10Hz����//////////////////////
    if(NOpoint)   Nopointtime++;
    else  Nopointtime=0;
    
    if(Nopointtime>6)
    {
      RealNopoint=1;
      if(Nopointtime>100) Nopointtime=12;
    }
    else
    {
      RealNopoint=0;
    }
  }






//------------------------------------------------------------------------------
//
//����ʱ������Ч����,������Figure_Process����
//
//------------------------------------------------------------------------------

unsigned char NoiseBuffer3[4];
void Noisefilter(void)
{
 uint8 i,j;
 if(WhichBuffer == 2)//���ڴ�ͼ2
  {
     lock=1;//��ͼ1������ͼ1
     for (i=0;i<ROW;i++)
     {
       for (j=0;j<COLUMN;j++)//��������ɨ��
       {
         NoiseBuffer3[0]=NoiseBuffer3[1];
         NoiseBuffer3[1]=NoiseBuffer3[2];
         NoiseBuffer3[2]=NoiseBuffer3[3];
         NoiseBuffer3[3]=Image1[i][j];   
         Image3[i][j]=Image1[i][j];
         if(NoiseBuffer3[3]<setvalue&&NoiseBuffer3[0]<setvalue)
         {
            Image3[i][j-1]=0;
            Image3[i][j-2]=0;
         }
       }//�д������
     }//һ��ͼ�������
  }

  else if(WhichBuffer == 1)//���ڴ�ͼ1
   {
     lock=2;//��ͼ1������ͼ1
     for (i=0;i<ROW;i++)
     {
       for (j=0;j<COLUMN;j++)//��������ɨ��
        {

        }//�д������
     }//һ��ͼ�������
   }
  lock=0; //���ͼ������
   if(start_flag==0)
    {
      if(WhichBuffer==1)
        LPLD_DMA_LoadDstAddr(DMA_CH0,(Image1[0]));//���ػ����Ŀ�ĵ�ַ
      else
        LPLD_DMA_LoadDstAddr(DMA_CH0,(Image2[0]));
      LPLD_DMA_EnableReq(DMA_CH0);//��ʼ�����趨��ѭ��������������Դ��������ʹ������Դ
      start_flag=1;
    }

}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED��ʾͼ��
//  @param      high  		ͼ��߶�
//  @param      width 		ͼ����
//  @param      *p    		ͼ���ַ�����飩
//  @param      value 		��ֵ����ֵ
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

void display_BarrierLine_100_60(float slope,int intercept)
{
  int16 i,j;
  uint16 temp,temp1;
  uint8 dat;
  for(j=0;j<60;j++)
  {
    i=(uint16)((j-intercept)/slope);
    temp=j/8;temp1=j%8;
    dat=1<<temp1;
    if(i>=0&&i<=100)
    {
      LED_Set_Pos(i,temp);
      LED_WrDat(dat);
    }
  }
}

void display_Line_coordinate_y_100_60(float slope_L,float slope_R,int intercept_L,int intercept_R,uint8 coordinate_y)
{
  uint8 i,mini,maxi;
  uint8 temp,temp1;
  uint8 dat;
  mini=(uint16)((coordinate_y-intercept_L)/slope_L);
  maxi=(uint16)((coordinate_y-intercept_R)/slope_R);
  temp=coordinate_y/8;temp1=coordinate_y%8;dat=1<<temp1;
  for(i=mini;i<maxi;i++)
  {
    LED_Set_Pos(i,temp);
    LED_WrDat(dat);
  }
}

void display_Center_100_60(uint8 *p,uint8 value,uint8 Barrier_x,uint8 Barrier_y)//������
{
  uint16 temp,temp1;
  uint8 dat,datbuffer;
  temp1=Barrier_y%8;
  if(temp1==0) temp=Barrier_y/8;
  else temp=Barrier_y/8+1;
  LED_Set_Pos(Barrier_x,temp);
  dat=0;
  if(  !temp1 || temp1>=1)dat |= (*(p+temp*8*100+100*0) > value? 1: 0)<<0;
  if(  !temp1 || temp1>=2)dat |= (*(p+temp*8*100+100*1) > value? 1: 0)<<1;
  if(  !temp1 || temp1>=3)dat |= (*(p+temp*8*100+100*2) > value? 1: 0)<<2;
  if(  !temp1 || temp1>=4)dat |= (*(p+temp*8*100+100*3) > value? 1: 0)<<3;
  if(  !temp1 || temp1>=5)dat |= (*(p+temp*8*100+100*4) > value? 1: 0)<<4;
  if(  !temp1 || temp1>=6)dat |= (*(p+temp*8*100+100*5) > value? 1: 0)<<5;
  if(  !temp1 || temp1>=7)dat |= (*(p+temp*8*100+100*6) > value? 1: 0)<<6;
  if(  !temp1 || temp1>=8)dat |= (*(p+temp*8*100+100*7) > value? 1: 0)<<7;
  datbuffer=~(1<<temp1);
  dat=dat&datbuffer;
  LED_WrDat(dat);
}

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
            if( i<(temp-1) || !temp1 || temp1>=1)dat |= (*(p+i*8*width*2*2+j*2+width*0*2*2) > value? 1: 0)<<0;
            if( i<(temp-1) || !temp1 || temp1>=2)dat |= (*(p+i*8*width*2*2+j*2+width*1*2*2) > value? 1: 0)<<1;
            if( i<(temp-1) || !temp1 || temp1>=3)dat |= (*(p+i*8*width*2*2+j*2+width*2*2*2) > value? 1: 0)<<2;
            if( i<(temp-1) || !temp1 || temp1>=4)dat |= (*(p+i*8*width*2*2+j*2+width*3*2*2) > value? 1: 0)<<3;
            if( i<(temp-1) || !temp1 || temp1>=5)dat |= (*(p+i*8*width*2*2+j*2+width*4*2*2) > value? 1: 0)<<4;
            if( i<(temp-1) || !temp1 || temp1>=6)dat |= (*(p+i*8*width*2*2+j*2+width*5*2*2) > value? 1: 0)<<5;
            if( i<(temp-1) || !temp1 || temp1>=7)dat |= (*(p+i*8*width*2*2+j*2+width*6*2*2) > value? 1: 0)<<6;
            if( i<(temp-1) || !temp1 || temp1>=8)dat |= (*(p+i*8*width*2*2+j*2+width*7*2*2) > value? 1: 0)<<7;
           
            LED_WrDat(dat);
        }
    }
}

void display_BarrierLine_80_50(float slope,int intercept)
{
  int16 i,j;
  uint16 temp,temp1;
  uint8 dat;
  for(j=0;j<15;j+=2)
  {
    i=(uint16)(((j-intercept)/slope)/2);
    temp=(j/2)/8;temp1=(j/2)%8;
    dat=1<<temp1;
    if(i>=0&&i<=80)
    {
      LED_Set_Pos(i,temp);
      LED_WrDat(dat);
    }
  }
}

void display_Line_coordinate_y_90_60(float slope_L,float slope_R,int intercept_L,int intercept_R,uint8 coordinate_y)
{
  uint8 i,mini,maxi;
  uint8 temp,temp1;
  uint8 dat;
  mini=(uint16)(((coordinate_y-intercept_L)/slope_L)/2);
  maxi=(uint16)(((coordinate_y-intercept_R)/slope_R)/2);
  temp=(coordinate_y/8)/2;temp1=(coordinate_y/2)%8;dat=1<<temp1;
  for(i=mini;i<maxi;i++)
  {
    LED_Set_Pos(i,temp);
    LED_WrDat(dat);
  }
}

void dis_bmp_half_two(uint16 high, uint16 width, uint8 *p,uint8 value)
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