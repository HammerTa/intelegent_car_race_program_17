#include "headfile.h"


struct STR   //����Ľṹ�壬���ڴ�ű߽���Ϣ
{
  uint8 cam_i;   //��ű߽����
  uint8 cam_j;   //��ű߽����
};

uint8 cam_static=90;//���������磬���߼���ĻҶ���ֵ�쳣ʱʹ��
uint8 Threshold0=90;
uint8 cam_thr_mid=0,cam_thr_max=0,cam_thr_min=0;
uint8 cam_thr=90,cam_thr0=100;//����ͷ����ֵ
uint8 HistoGram[256]; //�ڽ��д��ļ�����ֵʱ����ͳ������ָ������ĸ�����ֵ�ĸ�������
int LQ_ary[256];
uint8 cam_strong_light=0;//ǿ���־λ
uint8 cam_normal_light=0;//�������־λ
uint8 cam_wesk_light=0;  //�����־λ
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
    double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��;
    int16_t MinValue, MaxValue;
    uint8_t Threshold = 0;
    uint8_t HistoGram[256];              //

    for (j = 0; j < 256; j++)  HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ

    for (j =0; j < ROW; j++)
    {
        for (i = 0; i < COL; i++)
        {
            HistoGram[tmImage[j][i]]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ

    if (MaxValue == MinValue)     return (uint8_t)MaxValue;         // ͼ����ֻ��һ����ɫ
    if (MinValue + 1 == MaxValue)  return (uint8_t)MinValue;        // ͼ����ֻ�ж�����ɫ

    for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  ��������

    PixelIntegral = 0;
    for (j = MinValue; j <= MaxValue; j++)
    {
        PixelIntegral += HistoGram[j] * j;//�Ҷ�ֵ����
    }
    SigmaB = -1;
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
            Threshold = (uint8_t)j;
        }
    }
    return Threshold;
}
