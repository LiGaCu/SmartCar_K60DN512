#include "common.h"
#include "myflash.h"
#include "camera.h"

#define FLASH_TEST_SECTOR   (256-20)
#define FLASH_TEST_ADDR     (FLASH_TEST_SECTOR*2048)// FLASH_TEST_SECTOR*(2048)
#define DATA_SIZE           (50)

float write_buffer[DATA_SIZE]; //����д������
float *ptr;
uint8 result=NULL;

void parameter_flash(void)
{
  ptr = (float*)FLASH_TEST_ADDR;  //��
  
  setvalue=(uint8)*(ptr+0);
    
  LPLD_LPTMR_DelayMs(100);
}

void flash_config(void)
{
  
  result=LPLD_Flash_SectorErase(FLASH_TEST_ADDR); //����
  LPLD_LPTMR_DelayMs(100);
  
  write_buffer[0]=setvalue*1.0;
  
  result=LPLD_Flash_ByteProgram((uint32)FLASH_TEST_ADDR,(uint32*)write_buffer, sizeof(write_buffer)); //д
  
  LPLD_LPTMR_DelayMs(100);
}




