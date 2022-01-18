#ifndef __IMG_DEAL_H__
#define __IMG_DEAL_H__ 
#include "common.h"
#include "ZZK_Threshold.h"

#define COL 160
#define ROW 120


//----------------函数定义区--------------
void Img_Deal(void);
void Deal_Init(void);
void Get_Center_Line(void);
void Middle_Show(void);
void Data_collect_display(void);
//void Img_Send_To_Top( uint8 Image[ROW][COL]); 
void LeftStartFind(void);
void RightStartFind(void);

extern int v;
   
extern int dj_end;

 
extern int right_flag;
extern int left_flag;
extern int right_start;
extern int left_start;
extern int right_end;
extern int left_end;
extern int middle_start;
extern int middle_end;

extern int right_line[120];
extern int left_line[120];
extern int middleline[240];
extern int max;
extern int min;
extern int line[120];
extern int weight[240];

extern unsigned char stop;
#endif 
