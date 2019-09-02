#ifndef motor_FORFUN_H
#define motor_FORFUN_H
#include "common.h"

//extern float SpeedPID.Output[2];
extern uint8 car_run;
void motor_Init(void);
void MotorContrl(void);
void CarStop( void );
void MotorLeftForward(uint16 x);  
void MotorLeftBackward(uint16 x); 
void MotorRightForward(uint16 x);
void MotorRightBackward(uint16 x);


#endif