
#include "Fuzzy.h"

#define FMAX    100 //����ֵ������ֵ
//int PFF[4] = { 0, 20, 40, 60 };
//int DFF[4] = { 0, 5, 10, 15 };
int PFF[7] = { -80,-60,-30,0,30,60,80};//error
int DFF[5] = { -20, -8, 0, 8, 20 };     //deriv error
int UFF[7] = {  0, 25, 40,55,70,90,120};//����

/*�����˵������ӵĹ����,�����ʱƫ�����,С���ʱƫ�����仯*/  


//int rule[7][5]={
//  //  ecС��-8Ϊ-2  ec����-8С��0Ϊ-1  ec����0С��8Ϊ0    ec>8Ϊ1 
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
  { 2,  2,  0,  2,  2}, //   1    //  ���ĵ㼴Ϊ����Ϊ0 0 ���������ʱΪUFF��1��
  { 0,  1,  2,  2,  3}, //   2   
  { 1,  2,  4,  4,  5}, //   3    
  { 2,  3,  5,  6,  6}  //   
};
int    U;           /*ƫ��,ƫ��΢���Լ����ֵ�ľ�ȷ��*/
unsigned int   PF[2]={0}, DF[2]={0}, UF[4]={0};   /*ƫ��,ƫ��΢���Լ����ֵ��������*/
int    Pn, Dn, Un[4];
float   temp1, temp2;
float  a1=0,a2=0,a3=0,a4=0;
/**********************************************************/
float   Fuzzy(float P, float D)   /*ģ����������*/
{
	/*�����ȵ�ȷ��*/
	/*����PD��ָ������ֵ�����Ч������*/
	if (P>PFF[0] && P<PFF[6])
	{
		if (P <= PFF[1])
		{
			Pn = -2;
			PF[0] = (int)(FMAX*((float)(PFF[1] - P) / (PFF[1] - PFF[0])));//��pֵռ������ı�����Ϊ�ڸ�����ĸ���
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
	PF[1] = FMAX - PF[0];//��pֵ������������ĸ���
	
	
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
	/*ʹ����Χ�Ż���Ĺ����rule[7][7]*/
	/*���ֵʹ��13����������,����ֵ��UFF[7]ָ��*/
	/*һ�㶼���ĸ�������Ч*/
	Un[0] = rule[Pn - 1 + 3][Dn - 1 + 3];
	Un[1] = rule[Pn + 3][Dn - 1 + 3];
	Un[2] = rule[Pn - 1 + 3][Dn + 3];
	Un[3] = rule[Pn + 3][Dn + 3];
	if (PF[0] <= DF[0])
		UF[0] = PF[0];//Un[0]ȡֵ�ĸ���Ӧ��Ϊp��������ĸ��ʺ�d���ڵ�������ʵ���Сֵ
	else
		UF[0] = DF[0];
	
	if (PF[1] <= DF[0])
		UF[1] = PF[1];//Un[1]ȡֵ�ĸ���Ӧ��Ϊp�������������������ұ����򣩵ĸ��ʺ�d���ڵ�������ʵ���Сֵ
	else
		UF[1] = DF[0];
	
	if (PF[0] <= DF[1])
		UF[2] = PF[0];//Un[2]ȡֵ�ĸ���Ӧ��Ϊp��������ĸ��ʺ�d�������ڵ������������ұ����򣩸��ʵ���Сֵ
	else
		UF[2] = DF[1];
	
	if (PF[1] <= DF[1])  //Un[3]ȡֵ�ĸ���Ӧ��Ϊp�������������������ұ����򣩵ĸ��ʺ�d�������ڵ������������ұ����򣩸��ʵ���Сֵ
		UF[3] = PF[1];
	else
		UF[3] = DF[1];
	/*ͬ���������������ֵ���*/

	if (Un[0] == Un[1])
	{
		if (UF[0] > UF[1])//UF[0] > UF[1]��Ϊ��Un[0]�ĸ��ʴ���Un[1]�ĸ���
			UF[1] = 0;//����С������
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
	U = (int)(temp1 / temp2);//��Ȩƽ��
	return U;
}


/********************���ģ��*****************************/
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

int    Motor_U;           /*ƫ��,ƫ��΢���Լ����ֵ�ľ�ȷ��*/
unsigned int   Motor_PF[2]={0}, Motor_DF[2]={0}, Motor_UF[4]={0};   /*ƫ��,ƫ��΢���Լ����ֵ��������*/
int    Motor_Pn, Motor_Dn, Motor_Un[4];
float   Motor_temp1, Motor_temp2;
float  Motor_a1=0,Motor_a2=0,Motor_a3=0,Motor_a4=0;



int  Motor_Fuzzy(int P, int D)   /*ģ����������*/
{
	/*�����ȵ�ȷ��*/
	/*����PD��ָ������ֵ�����Ч������*/
	if (P>Motor_PFF[0] && P<Motor_PFF[6])
	{
		if (P <= Motor_PFF[1])
		{
			Motor_Pn = -2;
			Motor_PF[0] = (int)((float)(Motor_PFF[1] - P) / (Motor_PFF[1] - Motor_PFF[0]));//��pֵռ������ı�����Ϊ�ڸ�����ĸ���
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
	Motor_PF[1] = 1 - Motor_PF[0];//��pֵ������������ĸ���
	
/***********����DDDDDDDDD*****************/	
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
	/*ʹ����Χ�Ż���Ĺ����rule[7][7]*/
	/*���ֵʹ��13����������,����ֵ��UFF[7]ָ��*/
	/*һ�㶼���ĸ�������Ч*/
	Motor_Un[0] = Motor_rule[Motor_Pn - 1 + 3][Motor_Dn - 1 + 3];
	Motor_Un[1] = Motor_rule[Motor_Pn + 3][Motor_Dn - 1 + 3];
	Motor_Un[2] = Motor_rule[Motor_Pn - 1 + 3][Motor_Dn + 3];
	Motor_Un[3] = Motor_rule[Motor_Pn + 3][Motor_Dn + 3];
	if (Motor_PF[0] <= Motor_DF[0])
		Motor_UF[0] = Motor_PF[0];//Un[0]ȡֵ�ĸ���Ӧ��Ϊp��������ĸ��ʺ�d���ڵ�������ʵ���Сֵ
	else
		Motor_UF[0] = Motor_DF[0];
	
	if (Motor_PF[1] <= Motor_DF[0])
		Motor_UF[1] = Motor_PF[1];//Un[1]ȡֵ�ĸ���Ӧ��Ϊp�������������������ұ����򣩵ĸ��ʺ�d���ڵ�������ʵ���Сֵ
	else
		Motor_UF[1] = Motor_DF[0];
	
	if (Motor_PF[0] <= Motor_DF[1])
		Motor_UF[2] = Motor_PF[0];//Un[2]ȡֵ�ĸ���Ӧ��Ϊp��������ĸ��ʺ�d�������ڵ������������ұ����򣩸��ʵ���Сֵ
	else
		Motor_UF[2] = Motor_DF[1];
	
	if (Motor_PF[1] <= Motor_DF[1])  //Un[3]ȡֵ�ĸ���Ӧ��Ϊp�������������������ұ����򣩵ĸ��ʺ�d�������ڵ������������ұ����򣩸��ʵ���Сֵ
		Motor_UF[3] = Motor_PF[1];
	else
		Motor_UF[3] = Motor_DF[1];
	/*ͬ���������������ֵ���*/

	if (Motor_Un[0] == Motor_Un[1])
	{
		if (Motor_UF[0] > Motor_UF[1])//UF[0] > UF[1]��Ϊ��Un[0]�ĸ��ʴ���Un[1]�ĸ���
			Motor_UF[1] = 0;//����С������
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
	Motor_U = (int)(Motor_temp1 / Motor_temp2);//��Ȩƽ��
	return Motor_U;
}

