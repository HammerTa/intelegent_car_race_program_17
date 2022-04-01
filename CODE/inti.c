/*
 * @Author: your name
 * @Date: 2022-01-13 21:37:27
 * @LastEditTime: 2022-04-01 23:31:21
 * @LastEditors: Please set LastEditors
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
    //eeprom_erase_sector(0);//������ȡ��ע�ͣ�������ݼ���
    systick_delay_ms(STM0,1000);//�ȴ��豸�ϵ�
    lcd_init();         //Һ����ʼ��
    mt9v03x_init();     //����ͷ��ʼ��
    lcd_showstr(0,0,"Cm init done");
    steering_init();    //�����ʼ��
    lcd_showstr(0,1,"St init done");
    motor_init();       //�����ʼ��
    lcd_showstr(0,2,"Mt init done");
    Count_init();       //��������ʼ��
    lcd_showstr(0,3,"Ct init done");
    uart_init(UART_2, 115200, UART2_TX_P10_5, UART2_RX_P10_6);
    lcd_showstr(0,4,"UA init done");
    gpio_init(FMQ,GPO,0,PUSHPULL);
    Read_data();        //��ȡ��������
    systick_delay_ms(STM0,1000);
    lcd_showstr(0,5,"Ready to go");
    systick_delay_ms(STM0,1000);
    lcd_clear(WHITE);
}

void PIT_init()
{
    pit_interrupt_ms(CCU6_0, PIT_CH0, 5);//5ms�ж�
}

