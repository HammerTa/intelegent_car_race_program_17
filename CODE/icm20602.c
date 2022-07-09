/*
 * icm20602.c
 *
 *  Created on: 2022年7月9日
 *      Author: HammerTank
 *      代码来源网络，请自行查阅文章理解
 */

#include "headfile.h"
GYRO_VAR gyroscope;
float yaw,pit,roll;

void icm20602_get_data(GYRO_VAR *gyro_var)
{
    get_icm20602_accdata_spi();
    get_icm20602_gyro_spi();
    if (gyro_var->fiter.offset_flag)
    {
        gyro_var->orig.acc.x = icm_acc_x ;
        gyro_var->orig.acc.y = icm_acc_y ;
        gyro_var->orig.acc.z = icm_acc_z ;

        gyro_var->orig.gyro.x = icm_gyro_x - gyro_var->fiter.gyro_offset.x;
        gyro_var->orig.gyro.y = icm_gyro_y - gyro_var->fiter.gyro_offset.y;
        gyro_var->orig.gyro.z = icm_gyro_z - gyro_var->fiter.gyro_offset.z;
    }
    else
    {
        gyro_var->orig.acc.x = icm_acc_x;
        gyro_var->orig.acc.y = icm_acc_y;
        gyro_var->orig.acc.z = icm_acc_z;

        gyro_var->orig.gyro.x = icm_gyro_x;
        gyro_var->orig.gyro.y = icm_gyro_y;
        gyro_var->orig.gyro.z = icm_gyro_z;
    }
}

void LPF_1(float hz,float time,float in,float *out)
{
    *out += ( 1 / ( 1 + 1 / ( hz *6.28f *time ) ) ) *( in - *out );

}

void limit_filter(float T,float hz,_lf_t *data,float in)
{
    float abs_t;
    LPF_1(hz,T,  in,&(data->lpf_1));
    abs_t = ABS(data->lpf_1);
    data->out = LIMIT(in,-abs_t,abs_t);
}

/*
    陀螺仪采集零偏
*/
#define   OFFSET_COUNT   500
void IMU_offset(GYRO_VAR *gyro_var)
{
    uint32 i;
    int16 temp[6] = {0};

    for (i = 0; i < OFFSET_COUNT; i++)
    {
        //mpu6050_get_data(gyro_var);

        icm20602_get_data(gyro_var);
        //mpu6050_get_data(gyro_var);
        //spi_icm20602_get_data(gyro_var);
        systick_delay_ms(STM1,2);

        temp[0] += gyro_var->orig.acc.x;
        temp[1] += gyro_var->orig.acc.y;
        temp[2] += gyro_var->orig.acc.z;

        temp[3] += gyro_var->orig.gyro.x;
        temp[4] += gyro_var->orig.gyro.y;
        temp[5] += gyro_var->orig.gyro.z;
    }

    gyro_var->fiter.acc_offset.x = temp[0] / OFFSET_COUNT;
    gyro_var->fiter.acc_offset.y = temp[1] / OFFSET_COUNT;
    gyro_var->fiter.acc_offset.z = temp[2] / OFFSET_COUNT;

    gyro_var->fiter.gyro_offset.x = temp[3] / OFFSET_COUNT;
    gyro_var->fiter.gyro_offset.y = temp[4] / OFFSET_COUNT;
    gyro_var->fiter.gyro_offset.z = temp[5] / OFFSET_COUNT;

    //采集完标志
    gyro_var->fiter.offset_flag = 1;
}

//快速计算 Sqrt(x)
float my_sqrt(float number)
{
    long i;
    float x, y;
    const float f = 1.5F;
    x = number * 0.5F;
    y = number;
    i = * ( long * ) &y;
    i = 0x5f3759df - ( i >> 1 );

    y = * ( float * ) &i;
    y = y * ( f - ( x * y * y ) );
    y = y * ( f - ( x * y * y ) );
    return number * y;
}
/*
梯度下降滤波
*/

