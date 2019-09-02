
#include "Fuzzy.h"

#define FMAX    100 //语言值得满赋值
//int PFF[4] = { 0, 20, 40, 60 };
//int DFF[4] = { 0, 5, 10, 15 };
int PFF[7] = { -80,-60,-30,0,30,60,80};//error
int DFF[5] = { -20, -8, 0, 8, 20 };     //deriv error
int UFF[7] = {  0, 25, 40,55,70,90,120};//不改

/*采用了调整因子的规则表,大误差时偏重误差,小误差时偏重误差变化*/  


//int rule[7][5]={
//  //  ec小于-8为-2  ec大于-8小于0为-1  ec大于0小于8为0    ec>8为1 
////  -2  -1   0   1    ec    e 
//  { 6,  5,  4,  3,  3}, //  -2 e<-45
//  { 5,  4,  4,  3,  3}, //  -1   -45<e<-20
//  { 3,  2,  2,  1,  1}, //  0  -20<e<-0
//  { 1,  0,  0,  0,  1}, //   1    0<e<20
//  { 1,  1,  2,  2,  3}, //   2    20<e<45
//  { 3,  3,  4,  4,  5}, //   3    45<e
//  { 3,  3,  4,  5,  6}  //   
//};
//int rule[7][5]={ 
////  -2  -1   0   1    ec    e 
//  { 6,  5,  5,  4,  4}, //  -2 
//  { 5,  4,  3,  3,  2}, //  -1   
//  { 3,  2,  1,  1,  0}, //  0  
//  { 1,  0,  0,  0,  1}, //   1    
//  { 0,  1,  1,  2,  3}, //   2   
//  { 2,  3,  3,  4,  5}, //   3    
//  { 4,  4,  5,  5,  6}  //   
//};
int rule[7][5]={ 
//  -2  -1   0   1    ec    e 
  { 6,  6,  5,  3,  2}, //  -2 
  { 5,  4,  4,  2,  1}, //  -1   
  { 3,  2,  2,  1,  0}, //  0  
  { 2,  2,  0,  2,  2}, //   1    //  中心点即为输入为0 0 的输出，此时为UFF【1】
  { 0,  1,  2,  2,  3}, //   2   
  { 1,  2,  4,  4,  5}, //   3    
  { 2,  3,  5,  6,  6}  //   
};
int    U;           /*偏差,偏差微分以及输出值的精确量*/
unsigned int   PF[2]={0}, DF[2]={0}, UF[4]={0};   /*偏差,偏差微分以及输出值的隶属度*/
int    Pn, Dn, Un[4];
float   temp1, temp2;
float  a1=0,a2=0,a3=0,a4=0;
/**********************************************************/
float   Fuzzy(float P, float D)   /*模糊运算引擎*/
{
	/*隶属度的确定*/
	/*根据PD的指定语言值获得有效隶属度*/
	if (P>PFF[0] && P<PFF[6])
	{
		if (P <= PFF[1])
		{
			Pn = -2;
			PF[0] = (int)(FMAX*((float)(PFF[1] - P) / (PFF[1] - PFF[0])));//求p值占该论域的比例即为在该论域的概率
		}
		else if (P <=PFF[2])
		{
			Pn = -1;
			PF[0] = (int)(FMAX*((float)(PFF[2] - P) / (PFF[2] - PFF[1])));
		}
		else if (P <= PFF[3])
		{
			Pn = 0;
			PF[0] = (int)(FMAX*((float)(PFF[3] - P) / (PFF[3] - PFF[2])));
		}
		else if (P <= PFF[4])
		{
			Pn = 1; 
			PF[0] = (int)(FMAX*((float)(PFF[4] - P) / (PFF[4] - PFF[3])));
		}
		else if (P <= PFF[5])
		{
			Pn = 2; 
			PF[0] = (int)(FMAX*((float)(PFF[5] - P) / (PFF[5] - PFF[4])));
		}
		else if (P <= PFF[6])
		{
			Pn = 3; 
			PF[0] =(int)( FMAX*((float)(PFF[6] - P) / (PFF[6] - PFF[5])));
		}
	}
	else if (P <= PFF[0])
	{
	  Pn = -2;  
	  PF[0] = FMAX;
	}
	else if (P >= PFF[6])
	{
		Pn = 3;   PF[0] = 0;
	}
	PF[1] = FMAX - PF[0];//求p值不在所在论域的概率
	
	
	if (D > DFF[0] && D < DFF[4])//-10~10
	{
	  if (D <=DFF[1])//-10~-5
	  {
	    Dn = -2; 
	    DF[0] = (int)(FMAX*((float)(DFF[1] - D) / (DFF[1] - DFF[0])));
	  }
	  else if (D <= DFF[2])//-5~-0
	  {
	    Dn = -1;
	    DF[0] =(int)( FMAX*((float)(DFF[2] - D) / (DFF[2] - DFF[1])));
	  }
	  else if (D <= DFF[3])//0~5
	  {
	    Dn = 0;
	    DF[0] =(int)( FMAX*((float)(DFF[3] - D) / (DFF[3] - DFF[2])));
	  }
	  else if (D <= DFF[4])//5~10
	  {
	    Dn = 1;
	    DF[0] =(int)( FMAX*((float)(DFF[4] - D) / (DFF[4] - DFF[3])));
	  }
	}
	else if (D <= DFF[0])
	{
	  Dn = -2;
	  DF[0] = FMAX;
	}
	else if (D >= DFF[4])
	{
	  Dn = 1;
	  DF[0] = 0;
	}
	DF[1] = FMAX - DF[0];
	/*使用误差范围优化后的规则表rule[7][7]*/
	/*输出值使用13个隶属函数,中心值由UFF[7]指定*/
	/*一般都是四个规则有效*/
	Un[0] = rule[Pn - 1 + 3][Dn - 1 + 3];
	Un[1] = rule[Pn + 3][Dn - 1 + 3];
	Un[2] = rule[Pn - 1 + 3][Dn + 3];
	Un[3] = rule[Pn + 3][Dn + 3];
	if (PF[0] <= DF[0])
		UF[0] = PF[0];//Un[0]取值的概率应该为p所在论域的概率和d所在的论域概率的最小值
	else
		UF[0] = DF[0];
	
	if (PF[1] <= DF[0])
		UF[1] = PF[1];//Un[1]取值的概率应该为p不在所在论域（在相邻右边论域）的概率和d所在的论域概率的最小值
	else
		UF[1] = DF[0];
	
	if (PF[0] <= DF[1])
		UF[2] = PF[0];//Un[2]取值的概率应该为p所在论域的概率和d不在所在的论域（在相邻右边论域）概率的最小值
	else
		UF[2] = DF[1];
	
	if (PF[1] <= DF[1])  //Un[3]取值的概率应该为p不在所在论域（在相邻右边论域）的概率和d不在所在的论域（在相邻右边论域）概率的最小值
		UF[3] = PF[1];
	else
		UF[3] = DF[1];
	/*同隶属函数输出语言值求大*/

	if (Un[0] == Un[1])
	{
		if (UF[0] > UF[1])//UF[0] > UF[1]即为在Un[0]的概率大于Un[1]的概率
			UF[1] = 0;//概率小的清零
		else
			UF[0] = 0;
	}
	if (Un[0] == Un[2])
	{
		if (UF[0] > UF[2])
			UF[2] = 0;
		else
			UF[0] = 0;
	}
	if (Un[0] == Un[3])
	{
		if (UF[0] > UF[3])
			UF[3] = 0;
		else
			UF[0] = 0;
	}
	if (Un[1] == Un[2])
	{
		if (UF[1] > UF[2])
			UF[2] = 0;
		else
			UF[1] = 0;
	}
	if (Un[1] == Un[3])
	{
		if (UF[1] > UF[3])
			UF[3] = 0;
		else
			UF[1] = 0;
	}
	if (Un[2] == Un[3])
	{
		if (UF[2] > UF[3])
			UF[3] = 0;
		else
			UF[2] = 0;
	}

	a1=UF[0]*UFF[Un[0]];  
	a2=UF[1]*UFF[Un[1]];
	a3=UF[2]*UFF[Un[2]];
	a4=UF[3]*UFF[Un[3]];

	temp1 = a1+a2+a3+a4;
	temp2 = UF[0] + UF[1] + UF[2] + UF[3];
	U = (int)(temp1 / temp2);//加权平均
	return U;
}


