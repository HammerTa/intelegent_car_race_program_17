#include "headfile.h"


struct STR   //定义的结构体，用于存放边界信息
{
  uint8 cam_i;   //存放边界的行
  uint8 cam_j;   //存放边界的列
};

uint8 cam_static=90;//当赛道出界，或者计算的灰度阈值异常时使用
uint8 Threshold0=90;
uint8 cam_thr_mid=0,cam_thr_max=0,cam_thr_min=0;
uint8 cam_thr=90,cam_thr0=100;//摄像头的阈值
uint8 HistoGram[256]; //在进行大津的计算阈值时用于统计赛道指定区域的各个阈值的个数数组
int LQ_ary[256];
uint8 cam_strong_light=0;//强光标志位
uint8 cam_normal_light=0;//正常光标志位
uint8 cam_wesk_light=0;  //弱光标志位
int C_W=159;


uint8 GetOSTU(uint8 tmImage[ROW][COL])
{ 
    int i,j;
    uint32_t Amount = 0;
    uint32_t PixelBack = 0;
    uint32_t PixelIntegralBack = 0;
    uint32_t PixelIntegral = 0;
    int32_t PixelIntegralFore = 0;
    int32_t PixelFore = 0;
    double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
    int16_t MinValue, MaxValue;
    uint8_t Threshold = 0;
    uint8_t HistoGram[256];              //

    for (j = 0; j < 256; j++)  HistoGram[j] = 0; //初始化灰度直方图

    for (j =0; j < ROW; j++)
    {
        for (i = 0; i < COL; i++)
        {
            HistoGram[tmImage[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

    if (MaxValue == MinValue)     return (uint8_t)MaxValue;         // 图像中只有一个颜色
    if (MinValue + 1 == MaxValue)  return (uint8_t)MinValue;        // 图像中只有二个颜色

    for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  像素总数

    PixelIntegral = 0;
    for (j = MinValue; j <= MaxValue; j++)
    {
        PixelIntegral += HistoGram[j] * j;//灰度值总数
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++)
    {
        PixelBack = PixelBack + HistoGram[j];    //前景像素点数
        PixelFore = Amount - PixelBack;         //背景像素点数
        OmegaBack = (double)PixelBack / Amount;//前景像素百分比
        OmegaFore = (double)PixelFore / Amount;//背景像素百分比
        PixelIntegralBack += HistoGram[j] * j;  //前景灰度值
        PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
        MicroBack = (double)PixelIntegralBack / PixelBack;   //前景灰度百分比
        MicroFore = (double)PixelIntegralFore / PixelFore;   //背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//计算类间方差
        if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
        {
            SigmaB = Sigma;
            Threshold = (uint8_t)j;
        }
    }
    return Threshold;
}
