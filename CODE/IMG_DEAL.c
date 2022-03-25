#include "headfile.h"
//#define  IMG_DATA    mt9v03x_image
 
//----------------����������--------------
 
int  dj_end;
unsigned char stop=1;//ͣ����־λ����1��ʾͣ��
unsigned char go=0;//ֱ�б�־λ��1��ʾֱ��
unsigned char deal_flag=0;//�����־λ
unsigned char fork_flag=0;
//int i = 0,j = 0;

//********************************************

//Ѳ��
int right_flag = 0;		//�������ڱ�־
int left_flag = 0;
int right_start = 0;	//�ұ��������
int left_start = 0;
int right_end = 0;		//�ұ����յ���
int left_end = 0;
int middle_start = 0;	//���������
int middle_end = 0;
int middleline[240];
int middleline_l[240];
int middleline_r[240];
int SUM=0;
int wild1,wild10;
struct DIV 
{ 
	int Row[240]; 
	int Col[240]; 
};

struct AG
{
	float k;
	float SIN;
};

struct APEX
{
	int Apex_Row;
	int Apex_Col;
	int Mark;
};

struct Vector
{
    int star[2];
    int end[2];
};

struct DIV RacingLine;//Ŀ����·
struct DIV left,right;//���ұ���
struct DIV forck_L,forck_R;//�����ж���·
struct APEX left_apex,right_apex;
struct AG right_ag,left_ag;//Ѱ�������������
struct Vector Vector_r,Vector_l;//��ֹ�������������

int right_line[120] = {160};//�˴���Ҫ�ĳ�159������֪��Ϊ������û�����⣬������û����ֻ���󵥱ߵ������ȥѧУ����һ�³�����

int left_line[120] = {0};
int max = 0;
int min  = 0;
int line[120] = {0};
int Threshold;
unsigned char IMG_DATA[ROW][COL];
int mid_row_R[240];
int mid_row_L[240];
int mid_row[240];
int mid_flag=0;
int L_lenth,R_lenth;
//static int COUNT=0;
float Angle_end;
float Angle_End[10]={0};
float ang_l,ang_r,ang;//����Ƕ��ж�
//*********************************************

//**********************************************
//һЩ���˿��������Ķ���
#define WHITE_IMG    255
#define BLACK_IMG    0
#define RED_IMG      128
#define BLUE_IMG     254
#define GREEN_IMG    100

#define u8      unsigned char   
#define u16     unsigned short

#define uint16_t unsigned short
#define uint32_t unsigned int
#define int16_t  short int
#define int32_t  int
//**********************************************



//--------------------------
//����ͷǰհ������������ͷλ�����ã����ִ���Ȩ�ش�С�������1����Ϊ1�����0�е�Ȩ��Ϊ1
int weight[240]={
1,1,1,1,1,1,1,1,1,1,//1-10
3,3,3,3,3,3,3,3,3,3,//11-20
2,2,2,2,2,2,2,2,2,2,//21-30
4,4,4,4,4,4,4,4,4,4,//31-40
4,4,4,4,4,4,4,4,4,4,//41-50
6,6,6,6,6,6,6,6,6,6,//51-60
6,6,6,6,6,6,6,6,6,6,//61-70
1,1,1,1,1,1,1,1,1,1,//71-80
1,1,1,1,1,1,1,1,1,1,//81-90
0,0,0,0,0,0,0,0,0,0,//91-100
0,0,0,0,0,0,0,0,0,0,//101-110
0,0,0,0,0,0,0,0,0,0,//111-120
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,//111-120  
};

//--------------------------
int TrackWild[120]={
        29,30,31,32,33,34,35,36,37,38,40,41,42,43,44,45,46,47,48,
        49,51,52,53,54,55,56,57,58,59,60,62,63,64,65,66,67,68,69,
        70,72,73,74,75,76,77,78,79,80,81,83,84,85,86,87,88,89,90,
        91,92,94,95,96,97,98,99,100,101,102,103,105,106,107,108,109,110,111,
        112,113,114,116,117,118,119,120,121,122,123,124,125,127,128,129,130,131,132,
        133,134,135,136,138,139,140,141,142,143,144,145,146,147,149,150,151,152,153,
        154,155,156,157,158,160};


//***************************************************************
//* �������ƣ� 
//* ����˵���� ͼ���������
//* �������أ� ��
//* �� ע��    
//***************************************************************

//----------Ѳ��------------------------

// �������ǵõ���һ��ͼ����ӳ��С����ǰʱ���������ϵ�λ�á�
// ��ô����Ϊ�˱���С���ܳ����������Ⱦ�Ҫ�жϳ�ͼ����һ����Ϊ������
// ��ɫ����������ɫ������һ�������������ͷ��������ʮ�����ԡ�


