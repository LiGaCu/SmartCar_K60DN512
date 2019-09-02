
#ifndef SERVO_FORFUN_H
#define SERVO_FORFAN_H

#include "common.h"


#define SerLeftMax 1170 //1025
#define SerRightMin 900 //������� 707 870
#define SerMid 1030      //1.5ms 865


extern  uint16 time_servo;  //���ʱ��


void servo_pit_init(void);
void servo_control(void);
void pit1_isr(void) ;


#endif