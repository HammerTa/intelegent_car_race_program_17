#include "headfile.h"


uint8 Offset_OK;

S_INT16_XYZ GYRO,ACC;
S_INT16_XYZ GYRO_Offset,ACC_Offset;
S_INT16_XYZ ACC_Real,GYRO_Real;
S_INT16_XYZ Attitude_Angle;

void ICM20602_GetData(S_INT16_XYZ *GYRO, S_INT16_XYZ *ACC)
{
    get_icm20602_accdata_spi();
    get_icm20602_gyro_spi();
    if (Offset_OK)
    {
        ACC->X = icm_acc_x;   // 获取加速度计原始数据
        ACC->Y = icm_acc_y;
        ACC->Z = icm_acc_z;
//
        GYRO->X = icm_gyro_x - GYRO_Offset.X;  // 获取陀螺仪原始数据
        GYRO->Y = icm_gyro_y - GYRO_Offset.Y;
        GYRO->Z = icm_gyro_z - GYRO_Offset.Z;
    }
    else
    {
        ACC->X = icm_acc_x;   // 获取加速度计原始数据
        ACC->Y = icm_acc_y;
        ACC->Z = icm_acc_z;
//
        GYRO->X = icm_gyro_x;  // 获取陀螺仪原始数据
        GYRO->Y = icm_gyro_y;
        GYRO->Z = icm_gyro_z;

    }
}


void ICM20602_Offset(void)
{
    uint8 i, Count = 100;
    int64 temp[6] = {0};

    GYRO_Offset.X = 0;
    GYRO_Offset.Y = 0;
    GYRO_Offset.Z = 0;

    for (i = 0; i < Count; i++)
    {
        ICM20602_GetData(&GYRO, &ACC);  // 读取陀螺仪数据
        //DELAY_MS(2);
        systick_delay_ms(STM0,2);
        temp[0] += ACC.X;
        temp[1] += ACC.Y;
        temp[2] += ACC.Z;

        temp[3] += GYRO.X;
        temp[4] += GYRO.Y;
        temp[5] += GYRO.Z;
    }
    ACC_Offset.X = (uint16)temp[0] / Count;
    ACC_Offset.Y = (uint16)temp[1] / Count;
    ACC_Offset.Z = (uint16)temp[2] / Count;

    GYRO_Offset.X = (uint16)temp[3] / Count;
    GYRO_Offset.Y = (uint16)temp[4] / Count;
    GYRO_Offset.Z = (uint16)temp[5] / Count;

    Offset_OK = 1;
}