void Deal_Init()
{
    int i,j;
   //������ʼ��
    right_flag = 0;left_flag = 0;
    right_start = 254;left_start = 254;
    right_end = 120;left_end = 120;
    middle_start = 0;middle_end = 0;
    for(i=0;i<ROW;i++)
    {
        for(j=0;j<COL;j++)
        {
            IMG_DATA[i][j]=mt9v03x_image[i][j];
        }
    }
    Threshold=GetOSTU(IMG_DATA);
    for(i=0;i<ROW;i++)
    {
        for(j=0;j<COL;j++)
        {
            if(IMG_DATA[i][j] >= Threshold)
            {
                IMG_DATA[i][j]=WHITE_IMG;
            }
            else
            {
                IMG_DATA[i][j]=BLACK_IMG;
            }
        }
    }    
}

 void InitData() //��ʼ���߽���
{ 
        int pin; 
        for(pin=0;pin<240;pin++) 
	{ 
		left.Row[pin]=254;//254������Ч����
		left.Col[pin]=254; 
		right.Row[pin]=254;
		right.Col[pin]=254;
		forck_L.Row[pin]=254;
		forck_L.Col[pin]=254;
        forck_R.Row[pin]=254;
        forck_R.Col[pin]=254;
                RacingLine.Row[pin]=254;
                RacingLine.Col[pin]=254;
                middleline[pin]=254;
                middleline_l[pin]=254;
                middleline_r[pin]=254;
                mid_row_R[pin]=254;
                mid_row_L[pin]=254;
                mid_row[pin]=254;
	}
    gpio_set(FMQ,0);
    left_apex.Mark=254;
	right_apex.Mark=254;
	Vector_r.star[0]=254;
	Vector_r.star[1]=254;
	Vector_r.end[0]=254;
	Vector_r.end[1]=254;
    Vector_l.star[0]=254;
    Vector_l.star[1]=254;
    Vector_l.end[0]=254;
    Vector_l.end[1]=254;
    L_lenth=0;
    R_lenth=0;
    fork_flag=0;
    ang_l=0;
    ang_r=0;
    ang=0;
}

///***************************************************************
//* �������ƣ� 
//* ����˵���� �������
//* �������أ� 
//* �� ע������ͼ����ָ�ϱ�д    
//***************************************************************
void LeftStartFind() 
{ 
  int row,col;
  for(row=ROW-5;row>40 && left.Row[0]==254;row--)
  {
    for(col = 5;col<COL - 55 && left.Col[0] == 254;col++)
    {
      if(IMG_DATA[row][col]==BLACK_IMG && IMG_DATA[row][col+1]==BLACK_IMG)
      {
        if(IMG_DATA[row][col+2]==WHITE_IMG && IMG_DATA[row][col+3]==WHITE_IMG)
        {
           left.Row[0]=row;
           left.Col[0]=col+1;
           left_flag=1;
        }
      }
    }
  }
  left_start=left.Row[0];
}
  
//***************************************************************
//* �������ƣ� 
//* ����˵���� �������
//* �������أ� ��
//* �� ע��    
//***************************************************************
void RightStartFind() 
{
  int row,col;
  for(row=ROW-5;row > 40 && right.Row[0] == 254;row--)
  {
    for(col=COL - 5;col > 55 && right.Col[0] == 254;col--)
    {
      if(IMG_DATA[row][col]==BLACK_IMG && IMG_DATA[row][col-1]==BLACK_IMG)
      {
        if(IMG_DATA[row][col-2]==WHITE_IMG && IMG_DATA[row][col-3]==WHITE_IMG)
        {
        right.Row[0]=row;
        right.Col[0]=col-1;
        right_flag=1;
        }
      }
    }
  }
  right_start=right.Row[0];
}




