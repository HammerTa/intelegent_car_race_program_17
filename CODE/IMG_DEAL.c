#include "headfile.h"
//#define  IMG_DATA    mt9v03x_image
 
//----------------变量定义区--------------
 
int  dj_end;
unsigned char stop=1;//停车标志位，置1表示停车
unsigned char go=0;//直行标志位，1表示直行
unsigned char deal_flag=0;//处理标志位
unsigned char fork_flag=0;//三叉标志位
unsigned char T_go_flag[2]={0};//T弯补线标志位，0右1左
unsigned char T_go_flag_pin=0;//T弯补线标志位，0右1左
unsigned char fork_turn=1;//1左，2右
ALL_enum element_flag=NO_JUGED;
FORK_enum Fork_Flag=NO_FORK;
T_CONNER_enum T_flag=NO_T;
Garage_enmum Garage_flag=GET_OUT;
Roundabout_enmum Roundabout_flag=NO_ROUND;
Roundabout_position_enmum Roundabout_flag_position=NOROUND;
//int i = 0,j = 0;

//********************************************

//巡线
int right_flag = 0;		//右起点存在标志
int left_flag = 0;
int right_start = 0;	//右边线起点行
int left_start = 0;
int right_end_row = 0;		//右边线终点行
int left_end_row = 0;
int right_end_col = 0;      //右边线终点行
int left_end_col = 0;
int middle_start = 0;	//中线起点行
int middle_end = 0;
int middleline[240];
int middleline0[240];//上一中线
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
struct DIV T_line;//T弯判断线
struct DIV forck_line_L,forck_line_R;//三叉补线线路
struct DIV Round_L;Round_R;//环岛补线线路
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
float L_S,R_S;//边界方差
float K_left,K_right;
float CorssCol=0;
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
int weight[120]={
2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,//21-30
4,4,4,4,4,4,4,4,4,4,//1-10
8,8,8,8,8,8,8,8,8,8,//11-20
10,10,10,10,10,10,10,10,10,10,//21-30
12,12,12,12,12,12,12,12,12,12,//31-40
14,14,14,14,14,14,14,14,14,14,//41-50
16,16,16,16,16,16,16,16,16,16,//51-60
10,10,10,10,10,10,10,10,10,10,//61-70
8,8,8,8,8,8,8,8,8,8,//71-80
4,4,4,4,4,4,4,4,4,4,//81-90
3,3,3,3,3,3,3,3,3,3,//91-100
};//11120

//--------------------------
int TrackWild[120]={
        4,5,6,8,9,10,12,13,14,16,17,18,19,21,22,23,25,26,27,
        29,30,31,33,34,35,37,38,39,41,42,43,45,46,47,48,50,51,52,
        54,55,56,58,59,60,62,63,64,66,67,68,70,71,72,74,75,76,78,
        79,80,81,83,84,85,87,88,89,91,92,93,95,96,97,99,100,101,103,
        104,105,107,108,109,110,112,113,114,116,117,118,120,121,122,124,125,126,128,
        129,130,132,133,134,136,137,138,139,141,142,143,145,146,147,149,150,151,153,
        154,155,157,158,159,161};


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
    middle_start = 0;middle_end = 0;
