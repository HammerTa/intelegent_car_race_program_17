#include "headfile.h"
//#define  IMG_DATA    mt9v03x_image
 
//----------------变量定义区--------------
 
int  dj_end;
unsigned char stop=1;//停车标志位，置1表示停车
unsigned char go=0;//直行标志位，1表示直行
unsigned char deal_flag=0;//处理标志位
unsigned char fork_flag=0;
//int i = 0,j = 0;

//********************************************

//巡线
int right_flag = 0;		//右起点存在标志
int left_flag = 0;
int right_start = 0;	//右边线起点行
int left_start = 0;
int right_end = 0;		//右边线终点行
int left_end = 0;
int middle_start = 0;	//中线起点行
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

struct DIV RacingLine;//目标线路
struct DIV left,right;//左右边线
struct DIV forck_L,forck_R;//三叉判断线路
struct APEX left_apex,right_apex;
struct AG right_ag,left_ag;//寻找弯心所需变量
struct Vector Vector_r,Vector_l;//起止点近似切线向量

int right_line[120] = {160};//此处需要改成159，但不知道为何运行没出问题，可能是没遇到只有左单边的情况，去学校再跑一下车试试

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
float ang_l,ang_r,ang;//三叉角度判断
//*********************************************

//**********************************************
//一些让人看着舒服点的定义
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
//摄像头前瞻量，根据摄像头位置设置，数字代表权重大小，例如第1个数为1代表第0行的权重为1
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
//* 函数名称： 
//* 功能说明： 图像处理总入口
//* 函数返回： 无
//* 备 注：    
//***************************************************************

//----------巡线------------------------

// 现在我们得到了一幅图，反映了小车当前时刻在赛道上的位置。
// 那么我们为了避免小车跑出赛道，首先就要判断出图上哪一区域为赛道。
// 白色的赛道与蓝色背景，一般情况下在摄像头看来区别十分明显。


void Deal_Init()
{
    int i,j;
   //参数初始化
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

 void InitData() //初始化边界线
{ 
        int pin; 
        for(pin=0;pin<240;pin++) 
	{ 
		left.Row[pin]=254;//254代表无效数据
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
//* 函数名称： 
//* 功能说明： 找左起点
//* 函数返回： 
//* 备 注：参照图像处理指南编写    
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
//* 函数名称： 
//* 功能说明： 找右起点
//* 函数返回： 无
//* 备 注：    
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
//* 函数名称： fixpoint()
//* 功能说明：左右起始点修正
//* 函数返回： 
//* 备 注：    
//***************************************************************
void fixpoint()//左右边线起始点的修正
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
}//寻找左弯心

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
}//寻找右弯心

//***************************************************************
//* 函数名称： Get_Center_Line
//* 功能说明： 找中线
//* 函数返回： 
//* 备 注：    line[]数组需要自己定义，用来表示赛道的宽度，比如line[5]表示第五行赛道的实际宽度
//需要考虑到摄像头看赛到时，看的越远，赛道图像显示的越窄这个因素
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
//    }//上为丢线，以下为源程序改进，原版本于ADS查
//    else
//    {
//        //以左边线找中线
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
//        //以右边线找中线
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
//* 函数名称： void protect()
//* 功能说明： 车如果跑出赛道，停车
//* 函数返回： 
//* 备 注：    0表示黑色 255表示白色，大家自己思考一下怎么写
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
//* 函数名称： angle
//* 功能说明： 边线各角度数据
//* 函数返回：
//* 备 注：
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
//    Vector_l.star[0]=(float)left.Col[5]-left.Col[1];//左起点向量x
//    Vector_l.star[1]=(float)left.Row[5]-left.Row[1];//左起点向量y
//    Vector_l.end[0]=(float)left.Col[L_lenth-1]-left.Col[L_lenth-8];//左终点向量x
//    Vector_l.end[1]=(float)left.Row[L_lenth-1]-left.Row[L_lenth-8];//左终点向量y
//    Vector_r.star[0]=(float)right.Col[5]-right.Col[1];//左起点向量x
//    Vector_r.star[1]=(float)right.Row[5]-right.Row[1];//左起点向量y
//    Vector_r.end[0]=(float)right.Col[L_lenth-1]-right.Col[L_lenth-8];//左终点向量x
//    Vector_r.end[1]=(float)right.Row[L_lenth-1]-right.Row[L_lenth-8];//左终点向量y
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
//    }//滤波
//    if(Angle_end<0)
//    {
//        Angle_end=0-Angle_end;
//    }
//}

//***************************************************************
//* 函数名称： angle
//* 功能说明： 边线各角度数据
//* 函数返回：
//* 备 注：
//***************************************************************
void Conner_cheack()
{

}

//***************************************************************
//* 函数名称： Roundabout_Deal
//* 功能说明： 环岛处理函数
//* 函数返回：
//* 备 注：
//***************************************************************
void Roundabout_Deal()
{

}


//***************************************************************
//* 函数名称： T_Conner_Deal
//* 功能说明： T弯处理函数
//* 函数返回：
//* 备 注：
//**************************************************************
void T_Conner_Deal()
{

}


//***************************************************************
//* 函数名称： Fork_Deal
//* 功能说明： 三叉处理函数
//* 函数返回：
//* 备 注：
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
    //下为左
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
    //下为右
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
//* 函数名称： Garage_Deal
//* 功能说明： 车库函数
//* 函数返回：
//* 备 注：
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
//* 函数名称： RacingLine_L
//* 功能说明： 左边线寻到底，但是更偏(1/3)
//* 函数返回：
//* 备 注：
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
//* 函数名称： RacingLine_R
//* 功能说明： 右边线寻到底，但是更偏(1/3)
//* 函数返回：
//* 备 注：
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
//* 函数名称： Racing_Line_Fork_L
//* 功能说明： 三叉巡线策略
//* 函数返回：
//* 备 注：
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
//* 函数名称： FPS
//* 功能说明： 实际帧数计算
//* 函数返回：
//* 备 注：
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
//* 函数名称： Img_Deal
//* 功能说明： 图像处理主函数
//* 函数返回： 
//* 备 注：    
//***************************************************************
void Img_Deal()
{ 
    Deal_Init();
    InitData();
    deal_flag=1;
    //seekfree_sendimg_03x(UART_2,IMG_DATA,188,120);//传输原始图像
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
	//seekfree_sendimg_03x(UART_2,IMG_DATA,188,120);//传输处理后图像
	//Track();
	//lcd_displayimage032_zoom(IMG_DATA,188,120,160,128);//图像显示
	deal_flag=0;
}
