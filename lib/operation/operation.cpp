#include "motor.h"
#include "operation.h"
#include "driver/ledc.h"
#include "bleretro32.h"
#include <cstdint>
#include "timer.h"

enum VehicleMovement movement=Vehicle_Stop;
enum VehicleMovement PreviousMovement=Vehicle_Stop;
uint16_t MovementChangeFlag=0;
static uint16_t vehiclespeed=0;
uint16_t rotationspeed=0;
void Operate_Init (void)
{
    Motor_Init();
    Defined_Timer_Init();
}

void ModeFlag(enum VehicleMovement newmode)
{
    PreviousMovement=movement;
    movement=newmode;
    if (movement != PreviousMovement)
    {
        MovementChangeFlag = 1;
    }
    else
    {
        MovementChangeFlag = 0;
    }
}

void InterpretController(void){
    //the status button(preset,deletepreset,wristhorizontal,confirm,changefacing) are not included
	
	xbox_controller_data_t xboxdata=ReturnControllerData();
    XBOX_JoyconLocation xboxjoyconLdata=XBOX_InterpretJoycon(xboxdata,LEFTJOYCON);
    
	//motor
    if(xboxjoyconLdata.r>JOYCON_DEADZONE){
        if(xboxjoyconLdata.angle>=-PI*5/8&&xboxjoyconLdata.angle<=-PI*3/8){
            ModeFlag(Vehicle_Forward);
        }else if(xboxjoyconLdata.angle>=PI*3/8&&xboxjoyconLdata.angle<=PI*5/8){
            ModeFlag(Vehicle_Backward);
        }else if(xboxjoyconLdata.angle>=-PI*5/8&&xboxjoyconLdata.angle<=-PI*3/8&&xboxdata.trg_r>TRG_DEADZONE){
            ModeFlag(Vehicle_Right_Forward);
            rotationspeed=TrgToMotorSpeed(xboxdata.trg_r);
        }else if(xboxjoyconLdata.angle>=-PI*5/8&&xboxjoyconLdata.angle<=-PI*3/8&&xboxdata.trg_l>TRG_DEADZONE){
            ModeFlag(Vehicle_Left_Forward);
            rotationspeed=TrgToMotorSpeed(xboxdata.trg_l);
        }else if(xboxjoyconLdata.angle>=PI*3/8&&xboxjoyconLdata.angle<=PI*5/8&&xboxdata.trg_r>TRG_DEADZONE){
            ModeFlag(Vehicle_Right_Backward);
            rotationspeed=TrgToMotorSpeed(xboxdata.trg_r);
        }else if(xboxjoyconLdata.angle>=PI*3/8&&xboxjoyconLdata.angle<=PI*5/8&&xboxdata.trg_l>TRG_DEADZONE){
            ModeFlag(Vehicle_Left_Backward);
            rotationspeed=TrgToMotorSpeed(xboxdata.trg_l);
        }
        vehiclespeed=RadiusToMotorSpeed(xboxjoyconLdata.r);
        
    }else{
        ModeFlag(Vehicle_Stop);
        vehiclespeed=0;
    }
		
}
void Operate(void){
    //include servomode steppermode and vehiclemove
		Vehicle_Move(movement,vehiclespeed,rotationspeed);
}