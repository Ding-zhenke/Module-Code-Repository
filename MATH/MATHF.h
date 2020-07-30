#ifndef _MATHF_H
#define _MATHF_H
#include "stdlib.h"
#include "sys.h"
#include "math.h"
/*��ѧ�����⣬һЩ��ѧ����Ĺ�ʽ�ľ���ʵ���Ƿ���math.h�������:

1�� ���Ǻ���

double sin(double);����

double cos(double);����

double tan(double);����

2 �������Ǻ���

double asin (double); �������[-PI/2,PI/2]

double acos (double); �������[0,PI]

double atan (double); ������(��ֵ)���������[-PI/2,PI/2]

double atan2 (double,double); ������(��Բֵ)���������[-PI,PI]

3 ��˫�����Ǻ���

double sinh (double);

double cosh (double);

double tanh (double);

4 ��ָ�������

double frexp(double value,int *exp);����һ����valueֵ��ֳ�С������f��(��2Ϊ�׵�)ָ������exp��������С������f����f*2^exp������fȡֵ��0.5~1.0��Χ����0��

double ldexp(double x,int exp);��������պø������Ǹ�frexp���������෴�����ķ���ֵ��x*2^exp

double modf(double value,double *iptr);���valueֵ����������С�����֣�iptrָ���������֡�

double log (double); ��eΪ�׵Ķ���

double log10 (double);��10Ϊ�׵Ķ���

double pow(double x,double y);����x��y����

float powf(float x,float y); ������powһ�£�ֻ�������������Ϊ�����ȸ�����

double exp (double);��ȡ��Ȼ��e����

double sqrt (double);��ƽ����

5 ��ȡ��

double ceil (double); ȡ���������ز���xС����С����

double floor (double); ȡ���������ز���x����������������˹����[x]

6 ������ֵ

int abs(int i); �����͵ľ���ֵ

double fabs (double);��ʵ�͵ľ���ֵ

double cabs(struct complex znum);�����ľ���ֵ

7 ����׼��������

double frexp (double f,int *p); ��׼����������f = x * 2^p����֪f��x,p (x����[0.5,1])

double ldexp (double x,int p); ��frexp�෴����֪x,p��f

8 ��ȡ����ȡ��

double modf (double,double*); ����������������ͨ��ָ��ش�������С������

double fmod (double,double); �������������������

9 ������

double hypot(double x,double y);��ֱ֪������������ֱ�Ǳ߳��ȣ���б�߳���

double ldexp(double x,int exponent);����x*(2��ָ����)

double poly(double x,int degree,double coeffs []);�������ʽ

int matherr(struct exception *e);��ѧ������㴦�����

�۵��༭����ע������
û���ֳɵ�cot���Ǻ���������ʹ��tan(PI/2-x)��ʵ��
double atan2(double y,double x);ȡֵ��Χ��(PI,PI)֮��;����һ����̫�����ĺ�������Ҫ��������y/x�ķ�����ֵ��
ǿ��һ�㣬1-3�� ���ζ�������Ի��ȱ�ʾ����ֵ���ǽǶȱ�ʾ����ֵ��
����һ��Ķ�����⣬����������ѧ�ϵĶ���ת����ʵ�֡�
����fmod:���ǵ�%ֻ�������������ݣ��������һ��ר�����ʵ�����ݵ�ȡ������ĺ�����
int rand(void) ���⺯����ʱ��ǵ�Ҫ���������Ŷ��Ҫ���ó��Ĳ��������������.����������ӿ�����srand((unsigned int)time(NULL));�������ʱ���������������ˡ�
��gcc�У���Ҫʹ��-lm����math���ļ�����ʹ�á�
*/
double sum(double *p,int size);//�ۼӺ���
void change(double *n1,double *n2,double k,double b,int size);//�������Ա任
double MAX(double *p,int size);//���ֵ
double MIN(double *p,int size);//��Сֵ
double AVG(double *p,int size);//ƽ��ֵ
double RMS(double *p,int size);//������ֵ
double MID(double *p,int size);//�м�ֵ
void simulation(double *p,double *a,int size);//�������
//u16 my_abs(u16 x1,u16 x2);//��ľ���ֵ
void bubble(double *a, int n); /*�������������������׵�ַ�������С*/
void merge_sort(int *A,int length);
#endif


