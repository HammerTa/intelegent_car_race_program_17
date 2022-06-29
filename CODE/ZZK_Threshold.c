#include "ZZK_Threshold.h"

struct STR   //����Ľṹ�壬���ڴ�ű߽���Ϣ
{
  uint8 cam_i;   //��ű߽����
  uint8 cam_j;   //��ű߽����
};

uint8 cam_static=90;//���������磬���߼���ĻҶ���ֵ�쳣ʱʹ��
uint8 Threshold0=90;
uint8 cam_thr_mid=0,cam_thr_max=0,cam_thr_min=0;
uint8 cam_thr=90,cam_thr0=100;//����ͷ����ֵ
uint8_t HistoGram[256]; //�ڽ��д��ļ�����ֵʱ����ͳ������ָ������ĸ�����ֵ�ĸ�������
int LQ_ary[256];

uint8 cam_strong_light=0;//ǿ���־λ
uint8 cam_normal_light=0;//�������־λ
uint8 cam_wesk_light=0;  //�����־λ
int C_W=159;


//***************************************************************
//* �������ƣ�uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW])
//* ����˵��������ֵ��С
//* ����˵���������������������һ����ָ��P��ָ��������ͼ��ĵ�һ��λ�ã�ָ��Ϊȫ�ֱ�����
//*           ���������궨��LQOSTU_MIN02��LQOSTU_MAX02 �����Ƿֱ��ʾҪ������ֵ������Ĵ�С
//* �������أ���ֵ��С cam_thr
//* �޸�ʱ�䣺2018��3��27��
//*           ��һ���޸�
//* �� ע��
//Ostu������������������ͨ��ͳ������ͼ���ֱ��ͼ������ʵ��ȫ����ֵT���Զ�ѡȡ�����㷨����Ϊ��
//1) �ȼ���ͼ���ֱ��ͼ������ͼ�����е����ص㰴��0~255��256��bin��ͳ������ÿ��bin�����ص�����
//2) ��һ��ֱ��ͼ��Ҳ����ÿ��bin�����ص����������ܵ����ص�
//3) i��ʾ�������ֵ��Ҳ��һ���Ҷȼ�����0��ʼ����
//4) ͨ����һ����ֱ��ͼ��ͳ��0~i �Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ���ǰ������) ��ռ����ͼ��ı���w0����ͳ��ǰ�����ص�ƽ���Ҷ�u0��ͳ��i~255�Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ�����������) ��ռ����ͼ��ı���w1����ͳ�Ʊ������ص�ƽ���Ҷ�u1��
//5) ����ǰ�����غͱ������صķ��� g = w0*w1*(u0-u1) (u0-u1)
//6) i++��ת��4)��ֱ��iΪ256ʱ��������
//7�������g��Ӧ��iֵ��Ϊͼ���ȫ����ֵ
//ȱ��:OSTU�㷨�ڴ�����ղ����ȵ�ͼ���ʱ��Ч�������Բ��ã���Ϊ���õ���ȫ��������Ϣ��
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
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��;
  int16_t MinValue, MaxValue;
  uint8_t Threshold = 0;             //
  uint8 *tmImagep;
  uint8 *imgp;
  int tmImage_size=C_W*(LQOSTU_MAX-LQOSTU_MIN+1)/3-1;
  imgp=(uint8_t *)IMG_DATA;
  tmImagep=imgp+C_W*LQOSTU_MIN;
  for (j = 0; j < 256; j++)
  {
    HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ
    LQ_ary[j]=0;
  }
  while(tmImage_size)
  {
    HistoGram[*tmImagep]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    tmImagep=tmImagep+3;
    tmImage_size--;
  }

  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ

  if (MaxValue == MinValue)
    return MaxValue;         // ͼ����ֻ��һ����ɫ
  if (MinValue + 1 == MaxValue)
    return MinValue;        // ͼ����ֻ�ж�����ɫ

  for (j = MinValue; j <= MaxValue; j++)
    Amount += HistoGram[j];        //  ��������  256

  PixelIntegral = 0;
  for (j = MinValue; j <= MaxValue; j++)
  {
    PixelIntegral += HistoGram[j] * j;//�Ҷ�ֵ����  256
  }
  SigmaB = -1;
  //  str.Format("%d",MinValue);AfxMessageBox(str);
  for (j = MinValue; j < MaxValue; j++)
  {
    PixelBack = PixelBack + HistoGram[j];    //ǰ�����ص���
    PixelFore = Amount - PixelBack;         //�������ص���
    OmegaBack = (double)PixelBack / Amount;//ǰ�����ذٷֱ�
    OmegaFore = (double)PixelFore / Amount;//�������ذٷֱ�
    PixelIntegralBack += HistoGram[j] * j;  //ǰ���Ҷ�ֵ
    PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
    MicroBack = (double)PixelIntegralBack / PixelBack;   //ǰ���ҶȰٷֱ�
    MicroFore = (double)PixelIntegralFore / PixelFore;   //�����ҶȰٷֱ�
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//������䷽��
    if (Sigma > SigmaB)                    //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
    {
      SigmaB = Sigma;
      Threshold = j;
    }
  }
  //============����ֵ�������ص�ı�������ֹ���ֲ���ǿ������=========
  int LQ_cam_jishu,LQ_black=0,LQ_white=0;
  float LQ_thr_rate=0;
  int LQGTU_jishu;
  cam_thr_min=1;
  for(LQGTU_jishu=2;LQGTU_jishu<Threshold;LQGTU_jishu++)
  {
    LQ_black=LQ_black+HistoGram[LQGTU_jishu];//ͳ��С����ֵ�ĺڵ����
    if(HistoGram[LQGTU_jishu]>HistoGram[cam_thr_min])
      cam_thr_min=LQGTU_jishu;//�ҵ�С����ֵ��һ����ֵ
  }
  cam_thr_max=Threshold;
  for(LQGTU_jishu=Threshold+1;LQGTU_jishu<256;LQGTU_jishu++)
  {
    LQ_white=LQ_white+HistoGram[LQGTU_jishu];//ͳ�ƴ�����ֵ�İ׵�ĸ���
    if(HistoGram[LQGTU_jishu]>HistoGram[cam_thr_max])
      cam_thr_max=LQGTU_jishu;//�ҵ�������ֵ��һ����ֵ
  }
  LQ_thr_rate=LQ_black*1.0/LQ_white;//ͳ�ƺڵ���׵�ı���
  int LQ_G_white=0;
  for(LQGTU_jishu=LQOSTU_QIANGGUANG;LQGTU_jishu<256;LQGTU_jishu++)
  {
    LQ_G_white=LQ_G_white+HistoGram[LQGTU_jishu];//ͳ�����ȴ���200�ĵ���
  }
  //str.Format("%d",LQ_G_white);AfxMessageBox(str);
  if(LQ_G_white>LQOSTU_QIANGNUM&&0)//���ȴ���200�ĸ�������2000��ǿ��ɢ��
  {
    cam_strong_light=1;//ǿ���־λ
    cam_thr_mid=(cam_thr_max-cam_thr_min)/LQOSTU_THRERR;
  }
  else if(LQ_thr_rate>3&&0)
  {
    cam_wesk_light=1;  //�����־λ
    for(LQGTU_jishu=3;LQGTU_jishu<254;LQGTU_jishu++)//�˲�
    {
      LQ_ary[LQGTU_jishu]=(HistoGram[LQGTU_jishu-2]+HistoGram[LQGTU_jishu-1]+HistoGram[LQGTU_jishu]+HistoGram[LQGTU_jishu+1]+HistoGram[LQGTU_jishu+2])/5;
    }
    for(LQGTU_jishu=cam_thr_min-3;LQGTU_jishu>3;LQGTU_jishu--)
    {
      if(LQ_ary[LQGTU_jishu]>LQ_ary[LQGTU_jishu+1]&&LQ_ary[LQGTU_jishu+1]>LQ_ary[LQGTU_jishu+2])
        break;
    }
    int LQ_max,LQ_min=LQGTU_jishu;
    for(;LQGTU_jishu<cam_thr_min;LQGTU_jishu++)//�������µ���ֵ
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
    cam_normal_light=1;//�������־λ
//  Threshold=Threshold-5;
    cam_thr_mid=(cam_thr_max-cam_thr_min)/LQOSTU_THRERR;
  }
  //=================================================================
  Threshold0=Threshold;
  return Threshold;                        //���������ֵ;
}