void left_jump()
{
	int pin, i, j;
	int row, col,find;
	int row_temp, col_temp;
	int whitecounter = 0;
	int rowcheack[12] = {-1, 1, -1, 0, 2, 0, 0, -1, -1, 1, 0, 1};
	int colcheack[12] = {1, 0, -1, -1, 2, -1, -1, 0, 0, 1, -2, 1};
        int colmin, colmax;
        if(left.Row[0] == 254)
	{
		return;
	}
        pin=1;
        for(row=left.Row[0] - pin;pin<5;pin++)
	{
		find = 0;
		row = left.Row[0] - pin;
		colmin = left.Col[pin - 1] - 10;
		colmax = left.Col[pin - 1] + 10;
		for(col = colmin;col <= colmax;col++)
		{
			if(col < COL - 5 && col > -1)
			{
				if(IMG_DATA[row][col] == BLACK_IMG && IMG_DATA[row][col + 1] == BLACK_IMG)
				{
					if(IMG_DATA[row][col + 2] == WHITE_IMG && IMG_DATA[row][col + 3] == WHITE_IMG)
					{
						left.Row[pin] = row;
						left.Col[pin] = col + 1;
						IMG_DATA[row][col + 1] = RED_IMG;
						find = 1;
						L_lenth++;
						break;
					}
				}
			}
		}
		if(find == 0)
		{
			break;
		}
        }
	for(pin = 5;pin < 240;pin++)
	{
                if(left.Row[4] == 254)
		{
			break;
		}
		row = left.Row[pin - 1];
		col = left.Col[pin - 1];
		if(row < 30 || col < 5 || col > COL-5)
		{
			break;
		}
		for(i = 0;i < 8;i++)
		{
			row = row + rowcheack[i];
			col = col + colcheack[i];
			if(IMG_DATA[row][col] == BLACK_IMG)
			{
				row_temp = row;
				col_temp = col;
				for(j = 8;j < 12;j++)
				{
					row_temp = row_temp + rowcheack[j];
					col_temp = col_temp + colcheack[j];
					if(IMG_DATA[row_temp][col_temp] == WHITE_IMG)
					{
						whitecounter++;
					}
				}
			}
			if(whitecounter != 0)
			{
				whitecounter = 0;
				left.Row[pin] = row;
				left.Col[pin] = col;
				IMG_DATA[row][col] = RED_IMG;
				L_lenth++;
				break;
			}
		}
	}
  left_end=row;
}
void right_jump()
{
	int pin, i, j;
	int row, col,find;
	int row_temp, col_temp;
	int whitecounter = 0;
        int colmin, colmax;
	int rowcheack[12] = {-1, 1, -1, 0, 2, 0, 0, -1, -1, 1, 0, 1};
	int colcheack[12] = {-1, 0, 1, 1, -2, 1, 1, 0, 0, -1, 2, -1};
        if(right.Row[0] == 254)
	{
		return;
	}
        for(pin=1,row=right.Row[0] - pin;pin<5;pin++)
	{
		find = 0;
		row = right.Row[0] - pin;
		colmin = right.Col[pin - 1] - 10;
		colmax = right.Col[pin - 1] + 10;
		for(col = colmax;col >= colmin;col--)
		{
			if(col < COL + 1 && col > 5)
			{
				if(IMG_DATA[row][col] == BLACK_IMG && IMG_DATA[row][col - 1] == BLACK_IMG)
				{
					if(IMG_DATA[row][col - 2] == WHITE_IMG && IMG_DATA[row][col - 3] == WHITE_IMG)
					{
						right.Row[pin] = row;
						right.Col[pin] = col - 1;
						IMG_DATA[row][col - 1] = BLUE_IMG;
						find = 1;
						R_lenth++;
						break;
					}
				}
			}
		}
		if(find == 0)
		{
			break;
		}
	}
	for(pin = 5;pin < 240;pin++)
	{
		if(right.Row[4] == 254)
		{
			break;
		}
		row = right.Row[pin - 1];
		col = right.Col[pin - 1];
		if(row < 30 || col < 5 || col > COL-5)
		{
			break;
		}
		for(i = 0;i < 8;i++)
		{
			row = row + rowcheack[i];
			col = col + colcheack[i];
			if(IMG_DATA[row][col] == BLACK_IMG)
			{
				row_temp = row;
				col_temp = col;
				for(j = 8;j < 12;j++)
				{
					row_temp = row_temp + rowcheack[j];
					col_temp = col_temp + colcheack[j];
					if(IMG_DATA[row_temp][col_temp] == WHITE_IMG)
					{
						whitecounter++;
					}
				}
			}
			if(whitecounter != 0)
			{
				whitecounter = 0;
				right.Row[pin] = row;
				right.Col[pin] = col;
				IMG_DATA[row][col] = BLUE_IMG;
				R_lenth++;
				break;
			}
		}
	}
  right_end=row;
  
}

//***************************************************************
//* �������ƣ� fixpoint()
//* ����˵����������ʼ������
//* �������أ� 
//* �� ע��    
//***************************************************************
void fixpoint()//���ұ�����ʼ�������
{
        if(left.Col[0] == 254 && left.Row[0]==254)
	{
		return;
	}
	if(right.Col[0] == 254 && right.Row[0] == 254)
	{
		return;
	}
	float k = 0;
	int UpPointCol = 254, UpPointRow = 254;
	int DownPointCol = 254,DownPointRow = 254;
	int row, col, pin, BP;
	BP = 0;
	if(left.Row[0] < right.Row[0])
	{
		UpPointCol = left.Col[0];
		UpPointRow = left.Row[0];
		DownPointCol = right.Col[0];
		DownPointRow = right.Row[0];
	}
	else
	{
		UpPointCol = right.Col[0];
		UpPointRow = right.Row[0];
		DownPointCol = left.Col[0];
		DownPointRow = left.Row[0];
	}
	if(UpPointRow != DownPointRow)
	{
		k = (float)(UpPointCol - DownPointCol) / (UpPointRow - DownPointRow);
	}
	for(pin=0, row = DownPointRow;row >= UpPointRow;pin++,row--)
	{
		col = (int)(DownPointCol - k * pin);
		if(col > -1 && col < COL && row >-1 && row < ROW)
		{
			if(IMG_DATA[row][col] == BLACK)
			{
				BP++;
			}
		}
	}
	if(BP > 4)
	{
		if(left.Row[0] < right.Row[0])
		{
			left.Col[0]=254;
			left.Row[0]=254;
            left_flag=0;
		}
		else if(left.Row[0] > right.Row[0])
		{
			right.Col[0]=254;
			right.Row[0]=254;
            right_flag=0;
		}
	}
}

