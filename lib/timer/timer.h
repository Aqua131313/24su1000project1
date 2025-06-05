#ifndef DEFINED_TIMER_H
#define DEFINED_TIMER_H
#include <Arduino.h>  

#define TIMER_BASE_FREQ 80000000

#define MOTOR1_INTR_TIMER_IDX 0
#define MOTOR1_INTR_TIMER_DIVIDER 80 //1 us
#define MOTOR1_INTR_TIMER_ALARM_VALUE 1000 // 1ms

void Defined_Timer_Init(void);
void IRAM_ATTR motor1timer_intr(void);

#endif
