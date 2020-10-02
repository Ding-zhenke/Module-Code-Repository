#ifndef _NUMC_H
#define _NUMC_H
#include "sys.h"
#include "arm_math.h"
#include "math.h"
#include "usmart.h"
/************************************************
 ALIENTEK ������STM32F7������
 ��numpy��ʵ��
 ���ߣ�Dcount
 create:2020.6.8
 update 2020.7.21
************************************************/

//�Ӽ��˳�,���
#define arr_add 0x01
#define arr_sub 0x02
#define arr_mul 0x03//��Ӧ�����
#define arr_div 0x04
#define arr_dot 0x05//���
 /**
 * @brief  �����������
 * @param[in]  a      input array a 
 * @param[in]  b      input array b
 * @param[in]  size   the length of array
 * @param[in]  ch     choose the culculation way
 * @param[out] c  	c = f(a,b) 	
 */
void arr_base(float *a,float *b,float *c,int size,int ch);


/**
* @brief  �������Ա任
* @param[in]  init      input array 
* @param[in]  size   	the length of array
* @param[in]  offset    bias
* @param[in]  scale     scalse
* @param[out] traget	target = scale*init+offset	
*/
void arr_line(float *init,float offset,float scale,float *target,int size);


/**
* @brief  �������
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
* @brief  ͳ�ƺ����������������
* @param[in]  init      input array 
* @param[in]  size   	the length of array
* @param[in]  mode  	choose the way of statistic function
* @param[out] result  	the output,maybe is array, maybe is a number 
*/
#define arr_mean  0x20 	//ƽ��
#define arr_std	  0x21	//��׼��,������
#define arr_power 0x22  //����,ÿ����ƽ��֮���ۼ�
#define arr_rms	  0x23	//���ָ�
#define arr_var   0x24	//����
#define arr_copy  0x25	//��������
#define arr_negate 0x26 //�෴��
#define arr_sqrt  0x27  //����
#define arr_cos   0x28  //cos
#define arr_sin   0x29  //sin
#define arr_square 0x2a //x^2
#define arr_abs   0x2b //abs
#define arr_sum	  0x2c  //���
#define arr_sort  0x2d  //����
void arr_stat(float *init, int size, float *result,int mode);

/**
* @brief  ��ӡ����
* @param[in]  init      input array 
* @param[in]  name      the str of input array's name 
* @param[in]  size   	the length of array
*/
void arr_print(float *a,u8 *name,int size);




#endif
