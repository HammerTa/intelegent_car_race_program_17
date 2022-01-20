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
static int COUNT=0;

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
void steering_init()
{
    gtm_pwm_init(steering_gear,steering_frequency,S3010_Middle);
}

///***************************************************************
//* 函数名称： motor_inti
//* 功能说明： 电机初始化
//* 函数返回： 无
//* 备 注：
//***************************************************************
void motor_init()
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

///***************************************************************
//* 函数名称： Control
//* 功能说明： 总控制函数
//* 函数返回： 无
//* 备 注：
//***************************************************************
void Control()
{
    Speed_Get();
    angle_deal();
    motor_DiffSpeed();
    motor_pid();
    pwm_out();
}

///***************************************************************
//* 函数名称： angle_deal
//* 功能说明： 舵机打角输出
//* 函数返回： 无
//* 备 注：
//***************************************************************
void angle_deal()
{
  int i,j;
  int weight_sum=0;
  //================================
  //=====计算打角偏差===============
  error0 = error;
  if(left_flag==1||right_flag==1)
  {
    error = 0;
    weight_sum = 0;

    for(i = 119;i >= 0;i--)
    {
      if(middleline[i]!=254)
      {
        error += weight[i]*(middleline[i] - COL/2);
        weight_sum += weight[i];//此处可以进行算法复杂度优化
      }
    }
    if(weight_sum > 0)
      error /= -weight_sum;
    else
      error = error0;
  }
  else
    error = error0;
  //================================
  ERROR[COUNT]=(int)(error*0.2);
  COUNT++;
  error=0;
  for(j=0;j<5;j++)
  {
    error+=ERROR[j];
  }
  if(COUNT>=5)
  {
    COUNT=0;
  }//滤波
  //======打角pd控制================
  angle=(int)(duoji_kp*error+duoji_kd*(error-error0));
  angle_pwm_out= S3010_Middle-angle;
  if(angle_pwm_out>S3010_Right)
  {
    angle_pwm_out=S3010_Right;
  }
  if(angle_pwm_out<S3010_Left)
  {
    angle_pwm_out=S3010_Left;
  }
  pwm_duty(steering_gear,angle_pwm_out);
}

///***************************************************************
//* 函数名称： Speed_Get
//* 功能说明： 获取编码器读数
//* 函数返回： 无
//* 备 注：
//***************************************************************
void Speed_Get()
{
    speed_l=gpt12_get(encoder_GPT_l);
    speed_r=gpt12_get(encoder_GPT_r);
    //获得读数
    gpt12_clear(encoder_GPT_l);
    gpt12_clear(encoder_GPT_r);
    //清除读数
}

///***************************************************************
//* 函数名称： motor_DiffSpeed
//* 功能说明： 差速计算，停车控制
//* 函数返回： 无
//* 备 注：
//***************************************************************
void motor_DiffSpeed()
{
  if(stop==1)
  {
    setspeed=0;
  }
  chasu=(chasu_k*(float)fabs(error))/(2.0+chasu_k*(float)fabs(error))*(float)setspeed;//差速计算公式
  if(error==0)
  {
    setspeed_L=setspeed;
    setspeed_R=setspeed;
  }
  else if(error<0)
  {
    if(chasu>CS_lim*setspeed)//差速限幅，不一定是0.9
    {
     chasu=CS_lim*setspeed;
    }
    setspeed_L=(int)(setspeed+chasu/2);
    setspeed_R=(int)(setspeed-chasu/2);
  }
  else
  {
    if(chasu>CS_lim*setspeed)
    {
     chasu=CS_lim*setspeed;
    }
    setspeed_L=(int)(setspeed-chasu/2);
    setspeed_R=(int)(setspeed+chasu/2);
  }
}

///***************************************************************
//* 函数名称： motor_pid
//* 功能说明： PID电机控制
//* 函数返回： 无
//* 备 注：
//***************************************************************
int uil=0,uir=0;
void motor_pid()
{
  //左电机计算
  speed_error_L0=setspeed_L-speed_l;
  left_pwm=(int)((speed_error_L0-speed_error_L1)*left_motor_kp+speed_error_L0*left_motor_ki+(speed_error_L0-2*speed_error_L1+speed_error_L2)*left_motor_kd);
  left_pwm_out+=left_pwm;
  speed_error_L2=speed_error_L1;
  speed_error_L1=speed_error_L0;
//***************************************************************
//  speed_error_L0=setspeed_L-speed_l;
//  uil+=1.0*speed_error_L0*left_motor_ki/100;
//  if( uil > ui_lim*100 )uil = ui_lim*100;
//  else if ( uil < -ui_lim*100 )uil = -ui_lim*100;
//  left_pwm_out=(int)((speed_error_L0)*1.0*left_motor_kp+uil+(speed_error_L0-speed_error_L1)*left_motor_kd*1.0/100);
//  speed_error_L1=speed_error_L0;

  //右电机计算
  speed_error_R0=setspeed_R-speed_r;
  right_pwm=(int)((speed_error_R0-speed_error_R1)*right_motor_kp+speed_error_R0*right_motor_ki+(speed_error_R0-2*speed_error_R1+speed_error_R2)*right_motor_kd);
  right_pwm_out+=right_pwm;
  speed_error_R2=speed_error_R1;
  speed_error_R1=speed_error_R0;

  //位置式
//  speed_error_R0=setspeed_R-speed_r;
//  uir+=1.0*speed_error_R0*right_motor_ki/100;
//  if( uir > ui_lim*100 )uir = ui_lim*100;
//  else if ( uir < -ui_lim*100 )uir = -ui_lim*100;
//  right_pwm_out=(int)((speed_error_R0)*1.0*right_motor_kp+uir+(speed_error_R0-speed_error_R1)*right_motor_kd*1.0/100);
//  speed_error_R1=speed_error_R0;

//-----pwm限幅----
  if(left_pwm_out>lim_pwm)
  {
    left_pwm_out=lim_pwm;
  }
  else if(left_pwm_out<-lim_pwm)
  {
    left_pwm_out=-lim_pwm;
  }

   if(right_pwm_out>lim_pwm)
  {
    right_pwm_out=lim_pwm;
  }
  else if(right_pwm_out<-lim_pwm)
  {
    right_pwm_out=-lim_pwm;
  }
}

///***************************************************************
//* 函数名称： pwm_out
//* 功能说明： 电机PWM输出
//* 函数返回： 无
//* 备 注：
//***************************************************************
void pwm_out()
{
    //left_pwm_out=3000;
    //right_pwm_out=3000;//如果注释取消 则为开环
    if(left_pwm_out>=0)
    {
        pwm_duty(motor_l_0,left_pwm_out);
        pwm_duty(motor_l_1,0);
    }
    else
    {
        pwm_duty(motor_l_0,0);
        pwm_duty(motor_l_1,left_pwm_out);
    }
    if(right_pwm_out>=0)
    {
        pwm_duty(motor_l_0,right_pwm_out);
        pwm_duty(motor_l_1,0);
    }
    else
    {
        pwm_duty(motor_l_0,0);
        pwm_duty(motor_l_1,right_pwm_out);
    }
}