/********************电机模糊*****************************/
int Motor_PFF[7] = { -100,-60,-30,0,30,60,100};
int Motor_DFF[5] = { -30, -10, 0, 10,30 };
int Motor_UFF[7] = {  0, 10, 15,20,25,30,40};

int Motor_rule[7][5]={ 
//  -2  -1   0   1    ec    e 
  { 6,  6,  5,  3,  2}, //  -2 
  { 5,  4,  4,  2,  1}, //  -1   
  { 3,  2,  2,  1,  0}, //  0  
  { 2,  2,  1,  0,  0}, //   1    
  { 0,  1,  2,  2,  3}, //   2   
  { 1,  2,  4,  4,  5}, //   3    
  { 2,  3,  5,  6,  6}  //   
};

int    Motor_U;           /*偏差,偏差微分以及输出值的精确量*/
unsigned int   Motor_PF[2]={0}, Motor_DF[2]={0}, Motor_UF[4]={0};   /*偏差,偏差微分以及输出值的隶属度*/
int    Motor_Pn, Motor_Dn, Motor_Un[4];
float   Motor_temp1, Motor_temp2;
float  Motor_a1=0,Motor_a2=0,Motor_a3=0,Motor_a4=0;



int  Motor_Fuzzy(int P, int D)   /*模糊运算引擎*/
{
	/*隶属度的确定*/
	/*根据PD的指定语言值获得有效隶属度*/
	if (P>Motor_PFF[0] && P<Motor_PFF[6])
	{
		if (P <= Motor_PFF[1])
		{
			Motor_Pn = -2;
			Motor_PF[0] = (int)((float)(Motor_PFF[1] - P) / (Motor_PFF[1] - Motor_PFF[0]));//求p值占该论域的比例即为在该论域的概率
		}
		else if (P <=Motor_PFF[2])
		{
			Motor_Pn = -1;
			Motor_PF[0] = (int)((float)(Motor_PFF[2] - P) / (Motor_PFF[2] - Motor_PFF[1]));
		}
		else if (P <= Motor_PFF[3])
		{
			Motor_Pn = 0;
			Motor_PF[0] = (int)((float)(Motor_PFF[3] - P) / (Motor_PFF[3] - Motor_PFF[2]));
		}
		else if (P <= Motor_PFF[4])
		{
			Motor_Pn = 1; 
			Motor_PF[0] = (int)((float)(Motor_PFF[4] - P) / (Motor_PFF[4] - Motor_PFF[3]));
		}
		else if (P <= Motor_PFF[5])
		{
			Motor_Pn = 2; 
			Motor_PF[0] = (int)((float)(Motor_PFF[5] - P) / (Motor_PFF[5] - Motor_PFF[4]));
		}
		else if (P <= Motor_PFF[6])
		{
			Motor_Pn = 3; 
			Motor_PF[0] =(int)((float)(Motor_PFF[6] - P) / (Motor_PFF[6] - Motor_PFF[5]));
		}
	}
	else if (P <= Motor_PFF[0])
	{
	  Motor_Pn = -2;  
	  Motor_PF[0] = 1;
	}
	else if (P >= Motor_PFF[6])
	{
		Motor_Pn = 3;   Motor_PF[0] = 0;
	}
	Motor_PF[1] = 1 - Motor_PF[0];//求p值不在所在论域的概率
	
/***********计算DDDDDDDDD*****************/	
	if (D > Motor_DFF[0] && D < Motor_DFF[4])//-10~10
	{
	  if (D <=Motor_DFF[1])//-10~-5
	  {
	    Motor_Dn = -2; 
	    Motor_DF[0] = (int)((float)(Motor_DFF[1] - D) / (Motor_DFF[1] - Motor_DFF[0]));
	  }
	  else if (D <= Motor_DFF[2])//-5~-0
	  {
	    Motor_Dn = -1;
	    Motor_DF[0] = (int)((float)(Motor_DFF[2] - D) / (Motor_DFF[2] - Motor_DFF[1]));
	  }
	  else if (D <= Motor_DFF[3])//0~5
	  {
	    Motor_Dn = 0;
	    Motor_DF[0] = (int)((float)(Motor_DFF[3] - D) / (Motor_DFF[3] - Motor_DFF[2]));
	  }
	  else if (D <= Motor_DFF[4])//5~10
	  {
	    Motor_Dn = 1;
	    Motor_DF[0] = (int)((float)(Motor_DFF[4] - D) / (Motor_DFF[4] - Motor_DFF[3]));
	  }
	}
	else if (D <= Motor_DFF[0])
	{
	  Motor_Dn = -2;
	  Motor_DF[0] = 1;
	}
	else if (D >= Motor_DFF[4])
	{
	  Motor_Dn = 1;
	  Motor_DF[0] = 0;
	}
	Motor_DF[1] = 1 - Motor_DF[0];
	/*使用误差范围优化后的规则表rule[7][7]*/
	/*输出值使用13个隶属函数,中心值由UFF[7]指定*/
	/*一般都是四个规则有效*/
	Motor_Un[0] = Motor_rule[Motor_Pn - 1 + 3][Motor_Dn - 1 + 3];
	Motor_Un[1] = Motor_rule[Motor_Pn + 3][Motor_Dn - 1 + 3];
	Motor_Un[2] = Motor_rule[Motor_Pn - 1 + 3][Motor_Dn + 3];
	Motor_Un[3] = Motor_rule[Motor_Pn + 3][Motor_Dn + 3];
	if (Motor_PF[0] <= Motor_DF[0])
		Motor_UF[0] = Motor_PF[0];//Un[0]取值的概率应该为p所在论域的概率和d所在的论域概率的最小值
	else
		Motor_UF[0] = Motor_DF[0];
	
	if (Motor_PF[1] <= Motor_DF[0])
		Motor_UF[1] = Motor_PF[1];//Un[1]取值的概率应该为p不在所在论域（在相邻右边论域）的概率和d所在的论域概率的最小值
	else
		Motor_UF[1] = Motor_DF[0];
	
	if (Motor_PF[0] <= Motor_DF[1])
		Motor_UF[2] = Motor_PF[0];//Un[2]取值的概率应该为p所在论域的概率和d不在所在的论域（在相邻右边论域）概率的最小值
	else
		Motor_UF[2] = Motor_DF[1];
	
	if (Motor_PF[1] <= Motor_DF[1])  //Un[3]取值的概率应该为p不在所在论域（在相邻右边论域）的概率和d不在所在的论域（在相邻右边论域）概率的最小值
		Motor_UF[3] = Motor_PF[1];
	else
		Motor_UF[3] = Motor_DF[1];
	/*同隶属函数输出语言值求大*/

	if (Motor_Un[0] == Motor_Un[1])
	{
		if (Motor_UF[0] > Motor_UF[1])//UF[0] > UF[1]即为在Un[0]的概率大于Un[1]的概率
			Motor_UF[1] = 0;//概率小的清零
		else
			Motor_UF[0] = 0;
	}
	if (Motor_Un[0] == Motor_Un[2])
	{
		if (Motor_UF[0] > Motor_UF[2])
			Motor_UF[2] = 0;
		else
			Motor_UF[0] = 0;
	}
	if (Motor_Un[0] == Motor_Un[3])
	{
		if (Motor_UF[0] > Motor_UF[3])
			Motor_UF[3] = 0;
		else
			Motor_UF[0] = 0;
	}
	if (Motor_Un[1] == Motor_Un[2])
	{
		if (Motor_UF[1] > Motor_UF[2])
			Motor_UF[2] = 0;
		else
			Motor_UF[1] = 0;
	}
	if (Motor_Un[1] == Motor_Un[3])
	{
		if (Motor_UF[1] > Motor_UF[3])
			Motor_UF[3] = 0;
		else
			Motor_UF[1] = 0;
	}
	if (Motor_Un[2] == Motor_Un[3])
	{
		if (Motor_UF[2] > Motor_UF[3])
			Motor_UF[3] = 0;
		else
			Motor_UF[2] = 0;
	}

	Motor_a1=Motor_UF[0]*Motor_UFF[Motor_Un[0]];  
	Motor_a2=Motor_UF[1]*Motor_UFF[Motor_Un[1]];
	Motor_a3=Motor_UF[2]*Motor_UFF[Motor_Un[2]];
	Motor_a4=Motor_UF[3]*Motor_UFF[Motor_Un[3]];

	Motor_temp1 = Motor_a1+Motor_a2+Motor_a3+Motor_a4;
	Motor_temp2 = Motor_UF[0] + Motor_UF[1] + Motor_UF[2] + Motor_UF[3];
	Motor_U = (int)(Motor_temp1 / Motor_temp2);//加权平均
	return Motor_U;
}

