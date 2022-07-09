/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2015,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_computer.c
 * @brief      ɽ��๦�ܵ���������λ����������
 * @author     ɽ��Ƽ�
 * @version    v5.2.2
 * @date       2015-03-24
 */


/*
 * ����ͷ�ļ�
 */
#include "common.h"
#include "zf_uart.h"
#include "headfile.h"

int bluetooth[8]={0};
int send_flag=0;
/*!
 *  @brief      ɽ��๦�ܵ���������λ��������ͷ��ʾ����
 *  @param      imgaddr    ͼ����ʼ��ַ
 *  @param      imgsize    ͼ��ռ�ÿռ�Ĵ�С
 *  @since      v5.0
*  Sample usage:
             �����÷��ο�������:
            ��ɽ������ͷ��ӥ����λ�����̺�΢��Ч�� - ���ܳ�������
             http://vcan123.com/forum.php?mod=viewthread&tid=6242&ctid=27
 */
void vcan_sendimg(void *imgaddr, uint32_t imgsize)
{
#define CMD_IMG     1
    uint8_t cmdf[2] = {CMD_IMG, ~CMD_IMG};    //ɽ����λ�� ʹ�õ�����
    uint8_t cmdr[2] = {~CMD_IMG, CMD_IMG};    //ɽ����λ�� ʹ�õ�����

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //�ȷ�������

    uart_putbuff(VCAN_PORT, (uint8_t *)imgaddr, imgsize); //�ٷ���ͼ��

    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //�ȷ�������
}


/*!
 *  @brief      ɽ��๦�ܵ���������λ��������CCD��ʾ����
 *  @param      ccdaddr    CCDͼ����ʼ��ַ
 *  @param      ccdsize    CCDͼ��ռ�ÿռ�Ĵ�С
 *  @since      v5.0
*  Sample usage:
             �����÷��ο�������:
            ��ɽ������CCD����λ����ʾ���̺ͳ���ǲɼ�Ч�� - ���ܳ�������
             http://vcan123.com/forum.php?mod=viewthread&tid=6340&ctid=27
 */
void vcan_sendccd(void *ccdaddr, uint32_t ccdsize)
{
#define CMD_CCD     2
    uint8_t cmdf[2] = {CMD_CCD, ~CMD_CCD};    //��ͷ����
    uint8_t cmdr[2] = {~CMD_CCD, CMD_CCD};    //��β����

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //�ȷ�������

    uart_putbuff(VCAN_PORT, (uint8_t *)ccdaddr, ccdsize); //�ٷ���ͼ��

    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //�ٷ�������
}


/*!
 *  @brief      ɽ��๦�ܵ���������λ��������ʾ������ʾ����
 *  @param      wareaddr    ����������ʼ��ַ
 *  @param      waresize    ��������ռ�ÿռ�Ĵ�С
 *  @since      v5.0
*  Sample usage:
             �����÷��ο�������:
            ��ɽ�����ϡ������Ǻͼ��ٶ� ��λ����ʾ���� - ���ܳ�������
             http://vcan123.com/forum.php?mod=viewthread&tid=6253&ctid=27
 */
void vcan_sendware(void *wareaddr, uint32_t waresize)
{
#define CMD_WARE     3
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //���ڵ��� ʹ�õ�ǰ����
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //���ڵ��� ʹ�õĺ�����

    uart_putbuff(VCAN_PORT, &cmdf, sizeof(cmdf));    //�ȷ���ǰ����
    uart_putbuff(VCAN_PORT, (uint8_t *)wareaddr, waresize);    //��������
    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //���ͺ�����

}

void bluesend (void)
{
   //�������� ɽ��
      bluetooth[0]=(int)speed_l;
      bluetooth[1]=(int)speed_r;
      bluetooth[2]=(int)(speed);
      bluetooth[3]=(int)error;
      bluetooth[4]=(int)setspeed_used;
      bluetooth[5]=(int)0;
      bluetooth[6]=(int)0;
      bluetooth[7]=(int)0;
      vcan_sendware(bluetooth,sizeof(bluetooth));
}
