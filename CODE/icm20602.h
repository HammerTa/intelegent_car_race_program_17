/*
 * icm20602.h
 *
 *  Created on: 2022Äê7ÔÂ9ÈÕ
 *      Author: HammerTank
 */

#ifndef CODE_ICM20602_H_
#define CODE_ICM20602_H_
#include "SEEKFREE_ICM20602.h"

typedef struct
{
    int16 x;
    int16 y;
    int16 z;
} INT16_XYZ;

typedef struct
{
    float x;
    float y;
    float z;
} FLOAT_XYZ;

typedef struct
{
    float pit;
    float roll;
    float yaw;
} FLOAT_EULER;

typedef struct
{
    int32 x;
    int32 y;
    int32 z;

} INT32_XYZ ;

typedef struct
{
    INT16_XYZ gyro;
    INT16_XYZ acc;
} GYRO_ORIG;

typedef struct
{
    FLOAT_XYZ gyro_fiter;
    FLOAT_XYZ acc_fiter;
    INT16_XYZ gyro_offset;
    INT16_XYZ acc_offset;
    uint8     offset_flag;
} GYRO_FITER;

typedef struct
{
    INT32_XYZ tar_ang_vel;
    INT32_XYZ tar_ang_vel_last;
    INT32_XYZ tar_ang;
} GYRO_TAR_ANG;

typedef struct
{
    GYRO_TAR_ANG target;
    GYRO_ORIG    orig;
    GYRO_FITER   fiter;
    INT16_XYZ    newTd;
    INT16_XYZ    acc_res;
    INT16_XYZ    gyro_res;
    FLOAT_EULER    euler;

} GYRO_VAR;

typedef struct
{
    uint8 cnt;
    int32 lst_pow_sum;
    int32 now_out;
    int32 lst_out;
    int32 now_velocity_xdt;
} _steepest_st;

typedef struct
{
    float x;
    float y;
    float z;
} _xyz_f_st;

typedef struct
{
    float lpf_1;

    float out;
}_lf_t;
extern float yaw,pit,roll;
extern GYRO_VAR gyroscope;
#define M_PI_F 3.141592653589793f
#define ABS(x) ( (x)>0?(x):-(x) )//????
#define my_pow(a) ((a)*(a))
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
float my_sqrt(float number);
void IMU_offset(GYRO_VAR *gyro_var);
void Q_IMUupdata(GYRO_VAR *gyro_var);
void Data_steepest(GYRO_VAR *gyro_var);
void steepest_descend(int32 arr[],uint8 len,_steepest_st *steepest,uint8 step_num,int32 in);
void IMU_offset(GYRO_VAR *gyro_var);
void IMU_quaterToEulerianAngles(void);




#endif /* CODE_ICM20602_H_ */
