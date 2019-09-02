#ifndef QD_H
#define QD_H

#include "common.h"



void pit0_init(void);
void QD_init(void);
void pit0_isr(void);
void Obstacle_init(void);
extern int16 speed_right,speed_left;
extern uint8 RightSide_Go,Turn_lock,Surround_Go;

extern uint8 flagSpeed;
extern uint8 ChooseSide_Num;
extern uint8 ChooseSide_Buffer[18];
extern uint16 TenSecondsRecordLength;
extern vuint16 TenSecondsRecordBegin;

extern float SpeedTarget__R[2];
extern float SpeedTarget__L[2];
extern float SpeedTarget_;

extern int nobrakeNum,StopNum;
extern uint8 IntelligentSteeringLock;
extern uint8 TurnInAdvanceDistance;
extern uint8 TurnInAdvanceDistance2;
extern float emergy_speed,acceleration_speed;
extern uint8 MadStopLock,AccelerationLock;
extern uint8 RealNoSignal;
extern uint16 Nopointtime;
#define CAR_PLUSE_ONE_METER 5770;

#endif