void Left_Apex()
{
	if(left.Row[0]==254&&left.Col[0]==254)
	{
		return;
	}
	int pin;
	int row_flag,col_flag;
	float row,col;
	float SIN_min=1;
	for(pin=0;pin<240;pin++)
	{
		if(left.Row[pin]==254)
		{
			break;
		}
		row=(float)left.Row[pin];
		col=(float)left.Col[pin];
		left_ag.k=(160.0-row)/(60.5-col);
		left_ag.SIN=(60.5-col)/sqrt((160.0-row)*(160-row)+(60.5-col)*(60.5-col));
		if(left_ag.SIN < SIN_min)
		{
			SIN_min=left_ag.SIN;
			row_flag=(int)row;
			col_flag=(int)col;
			left_apex.Mark=pin;
		}
	}
	left_apex.Apex_Row=row_flag;
	left_apex.Apex_Col=col_flag;
}//Ѱ��������

void Right_Apex()
{
	if(right.Row[0]==254&&right.Col[0]==254)
	{
		return;
	}
	int pin;
	int row_flag,col_flag;
	float row,col;
	float SIN_max=-1;
	for(pin=0;pin<240;pin++)
	{
		if(right.Row[pin]==254)
		{
			break;
		}
		row=(float)right.Row[pin];
		col=(float)right.Col[pin];
		right_ag.k=(160.0-row)/(60.5-col);
		right_ag.SIN=(60.5-col)/sqrt((160.0-row)*(160-row)+(60.5-col)*(60.5-col));
		if(right_ag.SIN > SIN_max)
		{
			SIN_max=right_ag.SIN;
			row_flag=(int)row;
			col_flag=(int)col;
			right_apex.Mark=pin;
		}
	}
	right_apex.Apex_Row=row_flag;
	right_apex.Apex_Col=col_flag;
}//Ѱ��������

