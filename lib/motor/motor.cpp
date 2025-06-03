#include "motor.h"
#include <Arduino.h>
void Motor_Init(void)
{
    Defined_PWM_Init();
    // and the GPIO for dir pins
    pinMode(Motor1_Dir_Pin1, OUTPUT);
    pinMode(Motor1_Dir_Pin2, OUTPUT);
    pinMode(Motor2_Dir_Pin1, OUTPUT);
    pinMode(Motor2_Dir_Pin2, OUTPUT);
}

void Motor1_Operate(VehicleDirection direction, int32_t rotationspeed)
{
    if (rotationspeed < -MOTORARR)
        rotationspeed = -MOTORARR;
    else if (rotationspeed > MOTORARR)
        rotationspeed = MOTORARR;

    if (direction == Left)
    {
        digitalWrite(Motor1_Dir_Pin1, HIGH);
        digitalWrite(Motor1_Dir_Pin2, LOW);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, MOTOR1_PWM_CHANNEL, rotationspeed);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, MOTOR1_PWM_CHANNEL);
    }
    else if (direction == Right)
    {
        digitalWrite(Motor1_Dir_Pin1, LOW);
        digitalWrite(Motor1_Dir_Pin2, HIGH);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, MOTOR1_PWM_CHANNEL, -rotationspeed);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, MOTOR1_PWM_CHANNEL);
    }
    else
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, MOTOR1_PWM_CHANNEL, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, MOTOR1_PWM_CHANNEL);
    }
}
void Motor2_Operate(int32_t speed)
{
    if (speed < -MOTORARR)
        speed = -MOTORARR;
    else if (speed > MOTORARR)
        speed = MOTORARR;

    if (speed > 0)
    {
        digitalWrite(Motor2_Dir_Pin1, HIGH);
        digitalWrite(Motor2_Dir_Pin2, LOW);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, MOTOR2_PWM_CHANNEL, speed);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, MOTOR2_PWM_CHANNEL);
    }
    else if (speed < 0)
    {
        digitalWrite(Motor2_Dir_Pin1, LOW);
        digitalWrite(Motor2_Dir_Pin2, HIGH);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, MOTOR2_PWM_CHANNEL, -speed);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, MOTOR2_PWM_CHANNEL);
    }
    else
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, MOTOR2_PWM_CHANNEL, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, MOTOR2_PWM_CHANNEL);
    }
}

void Motor_Forward(uint16_t speed)
{
    Motor1_Operate(None,0);
    Motor2_Operate(speed);
}

void Motor_Backward(uint16_t speed)
{
    Motor1_Operate(None,0);
    Motor2_Operate(-speed);
}
void Motor_RightForward(uint16_t speed,uint16_t rotationspeed)
{
    Motor1_Operate(Right,speed);
    Motor2_Operate(0);
}
void Motor_LeftBackward(uint16_t speed,uint16_t rotationspeed)
{
    Motor1_Operate(Right,-speed);
    Motor2_Operate(-speed);
}

void Motor_LeftForward(uint16_t speed,uint16_t rotationspeed)
{
    Motor1_Operate(Left,speed);
    Motor2_Operate(speed);
}

void Motor_RightBackward(uint16_t speed,uint16_t rotationspeed)
{
    Motor1_Operate(None,0);
    Motor2_Operate(-speed);
}

void Motor_Stop(void)
{
    Motor1_Operate(None,0);
    Motor2_Operate(0);
        
}
void Vehicle_Move(enum VehicleMovement move, uint16_t speed,uint16_t rotationspeed)
{
    switch (move)
    {
    case Vehicle_Forward:
        Motor_Forward(speed);
        break;
    case Vehicle_Backward:
        Motor_Backward(speed);
        break;
    case Vehicle_Right_Forward:
        Motor_RightForward(speed,rotationspeed);
        break;
    case Vehicle_Left_Backward:
        Motor_LeftBackward(speed,rotationspeed);
        break;
    case Vehicle_Left_Forward:
        Motor_LeftForward(speed,rotationspeed);
        break;
    case Vehicle_Right_Backward:
        Motor_RightBackward(speed,rotationspeed);
        break;
    default:
        Motor_Stop();
        break;
    }
}