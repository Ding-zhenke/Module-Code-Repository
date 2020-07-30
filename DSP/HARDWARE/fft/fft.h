#ifndef _FFT_H
#define _FFT_H
#include "sys.h"
#include "arm_math.h"
#include "math.h"

#define pi 3.1415926

#define square 0x10
#define hanning 0x20
#define blackman 0x30
#define hamming 0x40
#define flattop 0x50
#define kaiser 0x60//̫�����Ҳ�������


#define dB 0x01
#define dBm 0x02
#define dBFS 0X03
#define dBmhz 0x04

/*
arm_abs_f32(float *init,float *target,int size) init to target
�����Ըĳ�q31,q15,q7,������ת���������ѣ�ͨ��

arm_add_f32(float *a,float *b,float *c,int size)
c=a+b����Ӧ�����

arm_dot_prod_f32(float *a,float *b,int size��float *sum)
sum =a *b �������

arm_mult_f32(float *a,float *b,float *c,int size)
��Ӧ�����

arm_negate_f32(float *init,float *target,int size)
�෴��

arm_offset_f32(float *init,float offset, float *target,int size)
ƫ��

arm_shift_q31(float *init,int shift_bits, float *target,int size)
��λ���ڶ��������������ƣ�����Ҽ�     target = init << shift_bits

arm_sub_f32(float *a,float *b,float *c,int size)
c =a-b ��Ӧ�����

arm_scale_f32(float *a, float scale, float *result, int size)
��������

arm_cos_f32(float x) x �ǻ�����
arm_sin_f32(float x)

arm_sqrt_f32(float in float *pout)

arm_max_f32(float *init, int size, float *max, int max_place)
�ҳ����ݵ����ֵ����λ��

arm_min_f32(float *init, int size, float *min, int min_place)


arm_mean_f32(float *init, int size, float *mean)
��ƽ��ֵ

arm_power_f32(float *init, int size, float *power)
power = init **2 �ۼ�


arm_std_f32(float *init, int size, float *std)
��׼��

arm_rms_f32(float *init, int size, float *rms)
������

arm_var_f32(float *init, int size, float *var)
����

arm_copy_f32(float *init, float *target, int size)
���ݿ���

arm_full_f32(float value, float *init, int size)
�������

arm_sin_cos_f32(float theta,float sin,float cos)
����������-180�㵽179��

arm_cmplx_conj_f32(float *init, float *result, int size)
���������ʵ�齻��

arm_cmplx_dot_prod_f32(float *a,float *b,int size��float *real, float *imag)
�������

arm_cmplx_mag_f32(float *init, float *result, int size)
��ģ

arm_snr_f32(float *ref,float *test,int size)
* @param float* Pointer to the reference buffer ��������
* @param float* Pointer to the test buffer  ��������
������� 10lg (Psignal/Pnoise)
*/
/*
����Ҫ���õ���ADC_bit,Fs��ADC_Vpp,size,winname,unit,BW
*/
typedef struct
{
//ADC������Ϣ
	int ADC_bit;//12
	int Fs;//��������
	float ADC_Vpp;
	float Fullscale;//14.08;
	int size;//�����ݵ���
//������ѡ��
	//int data_size;//��0,�����㷨��ûŪ��
	int winname;//����������
	float powerrecover;//=1.0;
	float altituderecover;//=1.0;
//�������
	int unit; //������ݵĵ�λ
	float *altitude;//��������
	float *power;//��������
	//float
	int BW;
	float ENBW;
	float df;//�ֱ���

}fft_choice;

/*void fft_init(fft_choice *S);�������б�д
void fft_init(fft_choice *S)
{
	S->ADC_bit=12;//12
	S->Fs=1.8e6;//��������
	S->ADC_Vpp=3.3;
	S->size=FFT_LENGTH;//�����ݵ���
//������ѡ��
	
	S->winname=hanning;//����������

//�������
	S->unit=dBmhz; //������ݵĵ�λ
	S->altitude=fft_outputA;
	S->power=fft_outputpower;
	//float
    S->BW=3e6;//����3M
}
*/
void cfft(fft_choice *S,float *data,float *out);

void win(float *a,fft_choice *S);


#endif
