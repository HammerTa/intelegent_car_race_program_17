#ifndef _gyroscope_h
#define _gyroscope_h

#include "SEEKFREE_ICM20602.h"

typedef struct
{
    int16 X;
    int16 Y;
    int16 Z;
}S_INT16_XYZ;
extern float g_fBalance_Angle;
extern S_INT16_XYZ Attitude_Angle;
extern S_INT16_XYZ GYRO,ACC;
extern void Get_Attitude(void);
extern void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);
extern void Data_Filter(void);  // Êý¾ÝÂË²¨
extern void ICM20602_GetData(S_INT16_XYZ *GYRO, S_INT16_XYZ *ACC);
extern void Get_Balance_Angle(void);
#endif
