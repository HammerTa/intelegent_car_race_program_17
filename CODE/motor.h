/*
 * motor.h
 *
 *  Created on: 2022��1��17��
 *      Author: HammerTank
 */

#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_

void steering_init(void);
void motor_init(void);
void Count_init(void);
void angle_deal(void);
void Speed_Get(void);
void motor_DiffSpeed(void);
void motor_pid(void);
void pwm_out(void);

#endif /* CODE_MOTOR_H_ */
