/*
 * motor.h
 *
 *  Created on: 2022��1��17��
 *      Author: HammerTank
 */

#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_

extern int speed_l;
extern int speed_r;
extern int setspeed_L,setspeed_R;
extern int error;
extern int error0;
extern float duoji_kp,duoji_kd;    //��������ڴ��ת��
extern float duoji_kp0,duoji_kd0;  //ֱ��pd
extern float duoji_kp1,duoji_kd1;  //���pd
extern float chasu,chasu_k;        //���٣�����ת�䣬����ʱ����Ҫ
extern float P_rate,D_rate;
extern float left_motor_kp,left_motor_ki,left_motor_kd;    //���pid�����ȶ���������ת��
extern float right_motor_kp,right_motor_ki,right_motor_kd;
extern float error_k,temp_cheak;
extern float Gain_L,Gain_R;//����
extern int setspeed;     //���̫��һ��ʼ���Ը�20
extern int min_speed;
extern int left_pwm_out;
extern int right_pwm_out;
extern int speed_error_L0,speed_error_L1,speed_error_L2;
extern int speed_error_R0,speed_error_R1,speed_error_R2;
extern int left_pwm,right_pwm;
extern int e_lim;
extern uint8 pwm0_flag;


void steering_init(void);
void motor_init(void);
void Count_init(void);
void angle_deal(void);
void Speed_Get(void);
void motor_DiffSpeed(void);
void motor_pid(void);
void pwm_out(void);
void Control(void);

#endif /* CODE_MOTOR_H_ */
