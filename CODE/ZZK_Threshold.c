#include "ZZK_Threshold.h"

struct STR   //定义的结构体，用于存放边界信息
{
  uint8 cam_i;   //存放边界的行
  uint8 cam_j;   //存放边界的列
};

uint8 cam_static=90;//当赛道出界，或者计算的灰度阈值异常时使用
uint8 Threshold0=90;
uint8 cam_thr_mid=0,cam_thr_max=0,cam_thr_min=0;
uint8 cam_thr=90,cam_thr0=100;//摄像头的阈值
uint8_t HistoGram[256]; //在进行大津的计算阈值时用于统计赛道指定区域的各个阈值的个数数组
int LQ_ary[256];

uint8 cam_strong_light=0;//强光标志位
uint8 cam_normal_light=0;//正常光标志位
uint8 cam_wesk_light=0;  //弱光标志位
int C_W=159;


//***************************************************************
//* 函数名称：uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW])
//* 功能说明：求阈值大小
//* 参数说明：有三个输入变量，第一个是指针P，指向待处理的图像的第一个位置，指针为全局变量，
//*           还有两个宏定义LQOSTU_MIN02，LQOSTU_MAX02 ，他们分别表示要计算阈值的区域的大小
//* 函数返回：阈值大小 cam_thr
//* 修改时间：2018年3月27日
//*           第一次修改
//* 备 注：
//Ostu方法又名最大类间差方法，通过统计整个图像的直方图特性来实现全局阈值T的自动选取，其算法步骤为：
//1) 先计算图像的直方图，即将图像所有的像素点按照0~255共256个bin，统计落在每个bin的像素点数量
//2) 归一化直方图，也即将每个bin中像素点数量除以总的像素点
//3) i表示分类的阈值，也即一个灰度级，从0开始迭代
//4) 通过归一化的直方图，统计0~i 灰度级的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例w0，并统计前景像素的平均灰度u0；统计i~255灰度级的像素(假设像素值在此范围的像素叫做背景像素) 所占整幅图像的比例w1，并统计背景像素的平均灰度u1；
//5) 计算前景像素和背景像素的方差 g = w0*w1*(u0-u1) (u0-u1)
//6) i++；转到4)，直到i为256时结束迭代
//7）将最大g相应的i值作为图像的全局阈值
//缺陷:OSTU算法在处理光照不均匀的图像的时候，效果会明显不好，因为利用的是全局像素信息。
//******************************************************************************
uint8_t zzkLQGetOSTU0200(uint8 LQOSTU_MIN,uint8 LQOSTU_MAX)
{ 
  int16_t j;
  uint32_t Amount = 0;
  uint32_t PixelBack = 0;
  uint32_t PixelIntegralBack = 0;
  uint32_t PixelIntegral = 0;
  int32_t PixelIntegralFore = 0;
  int32_t PixelFore = 0;
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
  int16_t MinValue, MaxValue;
  uint8_t Threshold = 0;             //
  uint8 *tmImagep;
  uint8 *imgp;
  int tmImage_size=C_W*(LQOSTU_MAX-LQOSTU_MIN+1)/3-1;
  imgp=(uint8_t *)IMG_DATA;
  tmImagep=imgp+C_W*LQOSTU_MIN;
  for (j = 0; j < 256; j++)
  {
    HistoGram[j] = 0; //初始化灰度直方图
    LQ_ary[j]=0;
  }
  while(tmImage_size)
  {
    HistoGram[*tmImagep]++; //统计灰度级中每个像素在整幅图像中的个数
    tmImagep=tmImagep+3;
    tmImage_size--;
  }

  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

  if (MaxValue == MinValue)
    return MaxValue;         // 图像中只有一个颜色
  if (MinValue + 1 == MaxValue)
    return MinValue;        // 图像中只有二个颜色

  for (j = MinValue; j <= MaxValue; j++)
    Amount += HistoGram[j];        //  像素总数  256

  PixelIntegral = 0;
  for (j = MinValue; j <= MaxValue; j++)
  {
    PixelIntegral += HistoGram[j] * j;//灰度值总数  256
  }
  SigmaB = -1;
  //  str.Format("%d",MinValue);AfxMessageBox(str);
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
      Threshold = j;
    }
  }
  //============求阈值左右像素点的比例，防止出现部分强光现象=========
  int LQ_cam_jishu,LQ_black=0,LQ_white=0;
  float LQ_thr_rate=0;
  int LQGTU_jishu;
  cam_thr_min=1;
  for(LQGTU_jishu=2;LQGTU_jishu<Threshold;LQGTU_jishu++)
  {
    LQ_black=LQ_black+HistoGram[LQGTU_jishu];//统计小于阈值的黑点个数
    if(HistoGram[LQGTU_jishu]>HistoGram[cam_thr_min])
      cam_thr_min=LQGTU_jishu;//找到小于阈值的一个峰值
  }
  cam_thr_max=Threshold;
  for(LQGTU_jishu=Threshold+1;LQGTU_jishu<256;LQGTU_jishu++)
  {
    LQ_white=LQ_white+HistoGram[LQGTU_jishu];//统计大于阈值的白点的个数
    if(HistoGram[LQGTU_jishu]>HistoGram[cam_thr_max])
      cam_thr_max=LQGTU_jishu;//找到大于阈值的一个峰值
  }
  LQ_thr_rate=LQ_black*1.0/LQ_white;//统计黑点与白点的比例
  int LQ_G_white=0;
  for(LQGTU_jishu=LQOSTU_QIANGGUANG;LQGTU_jishu<256;LQGTU_jishu++)
  {
    LQ_G_white=LQ_G_white+HistoGram[LQGTU_jishu];//统计亮度大于200的点数
  }
  //str.Format("%d",LQ_G_white);AfxMessageBox(str);
  if(LQ_G_white>LQOSTU_QIANGNUM&&0)//亮度大于200的个数大于2000，强光散射
  {
    cam_strong_light=1;//强光标志位
    cam_thr_mid=(cam_thr_max-cam_thr_min)/LQOSTU_THRERR;
  }
  else if(LQ_thr_rate>3&&0)
  {
    cam_wesk_light=1;  //弱光标志位
    for(LQGTU_jishu=3;LQGTU_jishu<254;LQGTU_jishu++)//滤波
    {
      LQ_ary[LQGTU_jishu]=(HistoGram[LQGTU_jishu-2]+HistoGram[LQGTU_jishu-1]+HistoGram[LQGTU_jishu]+HistoGram[LQGTU_jishu+1]+HistoGram[LQGTU_jishu+2])/5;
    }
    for(LQGTU_jishu=cam_thr_min-3;LQGTU_jishu>3;LQGTU_jishu--)
    {
      if(LQ_ary[LQGTU_jishu]>LQ_ary[LQGTU_jishu+1]&&LQ_ary[LQGTU_jishu+1]>LQ_ary[LQGTU_jishu+2])
        break;
    }
    int LQ_max,LQ_min=LQGTU_jishu;
    for(;LQGTU_jishu<cam_thr_min;LQGTU_jishu++)//计算信新的阈值
    {
      if(HistoGram[LQGTU_jishu]<HistoGram[LQ_min])
        LQ_min=LQGTU_jishu;
    }
    Threshold=LQ_min+10;
    cam_thr_max=cam_thr_min;
    //    str.Format("%d",Threshold);AfxMessageBox(str);
    LQ_max=2;
    for(LQGTU_jishu=2;LQGTU_jishu<Threshold;LQGTU_jishu++)
    {
      if(HistoGram[LQGTU_jishu]>HistoGram[LQ_max])
        LQ_max=LQGTU_jishu;
    }
    // str.Format("%d",Threshold);AfxMessageBox(str);
    cam_thr_min=cam_thr_max;
    cam_thr_mid=(cam_thr_max-cam_thr_min)/LQOSTU_THRERR;
  }
  else
  {
      if(Threshold0-Threshold>20)
          Threshold=Threshold0-20;
      else if(Threshold0-Threshold<-20)
          Threshold=Threshold0+20;
    //  str.Format("%d",cam_thr);AfxMessageBox(str);
    //  cam_thr=200;
    cam_normal_light=1;//正常光标志位
//  Threshold=Threshold-5;
    cam_thr_mid=(cam_thr_max-cam_thr_min)/LQOSTU_THRERR;
  }
  //=================================================================
  Threshold0=Threshold;
  return Threshold;                        //返回最佳阈值;
}

