#ifndef _NUMC_H
#define _NUMC_H
#include "sys.h"
#include "arm_math.h"
#include "math.h"
#include "usmart.h"
/************************************************
 ALIENTEK 阿波罗STM32F7开发板
 仿numpy库实验
 作者：Dcount
 create:2020.6.8
 update 2020.7.21
************************************************/

//加减乘除,点乘
#define arr_add 0x01
#define arr_sub 0x02
#define arr_mul 0x03//对应项相乘
#define arr_div 0x04
#define arr_dot 0x05//点乘
 /**
 * @brief  数组基础运算
 * @param[in]  a      input array a 
 * @param[in]  b      input array b
 * @param[in]  size   the length of array
 * @param[in]  ch     choose the culculation way
 * @param[out] c  	c = f(a,b) 	
 */
void arr_base(float *a,float *b,float *c,int size,int ch);


/**
* @brief  数组线性变换
* @param[in]  init      input array 
* @param[in]  size   	the length of array
* @param[in]  offset    bias
* @param[in]  scale     scalse
* @param[out] traget	target = scale*init+offset	
*/
void arr_line(float *init,float offset,float scale,float *target,int size);


/**
* @brief  数组查找
* @param[in]  init      input array 
* @param[in]  size   	the length of array
* @param[in]  w   	choose the mode , max or min?
* @param[out]  value   	the max value or the min value of the array
* @param[out]  place  	the max or the min of the array 's place 
*/
#define arr_max 0x10
#define arr_min 0x11
void arr_find(float *init, int size, float *value, uint32_t *place,int w);


/**
* @brief  统计函数与自身操作函数
* @param[in]  init      input array 
* @param[in]  size   	the length of array
* @param[in]  mode  	choose the way of statistic function
* @param[out] result  	the output,maybe is array, maybe is a number 
*/
#define arr_mean  0x20 	//平均
#define arr_std	  0x21	//标准差,均方差
#define arr_power 0x22  //功率,每个点平方之后累加
#define arr_rms	  0x23	//均分根
#define arr_var   0x24	//方差
#define arr_copy  0x25	//拷贝数据
#define arr_negate 0x26 //相反数
#define arr_sqrt  0x27  //根号
#define arr_cos   0x28  //cos
#define arr_sin   0x29  //sin
#define arr_square 0x2a //x^2
#define arr_abs   0x2b //abs
#define arr_sum	  0x2c  //求和
#define arr_sort  0x2d  //排序
void arr_stat(float *init, int size, float *result,int mode);

/**
* @brief  打印数组
* @param[in]  init      input array 
* @param[in]  name      the str of input array's name 
* @param[in]  size   	the length of array
*/
void arr_print(float *a,u8 *name,int size);




#endif
