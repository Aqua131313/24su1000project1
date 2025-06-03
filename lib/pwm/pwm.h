#ifndef DEFINED_PWM_H
#define DEFINED_PWM_H
#include "driver/ledc.h"

#define MOTOR_PWM_TIMER LEDC_TIMER_0
#define MOTOR_PWM_FREQUENCY 6250
#define MOTOR_PWM_RESOLUTION LEDC_TIMER_7_BIT //the duty could be 0-127
#define MOTOR1_PWM_CHANNEL LEDC_CHANNEL_1
#define MOTOR2_PWM_CHANNEL LEDC_CHANNEL_2

#define MOTOR1_PWM_PIN GPIO_NUM_1
#define MOTOR2_PWM_PIN GPIO_NUM_2

void Defined_PWM_Init(void);



#endif