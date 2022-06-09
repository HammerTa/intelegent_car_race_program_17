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
int key_5_staut=1;
int key_6_staut=1;
int key_7_staut=1;
int key_8_staut=1;
int key_9_staut=1;
int key_10_staut=1;
int key_11_staut=1;
int key_12_staut=1;
int key_1_col_staut=1;
int key_2_col_staut=1;
int key_3_col_staut=1;
//当前按键状态

int key_1_staut0=0;
int key_2_staut0=0;
int key_3_staut0=0;
int key_4_staut0=0;
int key_5_staut0=0;
int key_6_staut0=0;
int key_7_staut0=0;
int key_8_staut0=0;
int key_9_staut0=0;
int key_10_staut0=0;
int key_11_staut0=0;
int key_12_staut0=0;
int key_1_col_staut0=0;
int key_2_col_staut0=0;
int key_3_col_staut0=0;
//上次按键状态

int8 title1[20]="steer gear";
int8 title2[20]="motor left";
int8 title3[20]="motor right";
int8 title4[20]="diff speed";
//页面标题

float gear_data[4];//={duoji_kp0,duoji_kd0,duoji_kp,duoji_kd};//舵机
float L_motor_data[3];//={left_motor_kp,left_motor_ki,left_motor_kd};//左电机
float R_motor_data[3];//={right_motor_kp,right_motor_ki,right_motor_kd};//右电机
float speed_data[3];//={setspeed,chasu_k};//差速与速度设置
float Gain_data[2];//左右误差增益
//需要改变的数据

int key_value=0;
int key_value_col=0;
int show_value=0;
uint8 show_img=0;
int title_flag=4;
int point_flag=1;
int key_time=0;//按键时间
int Save[100];




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
    speed_data[1]=(float)min_speed;
    speed_data[2]=chasu_k;
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
            lcd_showfloat(0,1,L_motor_data[0],3,1);
            lcd_showfloat(0,2,L_motor_data[1],2,1);
            lcd_showfloat(0,3,L_motor_data[2],2,1);
            lcd_showint32(0,7,point_flag,5);
            break;
        case 2:
            lcd_showstr(0,0,title3);
            lcd_showfloat(0,1,R_motor_data[0],3,1);
            lcd_showfloat(0,2,R_motor_data[1],2,1);
            lcd_showfloat(0,3,R_motor_data[2],2,1);
            lcd_showint32(0,7,point_flag,5);
            break;
        case 3:
            lcd_showstr(0,0,title4);
            lcd_showfloat(0,1,speed_data[0],2,1);
            lcd_showfloat(0,2,speed_data[1],2,1);
            lcd_showfloat(0,3,speed_data[2],2,2);
            lcd_showint32(0,7,point_flag,5);
            break;
        case 4:
            lcd_showstr(0,0,"LiveData");
            lcd_showfloat(0,1,(float)setspeed,2,4);
            lcd_showfloat(0,2,(float)error,2,4);
            lcd_showfloat(0,3,(float)L_S,2,5);
            lcd_showfloat(0,4,(float)R_S,2,5);
            lcd_showfloat(0,5,(float)speed_l,2,5);
            lcd_showfloat(0,6,(float)speed_r,2,5);
            lcd_showint32(0,7,point_flag,5);
            break;
        case 5:
            lcd_showstr(0,0,"T_turning");
            lcd_showint32(0,1,T_go_flag[0],2);
            lcd_showint32(0,2,T_go_flag[1],2);
            lcd_showint32(0,3,(int)fork_turn,2);
            lcd_showint32(0,7,point_flag,5);
            break;
        case 6:
            lcd_showstr(0,0,"Gain");
            lcd_showfloat(0,1,Gain_data[0],2,2);
            lcd_showfloat(0,2,Gain_data[1],2,2);
            lcd_showint32(0,7,point_flag,5);
            break;
    }
}

