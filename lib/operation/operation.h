#ifndef OPERATION_H
#define OPERATION_H
#include "motor.h"
#define JOYCON_DEADZONE 5000
#define TRG_DEADZONE 10

extern  VehicleMovement movement;
extern uint16_t rotationspeed;
extern uint16_t MovementChangeFlag;

void Operate_Init (void);

inline uint16_t RadiusToMotorSpeed(double r){return ((uint16_t)(r-JOYCON_DEADZONE)/220<127?(uint16_t)(r-JOYCON_DEADZONE)/220:127);}
inline uint16_t TrgToMotorSpeed(uint32_t trg){return (trg+1)/8-1;}
uint16_t GetMovementChangeFlag(void);

void InterpretController(void);
void Operate(void);
#endif
