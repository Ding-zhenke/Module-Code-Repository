#ifndef _MATHF_H
#define _MATHF_H
#include "stdlib.h"
#include "sys.h"
#include "math.h"
/*数学函数库，一些数学计算的公式的具体实现是放在math.h里，具体有:

1、 三角函数

double sin(double);正弦

double cos(double);余弦

double tan(double);正切

2 、反三角函数

double asin (double); 结果介于[-PI/2,PI/2]

double acos (double); 结果介于[0,PI]

double atan (double); 反正切(主值)，结果介于[-PI/2,PI/2]

double atan2 (double,double); 反正切(整圆值)，结果介于[-PI,PI]

3 、双曲三角函数

double sinh (double);

double cosh (double);

double tanh (double);

4 、指数与对数

double frexp(double value,int *exp);这是一个将value值拆分成小数部分f和(以2为底的)指数部分exp，并返回小数部分f，即f*2^exp。其中f取值在0.5~1.0范围或者0。

double ldexp(double x,int exp);这个函数刚好跟上面那个frexp函数功能相反，它的返回值是x*2^exp

double modf(double value,double *iptr);拆分value值，返回它的小数部分，iptr指向整数部分。

double log (double); 以e为底的对数

double log10 (double);以10为底的对数

double pow(double x,double y);计算x的y次幂

float powf(float x,float y); 功能与pow一致，只是输入与输出皆为单精度浮点数

double exp (double);求取自然数e的幂

double sqrt (double);开平方根

5 、取整

double ceil (double); 取上整，返回不比x小的最小整数

double floor (double); 取下整，返回不比x大的最大整数，即高斯函数[x]

6 、绝对值

int abs(int i); 求整型的绝对值

double fabs (double);求实型的绝对值

double cabs(struct complex znum);求复数的绝对值

7 、标准化浮点数

double frexp (double f,int *p); 标准化浮点数，f = x * 2^p，已知f求x,p (x介于[0.5,1])

double ldexp (double x,int p); 与frexp相反，已知x,p求f

8 、取整与取余

double modf (double,double*); 将参数的整数部分通过指针回传，返回小数部分

double fmod (double,double); 返回两参数相除的余数

9 、其他

double hypot(double x,double y);已知直角三角形两个直角边长度，求斜边长度

double ldexp(double x,int exponent);计算x*(2的指数幂)

double poly(double x,int degree,double coeffs []);计算多项式

int matherr(struct exception *e);数学错误计算处理程序

折叠编辑本段注意事项
没有现成的cot三角函数，可以使用tan(PI/2-x)来实现
double atan2(double y,double x);取值范围在(PI,PI)之间;这是一个不太常见的函数，主要用来返回y/x的反正切值。
强调一点，1-3类 传参都是针对以弧度表示的数值，非角度表示的数值。
对于一般的对数求解，考虑利用数学上的对数转换来实现。
关于fmod:考虑到%只适用与整型数据，这里提出一个专门针对实型数据的取余运算的函数。
int rand(void) 用这函数的时候记得要给随机种子哦，要不得出的不是真正的随机数.产生随机种子可以用srand((unsigned int)time(NULL));这就是由时间产生的随机种子了。
在gcc中，需要使用-lm链接math库文件方可使用。
*/
double sum(double *p,int size);//累加函数
void change(double *n1,double *n2,double k,double b,int size);//数组线性变换
double MAX(double *p,int size);//最大值
double MIN(double *p,int size);//最小值
double AVG(double *p,int size);//平均值
double RMS(double *p,int size);//均方根值
double MID(double *p,int size);//中间值
void simulation(double *p,double *a,int size);//线性拟合
//u16 my_abs(u16 x1,u16 x2);//差的绝对值
void bubble(double *a, int n); /*定义两个参数：数组首地址与数组大小*/
void merge_sort(int *A,int length);
#endif


