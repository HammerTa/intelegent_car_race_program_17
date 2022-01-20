/*
 * @Author: your name
 * @Date: 2022-01-13 21:37:27
 * @LastEditTime: 2022-01-18 21:25:07
 * @LastEditors: your name
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \NationalRace_Program\CODE\inti.c
 */
/*
 * inti.c
 *
 *  Created on: 2022年1月13日
 *      Author: HammerTank
 *      初始化函数
 */
#include "headfile.h"

///***************************************************************
//* 函数名称： inti_all
//* 功能说明： 全部外设初始化函数
//* 函数返回： 无
//* 备 注：    我知道应该是Init了，别骂了别骂了，孩子知道错了
//***************************************************************
void inti_all()
{
    systick_delay_ms(STM0,1000);//等待设备上电
    lcd_init();         //液晶初始化
    //mt9v03x_init();     //摄像头初始化
    //lcd_showstr(0,0,"Cm init done");
    steering_init();    //舵机初始化
    lcd_showstr(0,0,"St init done");
    motor_init();       //电机初始化
    lcd_showstr(0,1,"Mt init done");
    Count_init();       //编码器初始化
    lcd_showstr(0,2,"Ct init done");
    systick_delay_ms(STM0,1000);
    lcd_showstr(0,3,"Ready to go");
}



