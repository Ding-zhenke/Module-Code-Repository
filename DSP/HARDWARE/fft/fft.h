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
	float load_r;

}fft_choice;

void fft_init(fft_choice *S);
/*
void fft_init(fft_choice *S)
{
	S->ADC_bit=12;//12
	S->Fs=1.8e6;//��������
	S->ADC_Vpp=3.3;
	S->size=FFT_LENGTH;//�����ݵ���
//������ѡ��
	S->winname=hanning;//����������
	S->load_r = 50;// ���ص���Ϊ50ŷķ
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
