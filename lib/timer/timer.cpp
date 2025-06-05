#include "timer.h"
#include "Arduino.h"
#include "motor.h"
void IRAM_ATTR servotimer_intr(void);
void IRAM_ATTR buttontimer_intr(void);
void Defined_Timer_Init(void){
    //motor1
    hw_timer_t *motor1timer=timerBegin(MOTOR1_INTR_TIMER_IDX,MOTOR1_INTR_TIMER_DIVIDER,true);
    timerAttachInterrupt(motor1timer,motor1timer_intr,true);
    timerAlarmWrite(motor1timer,MOTOR1_INTR_TIMER_ALARM_VALUE,true);
    timerAlarmEnable(motor1timer);
}
void IRAM_ATTR motor1timer_intr(void)
{
    Motor_Tick();
}