//***************************************************************
//* �������ƣ� Get_Center_Line
//* ����˵���� ������
//* �������أ� 
//* �� ע��    line[]������Ҫ�Լ����壬������ʾ�����Ŀ�ȣ�����line[5]��ʾ������������ʵ�ʿ��
//��Ҫ���ǵ�����ͷ������ʱ������ԽԶ������ͼ����ʾ��Խխ�������
//***************************************************************
void Racing_Line()
{
//    int pin,i;
//    int row,col;
//    int flag;
//    int mark;
//    int l_flag,r_flag;
//    l_flag=0;
//    r_flag=0;
//    i=0;
//    if(left_flag==0&&right_flag==0)
//    {
//      return;
//    }
//    if(left_flag==0)
//    {
//        mark=right_apex.Mark+(115-right.Row[0]);
//        row=right.Row[0];
//        for(pin=0;pin<mark;pin++)
//        {
//            flag=119-pin;
//            if(flag>right.Row[0])
//            {
//              middleline[pin]=right.Col[0]-TrackWild[row]/2;
//              col=middleline[pin];
//              mid_row[pin]=flag;
//            }
//            else
//            {
//
//              row=right.Row[i];
//              col=right.Col[i]-TrackWild[row]/2;
//              middleline[pin]=col;
//              mid_row[pin]=row;
//              i++;
//            }
//        }
//        mid_flag++;
//        return;
//    }
//    else if(right_flag==0)
//    {
//      mark=left_apex.Mark+(115-left.Row[0]);
//      row=left.Row[0];
//      for(pin=0;pin<mark;pin++)
//        {
//          flag=119-pin;
//          if(flag>left.Row[0])
//          {
//            middleline[pin]=left.Col[0]+TrackWild[row]/2;
//              col=middleline[pin];
//              mid_row[pin]=flag;
//          }
//          else
//          {
//            row=left.Row[i];
//            col=left.Col[i]+TrackWild[row]/2;
//            middleline[pin]=col;
//            mid_row[pin]=row;
//            i++;
//          }
//        }
//      mid_flag++;
//        return;
//    }//��Ϊ���ߣ�����ΪԴ����Ľ���ԭ�汾��ADS��
//    else
//    {
//        //�������������
//        mark=left_apex.Mark+(115-left.Row[0]);
//        row=left.Row[0];
//        for(pin=0;pin<mark;pin++)
//        {
//            flag=119-pin;
//            if(flag>left.Row[0])
//            {
//                middleline_l[pin]=left.Col[0]+TrackWild[row]/2;
//                col=middleline_l[pin];
//                mid_row_L[pin]=flag;
//           }
//            else
//            {
//                row=left.Row[i];
//                 col=left.Col[i]+TrackWild[row]/2;
//                middleline_l[pin]=col;
//                mid_row_L[pin]=row;
//                i++;
//             }
//        }
//        //���ұ���������
//        i=0;
//        mark=right_apex.Mark+(115-right.Row[0]);
//        row=right.Row[0];
//        for(pin=0;pin<mark;pin++)
//        {
//          flag=119-pin;
//          if(flag>right.Row[0])
//          {
//            middleline_r[pin]=right.Col[0]-TrackWild[row]/2;
//              col=middleline_r[pin];
//              mid_row_R[pin]=flag;
//          }
//          else
//          {
//            row=right.Row[i];
//            col=right.Col[i]-TrackWild[row]/2;
//            middleline_r[pin]=col;
//            mid_row_R[pin]=row;
//            i++;
//          }
//          r_flag++;
//        }
//    }
//    if(l_flag<r_flag)
//    {
//        mid_flag=r_flag;
//        for(i=0;i<r_flag;i++)
//        {
//            mid_row[i]=mid_row_R[i];
//            if(i<l_flag) middleline[i]=(middleline_r[i]+middleline_l[i])/2;
//            else middleline[i]=middleline_r[i];
//        }
//    }
//    else
//    {
//        mid_flag=l_flag;
//        for(i=0;i<l_flag;i++)
//        {
//            mid_row[i]=mid_row_L[i];
//            if(i<r_flag) middleline[i]=(middleline_r[i]+middleline_l[i])/2;
//            else middleline[i]=middleline_l[i];
//        }
//    }


        int pin,i;
        int row,col;
        int flag;
        int mark;
        int l_flag,r_flag;
        l_flag=0;
        r_flag=0;
        i=0;
        if(left_flag==0&&right_flag==0)
        {
          return;
        }
        if(left_flag==0)
        {
            mark=right_apex.Mark+(115-right.Row[0]);
            row=right.Row[0];
            for(pin=0;pin<mark;pin++)
            {
                flag=119-pin;
                if(flag>right.Row[0])
                {
                  middleline[pin]=right.Col[0]-TrackWild[row]/2;
                  col=middleline[pin];
                  mid_row[pin]=flag;
                  IMG_DATA[flag][col]=GREEN_IMG;
                }
                else
                {

                  row=right.Row[i];
                  col=right.Col[i]-TrackWild[row]/2;
                  middleline[pin]=col;
                  mid_row[pin]=row;
                  i++;
                  if(row>5&&row<ROW-5)
                  {
                    if(col>5&&col<COL-5)
                    {
                        IMG_DATA[row][col]=GREEN_IMG;
                    }
                  }
                }
            }
            return;
        }
        else if(right_flag==0)
        {
          mark=left_apex.Mark+(115-left.Row[0]);
          row=left.Row[0];
          for(pin=0;pin<mark;pin++)
            {
              flag=119-pin;
              if(flag>left.Row[0])
              {
                middleline[pin]=left.Col[0]+TrackWild[row]/2;
                  col=middleline[pin];
                  IMG_DATA[flag][col]=GREEN_IMG;
                  mid_row[pin]=flag;
              }
              else
              {
                row=left.Row[i];
                col=left.Col[i]+TrackWild[row]/2;
                middleline[pin]=col;
                mid_row[pin]=row;
                i++;
                if(row>5&&row<ROW-5)
                {
                  if(col>5&&col<COL-5)
                  {
                      IMG_DATA[row][col]=GREEN_IMG;
                  }
                }
              }
            }
            return;
        }
        if(left_apex.Mark>=right_apex.Mark)
        {
            mark=left_apex.Mark+(115-left.Row[0]);
            row=left.Row[0];
            for(pin=0;pin<mark;pin++)
            {
              flag=119-pin;
              if(flag>left.Row[0])
              {
                middleline[pin]=left.Col[0]+TrackWild[row]/2;
                  col=middleline[pin];
                  mid_row[pin]=flag;
                  IMG_DATA[flag][col]=GREEN_IMG;
              }
              else
              {
                row=left.Row[i];
                col=left.Col[i]+TrackWild[row]/2;
                middleline[pin]=col;
                mid_row[pin]=row;
                i++;
                if(row>5&&row<ROW-5)
                {
                  if(col>5&&col<COL-5)
                  {
                      IMG_DATA[row][col]=GREEN_IMG;
                  }
                }
              }
              l_flag++;
            }
            return;
        }
        else if(left_apex.Mark<right_apex.Mark)
        {
            mark=right_apex.Mark+(115-right.Row[0]);
            row=right.Row[0];
            for(pin=0;pin<mark;pin++)
            {
              flag=119-pin;
              if(flag>right.Row[0])
              {
                middleline[pin]=right.Col[0]-TrackWild[row]/2;
                  col=middleline[pin];
                  mid_row[pin]=flag;
                  IMG_DATA[flag][col]=GREEN_IMG;
              }
              else
              {
                row=right.Row[i];
                col=right.Col[i]-TrackWild[row]/2;
                middleline[pin]=col;
                i++;
                mid_row[pin]=row;
                if(row>5&&row<ROW-5)
                {
                  if(col>5&&col<COL-5)
                  {
                      IMG_DATA[row][col]=GREEN_IMG;
                  }
                }
              }
              r_flag++;
            }
            return;
        }
}

 //***************************************************************
