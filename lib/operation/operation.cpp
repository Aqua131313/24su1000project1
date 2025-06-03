#include "operation.h"
#include "driver/ledc.h"

static enum VehicleMovement movement=Vehicle_Stop;
static uint16_t vehiclespeed=0;
static uint16_t rotationrotationspeed=0;
void Operate_Init (void)
{
    Motor_Init();
}

void InterpretController(void){
    //the status button(preset,deletepreset,wristhorizontal,confirm,changefacing) are not included
	
	xbox_controller_data_t xboxdata=ReturnControllerData();
    XBOX_JoyconLocation xboxjoyconLdata=XBOX_InterpretJoycon(xboxdata,LEFTJOYCON);
    
	//motor
    if(xboxjoyconLdata.r>JOYCON_DEADZONE){
        if(xboxjoyconLdata.angle>=-PI*5/8&&xboxjoyconLdata.angle<=-PI*3/8){
            movement=Vehicle_Forward;
        }else if(xboxjoyconLdata.angle>=PI*3/8&&xboxjoyconLdata.angle<=PI*5/8){
            movement=Vehicle_Backward;
        }else if(xboxjoyconLdata.angle>=-PI*5/8&&xboxjoyconLdata.angle<=-PI*3/8&&xboxdata.trg_r>TRG_DEADZONE){
            movement=Vehicle_Right_Forward;
            rotationrotationspeed=TrgToMotorSpeed(xboxdata.trg_r);
        }else if(xboxjoyconLdata.angle>=-PI*5/8&&xboxjoyconLdata.angle<=-PI*3/8&&xboxdata.trg_l>TRG_DEADZONE){
            movement=Vehicle_Left_Forward;
            rotationrotationspeed=TrgToMotorSpeed(xboxdata.trg_l);
        }else if(xboxjoyconLdata.angle>=PI*3/8&&xboxjoyconLdata.angle<=PI*5/8&&xboxdata.trg_r>TRG_DEADZONE){
            movement=Vehicle_Right_Backward;
            rotationrotationspeed=TrgToMotorSpeed(xboxdata.trg_r);
        }else if(xboxjoyconLdata.angle>=PI*3/8&&xboxjoyconLdata.angle<=PI*5/8&&xboxdata.trg_l>TRG_DEADZONE){
            movement=Vehicle_Left_Backward;
            rotationrotationspeed=TrgToMotorSpeed(xboxdata.trg_l);
        }
        vehiclespeed=RadiusToMotorSpeed(xboxjoyconLdata.r);
        
    }else{
        movement=Vehicle_Stop;
        vehiclespeed=0;
    }
		
}
void Operate(void){
    //include servomode steppermode and vehiclemove
		Vehicle_Move(movement,vehiclespeed,rotationrotationspeed);
}