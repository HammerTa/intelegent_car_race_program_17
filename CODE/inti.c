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
    lcd_init();         //Һ����ʼ��
    mt9v03x_init();     //����ͷ��ʼ��
    steering_inti();    //�����ʼ��
    motor_inti();       //�����ʼ��
    Count_init();       //��������ʼ��
}



