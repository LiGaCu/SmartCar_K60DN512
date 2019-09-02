#include "common.h"
#include "QD.h"  
#include "myflash.h"

#define FLASH_TEST_SECTOR   (256-20)
#define FLASH_TEST_ADDR     (FLASH_TEST_SECTOR*2048)// FLASH_TEST_SECTOR*(2048)
#define DATA_SIZE           (50)

float write_buffer[DATA_SIZE]; //¶¨ÒåÐ´»º³åÇø
float *ptr;
uint8 result=NULL;

void parameter_flash(void)
{
  ptr = (float*)FLASH_TEST_ADDR;  //¶Á
  
  ChooseSide_Buffer[0]=(uint8)*(ptr+0);
  ChooseSide_Buffer[1]=(uint8)*(ptr+1);
  ChooseSide_Buffer[2]=(uint8)*(ptr+2);
  ChooseSide_Buffer[3]=(uint8)*(ptr+3);
  ChooseSide_Buffer[4]=(uint8)*(ptr+4);
  ChooseSide_Buffer[5]=(uint8)*(ptr+5);
  ChooseSide_Buffer[6]=(uint8)*(ptr+6);
  ChooseSide_Buffer[7]=(uint8)*(ptr+7);
  ChooseSide_Buffer[8]=(uint8)*(ptr+8);
  ChooseSide_Buffer[9]=(uint8)*(ptr+9);
  ChooseSide_Buffer[10]=(uint8)*(ptr+10);
  ChooseSide_Buffer[11]=(uint8)*(ptr+11);
  ChooseSide_Buffer[12]=(uint8)*(ptr+12);
  ChooseSide_Buffer[13]=(uint8)*(ptr+13);
  ChooseSide_Buffer[14]=(uint8)*(ptr+14);
  ChooseSide_Buffer[15]=(uint8)*(ptr+15);
  ChooseSide_Buffer[16]=(uint8)*(ptr+16);
  ChooseSide_Buffer[17]=(uint8)*(ptr+17);
  SpeedTarget_=(float)*(ptr+18);
  TurnInAdvanceDistance=(uint8)*(ptr+19);
  TurnInAdvanceDistance2=(uint8)*(ptr+20);
  emergy_speed=(float)*(ptr+21);
  nobrakeNum=(int)*(ptr+22);
  MadStopLock=(uint8)*(ptr+23);
  acceleration_speed=(float)*(ptr+24);
  AccelerationLock=(uint8)*(ptr+25);
  Surround_Go=(uint8)*(ptr+26);
  StopNum=(uint8)*(ptr+27);
  IntelligentSteeringLock=(uint8)*(ptr+28);
  
  LPLD_LPTMR_DelayMs(100);
}
void flash_config(void)
{
  
  result=LPLD_Flash_SectorErase(FLASH_TEST_ADDR); //²Á³ý
  LPLD_LPTMR_DelayMs(100);
  
  write_buffer[0]=ChooseSide_Buffer[0]*1.0;
  write_buffer[1]=ChooseSide_Buffer[1]*1.0;
  write_buffer[2]=ChooseSide_Buffer[2]*1.0;
  write_buffer[3]=ChooseSide_Buffer[3]*1.0;
  write_buffer[4]=ChooseSide_Buffer[4]*1.0;
  write_buffer[5]=ChooseSide_Buffer[5]*1.0;
  write_buffer[6]=ChooseSide_Buffer[6]*1.0;
  write_buffer[7]=ChooseSide_Buffer[7]*1.0;
  write_buffer[8]=ChooseSide_Buffer[8]*1.0;
  write_buffer[9]=ChooseSide_Buffer[9]*1.0;
  write_buffer[10]=ChooseSide_Buffer[10]*1.0;
  write_buffer[11]=ChooseSide_Buffer[11]*1.0;
  write_buffer[12]=ChooseSide_Buffer[12]*1.0;
  write_buffer[13]=ChooseSide_Buffer[13]*1.0;
  write_buffer[14]=ChooseSide_Buffer[14]*1.0;
  write_buffer[15]=ChooseSide_Buffer[15]*1.0;
  write_buffer[16]=ChooseSide_Buffer[16]*1.0;
  write_buffer[17]=ChooseSide_Buffer[17]*1.0;
  write_buffer[18]=SpeedTarget_*1.0;
  write_buffer[19]=TurnInAdvanceDistance*1.0;
  write_buffer[20]=TurnInAdvanceDistance2*1.0;
  write_buffer[21]=emergy_speed*1.0;
  write_buffer[22]=nobrakeNum*1.0;
  write_buffer[23]=MadStopLock*1.0;
  write_buffer[24]=acceleration_speed*1.0;
  write_buffer[25]=AccelerationLock*1.0;
  write_buffer[26]=Surround_Go*1.0;
  write_buffer[27]=StopNum*1.0;
  write_buffer[28]=IntelligentSteeringLock*1.0;
  
  result=LPLD_Flash_ByteProgram((uint32)FLASH_TEST_ADDR,(uint32*)write_buffer, sizeof(write_buffer)); //Ð´
  
  LPLD_LPTMR_DelayMs(100);
}




