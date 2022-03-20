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
extern int error;
extern float duoji_kp,duoji_kd;    //��������ڴ��ת��
extern float duoji_kp0,duoji_kd0;  //ֱ��pd
extern float duoji_kp1,duoji_kd1;  //���pd
extern float chasu,chasu_k;        //���٣�����ת�䣬����ʱ����Ҫ
extern float P_rate,D_rate;
extern int left_motor_kp,left_motor_ki,left_motor_kd;    //���pid�����ȶ���������ת��
extern int right_motor_kp,right_motor_ki,right_motor_kd;
extern int setspeed;     //���̫��һ��ʼ���Ը�20
extern int left_pwm_out;
extern int right_pwm_out;


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
