/*
design by Dcount
2020.3.17
update 2020.3.28
*/
#include "fft.h"

/*
extern float fft_input[FFT_LENGTH*2];	//FFT��������
extern float fft_output[FFT_LENGTH];	//FFT�������
extern arm_cfft_radix4_instance_f32 scfft;
*/


/*

������/��������=�ֱ���
FN =(N-1)*�ֱ��ʡ�
N=1 Ϊֱ��
A= sqrt(a*a+b*b)
PN = arctan b/a
�ع��ź�Ϊ 2A/N *cos(2*pi*FN*T+PN)
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
	arm_scale_f32(a, S->altituderecover,a,S->size);//�������油��

}
/*
data��ADCԭʼ���ݣ������κζ���

*/
void cfft(fft_choice *S,float *data,float *out)
{
	float *a,*output;
	int i,FFT_LEN;//FFT_LEN��һ���ǲ�������������������ADC����
	arm_cfft_radix4_instance_f32 scfft;
	FFT_LEN = S->size;
	S->Fullscale=10*log10(S->ADC_Vpp*S->ADC_Vpp/8/50*1000);//rms�ڸ���50ŷķ�������
	S->BW*=1.57f;//һ��RC
	S->df=S->Fs/S->size;//�ֱ��ʵ���Fs/n
	//correction = 10*log10(S->Fs/S->size*S->ENBW)
	win(data,S);
	for(i=0;i<FFT_LEN;i++)
	{
		a[2*i]= data[i]/pow(2,S->ADC_bit-1);
		a[2*i+1] = 0;
	}


	
	scfft.onebyfftLen=1/(float)(FFT_LEN);//��һ��
	arm_cfft_radix4_init_f32(&scfft,FFT_LEN,0,1);//��ʼ��scfft�ṹ�壬�趨FFT��ز���
	arm_cfft_radix4_f32(&scfft,a);	//FFT���㣨��4��
	arm_cmplx_mag_f32(a,output,FFT_LEN);	//��������������ģ�÷���
	arm_scale_f32(output, 2*S->ADC_Vpp,S->altitude, FFT_LEN);//2A/N����÷�ֵ
	S->altitude[0]/=2;//ֱ���������ó���2
	arm_mult_f32(S->altitude,S->altitude,S->power,FFT_LEN);//����
	//arm_power_f32(S->altitude, FFT_LEN, S->power);//�ź��ܹ���
	for(i=0;i<FFT_LEN;i++)
	{
		if((S->unit==dBFS)||(S->unit==dBm)||(S->unit==dBmhz))
		{
			output[i]=20*log10(sqrt(2)*output[i]);     //��dBFS
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
