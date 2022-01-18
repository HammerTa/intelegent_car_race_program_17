/*
 * @Author: your name
 * @Date: 2022-01-17 12:14:16
 * @LastEditTime: 2022-01-18 21:22:26
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \NationalRace_Program\CODE\motor.c
 */
/*
 * motor.c
 *
 *  Created on: 2022年1月17日
 *      Author: HammerTank
 *      控制代码
 */
#include "headfile.h"


//舵机打角计算
int angle=0;
int angle_pwm_out=0;

//速度
int speed_l=0,speed_r=0;
int direction_r=0;
int setspeed_L=0,setspeed_R=0;
int speed_error_L0=0,speed_error_L1=0,speed_error_L2=0;
int speed_error_R0=0,speed_error_R1=0,speed_error_R2=0;
int left_pwm=0,right_pwm=0;

int left_pwm_out=0,right_pwm_out=0;
int ui_lim=0;
int error=0;
int error0=0;
int lim_pwm=4000;
float CS_lim=0.95;
int ERROR[5]={0,0,0,0,0};
//static int COUNT=0;

//================需要修改的================
float duoji_kp=2,duoji_kd=2.7;    //舵机，用于打角转弯

float chasu=0,chasu_k=0.03;      //差速，用于转弯，低速时不需要

int left_motor_kp=20,left_motor_ki=5,left_motor_kd=5;    //电机pid用于稳定控制轮子转速
int right_motor_kp=20,right_motor_ki=8,right_motor_kd=5;
int setspeed=40;     //别给太大，一开始可以给20


///***************************************************************
//* 函数名称： steering_inti
//* 功能说明： 舵机初始化
//* 函数返回： 无
//* 备 注：
//***************************************************************
void steering_inti()
{
    gtm_pwm_init(steering_gear,steering_frequency,S3010_Middle);
}

///***************************************************************
//* 函数名称： motor_inti
//* 功能说明： 电机初始化
//* 函数返回： 无
//* 备 注：
//***************************************************************
void motor_inti()
{
    gtm_pwm_init(motor_l_0,motor_frequency,0);
    gtm_pwm_init(motor_l_1,motor_frequency,0);
    gtm_pwm_init(motor_r_0,motor_frequency,0);
    gtm_pwm_init(motor_r_1,motor_frequency,0);
}

///***************************************************************
//* 函数名称： Count_init
//* 功能说明： 编码器初始化
//* 函数返回： 无
//* 备 注：
//***************************************************************
void Count_init()
{
    gpt12_init(encoder_GPT_l, encoder_IN_l, encoder_EUD_l);
    gpt12_init(encoder_GPT_r, encoder_IN_r, encoder_EUD_r);
}
