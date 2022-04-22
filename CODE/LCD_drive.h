/*
 * LCD_drive.h
 *
 *  Created on: 2022Äê3ÔÂ12ÈÕ
 *      Author: HammerTank
 */

#ifndef CODE_LCD_DRIVE_H_
#define CODE_LCD_DRIVE_H_


#include "headfile.h"

extern uint8 show_img;


void show(void);
void drive_data_get(void);
void key_scan(void);
void data_change(int);
void key_control(void);
void drive_data_init(void);
void changing(void);
void Data_save(void);
void Read_data(void);
void changing_program(void);

#endif /* CODE_LCD_DRIVE_H_ */
