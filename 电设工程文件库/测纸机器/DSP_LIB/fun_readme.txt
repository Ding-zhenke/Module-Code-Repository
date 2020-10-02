arm_abs_f32(float *init,float *target,int size) init to target
其后可以改成q31,q15,q7,浮点数转定点数而已，通用

arm_add_f32(float *a,float *b,float *c,int size)
c=a+b，对应项相加

arm_dot_prod_f32(float *a,float *b,int size，float *sum)
sum =a *b 向量点乘

arm_mult_f32(float *a,float *b,float *c,int size)
对应项相乘

arm_negate_f32(float *init,float *target,int size)
相反数

arm_offset_f32(float *init,float offset, float *target,int size)
偏移

arm_shift_q31(float *init,int shift_bits, float *target,int size)
移位，第二个参数是左右移，左加右减     target = init << shift_bits

arm_sub_f32(float *a,float *b,float *c,int size)
c =a-b 对应项相减

arm_scale_f32(float *a, float scale, float *result, int size)
比例因子

arm_cos_f32(float x) x 是弧度制
arm_sin_f32(float x)

arm_sqrt_f32(float in float *pout)

arm_max_f32(float *init, int size, float *max, int max_place)
找出数据的最大值及其位置

arm_min_f32(float *init, int size, float *min, int min_place)


arm_mean_f32(float *init, int size, float *mean)
求平均值

arm_power_f32(float *init, int size, float *power)
power = init **2 累加


arm_std_f32(float *init, int size, float *std)
标准差

arm_rms_f32(float *init, int size, float *rms)
均方根

arm_var_f32(float *init, int size, float *var)
方差

arm_copy_f32(float *init, float *target, int size)
数据拷贝

arm_full_f32(float value, float *init, int size)
数据填充

arm_sin_cos_f32(float theta,float sin,float cos)
输入数据是-180°到179°

arm_cmplx_conj_f32(float *init, float *result, int size)
求共轭，储存是实虚交替

arm_cmplx_dot_prod_f32(float *a,float *b,int size，float *real, float *imag)
复数点乘

arm_cmplx_mag_f32(float *init, float *result, int size)
求模

arm_snr_f32(float *ref,float *test,int size)
* @param float* Pointer to the reference buffer 基波功率
* @param float* Pointer to the test buffer  采样功率
求信噪比 10lg (Psignal/Pnoise)
