/*
 * @Author: your name
 * @Date: 2022-01-13 21:37:27
 * @LastEditTime: 2022-01-18 21:25:07
 * @LastEditors: your name
 * @Description: ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \NationalRace_Program\CODE\inti.c
 */
/*
 * inti.c
 *
 *  Created on: 2022��1��13��
 *      Author: HammerTank
 *      ��ʼ������
 */
#include "headfile.h"

///***************************************************************
//* �������ƣ� inti_all
//* ����˵���� ȫ�������ʼ������
//* �������أ� ��
//* �� ע��    ��֪��Ӧ����Init�ˣ������˱����ˣ�����֪������
//***************************************************************
void inti_all()
{
    systick_delay_ms(STM0,1000);//�ȴ��豸�ϵ�
    lcd_init();         //Һ����ʼ��
    //mt9v03x_init();     //����ͷ��ʼ��
    //lcd_showstr(0,0,"Cm init done");
    steering_init();    //�����ʼ��
    lcd_showstr(0,0,"St init done");
    motor_init();       //�����ʼ��
    lcd_showstr(0,1,"Mt init done");
    Count_init();       //��������ʼ��
    lcd_showstr(0,2,"Ct init done");
    systick_delay_ms(STM0,1000);
    lcd_showstr(0,3,"Ready to go");
}



