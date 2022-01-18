#ifndef _ZZK_Threshold_H
#define _ZZK_Threshold_H

#include "headfile.h"

#define LQOSTU_MIN02         ALL_IMG_END   //大津法计算阈值时的最小行数
#define LQOSTU_MAX02         (CAMERA_H-1) //大津法计算阈值时的最大行数
#define LQOSTU_THRERR        3     //动态，跳变沿高度取值
#define LQOSTU_QIANGGUANG    200   //强光阈值，大于阈值的亮度认为是强光
#define LQOSTU_QIANGNUM      2000   //强光总点数的阈值
//--------九乾添加的--------
#define ROW 120
#define COL 160
//--------end---------
extern uint8 cam_static;//当赛道出界，或者计算的灰度阈值异常时使用
extern uint8 cam_thr_mid,cam_thr_max,cam_thr_min;
extern uint8 cam_thr,cam_thr0;//摄像头的阈值
extern uint8_t HistoGram[256]; //在进行大津的计算阈值时用于统计赛道指定区域的各个阈值的个数数组
extern int LQ_ary[256];

extern uint8 cam_strong_light;//强光标志位
extern uint8 cam_normal_light;//正常光标志位
extern uint8 cam_wesk_light;  //弱光标志位
uint8 GetOSTU(uint8 tmImage[ROW][COL]);
//----------
#endif