void key_scan_row()
{
    gpio_init(KEY_1,GPI,0,PULLDOWN);
    gpio_init(KEY_2,GPI,0,PULLDOWN);
    gpio_init(KEY_3,GPI,0,PULLDOWN);
    gpio_init(KEY_4,GPI,0,PULLDOWN);
    gpio_init(KEY_COL1,GPO,1,PUSHPULL);
    gpio_init(KEY_COL2,GPO,0,PUSHPULL);
    gpio_init(KEY_COL3,GPO,0,PUSHPULL);
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
    systick_delay_ms(STM0,50);
    gpio_init(KEY_1,GPI,0,PULLDOWN);
    gpio_init(KEY_2,GPI,0,PULLDOWN);
    gpio_init(KEY_3,GPI,0,PULLDOWN);
    gpio_init(KEY_4,GPI,0,PULLDOWN);
    gpio_init(KEY_COL1,GPO,0,PUSHPULL);
    gpio_init(KEY_COL2,GPO,1,PUSHPULL);
    gpio_init(KEY_COL3,GPO,0,PUSHPULL);
    key_5_staut0=key_5_staut;
    key_6_staut0=key_6_staut;
    key_7_staut0=key_7_staut;
    key_8_staut0=key_8_staut;
    key_5_staut=gpio_get(KEY_1);
    key_6_staut=gpio_get(KEY_2);
    key_7_staut=gpio_get(KEY_3);
    key_8_staut=gpio_get(KEY_4);
    if(key_5_staut==1 && key_5_staut0==0) key_value=5;
    if(key_6_staut==1 && key_6_staut0==0) key_value=6;
    if(key_7_staut==1 && key_7_staut0==0) key_value=7;
    if(key_8_staut==1 && key_8_staut0==0) key_value=8;
    systick_delay_ms(STM0,50);
    gpio_init(KEY_1,GPI,0,PULLDOWN);
    gpio_init(KEY_2,GPI,0,PULLDOWN);
    gpio_init(KEY_3,GPI,0,PULLDOWN);
    gpio_init(KEY_4,GPI,0,PULLDOWN);
    gpio_init(KEY_COL1,GPO,0,PUSHPULL);
    gpio_init(KEY_COL2,GPO,0,PUSHPULL);
    gpio_init(KEY_COL3,GPO,1,PUSHPULL);
    key_9_staut0=key_9_staut;
    key_10_staut0=key_10_staut;
    key_11_staut0=key_11_staut;
    key_12_staut0=key_12_staut;
    key_9_staut=gpio_get(KEY_1);
    key_10_staut=gpio_get(KEY_2);
    key_11_staut=gpio_get(KEY_3);
    key_12_staut=gpio_get(KEY_4);
    if(key_9_staut==1 && key_9_staut0==0) key_value=9;
    if(key_10_staut==1 && key_10_staut0==0) key_value=10;
    if(key_11_staut==1 && key_11_staut0==0) key_value=11;
    if(key_12_staut==1 && key_12_staut0==0) key_value=12;
    systick_delay_ms(STM0,50);
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
    systick_delay_ms(STM0,50);
}

void data_change(int key_input)
{
    switch(key_input)
    {
        case 1:
            lcd_clear(WHITE);
            title_flag++;
            if(title_flag>=7)
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
                    gear_data[point_flag-1]-=0.1;
                    break;
                case 1:
                    L_motor_data[point_flag-1]-=0.1;
                    break;
                case 2:
                    R_motor_data[point_flag-1]-=0.1;
                    break;
                case 3:
                    if(point_flag==3)
                        speed_data[point_flag-1]-=0.01;
                    else
                        speed_data[point_flag-1]-=1;
                    break;
                case 4:
                    break;
                case 5:
                    if(point_flag<=2) T_go_flag[point_flag-1]=0;
                    else if(point_flag==3) fork_turn=1;
                    break;
                case 6:
                    Gain_data[point_flag-1]-=0.01;
                    break;
            }
            key_value=0;
            break;
        case 4:
            switch(title_flag)
            {
                case 0:
                    gear_data[point_flag-1]+=0.1;
                    break;
                case 1:
                        L_motor_data[point_flag-1]+=0.1;
                    break;
                case 2:
                        R_motor_data[point_flag-1]+=0.1;
                    break;
                case 3:
                    if(point_flag==3)
                        speed_data[point_flag-1]+=0.01;
                    else
                        speed_data[point_flag-1]+=1;
                    break;
                case 4:
                    break;
                case 5:
                    if(point_flag<=2) T_go_flag[point_flag-1]=1;
                    else if(point_flag==3) fork_turn=2;
                    break;
                case 6:
                    Gain_data[point_flag-1]+=0.01;
                    break;
            }
            key_value=0;
            break;
        case 5:
            lcd_clear(WHITE);
            title_flag--;
            if(title_flag<0)
            {
                title_flag=6;
            }
            key_value=0;
            point_flag=1;
            break;
        case 6:
            point_flag--;
            if(point_flag<1)
            {
                point_flag=4;
            }
            key_value=0;
            break;
        case 7:
            switch(title_flag)
            {
                case 0:
                    gear_data[point_flag-1]-=1;
                    break;
                case 1:
                    L_motor_data[point_flag-1]-=1;
                    break;
                case 2:
                    R_motor_data[point_flag-1]-=1;
                    break;
                case 3:
                    if(point_flag==3)
                        speed_data[point_flag-1]-=0.1;
                    else
                        speed_data[point_flag-1]-=1;
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    break;
            }
            key_value=0;
            break;
        case 8:
            switch(title_flag)
            {
                case 0:
                    gear_data[point_flag-1]+=1;
                    break;
                case 1:
                        L_motor_data[point_flag-1]+=1;
                    break;
                case 2:
                        R_motor_data[point_flag-1]+=1;
                    break;
                case 3:
                    if(point_flag==3)
                        speed_data[point_flag-1]+=0.1;
                    else
                        speed_data[point_flag-1]+=1;
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    break;
           }
            key_value=0;
            break;
        case 9:
            eeprom_erase_sector(0);
            systick_delay_ms(STM0,1000);
            Data_save();
            send_flag=1;
            systick_delay_ms(STM0,1000);
            changing();
//            changing_program();
            key_value=0;
            break;
        case 10:
            if(pwm0_flag==1) pwm0_flag=0;
            else pwm0_flag=1;
            key_value=0;
            break;
        case 11:
            switch(title_flag)
            {
                case 0:
                    gear_data[point_flag-1]-=10;
                    break;
                case 1:
                    L_motor_data[point_flag-1]-=10;
                    break;
                case 2:
                    R_motor_data[point_flag-1]-=10;
                    break;
                case 3:
                    if(point_flag==3)
                        speed_data[point_flag-1]-=1;
                    else
                        speed_data[point_flag-1]-=10;
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    break;
            }
            key_value=0;
            break;
        case 12:
            switch(title_flag)
            {
                case 0:
                    gear_data[point_flag-1]+=10;
                    break;
                case 1:
                        L_motor_data[point_flag-1]+=10;
                    break;
                case 2:
                        R_motor_data[point_flag-1]+=10;
                    break;
                case 3:
                    if(point_flag==3)
                        speed_data[point_flag-1]+=1;
                    else
                        speed_data[point_flag-1]+=10;
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    break;
            }
            key_value=0;
            break;
    }
}

