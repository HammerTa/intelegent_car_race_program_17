/*
 * motor.c
 *
 *  Created on: 2022年1月17日
 *      Author: HammerTank
 *      控制代码
 */
#include "headfile.h"


void steering_inti()
{
    gtm_pwm_init(steering_gear,steering_frequency,S3010_Middle);
}

void motor_inti()
{
    gtm_pwm_init(motor_l_0,motor_frequency,0);
    gtm_pwm_init(motor_l_1,motor_frequency,0);
    gtm_pwm_init(motor_r_0,motor_frequency,0);
    gtm_pwm_init(motor_r_1,motor_frequency,0);
}