void steepest_descend(int32 arr[],uint8 len,_steepest_st *steepest,uint8 step_num,int32 in)
{
    uint8 updw = 1;//0 dw,1up
    int16 i;
    uint8 step_cnt=0;
    uint8 step_slope_factor=1;
    uint8 on = 1;
    int8 pn = 1;
    //float last = 0;
    float step = 0;
    int32 start_point = 0;
    int32 pow_sum = 0;

    steepest->lst_out = steepest->now_out;

    if( ++(steepest->cnt) >= len )
    {
        (steepest->cnt) = 0; //now
    }

    //last = arr[ (steepest->cnt) ];

    arr[ (steepest->cnt) ] = in;

    step = (float)(in - steepest->lst_out)/step_num ;//梯度

    if(ABS(step)<1)//整形数据<1的有效判定
    {
        if(ABS(step)*step_num<2)
        {
            step = 0;
        }
        else
        {
          step = (step > 0) ? 1 : -1.0;
        }
    }

    start_point = steepest->lst_out;
    do
    {
        //start_point = steepest->lst_out;
        for(i=0;i<len;i++)
        {
//          j = steepest->cnt + i + 1;
//          if( j >= len )
//          {
//              j = j - len; //顺序排列
//          }
            pow_sum += my_pow(arr[i] - start_point );// /step_num;//除法减小比例**

            //start_point += pn *(step_slope_factor *step/len);
        }

        if(pow_sum - steepest->lst_pow_sum > 0)
        {
            if(updw==0)
            {
                on = 0;
            }
            updw = 1;//上升了
            pn = (pn == 1 )? -1:1;

        }
        else
        {
            updw = 0; //正在下降
            if(step_slope_factor<step_num)
            {
                step_slope_factor++;
            }
        }

        steepest->lst_pow_sum = pow_sum;
        pow_sum = 0;
        start_point += pn *step;//调整

        if(++step_cnt > step_num)//限制计算次数
        {
            on = 0;
        }
            //
            if(step_slope_factor>=2)//限制下降次数1次，节省时间，但会增大滞后，若cpu时间充裕可不用。
            {
                on = 0;

            }
            //

    }
    while(on==1);

    steepest->now_out = start_point ;//0.5f *(start_point + steepest->lst_out);//

    steepest->now_velocity_xdt = steepest->now_out - steepest->lst_out;
}



#define MPU_WINDOW_NUM 5
#define MPU_STEEPEST_NUM 5

#define MPU_WINDOW_NUM_ACC 15
#define MPU_STEEPEST_NUM_ACC 15


_steepest_st steepest_ax;
_steepest_st steepest_ay;
_steepest_st steepest_az;
_steepest_st steepest_gx;
_steepest_st steepest_gy;
_steepest_st steepest_gz;

int32 steepest_ax_arr[MPU_WINDOW_NUM_ACC ];
int32 steepest_ay_arr[MPU_WINDOW_NUM_ACC ];
int32 steepest_az_arr[MPU_WINDOW_NUM_ACC ];
int32 steepest_gx_arr[MPU_WINDOW_NUM ];
int32 steepest_gy_arr[MPU_WINDOW_NUM ];
int32 steepest_gz_arr[MPU_WINDOW_NUM ];
float values[6];
void Data_steepest(GYRO_VAR *gyro_var)
{
   // icm20602_get_data(&gyroscope);
    steepest_descend(steepest_ax_arr ,MPU_WINDOW_NUM_ACC ,&steepest_ax ,MPU_STEEPEST_NUM_ACC,(int32)gyro_var->orig.acc.x);
    steepest_descend(steepest_ay_arr ,MPU_WINDOW_NUM_ACC ,&steepest_ay ,MPU_STEEPEST_NUM_ACC,(int32) gyro_var->orig.acc.y);
    steepest_descend(steepest_az_arr ,MPU_WINDOW_NUM_ACC ,&steepest_az ,MPU_STEEPEST_NUM_ACC,(int32) gyro_var->orig.acc.z);
    steepest_descend(steepest_gx_arr ,MPU_WINDOW_NUM ,&steepest_gx ,MPU_STEEPEST_NUM,(int32) gyro_var->orig.gyro.x);
    steepest_descend(steepest_gy_arr ,MPU_WINDOW_NUM ,&steepest_gy ,MPU_STEEPEST_NUM,(int32) gyro_var->orig.gyro.y);
    steepest_descend(steepest_gz_arr ,MPU_WINDOW_NUM ,&steepest_gz ,MPU_STEEPEST_NUM,(int32) gyro_var->orig.gyro.z);

    gyro_var->acc_res.z = (int16)steepest_az.now_out;
    gyro_var->acc_res.y = (int16)steepest_ay.now_out;
    gyro_var->acc_res.x = (int16)steepest_ax.now_out;
    gyro_var->fiter.acc_fiter.x =   steepest_ax.now_out *2.3926f;
    gyro_var->fiter.acc_fiter.y =   steepest_ay.now_out *2.3926f;
    gyro_var->fiter.acc_fiter.z =   steepest_az.now_out *2.3926f;

    gyro_var->gyro_res.x = (int16)steepest_gx.now_out;
    gyro_var->gyro_res.y = (int16)steepest_gy.now_out;
    gyro_var->gyro_res.z = (int16)steepest_gz.now_out;

    gyro_var->fiter.gyro_fiter.x = gyro_var->gyro_res.x *0.0610f;
    gyro_var->fiter.gyro_fiter.y = gyro_var->gyro_res.y *0.0610f;
    gyro_var->fiter.gyro_fiter.z = gyro_var->gyro_res.z *0.0610f;


     values[0]=gyro_var->fiter.acc_fiter.x;
     values[1]= gyro_var->fiter.acc_fiter.y ;
     values[2]=gyro_var->fiter.acc_fiter.z;
     values[3]=gyro_var->fiter.gyro_fiter.x ;
     values[4]=gyro_var->fiter.gyro_fiter.y ;
     values[5]=gyro_var->fiter.gyro_fiter.z;

}