//***************************************************************
//* �������ƣ�uint8_t zzkLQGetOSTU0200_col(uint8_t tmImage[IMAGEH][IMAGEW])
//* ����˵������������ֵ��С
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
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��;
  int16 MinValue, MaxValue;
  uint8 Threshold = 0;             //
  uint8 *tmImagep;
  uint8 *imgp;
  imgp=(uint8 *)IMG_DATA;
  tmImagep=imgp+C_W*LQOSTU_MIN+lie_min;
  //int tmImage_size=(lie_max-lie_min+1)*(LQOSTU_MAX-LQOSTU_MIN+1)/3-(lie_max-lie_min+1)*(LQOSTU_MAX-LQOSTU_MIN+1)%3;
  for (j = 0; j < 256; j++)
  {
    HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ
    LQ_ary[j]=0;
  }
  // while(tmImage_size)
  // {
    // HistoGram[*tmImagep]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
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

  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ

  if (MaxValue == MinValue)
    return MaxValue;         // ͼ����ֻ��һ����ɫ
  if (MinValue + 1 == MaxValue)
    return MinValue;        // ͼ����ֻ�ж�����ɫ

  for (j = MinValue; j <= MaxValue; j++)
    Amount += HistoGram[j];        //  ��������  256

  PixelIntegral = 0;
  for (j = MinValue; j <= MaxValue; j++)
  {
    PixelIntegral += HistoGram[j] * j;//�Ҷ�ֵ����  256
  }
  SigmaB = -1;
  //  str.Format("%d",MinValue);AfxMessageBox(str);
  for (j = MinValue; j < MaxValue; j++)
  {
    PixelBack = PixelBack + HistoGram[j];    //ǰ�����ص���
    PixelFore = Amount - PixelBack;         //�������ص���
    OmegaBack = (double)PixelBack / Amount;//ǰ�����ذٷֱ�
    OmegaFore = (double)PixelFore / Amount;//�������ذٷֱ�
    PixelIntegralBack += HistoGram[j] * j;  //ǰ���Ҷ�ֵ
    PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
    MicroBack = (double)PixelIntegralBack / PixelBack;   //ǰ���ҶȰٷֱ�
    MicroFore = (double)PixelIntegralFore / PixelFore;   //�����ҶȰٷֱ�
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//������䷽��
    if (Sigma > SigmaB)                    //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
    {
      SigmaB = Sigma;
      Threshold = j;
    }
  }
  //=================================================================
  Threshold0=Threshold;
  return Threshold;                        //���������ֵ;
}
