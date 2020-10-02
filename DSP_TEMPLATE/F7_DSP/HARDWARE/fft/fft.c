/*
design by Dcount
2020.3.17
update 2020.3.28
*/
#include "fft.h"

/*
extern float fft_input[FFT_LENGTH*2];	//FFT输入数组
extern float fft_output[FFT_LENGTH];	//FFT输出数组
extern arm_cfft_radix4_instance_f32 scfft;
*/


/*

采样率/采样点数=分辨率
FN =(N-1)*分辨率、
N=1 为直流
A= sqrt(a*a+b*b)
PN = arctan b/a
重构信号为 2A/N *cos(2*pi*FN*T+PN)
*/

fft_choice S;

void win(float *a,fft_choice *S)
{
	float *p;
	int i;
	if(S->winname == square)
	{
		S->powerrecover=1;
		S->altituderecover=1;
		S->ENBW=1;
	}
	else if(S->winname == hanning)//the medium accuracy of f and power
	{
		for(i=0;i<S->size;i++)
		{
			p[i]=0.50f-0.50f*arm_cos_f32(2*pi*(i+1)/S->size);
			a[i]*=p[i];
		}
		S->powerrecover=1.663f;
		S->altituderecover=2;
		S->ENBW=1.50f;
	}
	else if(S->winname == blackman)//two f are simular but altitude is different
	{
		for(i=0;i<S->size;i++)
		{
			p[i]=0.420f-0.50f*arm_cos_f32(2*pi*i/(S->size-1))+0.08f*arm_cos_f32(4*pi*i/(S->size-1));
			a[i]*=p[i];
		}
		S->powerrecover=1.812f;
		S->altituderecover=2.381f;
		S->ENBW=2.0f;
	}
	else if(S->winname == hamming)//two f are simular but altitude is different
	{
		for(i=0;i<S->size;i++)
		{
			p[i]=0.54f-0.46f*arm_cos_f32(2*pi*(i+1)/S->size);
			a[i]*=p[i];
		}
		S->powerrecover=1.586f;
		S->altituderecover=1.852f;
		S->ENBW=1.36f;
	}
	else if(S->winname == flattop)//two f are simular but altitude is different
	{
		for(i=0;i<(int)(S->size/10);i++)
		{
			p[i]=0.5f-0.5f*arm_cos_f32(10*pi*(i+1)/S->size);
			a[i]*=p[i];
		}
		for(i=(int)(S->size/10);i<(int)(9*S->size/10);i++)
		{
			p[i]=1;
			a[i]*=p[i];
		}
		for(i=(int)(9*S->size/10);i<S->size;i++)
		{
			p[i]=0.5f+0.5f*arm_cos_f32(10*pi*(i+1-9/10*S->size)/S->size);
			a[i]*=p[i];
		}
		S->powerrecover=1.069f;
		S->altituderecover=1.110f;
		S->ENBW=3.77f;
	}
	arm_scale_f32(a, S->altituderecover,a,S->size);//幅度增益补偿

}
/*
data是ADC原始数据，不加任何东西

*/
void cfft(fft_choice *S,float *data,float *out)
{
	float *a,*output;
	int i,FFT_LEN;//FFT_LEN不一定是采样点数，采样点数是ADC点数
	arm_cfft_radix4_instance_f32 scfft;
	FFT_LEN = S->size;
	S->Fullscale=10*log10(S->ADC_Vpp*S->ADC_Vpp/8/50*1000);//rms在负载50欧姆的情况下
	S->BW*=1.57f;//一阶RC
	S->df=S->Fs/S->size;//分辨率等于Fs/n
	//correction = 10*log10(S->Fs/S->size*S->ENBW)
	win(data,S);
	for(i=0;i<FFT_LEN;i++)
	{
		a[2*i]= data[i]/pow(2,S->ADC_bit-1);
		a[2*i+1] = 0;
	}


	
	scfft.onebyfftLen=1/(float)(FFT_LEN);//归一化
	arm_cfft_radix4_init_f32(&scfft,FFT_LEN,0,1);//初始化scfft结构体，设定FFT相关参数
	arm_cfft_radix4_f32(&scfft,a);	//FFT计算（基4）
	arm_cmplx_mag_f32(a,output,FFT_LEN);	//把运算结果复数求模得幅度
	arm_scale_f32(output, 2*S->ADC_Vpp,S->altitude, FFT_LEN);//2A/N，求得幅值
	S->altitude[0]/=2;//直流分量不用除以2
	arm_mult_f32(S->altitude,S->altitude,S->power,FFT_LEN);//功率
	//arm_power_f32(S->altitude, FFT_LEN, S->power);//信号总功率
	for(i=0;i<FFT_LEN;i++)
	{
		if((S->unit==dBFS)||(S->unit==dBm)||(S->unit==dBmhz))
		{
			output[i]=20*log10(sqrt(2)*output[i]);     //求到dBFS
			if((S->unit == dBm)||(S->unit == dBmhz))
			{   
				output[i]+=S->Fullscale;//dBm
				if(S->unit == dBmhz)
				{
					//dBmhz
					output[i]=output[i]-10*log10(S->Fs/S->size*S->ENBW)-10*log10(2*S->BW/S->Fs);
				}
			}	
		}
		else if(S->unit==dB)
			output[i]=20*log10(output[i]);     //dB
		else 
			break;
	}
	for(i=0;i<FFT_LEN/2;i++)
	{
      out[i]=output[i];
	}


}
