/*
 *file               WindowFunction.h
 *author          Vincent Cui
 *e-mail           whcui1987@163.com
 *version            0.3
 *data        31-Oct-2014
 *brief        各种窗函数的C语言实现
*/


#ifndef _WINDOWFUNCTION_H_
#define _WINDOWFUNCTION_H_

#include "GeneralConfig.h"

#define BESSELI_K_LENGTH    10

#define FLATTOPWIN_A0        0.215578995
#define FLATTOPWIN_A1        0.41663158
#define FLATTOPWIN_A2        0.277263158
#define FLATTOPWIN_A3        0.083578947
#define FLATTOPWIN_A4        0.006947368

#define NUTTALL_A0            0.3635819
#define NUTTALL_A1            0.4891775
#define NUTTALL_A2            0.1365995
#define NUTTALL_A3            0.0106411

#define BLACKMANHARRIS_A0    0.35875
#define BLACKMANHARRIS_A1    0.48829
#define BLACKMANHARRIS_A2    0.14128
#define BLACKMANHARRIS_A3    0.01168



dspErrorStatus    taylorWin(dspUint_16 N, dspUint_16 nbar, dspDouble sll, dspDouble **w);
dspErrorStatus    triangularWin(dspUint_16 N, dspDouble **w);
dspErrorStatus    tukeyWin(dspUint_16 N, dspDouble r, dspDouble **w);
dspErrorStatus    bartlettWin(dspUint_16 N, dspDouble **w);
dspErrorStatus    bartLettHannWin(dspUint_16 N, dspDouble **w);
dspErrorStatus    blackManWin(dspUint_16 N, dspDouble **w);
dspErrorStatus    blackManHarrisWin(dspUint_16 N, dspDouble **w);
dspErrorStatus    bohmanWin(dspUint_16 N, dspDouble **w);
dspErrorStatus    chebyshevWin(dspUint_16 N, dspDouble r, dspDouble **w);
dspErrorStatus    flatTopWin(dspUint_16 N, dspDouble **w);
dspErrorStatus    gaussianWin(dspUint_16 N, dspDouble alpha, dspDouble **w);
dspErrorStatus    hammingWin(dspUint_16 N, dspDouble **w);
dspErrorStatus    hannWin(dspUint_16 N, dspDouble **w);
dspErrorStatus    kaiserWin(dspUint_16 N, dspDouble beta, dspDouble **w);
dspErrorStatus    nuttalWin(dspUint_16 N, dspDouble **w);
dspErrorStatus    parzenWin(dspUint_16 N, dspDouble **w);
dspErrorStatus    rectangularWin(dspUint_16 N, dspDouble **w);
