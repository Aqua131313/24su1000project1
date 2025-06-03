#include "pwm.h"

#include "Arduino.h"
#include "driver/ledc.h"
#include "soc/ledc_struct.h"
#include "soc/ledc_reg.h"

void Defined_PWM_Init(void) {
    
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = MOTOR_PWM_RESOLUTION,
        .timer_num = MOTOR_PWM_TIMER,
        .freq_hz = MOTOR_PWM_FREQUENCY,
        .clk_cfg = LEDC_USE_APB_CLK
    };
    ledc_timer_config(&ledc_timer);
//motor 1
    ledc_channel_config_t ledc_motor1_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = MOTOR1_PWM_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = MOTOR_PWM_TIMER,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&ledc_motor1_channel);
//motor 2
    ledc_channel_config_t ledc_motor2_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = MOTOR2_PWM_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = MOTOR_PWM_TIMER,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&ledc_motor2_channel);
}
