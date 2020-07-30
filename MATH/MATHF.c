#include "stdlib.h"
#include "MATHF.h"
#include "math.h"
//累加函数
//*p数组
double sum(double *p,int size)
{
	double result=0;
	int c=0;
	for(c=0;c<size;c++)
		result+=p[c];
	return result;
}
//求平均值
double AVG(double *p,int size)
{
	double result=0,count=0;
	count=sum(p,size);
	result=count/size;
	return result;
}
//求均方根值
double RMS(double *p,int size)
{
	double result=0;
	int c=0;
	for(c=0;c<size;c++)
	{
		result+=p[c]*p[c];
	}
	result=sqrt(result);
	return result;
}
//求最大值
double MAX(double *p,int size)
{
	double result=0;
	int c=0;
	result=p[0];
	for(c=0;c<size;c++)
	{
		if(result<p[c])
		{
			result=p[c];
		}
	}
	return result;
}
//求最小值
double MIN(double *p,int size)
{
	double result=0;
	int c=0;
	result=p[0];
	for(c=0;c<size;c++)
	{
		if(result>p[c])
		{
			result=p[c];
		}
	}
	return result;
}
//冒泡排序
void bubble(double *a, int n) /*定义两个参数：数组首地址与数组大小*/
{
	int i,j,temp;	
	for(i = 0;i < n-1; i++)	
	{	
		for(j = i + 1; j < n; j++) /*注意循环的上下限*/
		{
			if(a[i] > a[j])
			{
				temp = a[i];		
				a[i] = a[j];		
				a[j] = temp;			
			}
		}
	}
}
//求中间值
double MID(double *p,int size)
{
	double result;
	int mark,count;
	bubble(p,size);
	if(size/2==0)
		mark=2;
	else mark=1;
	if(mark==1)
	{
		count=(1+size)/2;
		result=p[count];
	}		
	else 
	{
		count=size/2;
		result=(p[count]+p[count])/2;
	}
	return result;
}
//数据线性变换
//y=kx+b
//n1:待处理数组，n2结果数组
void change(double *n1,double *n2,double k,double b,int size)
{
	int c=0;
	for(c=0;c<size;c++)
	{
		n2[c]=k*n1[c]+b;
	}
}
//数据拟合-线性回归
//k:数据斜率；b:数据截距
//处理数组*p
//返回数组 *a;a[0]=k;a[1]=b
//a[0]=k;
//a[1]=b;
//a[2]=RSS;
void simulation(double *p,double *a,int size)
{
	double sum_xiyi,sum_xi,sum_yi,sum_xi2,RSS;
	int c=0;
//	x_avg=(1+size)/2;x_avg,y_avg,
//	y_avg=sum(p,size)/(double)size;
	for(c=0;c<size;c++)
	{
		sum_xiyi+=((c+1)*p[c]);
	}
	sum_xi=size*(size+1)/2;
	sum_yi=sum(p,size);
	sum_xi2=size*(size+1)*(2*size+1)/6;
	a[0]=(size*sum_xiyi-sum_xi*sum_yi)/(size*sum_xi2-sum_xi*sum_xi);
	a[1]=(sum_yi-a[0]*sum_xi)/size;
	for(c=0;c<size;c++)
	{
		RSS+=(p[c]-(a[0]*(c+1)+a[1]))*(p[c]-(a[0]*(c+1)+a[1]));
	}
	a[2]=RSS;
}
////两个数之差的绝对值 
////x1,x2：需取差值的两个数
////返回值：|x1-x2|
//u16 my_abs(u16 x1,u16 x2)
//{			 
//	if(x1>x2)return x1-x2;
//	else return x2-x1;
//}






/********************************************
输入一个数组
起始下标
终止下标
*********************************************/
void merge(int *A,int start,int end)
{
	int mid,L[1000],R[1000];
	int i,j,k;
	//length=end-start+1;
	mid=(end+start)/2;
	for(i=0;i<mid;i++)
	{
		L[i]=A[start+i];
	}
	for(i=0;i<end-mid+1;i++)
	{
		R[i]=A[mid+i];
	}
	L[mid]=99999999;
	R[end-mid+1]=99999999;
	i=j=0;
	for(k=start;k<end+1;k++)
	{
		if(L[i]<=R[j])
		{
			A[k]=L[i];
			i++;
		}
		else 
		{
			A[k]=R[j];
			j++;
		}
	}
}
void merge_sort(int *A,int length)
{
	int i,j,count;
	count=length/2;
	if(length%2==0)
	{
		for(j=1;j<count+1;j*=2)//确定几个几个的排
		{
			for(i=0;i<count;i++)//确定排序位置
			{
				merge(A,j*i,j*i+(j-1));
			}
		}
	}
	else 
	{
		for(j=1;j<count;j*=2)//确定几个几个的排
		{
			for(i=0;i<count;i++)//确定排序位置
			{
				merge(A,j*i,j*i+(j-1));
			}
		}
		merge(A,0,length);
	}
}	








