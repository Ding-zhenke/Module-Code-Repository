#include "numc.h"
/************************************************
 ALIENTEK 阿波罗STM32F7开发板
 仿numpy库实验
 作者：Dcount
 create:2020.6.8
 update 2020.7.21
************************************************/
void arr_base(float *a,float *b,float *c,int size,int ch)
{
	float *tp;
	int i;
	switch(ch)
	{
		case arr_add:
		{
			arm_add_f32(a,b,c,size);
			break;
		}			
		case arr_sub:
		{
			arm_sub_f32(a,b,c,size);
			break;
		}
		case arr_mul:
		{
			arm_mult_f32(a,b,c,size);
			break;
		}
		case arr_div:
		{
			for(i=0;i<size;i++)
			{
				tp[i]=1/b[i];
			}
			arm_mult_f32(a,tp,c,size);
			break;
		}
		case arr_dot:
		{
			arm_dot_prod_f32(a,b,size,c);
			break;
		}
		default: 
		{
			printf("mode error!模式输入错误！");
		}
	}
}	
//y=kx+b,先k后b
void arr_line(float *init,float offset, float scale, float *target,int size)
{
	
	arm_scale_f32(init, scale, target,size);//scale
	arm_offset_f32(init,offset, target,size);//bias
}

//最大最小
void arr_find(float *init, int size, float *value, uint32_t *place,int w)
{
	if(w == arr_max)
	{
		arm_max_f32(init, size, value, place);
//找出数据的最大值及其位置
	}
	else if(w == arr_min)
	{	
		arm_min_f32(init, size, value, place);
	}
	else 
		printf("mode error!模式输入错误！");
}
//统计与自身
void arr_stat(float *init, int size, float *result,int mode)
{
	int i;
	switch(mode)
	{
		case arr_mean:
		{
			arm_mean_f32(init, size, result);
			break;
		}
		case arr_std:
		{
			arm_std_f32(init, size,result);
			break;
		}
		case arr_power:
		{
			arm_power_f32(init,size,result);
			break;
		}
		case arr_rms:
		{
			arm_rms_f32(init,size,result);
			break;
		}
		case arr_var:
		{
			arm_var_f32(init,size,result);
			break;
		}
		case arr_copy:
		{
			arm_power_f32(init,size,result);
			break;
		}
		case arr_negate:
		{
			arm_negate_f32(init,result,size);
			break;
		}
		case arr_sqrt:
		{
			for(i=0;i<size;i++)
				arm_sqrt_f32(init[i],&result[i]);
			break;
		}
		case arr_cos:
		{
			for(i=0;i<size;i++)
				result[i]=arm_cos_f32(init[i]);
			break;
		}
		case arr_sin:
		{
			for(i=0;i<size;i++)
				result[i]=arm_sin_f32(init[i]);
			break;
		}
		case arr_square:
		{
			arm_mult_f32(init,init,result,size);
			break;
		}
		case arr_abs:
		{
			arm_abs_f32(init,result,size);
			break;
		}
		case arr_sum:
		{
			result = 0;
			for(i=0;i<size;i++)
            {
				*result += init[i];
            }
			break;
		}
		case arr_sort:
		{
			int i,j,temp;	
			for(i = 0;i < size-1; i++)	
			{	
				for(j = i + 1; j < size; j++) /*注意循环的上下限*/
				{
					if(init[i] > init[j])
					{
						temp = init[i];		
						init[i] = init[j];		
						init[j] = temp;			
					}
				}
			}
			break;
		}
		default: 
		{
			printf("mode error!模式输入错误！");
		}
	}
}

void arr_print(float *a,u8 *name,int size)
{
	int i;
	for(i=0;i<size;i++)
    {
		printf("%c[%d]=%f\r\n",*name,i,a[i]);
    }
}	


