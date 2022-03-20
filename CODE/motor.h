/*
 * motor.h
 *
 *  Created on: 2022年1月17日
 *      Author: HammerTank
 */

#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_

extern int speed_l;
extern int speed_r;
extern int error;
extern float duoji_kp,duoji_kd;    //舵机，用于打角转弯
extern float duoji_kp0,duoji_kd0;  //直道pd
extern float duoji_kp1,duoji_kd1;  //弯道pd
extern float chasu,chasu_k;        //差速，用于转弯，低速时不需要
extern float P_rate,D_rate;
extern int left_motor_kp,left_motor_ki,left_motor_kd;    //电机pid用于稳定控制轮子转速
extern int right_motor_kp,right_motor_ki,right_motor_kd;
extern int setspeed;     //别给太大，一开始可以给20
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
