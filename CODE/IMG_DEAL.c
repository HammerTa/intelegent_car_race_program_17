#include "IMG_DEAL.h"
#define  IMG_DATA    mt9v03x_image
 
//----------------变量定义区--------------
 
int  dj_end;
unsigned char stop=0;//停车标志位，置1表示停车
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
struct DIV 
{ 
	int Row[240]; 
	int Col[240]; 
};
struct DIV RacingLine;//目标线路
struct DIV left,right;//左右边线
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
struct APEX left_apex,right_apex; 



struct AG right_ag,left_ag;//寻找弯心所需变量

int right_line[120] = {160};//此处需要改成159，但不知道为何运行没出问题，可能是没遇到只有左单边的情况，去学校再跑一下车试试

int left_line[120] = {0};
int max = 0;
int min  = 0;
int line[120] = {0};
int Threshold;


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
3,3,3,3,3,3,3,3,3,3,//11-20
4,4,4,4,4,4,4,4,4,4,//21-30
6,6,6,6,6,6,6,6,6,6,//31-40
2,2,2,2,2,2,2,2,2,2,//41-50
1,1,1,1,1,1,1,1,1,1,//0-10
0,0,0,0,0,0,0,0,0,0,//51-60
0,0,0,0,0,0,0,0,0,0,//61-70
0,0,0,0,0,0,0,0,0,0,//71-80
0,0,0,0,0,0,0,0,0,0,//81-90
0,0,0,0,0,0,0,0,0,0,//91-100
0,0,0,0,0,0,0,0,0,0,//101-110
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
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,//111-120  
};

//--------------------------
int TrackWild[120]={
1,  2,  4,  6,  8, 10, 12, 13, 15, 17, 
19,21, 22, 24, 26, 28, 30, 32, 33,35,
37, 39, 41, 42, 44, 46, 48, 50, 
52, 53, 55, 57, 59, 61, 62, 64, 66,
68, 70, 72, 73, 75, 77, 79, 81, 82, 
84, 86, 88, 90, 92, 93, 95, 97, 99,
101,103, 104,106,108,110,112,113,115,  
117,119,121,123,124,126,128,130,132, 
133,135,137,139,141,143,144,146,148,
150,152,153,155,157,159,161,163,164,
166,168,170,172, 173,175,177,179,181,  
183,184,186,188,190,192,194,195,197,  
199,201,203,204,206,208,210,212,214,215,217};


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
    Threshold=GetOSTU(mt9v03x_image);
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
                RacingLine.Row[pin]=254;
                RacingLine.Col[pin]=254;
                middleline[pin]=254;
                middleline_l[pin]=0;
                middleline_r[pin]=160;
	}  
    	left_apex.Mark=254;
	right_apex.Mark=254;
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
      if(IMG_DATA[row][col]==BLACK && IMG_DATA[row][col+1]==BLACK)
      {
        if(IMG_DATA[row][col+2]==WHITE && IMG_DATA[row][col+3]==WHITE)
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
      if(IMG_DATA[row][col]==BLACK && IMG_DATA[row][col-1]==BLACK)
      {
        if(IMG_DATA[row][col-2]==WHITE && IMG_DATA[row][col-3]==WHITE)
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
				if(IMG_DATA[row][col] == BLACK && IMG_DATA[row][col + 1] == BLACK)
				{
					if(IMG_DATA[row][col + 2] == WHITE && IMG_DATA[row][col + 3] == WHITE)
					{
						left.Row[pin] = row;
						left.Col[pin] = col + 1;
						IMG_DATA[row][col + 1] = RED_IMG;
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
			if(IMG_DATA[row][col] == BLACK)
			{
				row_temp = row;
				col_temp = col;
				for(j = 8;j < 12;j++)
				{
					row_temp = row_temp + rowcheack[j];
					col_temp = col_temp + colcheack[j];
					if(IMG_DATA[row_temp][col_temp] == WHITE)
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
				if(IMG_DATA[row][col] == BLACK && IMG_DATA[row][col - 1] == BLACK)
				{
					if(IMG_DATA[row][col - 2] == WHITE && IMG_DATA[row][col - 3] == WHITE)
					{
						right.Row[pin] = row;
						right.Col[pin] = col - 1;
						IMG_DATA[row][col - 1] = BLUE;
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
			if(IMG_DATA[row][col] == BLACK)
			{
				row_temp = row;
				col_temp = col;
				for(j = 8;j < 12;j++)
				{
					row_temp = row_temp + rowcheack[j];
					col_temp = col_temp + colcheack[j];
					if(IMG_DATA[row_temp][col_temp] == WHITE)
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
                  IMG_DATA[flag][col]=GREEN_IMG;
                }
                else
                {

                  row=right.Row[i];
                  col=right.Col[i]-TrackWild[row]/2;
                  middleline[pin]=col;
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
              }
              else
              {
                row=left.Row[i];
                col=left.Col[i]+TrackWild[row]/2;
                middleline[pin]=col;
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
        if(left_apex.Mark>right_apex.Mark)
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
              }
              else
              {
                row=left.Row[i];
                col=left.Col[i]+TrackWild[row]/2;
                middleline[pin]=col; 
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
                  IMG_DATA[flag][col]=GREEN_IMG;
              }
              else
              {
                row=right.Row[i];
                col=right.Col[i]-TrackWild[row]/2;
                middleline[pin]=col; 
                i++;
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
//* 函数名称： void data_collect_display()
//* 功能说明： 将摄像头采集到的图像显示到液晶上
//* 函数返回： 无
//* 备 注： oled实际显示像素128*64  图像大小COL*ROW 
//  液晶函数尚未移植，不可用
//***************************************************************
void Data_collect_display(void)
{
    uint8 x,y,i;
    uint8 p[128];
    uint16 z=0;
    for(y=0;y<=7;y++)
    {
//        OLED_Set_Pos(0,y);
        for(x=0;x<=127;x++)
        {
            p[x]=0;
            
            for(i=8;i>0;i--)
            {
                p[x]=(p[x]<<1)&0xffu;
                if(IMG_DATA[(i-1+8*y)*(120-1)/63][z*(160-1)/127]==255)
                { 
                  
                    p[x]|=0x01u;
                }
            } 
            z++;

        }    
        z=0;
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
  for(i=0;i<160;i++)
  {
    if(IMG_DATA[115][i]==BLACK)
      sum++;
  }
  SUM=sum;
  if(sum==118)
    stop=1;
  return;
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
//***************************************************************
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
    //==================以下函数需要自己改写=====================
    protect();
	LeftStartFind() ;
	RightStartFind();
	fixpoint();
	left_jump();
	right_jump();
    Left_Apex();
    Right_Apex();
	Racing_Line();
    //wild();
    //赛道宽度检测在非必要时必须关闭
 
  //Data_collect_display();     //液晶显示黑白图像，车子跑起来的时候关掉（失效）

}