//    for(i=0;i<ROW;i++)
//    {
//        for(j=0;j<COL;j++)
//        {
//            IMG_DATA[i][j]=mt9v03x_image[i][j];
//        }
//    }
    Threshold=GetOSTU(IMG_DATA)-20;
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
		if(Fork_Flag==NO_FORK)
		{
		    forck_L.Row[pin]=254;
		    forck_L.Col[pin]=254;
		    forck_R.Row[pin]=254;
		    forck_R.Col[pin]=254;
		}
		T_line.Row[pin]=254;
		T_line.Col[pin]=254;
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
 //* 函数名称： Black_White_Zone
 //* 功能说明： 黑白区，扫描区域内黑白点
 //* 函数返回： 0，1（黑区或白区）
 //* 备 注：    左上到右下,x列,y行
 //***************************************************************
 int Black_White_Zone(int x1,int y1,int x2,int y2)
 {
     int row,col;
     int point=0;
     int Black_point=0;
     int White_point=0;
     float B_percent,W_percent;
     float k1,k2;
     k1=1.0*(y2-y1)/(x2-x1);
     k2=1.0*(y2-y1)/(x1-x2);
     for(col=x1;col<x2;col++)
     {
         row=(int)(k1*(col-x1)+y1);
         if(IMG_DATA[row][col]==BLACK_IMG) Black_point++;
         else White_point++;
         point++;
     }
     for(col=x1;col<x2;col++)
     {
         row=(int)(k1*(col-x1)+y2);
         if(IMG_DATA[row][col]==BLACK_IMG) Black_point++;
         else White_point++;
         point++;
     }
     row=(y1+y2)/2;
     for(col=x1;col<x2;col++)
     {
         if(IMG_DATA[row][col]==BLACK_IMG) Black_point++;
         else White_point++;
         point++;
     }
     col=(x1+x2)/2;
     for(row=y1;row<y2;row++)
     {
         if(IMG_DATA[row][col]==BLACK_IMG) Black_point++;
         else White_point++;
         point++;
     }
     B_percent=1.0*Black_point/point;
     W_percent=1.0*White_point/point;
     if(B_percent>0.6) return 1;
     else if(W_percent>0.6) return 2;
     else return 0;
 }

 ///***************************************************************
 //* 函数名称： Cross_col
 //* 功能说明： 贯通列，辅助判断直道
 //* 函数返回： 白点占比（float）
 //* 备 注：
 //***************************************************************
 float Cross_col()
 {
     float W_percent=0;
     int i,j;
     int White_point=0;
     int Black_point=0;
     int point=600;
     int col[5]={84,90,94,98,102};
     for(i=0;i<5;i++)
     {
         for(j=119;j>0;j--)
         {
             if(IMG_DATA[j][col[i]]==WHITE_IMG)
                 White_point++;
             else
             {
                 Black_point++;
                 if(Black_point>4)
                 {
                     Black_point=0;
                     break;
                 }
             }
         }
     }
     W_percent=1.0*White_point/point;
     return W_percent;
 }
 float CrossRow_L(int star)
 {
      float W_percent=0;
      int i,j;
      int White_point=0;
      int Black_point=0;
      int point=470;
      int row[5];
      if(star>=100) return -1;
      for(i=0;i<5;i++) row[i]=star+5*i;
      for(i=0;i<5;i++)
      {
          for(j=94;j>5;j--)
          {
              if(IMG_DATA[row[i]][j]==WHITE_IMG)
                  White_point++;
              else if(IMG_DATA[row[i]][j]==BLACK_IMG)
              {
                  Black_point++;
                  if(Black_point>4)
                  {
                      Black_point=0;
                      break;
                  }
              }
          }
      }
      W_percent=(float)White_point/point;
      return W_percent;
 }

 float CrossRow_R(int star)
 {
      float W_percent=0;
      int i,j;
      int White_point=0;
      int Black_point=0;
      int point=470;
      int row[5];
      if(star>=100) return -1;
      for(i=0;i<5;i++) row[i]=star+5*i;
      for(i=0;i<5;i++)
      {
          for(j=94;j<183;j++)
          {
              if(IMG_DATA[row[i]][j]==WHITE_IMG)
                  White_point++;
              else if(IMG_DATA[row[i]][j]==BLACK_IMG)
              {
                  Black_point++;
                  if(Black_point>4)
                  {
                      Black_point=0;
                      break;
                  }
              }
          }
      }
      W_percent=(float)White_point/point;
      return W_percent;
 }
 ///***************************************************************
 //* 函数名称： S_jugde
 //* 功能说明： 标准差判断，行为y
 //* 函数返回： 边界线与拟合曲线标准差
 //* 备 注：    判断边界标准误差
 //***************************************************************
 float S_jugde(int row[],int col[],int N)
 {
     int xy=0;
     int x_sq=0;
     float k,b;
     float S=0;
     float x_avg=0;
     float y_avg=0;
     float x_avg_sq=0;
     int y_tg;
     int i,num;
     for(i=0,num=0;i<240;i++)
     {
         if(col[i]==254||i==N)
         {
             x_avg=x_avg/num;
             break;
         }
         if(row[i]==row[i+1]) continue;
         x_avg+=col[i];
         num++;
     }
     x_avg_sq=x_avg*x_avg;
     for(i=0;i<240;i++)
     {
         if(row[i]==254||i==N)
         {
             y_avg=y_avg/num;
             break;
         }
         if(row[i]==row[i+1]) continue;
         y_avg+=row[i];
     }
     for(i=0;i<240;i++)
     {
         if(row[i]==254||col[i]==254||i==N) break;
         if(row[i]==row[i+1]) continue;
         xy+=(row[i]*col[i]);
     }
     for(i=0;i<240;i++)
     {
         if(col[i]==254||i==N) break;
         if(row[i]==row[i+1]) continue;
         x_sq+=(col[i]*col[i]);
     }
     k=(xy-num*x_avg*y_avg)/(x_sq-num*x_avg_sq);
     b=y_avg-k*x_avg;
     for(i=0;i<240;i++)
     {
         if(row[i]==254||i==N)
         {
             S=sqrt(S/(num-1));
             break;
         }
         if(row[i]==row[i+1]) continue;
         y_tg=k*col[i]+b;
         S+=(y_tg-row[i])*(y_tg-row[i]);
     }
     S=S/sqrt(1.0*num);
     return S;
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
	int pin;//, i, j;
	int row, col,find;
//	int row_temp, col_temp;
//	int whitecounter = 0;
//	int rowcheack[12] = {-1, 1, -1, 0, 2, 0, 0, -1, -1, 1, 0, 1};
//	int colcheack[12] = {1, 0, -1, -1, 2, -1, -1, 0, 0, 1, -2, 1};
    int colmin, colmax;
    if(left.Row[0] == 254)
	{
		return;
	}
    left_flag=1;
    pin=1;
    for(row=left.Row[0] - pin;pin<240;pin++)
	{
		find = 0;
		row = left.Row[0] - pin;
		if(row<=15)break;
		colmin = left.Col[pin - 1] - 10;
		colmax = left.Col[pin - 1] + 10;
		for(col = colmin;col <= colmax;col++)
		{
		    if(col<5) col=5;
			if(col < COL - 5)
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

						left_end_row=row;
						left_end_col=col;
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
	if(L_lenth<=5)
	{
	    left_flag=0;
	    left.Row[0]=254;
	    left.Col[0]=254;
	}

}
void right_jump()
{
	int pin;//, i, j;
	int row, col,find;
//	int row_temp, col_temp;
//	int whitecounter = 0;
    int colmin, colmax;
//	int rowcheack[12] = {-1, 1, -1, 0, 2, 0, 0, -1, -1, 1, 0, 1};
//	int colcheack[12] = {-1, 0, 1, 1, -2, 1, 1, 0, 0, -1, 2, -1};
    if(right.Row[0] == 254)
	{
		return;
	}
    right_flag=1;
    for(pin=1,row=right.Row[0] - pin;pin<240;pin++)
	{
		find = 0;
		row = right.Row[0] - pin;
		if(row<=15)break;
		colmin = right.Col[pin - 1] - 10;
		colmax = right.Col[pin - 1] + 10;
		for(col = colmax;col >= colmin;col--)
		{
		    if(col > COL - 5) col=COL - 5;
			if(col > 5)
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
						right_end_row=row;
						right_end_col=col;
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
	if(R_lenth<=5)
	{
	    right_flag=0;
	    right.Row[0]=254;
	    right.Col[0]=254;
	}
  

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
			if(IMG_DATA[row][col] == BLACK_IMG)
			{
				BP++;
			}
		}
	}
	if(BP > 8)
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
    int pin,col_max=0;
    int row_flag,col_flag;
    for(pin=0;pin<240;pin++)
    {
        if(left.Row[pin]==254)
        {
            break;
        }
        if(col_max < left.Col[pin])
        {
            col_max=left.Col[pin];
            row_flag=left.Row[pin];
            col_flag=left.Col[pin];
            left_apex.Mark=pin;
        }
    }
    left_apex.Apex_Row=row_flag;
    left_apex.Apex_Col=col_flag;
	L_S=S_jugde(left.Col,left.Row,left_apex.Mark);
}//寻找左弯心

void Right_Apex()
{
    if(right.Row[0]==254&&right.Col[0]==254)
    {
        return;
    }
    int pin;
    int row_flag,col_flag;
    int col_min=188;
    for(pin=0;pin<240;pin++)
    {
        if(right.Row[pin]==254)
        {
            break;
        }
        if(right.Col[pin] < col_min)
        {
            col_min=right.Col[pin];
            row_flag=right.Row[pin];
            col_flag=right.Col[pin];
            right_apex.Mark=pin;
        }
        //if(right.Row[pin]<row_min) break;
    }
    right_apex.Apex_Row=row_flag;
    right_apex.Apex_Col=col_flag;
	R_S=S_jugde(right.Col,right.Row,right_apex.Mark);
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
            mark=right_apex.Mark+(115-right.Row[5]);
            row=right.Row[5];
            i=5;
            for(pin=0;pin<mark;pin++)
            {
                flag=119-pin;
                if(flag>right.Row[5])
                {
                  middleline[pin]=right.Col[5]-TrackWild[row]/2;
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
          mark=left_apex.Mark+(115-left.Row[5]);
          row=left.Row[5];
          i=5;
          for(pin=0;pin<mark;pin++)
            {
              flag=119-pin;
              if(flag>left.Row[5])
              {
                middleline[pin]=left.Col[5]+TrackWild[row]/2;
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
            mark=left_apex.Mark+(115-left.Row[5]);
            row=left.Row[5];
            i=5;
            for(pin=0;pin<mark;pin++)
            {
              flag=119-pin;
              if(flag>left.Row[5])
              {
                middleline[pin]=left.Col[5]+TrackWild[row]/2;
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
            mark=right_apex.Mark+(115-right.Row[5]);
            row=right.Row[5];
            i=5;
            for(pin=0;pin<mark;pin++)
            {
              flag=119-pin;
              if(flag>right.Row[5])
              {
                middleline[pin]=right.Col[5]-TrackWild[row]/2;
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
  int row;
  if(Garage_flag==GET_IN) row=70;
  else row=115;
  sum=0;
  for(i=0;i<188;i++)
  {
    if(IMG_DATA[row][i]==BLACK_IMG)
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
    float cross_row_L,cross_row_R;
    float cross_col;
    cross_col=Cross_col();
    cross_row_L=CrossRow_L(75);
    cross_row_R=CrossRow_R(75);
    if(cross_row_L>0.9 && cross_row_R>0.9)
    {
        if(Roundabout_flag!=GET_IN_ROUND && Roundabout_flag!=GET_OUT_ROUND)
        {
            Roundabout_flag=NO_ROUND;
            element_flag=NO_JUGED;
            Roundabout_flag_position=NOROUND;
            return;
        }
    }
//    lcd_showfloat(0,0,cross_row_L,2,2);
//    lcd_showfloat(0,1,cross_row_R,2,2);
//    lcd_showint32(0,2,Roundabout_flag,5);
    switch(Roundabout_flag)
    {
        case NO_ROUND:
            if(cross_row_L>0.9 && cross_row_R<0.9)
            {
                Roundabout_flag_position=ROUND_L;
                Roundabout_flag=ROUND_READY;
                element_flag=NO_JUGED;
            }
            else if(cross_row_L<0.9 && cross_row_R>0.9)
            {
                Roundabout_flag_position=ROUND_R;
                Roundabout_flag=ROUND_READY;
                element_flag=NO_JUGED;
            }
            break;
        case ROUND_READY:
            switch (Roundabout_flag_position)
            {
                case ROUND_L:
                    if(cross_row_L<0.85) Roundabout_flag=GET_IN_READY;
                    element_flag=NO_JUGED;
                    break;
                case ROUND_R:
                    if(cross_row_R<0.85) Roundabout_flag=GET_IN_READY;
                    element_flag=NO_JUGED;
                    break;
                case NOROUND:
                    break;
            }
            break;
        case GET_IN_READY:
            switch (Roundabout_flag_position)
            {
                case ROUND_L:
                    if(cross_row_L>0.85) Roundabout_flag=GET_IN_ROUND;
                    element_flag=IN_JUGED;
                    break;
                case ROUND_R:
                    if(cross_row_R>0.85) Roundabout_flag=GET_IN_ROUND;
                    element_flag=IN_JUGED;
                    break;
                case NOROUND:
                    break;
            }
            break;
        case GET_IN_ROUND:
            break;
        case GET_OUT_ROUND:
            break;
    }
}


//**************************************************************
//* 函数名称： T_Conner_Deal
//* 功能说明： T弯处理函数
//* 函数返回：
//* 备 注：
//**************************************************************
void T_Conner_Deal()
{
    float L_cross,R_cross,Col_cross;
    int star_row;
    Col_cross=Cross_col();
    star_row=(ROW-(int)ROW*Col_cross)+5;
    if(star_row>100) return;
    L_cross=CrossRow_L(star_row);
    R_cross=CrossRow_R(star_row);
    if(element_flag==IN_JUGED)
    {
        if(T_flag==NO_T) return;
    }
    if(T_flag==IN_T)
    {
        if(Col_cross>0.7)
        {
            T_flag=NO_T;
            if(T_go_flag_pin==1) T_go_flag_pin=0;
            else T_go_flag_pin=1;
            element_flag=NO_JUGED;
        }
        else
        {
            //gpio_set(FMQ,1);
            return;
        }
    }
    if(L_cross>0.7 && R_cross>0.7 && Col_cross<0.7 && T_flag==NO_T)
    {
        int row_min,row_max,row,col;
        int sum=0;
        row_min=star_row-10;
        row_max=star_row+8;
        for(col=5;col<COL-5;col+=5)
        {
            for(row=row_max;row>row_min;row--)
            {
                if(row<5||row>115) break;
                if(IMG_DATA[row][col]==WHITE_IMG&&IMG_DATA[row-1][col]==WHITE_IMG)
                {
                    if(IMG_DATA[row-2][col]==BLACK_IMG && IMG_DATA[row-3][col]==BLACK_IMG)
                    {
                        sum++;
                    }
                }
            }
        }
        if(sum>25)
        {
            element_flag=IN_JUGED;
            T_flag=IN_T;
            //gpio_set(FMQ,1);
        }
    }
}

//**************************************************************
//* 函数名称： Cross_deal
//* 功能说明： 十字识别
//* 函数返回：
//* 备 注：
//**************************************************************
//void Cross_deal()
//{
//    float L_cross,R_cross,Col_cross;
//    int T_flag=0;
//    L_cross=CrossRow_L();
//    R_cross=CrossRow_R();
//    Col_cross=Cross_col();
//    if(L_cross>0.85 && R_cross>0.85 && Col_cross>0.85)
//    {
//        T_flag=IN_T;
//    }
//}

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
    int B_L,B_R,B_Top;
    B_L=Black_White_Zone(5,80,25,110);
    B_Top=Black_White_Zone(60,15,115,30);
    B_R=Black_White_Zone(170,80,185,110);
    if(element_flag==IN_JUGED)
    {
        if(Fork_Flag==NO_FORK) return;
    }
    if(Fork_Flag==IN_FORK)
    {
        if(B_Top!=1)
        {
            Fork_Flag=NO_FORK;
            element_flag=NO_JUGED;
            return;
        }
    }
    if(B_Top!=1) return;
    for (i=50;i>30;i--)
    {
        int min=COL/2-20,max=COL/2+20;
        for(j=min;j<max;j++)
        {
            if(IMG_DATA[i][j]==WHITE_IMG && IMG_DATA[i][j+1]==BLACK_IMG)
            {
                forck_L.Row[0]=i;
                forck_L.Col[0]=j+1;
                F_L_flag=1;
            }
            if(IMG_DATA[i][j]==BLACK_IMG && IMG_DATA[i][j+1]==WHITE_IMG)
            {
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
        if(forck_R.Row[0]<forck_L.Row[0]) return;//防误判2
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
    for(row=forck_L.Row[0] - pin;pin<15;pin++)
    {
        find = 0;
        row = forck_L.Row[0] - pin;
        colmin = forck_L.Col[pin - 1] - 10;
        colmax = forck_L.Col[pin - 1] + 10;
        for(col = colmin;col <= colmax;col++)
        {
            if(col < COL - 5 && col > 5)
            {
                if(IMG_DATA[row][col] == WHITE_IMG && IMG_DATA[row][col + 1] == WHITE_IMG)
                {
                    if(IMG_DATA[row][col + 2] == BLACK_IMG && IMG_DATA[row][col + 3] == BLACK_IMG)
                    {
                        forck_L.Row[pin] = row;
                        forck_L.Col[pin] = col + 2;
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
    for(row=right.Row[0] - pin;pin<15;pin++)
    {
        find = 0;
        row = forck_R.Row[0] - pin;
        colmin = forck_R.Col[pin - 1] - 10;
        colmax = forck_R.Col[pin - 1] + 10;
        for(col = colmax;col >= colmin;col--)
        {
            if(col < COL - 5 && col > 5)
            {
                if(IMG_DATA[row][col] == WHITE_IMG && IMG_DATA[row][col - 1] == WHITE_IMG)
                {
                    if(IMG_DATA[row][col - 2] == BLACK_IMG && IMG_DATA[row][col - 3] == BLACK_IMG)
                    {
                        forck_R.Row[pin] = row;
                        forck_R.Col[pin] = col - 2;
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
    if(forck_L.Row[4]==254||forck_L.Row[4]==254) return;
    V_L[0]=forck_L.Row[4]-forck_L.Row[0];
    V_L[1]=forck_L.Col[4]-forck_L.Col[0];
    V_R[0]=forck_R.Row[4]-forck_R.Row[0];
    V_R[1]=forck_R.Col[4]-forck_R.Col[0];
    ang_l=atan(1.0*V_L[0]/V_L[1]);
    ang_r=atan(1.0*V_R[0]/V_R[1]);
    ang_l=ang_l*180/3.14;
    ang_r=ang_r*180/3.14+180;
    ang=ang_r-ang_l;
    if(ang>150 && ang<180)
    {
        float cross_r,cross_l;
        cross_r=CrossRow_R(forck_R.Row[0]-2);
        cross_l=CrossRow_L(forck_L.Row[0]-2);
        if(cross_r>0.7 && cross_l>0.7)
        {
            element_flag=IN_JUGED;
            Fork_Flag=IN_FORK;
        }
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
    if(Garage_flag==GET_OUT)
    {
        element_flag=IN_JUGED;
        if(CorssCol>0.7)
        {
            Garage_flag=NO_GARAGE;
            element_flag=NO_JUGED;
        }
        return;
    }
    else if(Garage_flag!=GET_OUT)
    {
        if(element_flag==IN_JUGED) return;
        int row,col;
        int col_min,col_max;
        int T,T0,count,BM_count;
        T=0,T0=0;
        count=0;
        BM_count=0;
        for(row=110;row>40;row-=5)
        {
            col_min=COL/2-TrackWild[row]/2;
            col_max=COL/2+TrackWild[row]/2;
            for(col=col_min;col<col_max;col++)
            {
                if(IMG_DATA[row][col]==BLACK_IMG && IMG_DATA[row][col+1]==BLACK_IMG && IMG_DATA[row][col+2]==WHITE_IMG && IMG_DATA[row][col+3]==WHITE_IMG)
                {
                    if(T-T0>-5 && T-T0<5) count++;
                    T0=T;
                    T=0;
                }
                else
                {
                    T++;
                }
                if(count>=5)
                {
                    count=0;
                    BM_count++;
                    break;
                }
            }
        }
        if(BM_count>=2 && Garage_flag==NO_GARAGE)
            Garage_flag=FIRST;
        else if(BM_count==0 && Garage_flag==FIRST)
            Garage_flag=READY;
        else if(BM_count>=1 && Garage_flag==READY)
            Garage_flag=GET_IN;
    }
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
void RacingLine_L(int k)
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
            middleline[pin]=left.Col[0]+TrackWild[row]/k;
            col=middleline[pin];
            IMG_DATA[flag][col]=GREEN_IMG;
            mid_row[pin]=flag;
        }
        else
        {
            row=left.Row[i];
            col=left.Col[i]+TrackWild[row]/k;
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
void RacingLine_R(int k)
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
          middleline[pin]=right.Col[0]-TrackWild[row]/k;
          col=middleline[pin];
          mid_row[pin]=flag;
          IMG_DATA[flag][col]=GREEN_IMG;
        }
        else
        {
          row=right.Row[i];
          col=right.Col[i]-TrackWild[row]/k;
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
//* 功能说明： 三叉巡线策略，补右寻左
//* 函数返回：
//* 备 注：
//***************************************************************
void Racing_Line_Fork_L()
{
//    gpio_set(FMQ,1);
//    if(left.Col[0]>right.Col[0]) RacingLine_R(2);
//    else RacingLine_L(5);
//    return;
    int pin,i;
    int row,col;
    int flag;
    int l_flag,r_flag;
    l_flag=0;
    r_flag=0;
    i=0;
    row=forck_L.Row[0];
    gpio_set(FMQ,1);
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

//***************************************************************
//* 函数名称： Racing_Line_Fork_R
//* 功能说明： 三叉巡线策略，补左寻右
//* 函数返回：
//* 备 注：
//***************************************************************
void Racing_Line_Fork_R()
{
//    gpio_set(FMQ,1);
//    if(left.Col[0]>right.Col[0]) RacingLine_R(2);
//    else RacingLine_L(5);
//    return;
    int pin,i;
    int row,col;
    int flag;
    int l_flag,r_flag;
    l_flag=0;
    r_flag=0;
    i=0;
    row=forck_R.Row[0];
    gpio_set(FMQ,1);
    for(pin=0;pin<240;pin++)
    {
        flag=119-pin;
        if(flag>forck_L.Row[0])
        {
            middleline[pin]=forck_R.Col[0]+TrackWild[row]/2;
            col=middleline[pin];
            mid_row[pin]=flag;
            IMG_DATA[flag][col]=GREEN_IMG;
        }
        else
        {
              row=forck_R.Row[i];
              col=forck_R.Col[i]+TrackWild[row]/2;
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

//**************************************************************
//* 函数名称： Raceing_line_T_L
//* 功能说明： T补线，向左
//* 函数返回：
//* 备 注：
//**************************************************************
void Raceing_line_T_L()
{
    int i,pin;
    if(left_flag==0&&right_flag==0)
    {
        right_flag=1;
        left_flag=1;
        for(i=120,pin=0;i>60;i--)
        {
            middleline[pin]=0;
            mid_row[pin]=i;
            pin++;
        }
    }
    else if(left_flag==1)
    {
        RacingLine_L(10);
    }
    else
    {
        RacingLine_R(1);
    }
}

//**************************************************************
//* 函数名称： Raceing_line_T_R
//* 功能说明： T补线向右
//* 函数返回：
//* 备 注：
//**************************************************************
void Raceing_line_T_R()
{
    int i,pin;
    if(left_flag==0&&right_flag==0)
    {
        right_flag=1;
        left_flag=1;
        for(i=120,pin=0;i>60;i--)
        {
            middleline[pin]=COL;
            mid_row[pin]=i;
            pin++;
        }
    }
    else if(right_flag==1)
    {
        RacingLine_R(10);
    }
    else
    {
        RacingLine_L(1);
    }
}

//**************************************************************
//* 函数名称： Raceing_line_G_L
//* 功能说明： 出入车库（向左）
//* 函数返回：
//* 备 注：
//**************************************************************
void Raceing_line_G_L()
{
    int i,pin;
    if(Garage_flag==GET_IN)chasu_k=1;
    if(left_flag==0 || Garage_flag==GET_IN)
    {
        right_flag=1;
        left_flag=1;
        for(i=120,pin=0;i>60;i--)
        {
            middleline[pin]=0;
            mid_row[pin]=i;
            pin++;
        }
    }
    else if(left_flag==1)
    {
        RacingLine_L(10);
    }
}

//**************************************************************
//* 函数名称： Raceing_line_G_R
//* 功能说明： 出入车库（向右）
//* 函数返回：
//* 备 注：
//**************************************************************
void Raceing_line_G_R()
{
    int i,pin;
    if(Garage_flag==GET_IN)chasu_k=1;
    if(right_flag==0 || Garage_flag==GET_IN)
    {
        right_flag=1;
        left_flag=1;
        for(i=120,pin=0;i>60;i--)
        {
            middleline[pin]=188;
            mid_row[pin]=i;
            pin++;
        }
    }
    else if(right_flag==1)
    {
        RacingLine_R(10);
    }
}

void Raceing_line_RoundIn_L()
{
    int pin,i;
    int row,col;
    int flag;
    int l_flag,r_flag;
    l_flag=0;
    r_flag=0;
    i=left_apex.Mark;
    if(left_flag==0&&right_flag==0)
    {
        return;
    }
    row=left_apex.Apex_Row;
    for(pin=0;pin<240;pin++)
    {
        if(right.Row[i]==254) break;
        flag=119-pin;
        if(flag>left_apex.Apex_Row)
        {
            middleline[pin]=left_apex.Apex_Col+TrackWild[row]/10;
            col=middleline[pin];
            mid_row[pin]=flag;
            IMG_DATA[flag][col]=GREEN_IMG;
        }
        else
        {
            row=right.Row[i];
            col=right.Col[i]+TrackWild[row]/10;
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

//**************************************************************
//* 函数名称： Raceing_line_RoundIn_R
//* 功能说明： 右环岛进入补线
//* 函数返回：
//* 备 注：
//**************************************************************
void Raceing_line_RoundIn_R()
{
    int pin,i;
    int row,col;
    int flag;
    int l_flag,r_flag;
    l_flag=0;
    r_flag=0;
    i=right_apex.Mark;
    if(left_flag==0&&right_flag==0)
    {
        return;
    }
    row=right_apex.Apex_Row;
    for(pin=0;pin<240;pin++)
    {
        if(right.Row[i]==254) break;
        flag=119-pin;
        if(flag>right_apex.Apex_Row)
        {
            middleline[pin]=right_apex.Apex_Col-TrackWild[row]/10;
            col=middleline[pin];
            mid_row[pin]=flag;
            IMG_DATA[flag][col]=GREEN_IMG;
        }
        else
        {
            row=right.Row[i];
            col=right.Col[i]-TrackWild[row]/10;
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

//**************************************************************
//* 函数名称： Raceing_line_RoundIn_L
//* 功能说明： 左环岛离开补线
//* 函数返回：
//* 备 注：
//**************************************************************
void Raceing_line_RoundOUT_L()
{
    RacingLine_R(2);
}

//**************************************************************
//* 函数名称： Raceing_line_RoundIn_L
//* 功能说明： 右环岛离开补线
//* 函数返回：
//* 备 注：
//**************************************************************
void Raceing_line_RoundOUT_R()
{
    RacingLine_L(2);
}

void RaceLine()
{
    if(Fork_Flag==IN_FORK)
    {
        if(fork_turn==1)
            Racing_Line_Fork_L();
        else if(fork_turn==2)
            Racing_Line_Fork_R();
    }
    else if(T_flag==IN_T)
    {
        gpio_set(FMQ,1);
        if(T_go_flag[T_go_flag_pin]==0)
            Raceing_line_T_R();//往后需要添加额外判断，向左或者向右
        else if(T_go_flag[T_go_flag_pin])
            Raceing_line_T_L();
    }
    else if(Garage_flag==GET_OUT) Raceing_line_G_L();
    else if(Garage_flag==GET_IN) Raceing_line_G_L();
    else if(Roundabout_flag==GET_IN_ROUND)
    {
        stop=1;
    }
    else if(Roundabout_flag==GET_OUT_ROUND)
    {
        stop=1;
    }
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
    if(stop==1)
    {
        Garage_flag=GET_OUT;
        return;
    }
    Deal_Init();
    InitData();
    deal_flag=1;
    CorssCol=Cross_col();
    if(CorssCol<0.1) CorssCol=0.1;
    protect();
	LeftStartFind() ;
	RightStartFind();
	fixpoint();
	left_jump();
	right_jump();
    Left_Apex();
    Right_Apex();
    //Angle_IMG();
    Garage_Deal();
    Fork_Deal();
    T_Conner_Deal();
    //Roundabout_Deal();
    RaceLine();
	//seekfree_sendimg_03x(UART_2,IMG_DATA,188,120);//传输处理后图像
	//Track();
    //lcd_displayimage032_zoom((uint8)IMG_DATA,188,120,160,128);//图像显示
	deal_flag=0;
}
