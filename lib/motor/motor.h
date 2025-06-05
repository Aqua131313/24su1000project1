#ifndef MOTOR_H
#define MOTOR_H
#include <cstdint>
#include <cmath>
#include "../pwm/pwm.h"

#define MOTORARR (pow(2,(int)MOTOR_PWM_RESOLUTION))

enum VehicleMovement{
	Vehicle_Stop,
	Vehicle_Forward,
	Vehicle_Backward,	
    Vehicle_Right_Forward,
	Vehicle_Left_Backward,
	Vehicle_Right_Backward,
	Vehicle_Left_Forward
};

enum VehicleHead{
    ARMHEAD,
    BOARDHEAD
};
enum VehicleDirection{
	Left,
	Right,
	None
};
#define Motor1_Dir_Pin1	7
#define Motor1_Dir_Pin2 15    
#define Motor2_Dir_Pin1 17
#define Motor2_Dir_Pin2 18
#define Motor1DelayCount 300

//only temporary, need specification!!
void Motor_Init(void);
void Motor1_Operate(VehicleDirection direction, int32_t speed);
void Motor_Tick(void);
void Motor2_Operate(int32_t speed);

void Vehicle_Move(enum VehicleMovement move,uint16_t speed,uint16_t rotationspeed);
#endif
