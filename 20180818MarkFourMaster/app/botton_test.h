#ifndef BOTTON_TEST_H
#define BOTTON_TEST_H
#include "common.h"

void boma_init_gpio(void);
void Key_delay(void);
unsigned char KeyScan(void);
void Move_Process(void);
void ChooseSide_Configure(void);
void PID_Adjustment(void);
void Parameter_Adjustment(void);
#endif