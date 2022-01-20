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
#define key1 P22_0
#define key2 P22_1
#define key3 P22_2
#define key4 P22_3
//-----���
#define motor_l_0       ATOM0_CH0_P21_2//��  +
#define motor_l_1       ATOM0_CH1_P21_3//��  -
#define motor_r_0       ATOM0_CH2_P21_4//��  +
#define motor_r_1       ATOM0_CH3_P21_5//��  -
#define motor_frequency     13000
//-----���
#define steering_gear     ATOM1_CH1_P33_9
#define steering_frequency 50
#define S3010_Middle   965  //965
#define S3010_Left     800  //820
#define S3010_Right    1130 //1130
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
#endif