//* �������ƣ� void protect()
//* ����˵���� ������ܳ�������ͣ��
//* �������أ� 
//* �� ע��    0��ʾ��ɫ 255��ʾ��ɫ������Լ�˼��һ����ôд
//***************************************************************
void protect()
{
  unsigned char i;
  unsigned char sum;
  sum=0;
  for(i=0;i<188;i++)
  {
    if(IMG_DATA[115][i]==BLACK_IMG)
      sum++;
  }
  SUM=sum;
  if(SUM>=170)
    stop=1;
  return;
}

//***************************************************************
//* �������ƣ� angle
//* ����˵���� ���߸��Ƕ�����
//* �������أ�
//* �� ע��
//***************************************************************
//void Angle_IMG()
//{
//    float diancheng,mocheng;
//    if(L_lenth<10)
//    {
//        return;
//    }
//    if(R_lenth<10)
//    {
//        return;
//    }
//    Vector_l.star[0]=(float)left.Col[5]-left.Col[1];//���������x
//    Vector_l.star[1]=(float)left.Row[5]-left.Row[1];//���������y
//    Vector_l.end[0]=(float)left.Col[L_lenth-1]-left.Col[L_lenth-8];//���յ�����x
//    Vector_l.end[1]=(float)left.Row[L_lenth-1]-left.Row[L_lenth-8];//���յ�����y
//    Vector_r.star[0]=(float)right.Col[5]-right.Col[1];//���������x
//    Vector_r.star[1]=(float)right.Row[5]-right.Row[1];//���������y
//    Vector_r.end[0]=(float)right.Col[L_lenth-1]-right.Col[L_lenth-8];//���յ�����x
//    Vector_r.end[1]=(float)right.Row[L_lenth-1]-right.Row[L_lenth-8];//���յ�����y
//    diancheng=0.1*Vector_l.end[0]*Vector_r.end[0]+Vector_l.end[1]*Vector_r.end[1];
//    mocheng=0.1*(Vector_l.end[0]*Vector_l.end[0]+Vector_l.end[1]*Vector_l.end[1])*(Vector_r.end[0]*Vector_r.end[0]+Vector_r.end[1]*Vector_r.end[1]);
//    Angle_end=diancheng/sqrt(mocheng);
//    Angle_end=Angle_end*180/3.14;
//    Angle_End[COUNT]=Angle_end*0.1;
//    COUNT++;
//    Angle_end=0;
//    for(int j=0;j<10;j++)
//    {
//      Angle_end+=Angle_End[j];
//    }
//    if(COUNT>=10)
//    {
//      COUNT=0;
//    }//�˲�
//    if(Angle_end<0)
//    {
//        Angle_end=0-Angle_end;
//    }
//}

//***************************************************************
//* �������ƣ� angle
//* ����˵���� ���߸��Ƕ�����
//* �������أ�
//* �� ע��
//***************************************************************
void Conner_cheack()
{

}

//***************************************************************
//* �������ƣ� Roundabout_Deal
//* ����˵���� ����������
//* �������أ�
//* �� ע��
//***************************************************************
void Roundabout_Deal()
{

}


//***************************************************************
//* �������ƣ� T_Conner_Deal
//* ����˵���� T�䴦����
//* �������أ�
//* �� ע��
//**************************************************************
void T_Conner_Deal()
{

}