void changing()
{
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
    Gain_L=Gain_data[0];
    Gain_R=Gain_data[1];
    setspeed=(int)speed_data[0];
    min_speed=speed_data[1];
    chasu_k=speed_data[2];
    pwm0_flag=0;
    stop=0;
    speed_error_L0=0;
    speed_error_L1=0;
    speed_error_L2=0;
    speed_error_R0=0;
    speed_error_R1=0;
    speed_error_R2=0;
    error=0;
    error0=0;
    left_pwm=0;
    right_pwm=0;
}

void Data_save()
{
    Save[0]=(int)(gear_data[0]*10);
    Save[1]=(int)(gear_data[1]*10);
    Save[2]=(int)(gear_data[2]*10);
    Save[3]=(int)(gear_data[3]*10);
    Save[4]=(int)(L_motor_data[0]*10);
    Save[5]=(int)(L_motor_data[1]*10);
    Save[6]=(int)(L_motor_data[2]*10);
    Save[7]=(int)(R_motor_data[0]*10);
    Save[8]=(int)(R_motor_data[1]*10);
    Save[9]=(int)(R_motor_data[2]*10);
    Save[10]=(int)(speed_data[0]*10);
    Save[11]=(int)(speed_data[1]*10);
    Save[12]=(int)(speed_data[2]*100);
    Save[13]=(int)(Gain_data[0]*100);
    Save[14]=(int)(Gain_data[1]*100);
    for(int i=0;i<100;i++)
    {
        eeprom_page_program(0,i,&Save[i]);
    }
}

void Read_data()
{
    for(int i=0;i<100;i++)
    {
        Save[i]=flash_read(0,i,int);
    }
    gear_data[0]=0.1*(float)Save[0];
    gear_data[1]=0.1*(float)Save[1];
    gear_data[2]=0.1*(float)Save[2];
    gear_data[3]=0.1*(float)Save[3];
    L_motor_data[0]=0.1*(float)Save[4];
    L_motor_data[1]=0.1*(float)Save[5];
    L_motor_data[2]=0.1*(float)Save[6];
    R_motor_data[0]=0.1*(float)Save[7];
    R_motor_data[1]=0.1*(float)Save[8];
    R_motor_data[2]=0.1*(float)Save[9];
    speed_data[0]=0.1*(float)Save[10];
    speed_data[1]=0.1*(float)Save[11];
    speed_data[2]=0.01*(float)Save[12];
    Gain_data[0]=0.01*(float)Save[13];
    Gain_data[1]=0.01*(float)Save[14];
}

void long_prass_1(int key_press)
{
    if(key_press==2) key_time++;
    if(key_time>=4)
    {
        key_value=1;
        key_time=0;
    }
}//三号长按

void show_key()
{
    lcd_showint32(1,1,key_value,5);
}

void changing_program()//代码调参
{
    duoji_kp0=1.2;
    duoji_kd0=3;
    duoji_kp1=6;
    duoji_kd1=0;
    left_motor_kp=200;
    left_motor_ki=100;
    left_motor_kd=80;
    right_motor_kp=200;
    right_motor_ki=100;
    right_motor_kd=80;
    setspeed=60;
    min_speed=40;
    chasu_k=0.01;
    Gain_R=0.4;
    Gain_L=0.0;
    pwm0_flag=0;
    stop=0;
    speed_error_L0=0;
    speed_error_L1=0;
    speed_error_L2=0;
    speed_error_R0=0;
    speed_error_R1=0;
    speed_error_R2=0;
    error=0;
    error0=0;
    left_pwm=0;
    right_pwm=0;
}
void key_control()
{
    //key_scan();
    key_scan_row();
//    long_prass_1(key_value);
    data_change(key_value);
    show();
}
