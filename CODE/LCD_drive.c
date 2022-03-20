/*
 * LCD_drive.c
 *
 *  Created on: 2022年3月12日
 *      Author: HammerTank
 */
#include "headfile.h"


int key_1_staut=1;
int key_2_staut=1;
int key_3_staut=1;
int key_4_staut=1;
//当前按键状态

int key_1_staut0=0;
int key_2_staut0=0;
int key_3_staut0=0;
int key_4_staut0=0;
//上次按键状态

int8 title1[20]="steer gear";
int8 title2[20]="motor left";
int8 title3[20]="motor right";
int8 title4[20]="diff speed";
//页面标题

float gear_data[4];//={duoji_kp0,duoji_kd0,duoji_kp,duoji_kd};//舵机
int L_motor_data[3];//={left_motor_kp,left_motor_ki,left_motor_kd};//左电机
int R_motor_data[3];//={right_motor_kp,right_motor_ki,right_motor_kd};//右电机
float speed_data[2];//={setspeed,chasu_k};//差速与速度设置
//需要改变的数据

int key_value=0;
int show_value=0;
uint8 show_img=0;
int title_flag=0;
int point_flag=1;

void drive_data_get()
{
    gear_data[0]=duoji_kp0;
    gear_data[1]=duoji_kd0;
    gear_data[2]=duoji_kp1;
    gear_data[3]=duoji_kd1;
    L_motor_data[0]=left_motor_kp;
    L_motor_data[1]=left_motor_ki;
    L_motor_data[2]=left_motor_kd;
    R_motor_data[0]=right_motor_kp;
    R_motor_data[1]=right_motor_ki;
    R_motor_data[2]=right_motor_kd;
    speed_data[0]=(float)setspeed;
    speed_data[1]=chasu_k;
}

void show()
{
    switch (title_flag)
    {
        case 0:
            lcd_showstr(0,0,title1);
            lcd_showfloat(0,1,gear_data[0],2,1);
            lcd_showfloat(0,2,gear_data[1],2,1);
            lcd_showfloat(0,3,gear_data[2],2,1);
            lcd_showfloat(0,4,gear_data[3],2,1);
            lcd_showint32(0,7,point_flag,5);
            break;
        case 1:
            lcd_showstr(0,0,title2);
            lcd_showint32(0,1,L_motor_data[0],5);
            lcd_showint32(0,2,L_motor_data[1],5);
            lcd_showint32(0,3,L_motor_data[2],5);
            lcd_showint32(0,7,point_flag,5);
            break;
        case 2:
            lcd_showstr(0,0,title3);
            lcd_showint32(0,1,R_motor_data[0],5);
            lcd_showint32(0,2,R_motor_data[1],5);
            lcd_showint32(0,3,R_motor_data[2],5);
            lcd_showint32(0,7,point_flag,5);
            break;
        case 3:
            lcd_showstr(0,0,title4);
            lcd_showfloat(0,1,speed_data[0],2,1);
            lcd_showfloat(0,2,speed_data[1],2,2);
            lcd_showint32(0,7,point_flag,5);
            break;
        case 4:
            lcd_showstr(0,0,"Ready?");
            break;
    }
}

void key_scan()
{
    gpio_init(KEY_1,GPI,0,PULLDOWN);
    gpio_init(KEY_2,GPI,0,PULLDOWN);
    gpio_init(KEY_3,GPI,0,PULLDOWN);
    gpio_init(KEY_4,GPI,0,PULLDOWN);
    key_1_staut0=key_1_staut;
    key_2_staut0=key_2_staut;
    key_3_staut0=key_3_staut;
    key_4_staut0=key_4_staut;
    key_1_staut=gpio_get(KEY_1);
    key_2_staut=gpio_get(KEY_2);
    key_3_staut=gpio_get(KEY_3);
    key_4_staut=gpio_get(KEY_4);
    if(key_1_staut==1 && key_1_staut0==0) key_value=1;
    if(key_2_staut==1 && key_2_staut0==0) key_value=2;
    if(key_3_staut==1 && key_3_staut0==0) key_value=3;
    if(key_4_staut==1 && key_4_staut0==0) key_value=4;
    systick_delay_ms(STM0,10);
}

void data_change(int key_input)
{
    switch(key_input)
    {
        case 1:
            lcd_clear(WHITE);
            title_flag++;
            if(title_flag>=5)
            {
                title_flag=0;
            }
            key_value=0;
            point_flag=1;
            break;
        case 2:
            point_flag++;
            if(point_flag>=5)
            {
                point_flag=1;
            }
            key_value=0;
            break;
        case 3:
            switch(title_flag)
            {
                case 0:
                    gear_data[point_flag-1]+=0.1;
                    break;
                case 1:
                    L_motor_data[point_flag-1]+=1;
                    break;
                case 2:
                    R_motor_data[point_flag-1]+=1;
                    break;
                case 3:
                    if(point_flag==1)
                    {
                        stop=0;
                        speed_data[point_flag-1]+=1;
                    }
                    else
                        speed_data[point_flag-1]+=0.01;
                    break;
                case 4:
                    break;
            }
            key_value=0;
            break;
        case 4:
            switch(title_flag)
            {
                case 0:
                    gear_data[point_flag-1]-=0.1;
                    break;
                case 1:
                    L_motor_data[point_flag-1]-=1;
                    break;
                case 2:
                    R_motor_data[point_flag-1]-=1;
                    break;
                case 3:
                    if(point_flag==1)
                        speed_data[point_flag-1]-=1;
                    else
                        speed_data[point_flag-1]-=0.01;
                    break;
                case 4:
                    break;
            }
            key_value=0;
            break;
    }
    duoji_kp0=gear_data[0];
    duoji_kd0=gear_data[1];
    duoji_kp1=gear_data[2];
    duoji_kd1=gear_data[3];
    left_motor_kp=L_motor_data[0];
    left_motor_ki=L_motor_data[1];
    left_motor_kd=L_motor_data[2];
    right_motor_kp=R_motor_data[0];
    right_motor_ki=R_motor_data[1];
    right_motor_kd=R_motor_data[2];
    setspeed=(int)speed_data[0];
    chasu_k=speed_data[1];
}

void key_control()
{
    drive_data_get();
    key_scan();
    data_change(key_value);
    show();
}
