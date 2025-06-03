#ifndef XBOX_H
#define XBOX_H
#include <Arduino.h>
struct __attribute__((packed)) xbox_controller_data_t
{
    uint16_t joy_l_h; // bytes 0 1. 0 to 65535, middle 32768
    uint16_t joy_l_v; // bytes 2 3. 0 to 65535, middle 32768

    uint16_t joy_r_h; //  bytes 4 5. 0 to 65535, middle 32768
    uint16_t joy_r_v; //  bytes 6 7. 0 to 65535, middle 32768

    uint16_t trg_l; // bytes 8 9. 0 to 1023
    uint16_t trg_r; // bytes 10 11. 0 to 1023

    uint8_t dpad; // 12. 1 -> U, 2 -> UR, 3 -> R, 4 -> DR, 5 -> D, 6 -> DL, 7 -> L, 8 -> LU

    // uint8_t main_buttons; // 13
    bool btnA : 1;
    bool btnB : 1;
    bool _padding_1 : 1;
    bool btnX : 1;
    bool btnY : 1;
    bool _padding_2 : 1;
    bool btnL : 1;
    bool btnR : 1;

    // uint8_t secondary_buttons; // 14
    bool _padding_3 : 2;
    bool btnSelect : 1;
    bool btnStart : 1;
    bool btnXbox : 1;
    bool btnLStick : 1;
    bool btnRStick : 1;
    bool _padding_4 : 1;

    // uint8_t share_button; // 15
    bool btnShare : 1;
    bool _padding_5 : 7;
};

struct XBOX_JoyconLocation{
    double r;
    double angle;
};
/*  
       -pi/2
         |
       3 | 4
+/-pi----+----  0pi
       2 | 1
         |
       pi/2
*/


#define AXIS_MAX 65535
#define AXIS_MIDDLE 32767
#define TRI_MAX 1023
#define PRESSED 1
#define UNPRESSED 0

#define DPAD_U 1
#define DPAD_UR 2
#define DPAD_R 3
#define DPAD_DR 4
#define DPAD_D 5
#define DPAD_DL 6
#define DPAD_L 7
#define DPAD_UL 8
#define LEFTJOYCON 0
#define RIGHTJOYCON 1

XBOX_JoyconLocation XBOX_InterpretJoycon(xbox_controller_data_t xboxdata,uint8_t WhichJoyCon);
#endif