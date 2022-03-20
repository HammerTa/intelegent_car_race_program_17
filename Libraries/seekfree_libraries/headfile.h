/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		headfile
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/
 
#ifndef _headfile_h
#define _headfile_h

//-------变量名定义
#define uint8_t  unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define int16_t  short int
#define int32_t  int

//-----管脚定义
//-----键盘
#define KEY_1 P22_0
#define KEY_2 P22_1
#define KEY_3 P22_2
#define KEY_4 P22_3
//-----电机
#define motor_l_EN       P02_4//左  +
#define motor_l_PN       ATOM0_CH7_P02_7//左
#define motor_r_EN       P02_6//右  +
#define motor_r_PN       ATOM0_CH5_P02_5//右
#define motor_frequency     13000
//-----舵机
#define steering_gear     ATOM1_CH1_P33_9
#define steering_frequency 50
#define S3010_Middle   595  //舵机中值
#define S3010_Left     665  //左打死
#define S3010_Right    525 //右打死
//-----编码器
#define encoder_GPT_l     GPT12_T4
#define encoder_EUD_l     GPT12_T4EUDA_P00_9  //DIR
#define encoder_IN_l      GPT12_T4INA_P02_8   //LSB
#define encoder_GPT_r     GPT12_T2
#define encoder_EUD_r     GPT12_T2EUDB_P33_6  //DIR
#define encoder_IN_r      GPT12_T2INB_P33_7   //LSB
//-----陀螺仪

//-----蜂鸣器

//-----蓝牙
#define uart_blue UART_2
#define VCAN_PORT UART_2
#define i2c_delay     2        //两次读取或写入的时间间隔,单位MS
#define DATE_LEN 89//定义数据长度
#define cursor_min    0
#define cursor_max    6
#define menu_min      0
#define menu_max      7
#define AD 3
#define Len 10
#define menu_N        5    //最大位数
#define condition_input  (jianzhi==1||jianzhi==2||jianzhi==3||jianzhi==5||jianzhi==6||jianzhi==7||jianzhi==9||jianzhi==10||jianzhi==11||jianzhi==14)  //封装输入时的判定依据（这样写可简化函数）
#define cursor_select 70      //光标显示的位置
#define cursor_set    90      //输入数字时光标的位置

#include "SEEKFREE_PRINTF.h"

#include "zf_assert.h"
#include "stdio.h"
#include "math.h"
//官方头文件
#include "ifxAsclin_reg.h"
#include "SysSe/Bsp/Bsp.h"
#include "IfxCcu6_Timer.h"
#include "IfxScuEru.h"

//------逐飞科技单片机外设驱动头文件
#include "zf_gpio.h"
#include "zf_gtm_pwm.h"
#include "zf_uart.h"
#include "zf_ccu6_pit.h"
#include "zf_stm_systick.h"
#include "zf_spi.h"
#include "zf_eru.h"
#include "zf_eru_dma.h"
#include "zf_vadc.h"
#include "zf_gpt12.h"
#include "zf_eeprom.h"

//------逐飞科技产品驱动头文件
#include "SEEKFREE_18TFT.h"
#include "SEEKFREE_FONT.h"
#include "SEEKFREE_FUN.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_IPS114_SPI.h"
#include "SEEKFREE_OLED.h"
#include "SEEKFREE_VIRSCO.h"
#include "SEEKFREE_MT9V03X.h"
#include "SEEKFREE_ICM20602.h"
#include "SEEKFREE_MPU6050.h"
#include "SEEKFREE_MMA8451.h"
#include "SEEKFREE_L3G4200D.h"
#include "SEEKFREE_WIRELESS.h"
#include "SEEKFREE_IPS200_PARALLEL8.h"
#include "SEEKFREE_7725.h"
#include "SEEKFREE_RDA5807.h"
#include "SEEKFREE_7725_UART.h"
#include "SEEKFREE_BLUETOOTH_CH9141.h"

//--------我的头文件
//#include "ZZK_Threshold.h"
#include "IMG_DEAL.h"
#include "inti.h"
#include "motor.h"
#include "Key.h"
#include "VCAN_computer.h"
#include "LCD_drive.h"
#endif