//***************************************************************
//* 函数名称：uint8_t zzkLQGetOSTU0200_col(uint8_t tmImage[IMAGEH][IMAGEW])
//* 功能说明：按列求阈值大小
//******************************************************************************
uint8_t zzkLQGetOSTU0200_col(uint8 LQOSTU_MIN,uint8 LQOSTU_MAX,uint8 lie_min,uint8 lie_max)
{
  int16 i,j;
  uint32 Amount = 0;
  uint32 PixelBack = 0;
  uint32 PixelIntegralBack = 0;
  uint32 PixelIntegral = 0;
  int32 PixelIntegralFore = 0;
  int32 PixelFore = 0;
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
  int16 MinValue, MaxValue;
  uint8 Threshold = 0;             //
  uint8 *tmImagep;
  uint8 *imgp;
  imgp=(uint8 *)IMG_DATA;
  tmImagep=imgp+C_W*LQOSTU_MIN+lie_min;
  //int tmImage_size=(lie_max-lie_min+1)*(LQOSTU_MAX-LQOSTU_MIN+1)/3-(lie_max-lie_min+1)*(LQOSTU_MAX-LQOSTU_MIN+1)%3;
  for (j = 0; j < 256; j++)
  {
    HistoGram[j] = 0; //初始化灰度直方图
    LQ_ary[j]=0;
  }
  // while(tmImage_size)
  // {
    // HistoGram[*tmImagep]++; //统计灰度级中每个像素在整幅图像中的个数
    // tmImagep=tmImagep+3;
    // if(tmImagep%C_W>=lie_max)
        // while(tmImagep%C_W<lie_min)
            // tmImagep=tmImagep+3;
    // tmImage_size--;
  // }
  for(i=LQOSTU_MIN;i<LQOSTU_MAX;i++)
  {
      for(j=lie_min;j<lie_max;j+=3)
      {
         // tmImagep=img[i][j];
          HistoGram[IMG_DATA[i][j]]++;
      }
  }

  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

  if (MaxValue == MinValue)
    return MaxValue;         // 图像中只有一个颜色
  if (MinValue + 1 == MaxValue)
    return MinValue;        // 图像中只有二个颜色

  for (j = MinValue; j <= MaxValue; j++)
    Amount += HistoGram[j];        //  像素总数  256

  PixelIntegral = 0;
  for (j = MinValue; j <= MaxValue; j++)
  {
    PixelIntegral += HistoGram[j] * j;//灰度值总数  256
  }
  SigmaB = -1;
  //  str.Format("%d",MinValue);AfxMessageBox(str);
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
      Threshold = j;
    }
  }
  //=================================================================
  Threshold0=Threshold;
  return Threshold;                        //返回最佳阈值;
}