/*
    四元素解算
*/
#define RAD_PER_DEG     0.017453293f
#define Kp  110.0f //35.65f10.0f                // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki  0.005f //0.008f //0.005f    // integral gain governs rate of convergence of gyroscope biases
// 需要根据具体姿态更新周期来调整，T是姿态更新周期，T*角速度=微分角度
#define halfT 0.005f//0.005//0.00259  // half the sample period采样周期的一半

#define dT  0.01f

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;   // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;  // scaled integral error

_xyz_f_st vec_err_i;
    _xyz_f_st x_vec;
    _xyz_f_st y_vec;
    _xyz_f_st z_vec;
    _xyz_f_st a_acc;
    _xyz_f_st w_acc;

_lf_t err_lf_x;
_lf_t err_lf_y;
_lf_t err_lf_z;

void Q_IMUupdata(GYRO_VAR *gyro_var)
{
     //Data_steepest(&gyroscope);

    static float q0q1,q0q2,q1q1,q1q3,q2q2,q2q3,q3q3,q1q2,q0q3;
    float acc_length,q_length;
    float w_q,x_q,y_q,z_q;
    _xyz_f_st acc_norm;
    _xyz_f_st vec_err;
    static _xyz_f_st d_angle;


    w_q = q0;
    x_q = q1;
    y_q = q2;
    z_q = q3;

    // 先把这些用得到的值算好 减少计算量
    q0q1 = w_q * x_q;
    q0q2 = w_q * y_q;
    q1q1 = x_q * x_q;
    q1q3 = x_q * z_q;
    q2q2 = y_q * y_q;
    q2q3 = y_q * z_q;
    q3q3 = z_q * z_q;
    q1q2 = x_q * y_q;
    q0q3 = w_q * z_q;
    // normalise the measurements
    // 规范化测量(acc数据归一化)
    acc_length = my_sqrt(my_pow(gyro_var->fiter.acc_fiter.x) +
                   my_pow(gyro_var->fiter.acc_fiter.y) +
                   my_pow(gyro_var->fiter.acc_fiter.z));

    acc_norm.x = gyro_var->fiter.acc_fiter.x/acc_length;
    acc_norm.y = gyro_var->fiter.acc_fiter.y/acc_length;
    acc_norm.z = gyro_var->fiter.acc_fiter.z/acc_length;

    // estimated direction of gravity
    // 估计重力方向、流量
    x_vec.x = 1 - (2*q2q2 + 2*q3q3);
    x_vec.y = 2*q1q2 - 2*q0q3;
    x_vec.z = 2*q1q3 + 2*q0q2;

    y_vec.x = 2*q1q2 + 2*q0q3;
    y_vec.y = 1 - (2*q1q1 + 2*q3q3);
    y_vec.z = 2*q2q3 - 2*q0q1;

    z_vec.x = 2*q1q3 - 2*q0q2;
    z_vec.y = 2*q2q3 + 2*q0q1;
    z_vec.z = 1 - (2*q1q1 + 2*q2q2);

    // 计算载体坐标下的运动加速度。(与姿态解算无关)
    a_acc.x = gyro_var->fiter.acc_fiter.x - 9800 *z_vec.x;
    a_acc.y = gyro_var->fiter.acc_fiter.y - 9800 *z_vec.y;
    a_acc.z = gyro_var->fiter.acc_fiter.z - 9800 *z_vec.z;
    // 计算世界坐标下的运动加速度。(与姿态解算无关)
    w_acc.x = x_vec.x *a_acc.x + x_vec.y *a_acc.y + x_vec.z *a_acc.z;
    w_acc.y = y_vec.x *a_acc.x + y_vec.y *a_acc.y + y_vec.z *a_acc.z;
    w_acc.z = z_vec.x *a_acc.x + z_vec.y *a_acc.y + z_vec.z *a_acc.z;

    // 测量值与等效重力向量的叉积（计算向量误差）。
    vec_err.x =  (acc_norm.y * z_vec.z - z_vec.y * acc_norm.z);
    vec_err.y = -(acc_norm.x * z_vec.z - z_vec.x * acc_norm.z);
    vec_err.z = -(acc_norm.y * z_vec.x - z_vec.y * acc_norm.x);

    //截止频率1hz的低通限幅滤波
    limit_filter(dT,0.2f,&err_lf_x,vec_err.x);
    limit_filter(dT,0.2f,&err_lf_y,vec_err.y);
    limit_filter(dT,0.2f,&err_lf_z,vec_err.z);

    //误差积分
    vec_err_i.x += err_lf_x.out *dT *Ki;
    vec_err_i.y += err_lf_y.out *dT *Ki;
    vec_err_i.z += err_lf_z.out *dT *Ki;

     // 构造增量旋转（含融合纠正）。
    d_angle.x = (gyro_var->fiter.gyro_fiter.x *RAD_PER_DEG + (err_lf_x.out + vec_err_i.x) * Kp) * dT / 2 ;
    d_angle.y = (gyro_var->fiter.gyro_fiter.y *RAD_PER_DEG + (err_lf_y.out + vec_err_i.y) * Kp) * dT / 2 ;
    d_angle.z = (gyro_var->fiter.gyro_fiter.z *RAD_PER_DEG + (err_lf_z.out + vec_err_i.z) * Kp) * dT / 2 ;

     // 计算姿态。
    q0 = w_q - x_q*d_angle.x - y_q*d_angle.y - z_q*d_angle.z;
    q1 = w_q*d_angle.x + x_q           + y_q*d_angle.z - z_q*d_angle.y;
    q2 = w_q*d_angle.y - x_q*d_angle.z + y_q           + z_q*d_angle.x;
    q3 = w_q*d_angle.z + x_q*d_angle.y - y_q*d_angle.x + z_q;

    q_length = my_sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    // ormalise quaternion
    q0 /= q_length;
    q1 /= q_length;
    q2 /= q_length;
    q3 /= q_length;

    // 欧拉角转换
    gyro_var->euler.pit = (asin(2*q1q3 - 2*q0q2))*57.3f;
    gyro_var->euler.roll = (atan2(2*q2q3 + 2*q0q1, -2*q1q1-2*q2q2 + 1))*57.3f;
    gyro_var->euler.yaw = -(atan2(2*q1q2 + 2*q0q3, -2*q2q2-2*q3q3+1))*57.3f;
    yaw=gyro_var->euler.yaw;
    pit=gyro_var->euler.pit;
    roll=gyro_var->euler.roll;
}

float invSqrt(float x) {
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}


void IMU_quaterToEulerianAngles(void)
{
    icm20602_get_data(&gyroscope);
   Data_steepest(&gyroscope);
   Q_IMUupdata(&gyroscope);
  // delta_deal();
}

