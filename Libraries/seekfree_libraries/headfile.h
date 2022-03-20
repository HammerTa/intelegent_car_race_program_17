/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		headfile
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/
 
#ifndef _headfile_h
#define _headfile_h

//-------����������
#define uint8_t  unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define int16_t  short int
#define int32_t  int

//-----�ܽŶ���
//-----����
#define KEY_1 P22_0
#define KEY_2 P22_1
#define KEY_3 P22_2
#define KEY_4 P22_3
//-----���
#define motor_l_EN       P02_4//��  +
#define motor_l_PN       ATOM0_CH7_P02_7//��
#define motor_r_EN       P02_6//��  +
#define motor_r_PN       ATOM0_CH5_P02_5//��
#define motor_frequency     13000
//-----���
#define steering_gear     ATOM1_CH1_P33_9
#define steering_frequency 50
#define S3010_Middle   595  //�����ֵ
#define S3010_Left     665  //�����
#define S3010_Right    525 //�Ҵ���
//-----������
#define encoder_GPT_l     GPT12_T4
#define encoder_EUD_l     GPT12_T4EUDA_P00_9  //DIR
#define encoder_IN_l      GPT12_T4INA_P02_8   //LSB
#define encoder_GPT_r     GPT12_T2
#define encoder_EUD_r     GPT12_T2EUDB_P33_6  //DIR
#define encoder_IN_r      GPT12_T2INB_P33_7   //LSB
//-----������

//-----������

//-----����
#define uart_blue UART_2
#define VCAN_PORT UART_2
#define i2c_delay     2        //���ζ�ȡ��д���ʱ����,��λMS
#define DATE_LEN 89//�������ݳ���
#define cursor_min    0
#define cursor_max    6
#define menu_min      0
#define menu_max      7
#define AD 3
#define Len 10
#define menu_N        5    //���λ��
#define condition_input  (jianzhi==1||jianzhi==2||jianzhi==3||jianzhi==5||jianzhi==6||jianzhi==7||jianzhi==9||jianzhi==10||jianzhi==11||jianzhi==14)  //��װ����ʱ���ж����ݣ�����д�ɼ򻯺�����
#define cursor_select 70      //�����ʾ��λ��
#define cursor_set    90      //��������ʱ����λ��

#include "SEEKFREE_PRINTF.h"

#include "zf_assert.h"
#include "stdio.h"
#include "math.h"
//�ٷ�ͷ�ļ�
#include "ifxAsclin_reg.h"
#include "SysSe/Bsp/Bsp.h"
#include "IfxCcu6_Timer.h"
#include "IfxScuEru.h"

//------��ɿƼ���Ƭ����������ͷ�ļ�
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

//------��ɿƼ���Ʒ����ͷ�ļ�
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

//--------�ҵ�ͷ�ļ�
//#include "ZZK_Threshold.h"
#include "IMG_DEAL.h"
#include "inti.h"
#include "motor.h"
#include "Key.h"
#include "VCAN_computer.h"
#include "LCD_drive.h"
#endif