//***************************************************************
//* �������ƣ� Fork_Deal
//* ����˵���� ���洦����
//* �������أ�
//* �� ע��
//***************************************************************
void Fork_Deal()
{
    int i,j,pin;
    int F_L_flag=0;
    int F_R_flag=0;
    for (i=70;i>20;i--)
    {
        for(j=COL/2-20;j<COL/2+20;j++)
        {
            if(IMG_DATA[i][j]==WHITE_IMG && IMG_DATA[i][j+1]==BLACK_IMG)
            {
                IMG_DATA[i][j+1]=RED_IMG;
                forck_L.Row[0]=i;
                forck_L.Col[0]=j+1;
                F_L_flag=1;
            }
            if(IMG_DATA[i][j]==BLACK_IMG && IMG_DATA[i][j+1]==WHITE_IMG)
            {
                IMG_DATA[i][j+1]=BLUE_IMG;
                forck_R.Row[0]=i;
                forck_R.Col[0]=j-1;
                F_R_flag=1;
            }
        }
        if(F_L_flag==0 && F_R_flag==1)
        {
            return;
        }
        if(F_R_flag==0 && F_L_flag==1)
        {
            return;
        }
        if(F_R_flag==1 && F_L_flag==1)
        {
            break;
        }
    }
    int find;
    int row,col;
    int colmin,colmax;
    //��Ϊ��
    pin=1;
    for(row=forck_L.Row[0] - pin;pin<5;pin++)
    {
        find = 0;
        row = forck_L.Row[0] - pin;
        colmin = forck_L.Col[pin - 1] - 10;
        colmax = forck_L.Col[pin - 1] + 10;
        for(col = colmin;col <= colmax;col++)
        {
            if(col < COL - 5 && col > -1)
            {
                if(IMG_DATA[row][col] == WHITE_IMG && IMG_DATA[row][col + 1] == WHITE_IMG)
                {
                    if(IMG_DATA[row][col + 2] == BLACK_IMG && IMG_DATA[row][col + 3] == BLACK_IMG)
                    {
                        forck_L.Row[pin] = row;
                        forck_L.Col[pin] = col + 2;
                        IMG_DATA[row][col + 2] = RED_IMG;
                        find = 1;
                        break;
                    }
                }
            }
        }
        if(find == 0)
        {
            break;
        }
    }
    //��Ϊ��
    pin=1;
    for(row=right.Row[0] - pin;pin<5;pin++)
    {
        find = 0;
        row = forck_R.Row[0] - pin;
        colmin = forck_R.Col[pin - 1] - 10;
        colmax = forck_R.Col[pin - 1] + 10;
        for(col = colmax;col >= colmin;col--)
        {
            if(col < COL + 1 && col > 5)
            {
                if(IMG_DATA[row][col] == WHITE_IMG && IMG_DATA[row][col - 1] == WHITE_IMG)
                {
                    if(IMG_DATA[row][col - 2] == BLACK_IMG && IMG_DATA[row][col - 3] == BLACK_IMG)
                    {
                        forck_R.Row[pin] = row;
                        forck_R.Col[pin] = col - 2;
                        IMG_DATA[row][col - 2] = BLUE_IMG;
                        find = 1;
                        break;
                    }
                }
            }
        }
        if(find == 0)
        {
            break;
        }
    }
    int V_L[2],V_R[2];
    V_L[0]=forck_L.Row[4]-forck_L.Row[0];
    V_L[1]=forck_L.Col[4]-forck_L.Col[0];
    V_R[0]=forck_R.Row[4]-forck_R.Row[0];
    V_R[1]=forck_R.Col[4]-forck_R.Col[0];
    ang_l=atan(1.0*V_L[0]/V_L[1]);
    ang_r=atan(1.0*V_R[0]/V_R[1]);
    //ang_l=1.0*V_L[0]/V_L[1];
    //ang_r=1.0*V_R[0]/V_R[1];
    ang_l=ang_l*180/3.14;
    ang_r=ang_r*180/3.14+180;
    ang=ang_r-ang_l;
    if(ang>140)
    {
        //gpio_set(FMQ,1);
        fork_flag=1;
    }
}

//***************************************************************
//* �������ƣ� Garage_Deal
//* ����˵���� ���⺯��
//* �������أ�
//* �� ע��
//***************************************************************
void Garage_Deal()
{

}

void Track()
{
    int row1,col1;
    int row2,col2;
    int row3,col3;
    int row4,col4;
    row1=left.Row[0];
    col1=left.Col[0];
    row2=right.Row[0];
    col2=right.Col[0];
    row3=left.Row[10];
    col3=left.Col[10];
    row4=right.Row[10];
    col4=right.Col[10];
    lcd_showint32(0,0,row1,3);
    lcd_showint32(0,1,col1,3);
    lcd_showint32(0,2,row2,3);
    lcd_showint32(0,3,col2,3);
    lcd_showint32(0,4,row3,3);
    lcd_showint32(0,5,col3,3);
    lcd_showint32(0,6,row4,3);
    lcd_showint32(0,7,col4,3);
}

