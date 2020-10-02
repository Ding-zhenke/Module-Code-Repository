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
#define kaiser 0x60//太多卷积我不想做了


#define dB 0x01
#define dBm 0x02
#define dBFS 0X03
#define dBmhz 0x04


/*
你需要配置的是ADC_bit,Fs，ADC_Vpp,size,winname,unit,BW
*/
typedef struct
{
//ADC基本信息
	int ADC_bit;//12
	int Fs;//采样速率
	float ADC_Vpp;
	float Fullscale;//14.08;
	int size;//总数据点数
//窗函数选择
	//int data_size;//填0,具体算法还没弄懂
	int winname;//窗函数名字
	float powerrecover;//=1.0;
	float altituderecover;//=1.0;
//输出数据
	int unit; //输出数据的单位
	float *altitude;//幅度数组
	float *power;//功率数组
	//float
	int BW;
	float ENBW;
	float df;//分辨率
	float load_r;

}fft_choice;

void fft_init(fft_choice *S);
/*
void fft_init(fft_choice *S)
{
	S->ADC_bit=12;//12
	S->Fs=1.8e6;//采样速率
	S->ADC_Vpp=3.3;
	S->size=FFT_LENGTH;//总数据点数
//窗函数选择
	S->winname=hanning;//窗函数名字
	S->load_r = 50;// 负载电阻为50欧姆
//输出数据
	S->unit=dBmhz; //输出数据的单位
	S->altitude=fft_outputA;
	S->power=fft_outputpower;
	//float
    S->BW=3e6;//带宽3M
}
*/
void cfft(fft_choice *S,float *data,float *out);
void win(float *a,fft_choice *S);


#endif
