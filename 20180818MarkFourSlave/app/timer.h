#ifndef __TIMER_H__
#define __TIMER_H__

#include "common.h"

#define limit_size_S 1000           
#define limit_size_C 1000
#define limit_vertical 80
#define limit_range 2               //35,33

#define S_limit_slope_L -1.6597
#define S_limit_slope_R 1.3390         //limit_slope_R=1.1362;
#define S_limit_intercept_L 92
#define S_limit_intercept_R -132       //156£»-172
#define S_limit_a_L 0.0029
#define S_limit_b_L -0.0890
#define S_limit_c_L 26                //36.5
#define S_limit_a_R 0.0032
#define S_limit_b_R -1.0396
#define S_limit_c_R 102                //140.5

#define C_limit_slope_L -2.1851
#define C_limit_slope_R 1.4400         //limit_slope_R=1.1362;
#define C_limit_intercept_L 102
#define C_limit_intercept_R -152       //156£»-172
#define C_limit_a_L 0.0048
#define C_limit_b_L -0.2827
#define C_limit_c_L 25                 //36.5
#define C_limit_a_R 0.0049
#define C_limit_b_R -1.3594
#define C_limit_c_R 111                //140.5

void pit_init(void);
void pit0_isr(void);
//void Key_delay(void);
extern uint8 Line_Left_Flag,Line_Right_Flag,Circle_Left_Flag,Circle_Right_Flag;
#endif