#define AcceRatio   16384.0f
#define GyroRatio   3.4f
#define Gyro_Gr     0.0010653   // 角速度变成弧度  此参数对应陀螺2000度每秒
#define ACC_FILTER_NUM 5        // 加速度计滤波深度
#define GYRO_FILTER_NUM 2       // 陀螺仪滤波深度
int32 ACC_X_BUF[ACC_FILTER_NUM], ACC_Y_BUF[ACC_FILTER_NUM], ACC_Z_BUF[ACC_FILTER_NUM];  // 滤波缓存数组
int32 GYRO_X_BUF[GYRO_FILTER_NUM], GYRO_Y_BUF[GYRO_FILTER_NUM], GYRO_Z_BUF[GYRO_FILTER_NUM];
/*
 * 函数名：Data_Filter
 * 描述  ：数据滑动滤波
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void Data_Filter(void)  // 数据滤波
{

    uint8 i;
    //float ACC_Angle;
    int64 temp1 = 0, temp2 = 0, temp3 = 0, temp4 = 0, temp5 = 0, temp6 = 0;

    ACC_X_BUF[0] = ACC.X;   // 更新滑动窗口数组
    ACC_Y_BUF[0] = ACC.Y;
    ACC_Z_BUF[0] = ACC.Z;
    GYRO_X_BUF[0] = GYRO.X;
    GYRO_Y_BUF[0] = GYRO.Y;
    GYRO_Z_BUF[0] = GYRO.Z;

    for(i=0;i<ACC_FILTER_NUM;i++)
    {
        temp1 += ACC_X_BUF[i];
        temp2 += ACC_Y_BUF[i];
        temp3 += ACC_Z_BUF[i];

    }
    for(i=0;i<GYRO_FILTER_NUM;i++)
    {
        temp4 += GYRO_X_BUF[i];
        temp5 += GYRO_Y_BUF[i];
        temp6 += GYRO_Z_BUF[i];
    }

    ACC_Real.X = temp1 / 1.0f*ACC_FILTER_NUM / AcceRatio;
    ACC_Real.Y = temp2 / 1.0f*ACC_FILTER_NUM / AcceRatio;
    ACC_Real.Z = temp3 / 1.0f*ACC_FILTER_NUM / AcceRatio;
    GYRO_Real.X = temp4 / 1.0f*GYRO_FILTER_NUM / GyroRatio;
    GYRO_Real.Y = temp5 / 1.0f*GYRO_FILTER_NUM / GyroRatio;
    GYRO_Real.Z = temp6 / 1.0f*GYRO_FILTER_NUM / GyroRatio;

    for(i = 0; i < ACC_FILTER_NUM - 1; i++)
    {
        ACC_X_BUF[ACC_FILTER_NUM-1-i] = ACC_X_BUF[ACC_FILTER_NUM-2-i];
        ACC_Y_BUF[ACC_FILTER_NUM-1-i] = ACC_Y_BUF[ACC_FILTER_NUM-2-i];
        ACC_Z_BUF[ACC_FILTER_NUM-1-i] = ACC_Z_BUF[ACC_FILTER_NUM-2-i];
    }
    for(i = 0; i < GYRO_FILTER_NUM - 1; i++)
    {
        GYRO_X_BUF[GYRO_FILTER_NUM-1-i] = GYRO_X_BUF[GYRO_FILTER_NUM-2-i];
        GYRO_Y_BUF[GYRO_FILTER_NUM-1-i] = GYRO_Y_BUF[GYRO_FILTER_NUM-2-i];
        GYRO_Z_BUF[GYRO_FILTER_NUM-1-i] = GYRO_Z_BUF[GYRO_FILTER_NUM-2-i];
    }
}


void Get_Attitude(void) // 姿态解算
{
    IMUupdate(GYRO_Real.X*Gyro_Gr*GyroRatio,
              GYRO_Real.Y*Gyro_Gr*GyroRatio,
              GYRO_Real.Z*Gyro_Gr*GyroRatio,
              ACC_Real.X * AcceRatio,
              ACC_Real.Y * AcceRatio,
              ACC_Real.Z * AcceRatio);  // 姿态解算出欧拉角
}
//===============================四元素============================================
#define Kp 50.0f //10.0f                // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.2f//1.2f // //0.008f   // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.005f                    // half the sample period采样周期的一半
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;   // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;  // scaled integral error
/*
 * 函数名：IMUupdate
 * 描述  ：四元素解算欧拉角
 * 输入  ：陀螺仪 加速度计
 * 输出  ：无
 * 调用  ：内部调用
 */
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
    float norm;
    float vx, vy, vz;
    float ex, ey, ez;

    // 先把这些用得到的值算好
    float q0q0 = q0*q0;
    float q0q1 = q0*q1;
    float q0q2 = q0*q2;
    float q1q1 = q1*q1;
    float q1q3 = q1*q3;
    float q2q2 = q2*q2;
    float q2q3 = q2*q3;
    float q3q3 = q3*q3;

    if (ax*ay*az == 0)
    {
        return;
    }

    norm = sqrt(ax*ax + ay*ay + az*az); // acc数据归一化
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;

    // estimated direction of gravity and flux (v and w)    估计重力方向和流量/变迁
    vx = 2*(q1q3 - q0q2);                                   // 四元素中xyz的表示
    vy = 2*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3 ;

    // error is sum of cross product between reference direction of fields and direction measured by sensors
    ex = (ay*vz - az*vy) ;      // 向量外积在相减得到差分就是误差
    ey = (az*vx - ax*vz) ;
    ez = (ax*vy - ay*vx) ;

    exInt = exInt + ex * Ki;    // 对误差进行积分
    eyInt = eyInt + ey * Ki;
    ezInt = ezInt + ez * Ki;

    // adjusted gyroscope measurements
    gx = gx + Kp*ex + exInt;    // 将误差PI后补偿到陀螺仪，即补偿零点漂移
    gy = gy + Kp*ey + eyInt;
    gz = gz + Kp*ez + ezInt;    // 这里的gz由于没有观测者进行矫正会产生漂移，表现出来的就是积分自增或自减

    // integrate quaternion rate and normalise  // 四元素的微分方程
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

    // normalise quaternion
    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;

    Attitude_Angle.Y = asin(-2*q1*q3 + 2*q0*q2) * 57.3f; // pitch
    Attitude_Angle.X = atan2(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2 + 1)*57.3f; // roll
    Attitude_Angle.Z = atan2(2*q1*q2 + 2*q0*q3, -2*q2*q2 - 2*q3*q3 + 1)*57.3f; // yaw
//  Attitude_Angle.Z = 0;
}

//一阶互补滤波

int g_fBalance_Gyro = 0;
float g_fBalance_Angle = 0,g_fAccel_Angle = 0;//一阶互补滤波所得角度
void Get_Balance_Angle(void)
{

  uint8 gyro_offset = 1;//静置时角速度的偏移量
  float gyro_dt = 0.005f;//陀螺仪角速度积分系数,增长缓慢就增加
  float Filter_Weight = 0.1f;//滤波权重
  //Filter_Weight = (float)(Weight*0.01);
        get_icm20602_accdata_spi();
        get_icm20602_gyro_spi(); //读取六轴原始数值

  g_fBalance_Gyro = icm_gyro_y - gyro_offset;//原始角速度减去零偏值得到实际角速度
  g_fAccel_Angle = (float)atan2f(icm_acc_x,icm_acc_z) * 57.296f;//两轴加速度求反三角得到加速度角度，乘以57.296,是把弧度转化为度

  //一阶互补滤波核心公式,得到融合角度
  g_fBalance_Angle = Filter_Weight * g_fAccel_Angle + (1-Filter_Weight) * (g_fBalance_Angle - g_fBalance_Gyro * gyro_dt);
}
