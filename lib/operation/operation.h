#ifndef OPERATION_H
#define OPERATION_H
#include "bleretro32.h"
#include "motor.h"
#include <cstdint>
#define JOYCON_DEADZONE 5000
#define TRG_DEADZONE 10

void Operate_Init (void);
inline uint16_t RadiusToMotorSpeed(double r){return ((uint16_t)(r-JOYCON_DEADZONE)/220<127?(uint16_t)(r-JOYCON_DEADZONE)/220:127);}
inline uint16_t TrgToMotorSpeed(uint32_t trg){return (trg+1)/8-1;}
void InterpretController(void);
void Operate(void);
#endif
