#ifndef __IMG_DEAL_H__
#define __IMG_DEAL_H__ 
#include "common.h"
#include "ZZK_Threshold.h"

typedef enum
{
    NO_JUGED,
    IN_JUGED,
}ALL_enum;//����Ԫ�ر�־λ

typedef enum
{
    NO_FORK,
    IN_FORK,
}FORK_enum;//����

typedef enum
{
    NO_T,
    IN_T,
}T_CONNER_enum;//T��

typedef enum
{
    GET_OUT,
    READY,
    GET_IN,
    NO_GARAGE,
}Garage_enmum;//����

typedef enum
{
    GET_IN_ROUND,
    GET_OUT_ROUND,
    NO_ROUND,
}Roundabout_enmum;//����
//----------------����������--------------
void Img_Deal(void);
void Deal_Init(void);
void Data_collect_display(void);
//void Img_Send_To_Top( uint8 Image[ROW][COL]); 
void LeftStartFind(void);
void RightStartFind(void);
float Cross_col(void);
extern int v;
   
extern int dj_end;
 
extern int right_flag;
extern int left_flag;
extern int right_start;
extern int left_start;
extern int middle_start;
extern int middle_end;

extern int right_line[120];
extern int left_line[120];
extern int middleline[240];
extern int mid_row[240];
extern int max;
extern int min;
extern int line[120];
extern int weight[120];
extern int wild1;
extern int wild10;
extern float ang,ang_l,ang_r;
extern int L_lenth,R_lenth;
extern float L_S,R_S;
extern float CorssCol;

extern unsigned char stop;
extern unsigned char deal_flag;
extern unsigned char IMG_DATA[ROW][COL];
extern unsigned char fork_flag;
extern unsigned char fork_turn;
#endif 