//***************************************************************
//* �������ƣ� RacingLine_L
//* ����˵���� �����Ѱ���ף����Ǹ�ƫ(1/3)
//* �������أ�
//* �� ע��
//***************************************************************
void RacingLine_L()
{
    int pin,i;
    int row,col;
    int flag;
    int mark;
    int l_flag,r_flag;
    l_flag=0;
    r_flag=0;
    i=0;
    mark=L_lenth+(115-left.Row[0]);
    if(mark>240) mark=240;
    row=left.Row[0];
    for(pin=0;pin<240;pin++)
    {
        flag=119-pin;
        if(flag>left.Row[0])
        {
            middleline[pin]=left.Col[0]+TrackWild[row]/2;
            col=middleline[pin];
            IMG_DATA[flag][col]=GREEN_IMG;
            mid_row[pin]=flag;
        }
        else
        {
            row=left.Row[i];
            col=left.Col[i]+TrackWild[row]/2;
            middleline[pin]=col;
            mid_row[pin]=row;
            i++;
            if(row>5&&row<ROW-5)
            {
                if(col>5&&col<COL-5)
                {
                    IMG_DATA[row][col]=GREEN_IMG;
                }
            }
        }
        if(left.Row[pin+1]==254) break;
    }
    return;
}

//***************************************************************
//* �������ƣ� RacingLine_R
//* ����˵���� �ұ���Ѱ���ף����Ǹ�ƫ(1/3)
//* �������أ�
//* �� ע��
//***************************************************************
void RacingLine_R()
{
    int pin,i;
    int row,col;
    int flag;
    int mark;
    int l_flag,r_flag;
    l_flag=0;
    r_flag=0;
    i=0;
    if(left_flag==0&&right_flag==0)
    {
      return;
    }
        mark=R_lenth+(115-right.Row[0]);
        if(mark>240) mark=240;
        row=right.Row[0];
        for(pin=0;pin<mark;pin++)
        {
        flag=119-pin;
        if(flag>right.Row[0])
        {
          middleline[pin]=right.Col[0]-TrackWild[row]/2;
          col=middleline[pin];
          mid_row[pin]=flag;
          IMG_DATA[flag][col]=GREEN_IMG;
        }
        else
        {
          row=right.Row[i];
          col=right.Col[i]-TrackWild[row]/2;
          middleline[pin]=col;
          mid_row[pin]=row;
          i++;
          if(row>5&&row<ROW-5)
          {
            if(col>5&&col<COL-5)
            {
                IMG_DATA[row][col]=GREEN_IMG;
            }
          }
        }
    }
    return;
}

//***************************************************************
//* �������ƣ� Racing_Line_Fork_L
//* ����˵���� ����Ѳ�߲���
//* �������أ�
//* �� ע��
//***************************************************************
void Racing_Line_Fork_L()
{
    int pin,i;
    int row,col;
    int flag;
    int mark;
    int l_flag,r_flag;
    l_flag=0;
    r_flag=0;
    i=0;
    row=forck_L.Row[0];
    for(pin=0;pin<240;pin++)
    {
        flag=119-pin;
        if(flag>forck_L.Row[0])
        {
            middleline[pin]=forck_L.Col[0]-TrackWild[row]/2;
            col=middleline[pin];
            mid_row[pin]=flag;
            IMG_DATA[flag][col]=GREEN_IMG;
        }
        else
        {
              row=forck_L.Row[i];
              col=forck_L.Col[i]-TrackWild[row]/2;
              middleline[pin]=col;
              mid_row[pin]=row;
              i++;
              if(row>5&&row<ROW-5)
              {
                    if(col>5&&col<COL-5)
                    {
                        IMG_DATA[row][col]=GREEN_IMG;
                    }
              }
        }
        if(forck_L.Row[pin+1]==254) break;
     }
    return;
}

void RaceLine()
{
    if(fork_flag==1) Racing_Line_Fork_L();
    else Racing_Line();
}

//***************************************************************
//* �������ƣ� FPS
//* ����˵���� ʵ��֡������
//* �������أ�
//* �� ע��
//***************************************************************
//void FPS()
//{
//
//}

//void EM_show()
//{
//    lcd_showfloat();
//}

//***************************************************************
//* �������ƣ� Img_Deal
//* ����˵���� ͼ����������
//* �������أ� 
//* �� ע��    
//***************************************************************
void Img_Deal()
{ 
    Deal_Init();
    InitData();
    deal_flag=1;
    //seekfree_sendimg_03x(UART_2,IMG_DATA,188,120);//����ԭʼͼ��
    protect();
	LeftStartFind() ;
	RightStartFind();
	fixpoint();
	left_jump();
	right_jump();
    Left_Apex();
    Right_Apex();
    //Angle_IMG();
    Fork_Deal();
    RaceLine();
	//seekfree_sendimg_03x(UART_2,IMG_DATA,188,120);//���䴦���ͼ��
	//Track();
	//lcd_displayimage032_zoom(IMG_DATA,188,120,160,128);//ͼ����ʾ
	deal_flag=0;
}
