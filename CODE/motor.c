/*
 * @Author: your name
 * @Date: 2022-01-17 12:14:16
 * @LastEditTime: 2022-01-18 21:22:26
 * @LastEditors: Please set LastEditors
 * @Description: ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \NationalRace_Program\CODE\motor.c
 */
/*
 * motor.c
 *
 *  Created on: 2022��1��17��
 *      Author: HammerTank
 *      ���ƴ���
 */
#include "headfile.h"


//�����Ǽ���
int angle=0;
int angle_pwm_out=0;

//�ٶ�
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

//================��Ҫ�޸ĵ�================
float duoji_kp=2,duoji_kd=2.7;    //��������ڴ��ת��

float chasu=0,chasu_k=0.03;      //���٣�����ת�䣬����ʱ����Ҫ

int left_motor_kp=20,left_motor_ki=5,left_motor_kd=5;    //���pid�����ȶ���������ת��
int right_motor_kp=20,right_motor_ki=8,right_motor_kd=5;
int setspeed=40;     //���̫��һ��ʼ���Ը�20


///***************************************************************
//* �������ƣ� steering_inti
//* ����˵���� �����ʼ��
//* �������أ� ��
//* �� ע��
//***************************************************************
void steering_inti()
{
    gtm_pwm_init(steering_gear,steering_frequency,S3010_Middle);
}

///***************************************************************
//* �������ƣ� motor_inti
//* ����˵���� �����ʼ��
//* �������أ� ��
//* �� ע��
//***************************************************************
void motor_inti()
{
    gtm_pwm_init(motor_l_0,motor_frequency,0);
    gtm_pwm_init(motor_l_1,motor_frequency,0);
    gtm_pwm_init(motor_r_0,motor_frequency,0);
    gtm_pwm_init(motor_r_1,motor_frequency,0);
}

///***************************************************************
//* �������ƣ� Count_init
//* ����˵���� ��������ʼ��
//* �������أ� ��
//* �� ע��
//***************************************************************
void Count_init()
{
    gpt12_init(encoder_GPT_l, encoder_IN_l, encoder_EUD_l);
    gpt12_init(encoder_GPT_r, encoder_IN_r, encoder_EUD_r);
}
