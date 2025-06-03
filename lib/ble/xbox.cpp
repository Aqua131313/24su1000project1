#include "xbox.h"
#include <cmath>
XBOX_JoyconLocation XBOX_InterpretJoycon(xbox_controller_data_t xboxdata,uint8_t WhichJoyCon){
    int32_t xaxis=0;
    int32_t yaxis=0;
    XBOX_JoyconLocation location={0,0};
    if(WhichJoyCon==LEFTJOYCON){
        xaxis=xboxdata.joy_l_h-AXIS_MIDDLE;
        yaxis=xboxdata.joy_l_v-AXIS_MIDDLE;
    }else if(WhichJoyCon==RIGHTJOYCON){
        xaxis=xboxdata.joy_r_h-AXIS_MIDDLE;
        yaxis=xboxdata.joy_r_v-AXIS_MIDDLE;
    }
    //xaxis and yaxis are converted to standard coordinates
    location.r=pow(xaxis*xaxis+yaxis*yaxis,0.5);
    location.angle=atan2(yaxis,xaxis);
    return location;
}

