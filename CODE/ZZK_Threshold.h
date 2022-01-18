#ifndef _ZZK_Threshold_H
#define _ZZK_Threshold_H

#include "headfile.h"

#define LQOSTU_MIN02         ALL_IMG_END   //��򷨼�����ֵʱ����С����
#define LQOSTU_MAX02         (CAMERA_H-1) //��򷨼�����ֵʱ���������
#define LQOSTU_THRERR        3     //��̬�������ظ߶�ȡֵ
#define LQOSTU_QIANGGUANG    200   //ǿ����ֵ��������ֵ��������Ϊ��ǿ��
#define LQOSTU_QIANGNUM      2000   //ǿ���ܵ�������ֵ
//--------��Ǭ��ӵ�--------
#define ROW 120
#define COL 160
//--------end---------
extern uint8 cam_static;//���������磬���߼���ĻҶ���ֵ�쳣ʱʹ��
extern uint8 cam_thr_mid,cam_thr_max,cam_thr_min;
extern uint8 cam_thr,cam_thr0;//����ͷ����ֵ
extern uint8_t HistoGram[256]; //�ڽ��д��ļ�����ֵʱ����ͳ������ָ������ĸ�����ֵ�ĸ�������
extern int LQ_ary[256];

extern uint8 cam_strong_light;//ǿ���־λ
extern uint8 cam_normal_light;//�������־λ
extern uint8 cam_wesk_light;  //�����־λ
uint8 GetOSTU(uint8 tmImage[ROW][COL]);
//----------
#endif