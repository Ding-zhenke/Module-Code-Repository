/*
 *file               WindowFunction.c
 *author          Vincent Cui
 *e-mail            whcui1987@163.com
 *version            0.3
 *data        31-Oct-2014
 *brief        ���ִ�������C����ʵ��
*/


#include "WindowFunction.h"
#include "GeneralConfig.h"
#include "MathReplenish.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*��������taylorWin
 *˵��������̩�մ���̩�ռ�Ȩ����
 *���룺
 *�����
 *���أ�
 *���ã�prod()���˺���
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = taylorWin(99, 4, 40, &w); ע��˴���40������ ��ʾ-40dB
 */
dspErrorStatus    taylorWin(dspUint_16 N, dspUint_16 nbar, dspDouble sll, dspDouble **w)
{
    dspDouble A;
    dspDouble *retDspDouble;
    dspDouble *sf;
    dspDouble *result;
    dspDouble alpha,beta,theta;
    dspUint_16 i,j;

    /*A = R   cosh(PI, A) = R*/
    A = (dspDouble)acosh(pow((dspDouble)10.0,(dspDouble)sll/20.0)) / PI;
    A = A * A;
    
    /*�������ϵ���Ŀռ�*/
    retDspDouble = (dspDouble *)malloc(sizeof(dspDouble) * (nbar - 1));
    if(retDspDouble == NULL)
        return DSP_ERROR;
    sf = retDspDouble;

    /*�������ϵ���Ŀռ�*/
    retDspDouble = (dspDouble *)malloc(sizeof(dspDouble) * N);
    if(retDspDouble == NULL)
        return DSP_ERROR;
    result = retDspDouble;

    alpha = prod(1, 1, (nbar - 1));
    alpha *= alpha;
    beta = (dspDouble)nbar / sqrt( A + pow((nbar - 0.5), 2) );
    for(i = 1; i <= (nbar - 1); i++)
    {
        *(sf + i - 1) = prod(1,1,(nbar -1 + i)) * prod(1,1,(nbar -1 - i));
        theta = 1;
        for(j = 1; j <= (nbar - 1); j++)
        {
            theta *= 1 - (dspDouble)(i * i) / ( beta * beta * ( A + (j - 0.5) * (j - 0.5)) );
        }
        *(sf + i - 1) = alpha * (dspDouble)theta / (*(sf + i - 1));
    }

    /*������*/
    if((N % 2) == 1)
    {
        for(i = 0; i < N; i++)
        {
            alpha = 0;
            for(j = 1; j <= (nbar - 1); j++)
            {
                alpha += (*(sf + j - 1)) * cos( 2 * PI * j * (dspDouble)(i - ((N-1)/2))/N );
            }
            *(result + i) = 1 + 2 * alpha;
        }
    }
    /*ż����*/
    else
    {
        for(i = 0; i < N; i++)
        {
            alpha = 0;
            for(j = 1; j <= (nbar - 1); j++)
            {
                alpha += (*(sf + j - 1)) * cos( PI * j * (dspDouble)(2 * (i - (N/2)) + 1) / N );
            }
            *(result + i) = 1 + 2 * alpha;
            
        }
    }
    *w = result;
    free(sf);

    return DSP_SUCESS;

}


/*
 *��������triangularWin
 *˵�����������Ǵ�����
 *���룺
 *�����
 *���أ�
 *���ã�
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = triangularWin(99, &w); 
 */
dspErrorStatus    triangularWin(dspUint_16 N, dspDouble **w)
{
    dspDouble *ptr;
    dspUint_16 i;

    ptr = (dspDouble *)malloc(N * sizeof(dspDouble));
    if(ptr == NULL)
        return DSP_ERROR;


    /*����Ϊ��*/
    if((N % 2) == 1)
    {
        for(i = 0; i < ((N - 1)/2); i++)
        {
            *(ptr + i) = 2 * (dspDouble)(i + 1) / (N + 1);
        }
        for(i = ((N - 1)/2); i < N; i++)
        {
            *(ptr + i) = 2 * (dspDouble)(N - i) / (N + 1);
        }
    }
    /*����Ϊż*/
    else
    {
        for(i = 0; i < (N/2); i++)
        {
            *(ptr + i) = (i + i + 1) * (dspDouble)1 / N;
        }
        for(i = (N/2); i < N; i++)
        {
            *(ptr + i) = *(ptr + N - 1 - i);
        }
    }
    *w = ptr;

    return DSP_SUCESS;
}

/*
 *��������tukeyWin
 *˵��������tukey������
 *���룺
 *�����
 *���أ�linSpace()
 *���ã�
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = tukeyWin(99, 0.5, &w); 
 */
dspErrorStatus    tukeyWin(dspUint_16 N, dspDouble r, dspDouble **w)
{
    dspErrorStatus    retErrorStatus;
    dspUint_16        index;
    dspDouble        *x,*result,*retPtr;
    dspDouble        alpha;

    retErrorStatus = linSpace(0, 1, N, &x);
    if(retErrorStatus == DSP_ERROR)
        return DSP_ERROR;

    result = (dspDouble *)malloc(N * sizeof(dspDouble));
    if(result == NULL)
        return DSP_ERROR;

    /*r <= 0 ���Ǿ��δ�*/
    if(r <= 0)
    {
        retErrorStatus = rectangularWin(N, &retPtr);
        if(retErrorStatus == DSP_ERROR)
            return DSP_ERROR;
        /*�����ݿ������Ժ��ͷŵ��õĴ������Ŀռ�*/
        memcpy(result, retPtr, ( N * sizeof(dspDouble)));
        free(retPtr);
    }
    /*r >= 1 ���Ǻ�����*/
    else if(r >= 1)
    {
        retErrorStatus = hannWin(N, &retPtr);
        if(retErrorStatus == DSP_ERROR)
            return DSP_ERROR;
        /*�����ݿ������Ժ��ͷŵ��õĴ������Ŀռ�*/
        memcpy(result, retPtr, ( N * sizeof(dspDouble)));
        free(retPtr);
    }
    else
    {
        for(index = 0; index < N; index++)
        {
            alpha = *(x + index);
            if(alpha < (r/2))
            {
                *(result + index) = (dspDouble)(1 + cos( 2 * PI * (dspDouble)(alpha - (dspDouble)r/2)/r))/2;
            }
            else if((alpha >= (r/2)) && (alpha <(1 - r/2)))
            {
                *(result + index) = 1;
            }
            else
            {
                *(result + index) = (dspDouble)(1 + cos( 2 * PI * (dspDouble)(alpha - 1 + (dspDouble)r/2)/r))/2;
            }
            
        }
    }

    free(x);

    *w = result;

    return DSP_SUCESS;

}

/*
 *��������bartlettWin
 *˵��������bartlettWin������
 *���룺
 *�����
 *���أ�
 *���ã�
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = bartlettWin(99, &w); 
 */
dspErrorStatus    bartlettWin(dspUint_16 N, dspDouble **w)
{
    dspDouble *ret;
    dspUint_16 n;

    ret = (dspDouble *)malloc(N * sizeof(dspDouble));
    if(ret == NULL)
        return DSP_ERROR;

    for(n = 0; n < ( N - 1 ) / 2; n++)
    {
        *(ret + n) = 2 * (dspDouble)n / (N - 1);
    }

    for(n = ( N - 1 ) / 2; n < N; n++)
    {
        *(ret + n) = 2 - 2 * (dspDouble)n / (( N - 1 ));
    }

    *w = ret;

    return DSP_SUCESS;
}

/*
 *��������bartLettHannWin
 *˵��������bartLettHannWin������
 *���룺
 *�����
 *���أ�
 *���ã�
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = bartLettHannWin(99, &w); 
 */
dspErrorStatus    bartLettHannWin(dspUint_16 N, dspDouble **w)
{
    dspUint_16 n;
    dspDouble *ret;

    ret = (dspDouble *)malloc(N * sizeof(dspDouble));
    if(ret == NULL)
        return DSP_ERROR;
    /*��*/
    if(( N % 2 ) == 1)
    {
        for(n = 0; n < N; n++)
        {
            *(ret + n) = 0.62 - 0.48 * myAbs( ( (dspDouble)n / ( N - 1 ) ) - 0.5 ) + 0.38 * cos( 2 * PI * ( ((dspDouble)n / ( N - 1 ) ) - 0.5 ) );
        }
        for(n = 0; n < (N-1)/2; n++)
        {
            *(ret + n) = *(ret + N - 1 - n);
        }
    }
    /*ż*/
    else
    {
        for(n = 0; n < N; n++)
        {
            *(ret + n) = 0.62 - 0.48 * myAbs( ( (dspDouble)n / ( N - 1 ) ) - 0.5 ) + 0.38 * cos( 2 * PI * ( ((dspDouble)n / ( N - 1 ) ) - 0.5 ) );
        }
        for(n = 0; n < N/2; n++)
        {
            *(ret + n) = *(ret + N -1 - n);
        }
    }

    *w = ret;

    return DSP_SUCESS;

}

/*
 *��������blackManWin
 *˵��������blackManWin������
 *���룺
 *�����
 *���أ�
 *���ã�
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = blackManWin(99, &w); 
 */
dspErrorStatus    blackManWin(dspUint_16 N, dspDouble **w)
{
    dspUint_16 n;
    dspDouble *ret;
    ret = (dspDouble *)malloc(N * sizeof(dspDouble));
    if(ret == NULL)
        return DSP_ERROR;

    for(n = 0; n < N; n++)
    {
        *(ret + n) = 0.42 - 0.5 * cos(2 * PI * (dspDouble)n / ( N - 1 )) + 0.08 * cos( 4 * PI * ( dspDouble )n / ( N - 1 ) );
    }

    *w = ret;

    return DSP_SUCESS;
}

/*
 *��������blackManHarrisWin
 *˵��������blackManHarrisWin������
 *���룺
 *�����
 *���أ�
 *���ã�
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = blackManHarrisWin(99, &w); 
 *        minimum 4-term Blackman-harris window -- From Matlab
 */
dspErrorStatus    blackManHarrisWin(dspUint_16 N, dspDouble **w)
{
    dspUint_16 n;
    dspDouble *ret;
    
    ret = (dspDouble *)malloc(N * sizeof(dspDouble));
    if(ret == NULL)
        return DSP_ERROR;

    for(n = 0; n < N; n++)
    {
        *(ret + n) = BLACKMANHARRIS_A0 - BLACKMANHARRIS_A1 * cos(  2 * PI * (dspDouble)n / (N) ) + \
                                         BLACKMANHARRIS_A2 * cos(  4 * PI * (dspDouble)n/  (N) ) - \
                                         BLACKMANHARRIS_A3 * cos(  6 * PI * (dspDouble)n/  (N) );
    }

    *w = ret;

    return DSP_SUCESS;
}

/*
 *��������bohmanWin
 *˵��������bohmanWin������
 *���룺
 *�����
 *���أ�
 *���ã�
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = bohmanWin(99, &w); 
 */
dspErrorStatus    bohmanWin(dspUint_16 N, dspDouble **w)
{
    dspUint_16 n;
    dspDouble *ret;
    dspDouble x;
    ret = (dspDouble *)malloc(N * sizeof(dspDouble));
    if(ret == NULL)
        return DSP_ERROR;

    for(n = 0; n < N; n++)
    {
        x =  -1 +  n *  (dspDouble)2 / ( N - 1 ) ;
        /*ȡ����ֵ*/
        x = x >= 0 ? x : ( x * ( -1 ) );
        *(ret + n) =  ( 1 - x ) * cos( PI * x) + (dspDouble)(1 / PI) * sin( PI * x);
    }

    *w = ret;

    return DSP_SUCESS;
}

/*
 *��������chebyshevWin
 *˵��������chebyshevWin������
 *���룺
 *�����
 *���أ�
 *���ã�
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = chebyshevWin(99,100, &w); 
 */
dspErrorStatus    chebyshevWin(dspUint_16 N, dspDouble r, dspDouble **w)
{
    dspUint_16 n,index;
    dspDouble *ret;
    dspDouble x, alpha, beta, theta, gama;

    ret = (dspDouble *)malloc(N * sizeof(dspDouble));
    if(ret == NULL)
        return DSP_ERROR;


    /*10^(r/20)*/
    theta = pow((dspDouble)10, (dspDouble)(myAbs(r)/20));
    beta = pow(cosh(acosh(theta)/(N - 1)),2);
    alpha = 1 - (dspDouble)1 / beta;

    if((N % 2) == 1)
    {
        /*����һ�������*/
        for( n = 1; n < ( N + 1 ) / 2; n++ )
        {
            gama = 1;
            for(index = 1; index < n; index++)
            {
                x = index * (dspDouble)( N - 1 - 2 * n + index) /(( n - index ) * (n + 1 -index));
                gama = gama * alpha * x + 1;
            }
            *(ret + n) = (N - 1) * alpha * gama; 
        }

        theta = *( ret + (N - 1)/2 );
        *ret = 1;

        for(n = 0; n < ( N + 1 ) / 2; n++ )
        {
            *(ret + n) = (dspDouble)(*(ret + n)) / theta;
        }

        /*�����һ��*/
        for(; n < N; n++)
        {
            *(ret + n) = ret[N - n - 1];
        }
    }
    else
    {
        /*����һ�������*/
        for( n = 1; n < ( N + 1 ) / 2; n++ )
        {
            gama = 1;
            for(index = 1; index < n; index++)
            {
                x = index * (dspDouble)( N - 1 - 2 * n + index) /(( n - index ) * (n + 1 -index));
                gama = gama * alpha * x + 1;
            }
            *(ret + n) = (N - 1) * alpha * gama; 
        }

        theta = *( ret + (N/2) - 1);
        *ret = 1;

        for(n = 0; n < ( N + 1 ) / 2; n++ )
        {
            *(ret + n) = (dspDouble)(*(ret + n)) / theta;
        }

        /*�����һ��*/
        for(; n < N; n++)
        {
            *(ret + n) = ret[N - n - 1];
        }
    }
    

    *w = ret;

    return DSP_SUCESS;
}

/*
 *��������flatTopWin
 *˵��������flatTopWin������
 *���룺
 *�����
 *���أ�
 *���ã�
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = flatTopWin(99, &w); 
 */
dspErrorStatus    flatTopWin(dspUint_16 N, dspDouble **w)
{
    dspUint_16 n;
    dspDouble *ret;
    ret = (dspDouble *)malloc(N * sizeof(dspDouble));
    if(ret == NULL)
        return DSP_ERROR;

    for(n = 0; n < N; n++)
    {
        *(ret + n) = FLATTOPWIN_A0 - FLATTOPWIN_A1 * cos(2 * PI * (dspDouble)n / (N - 1)) +\
                     FLATTOPWIN_A2 * cos(4 * PI * (dspDouble)n / (N - 1)) -\
                     FLATTOPWIN_A3 * cos(6 * PI * (dspDouble)n / (N - 1)) +\
                     FLATTOPWIN_A4 * cos(8 * PI * (dspDouble)n / (N - 1));
    }

    *w = ret;

    return DSP_SUCESS;
}


/*
 *��������gaussianWin
 *˵��������gaussianWin������
 *���룺
 *�����
 *���أ�
 *���ã�
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = gaussianWin(99,2.5, &w); 
 */
dspErrorStatus    gaussianWin(dspUint_16 N, dspDouble alpha, dspDouble **w)
{
    dspUint_16 n;
    dspDouble k, beta, theta;
    dspDouble *ret;

    ret = (dspDouble *)malloc(N * sizeof(dspDouble));
    if(ret == NULL)
        return DSP_ERROR;
    
    for(n =0; n < N; n++)
    {
        if((N % 2) == 1)
        {
            k = n - (N - 1)/2;
            beta = 2 * alpha * (dspDouble)k / (N - 1);  
        }
        else
        {
            k = n - (N)/2;
            beta = 2 * alpha * (dspDouble)k / (N - 1);  
        }
        
        theta = pow(beta, 2);
        *(ret + n) = exp((-1) * (dspDouble)theta / 2);
    }

    *w = ret;

    return DSP_SUCESS;
}

/*
 *��������hammingWin
 *˵��������hammingWin������
 *���룺
 *�����
 *���أ�
 *���ã�
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = hammingWin(99, &w); 
 */
dspErrorStatus    hammingWin(dspUint_16 N, dspDouble **w)
{
    dspUint_16 n;
    dspDouble *ret;
    ret = (dspDouble *)malloc(N * sizeof(dspDouble));
    if(ret == NULL)
        return DSP_ERROR;

    for(n = 0; n < N; n++)
    {
        *(ret + n) = 0.54 - 0.46 * cos (2 * PI *  ( dspDouble )n / ( N - 1 ) );
    }

    *w = ret;

    return DSP_SUCESS;
}

/*
 *��������hannWin
 *˵��������hannWin������
 *���룺
 *�����
 *���أ�
 *���ã�
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = hannWin(99, &w); 
 */
dspErrorStatus    hannWin(dspUint_16 N, dspDouble **w)
{
    dspUint_16 n;
    dspDouble *ret;
    ret = (dspDouble *)malloc(N * sizeof(dspDouble));
    if(ret == NULL)
        return DSP_ERROR;

    for(n = 0; n < N; n++)
    {
        *(ret + n) = 0.5 * ( 1 - cos( 2 * PI * (dspDouble)n / (N - 1)));
    }

    *w = ret;

    return DSP_SUCESS;
}

/*
 *��������kaiserWin
 *˵��������kaiserWin������
 *���룺
 *�����
 *���أ�
 *���ã�besseli()��һ����������������
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = kaiserWin(99, 5, &w); 
 */
dspErrorStatus    kaiserWin(dspUint_16 N, dspDouble beta, dspDouble **w)
{
    dspUint_16 n;
    dspDouble *ret;
    dspDouble theta;

    ret = (dspDouble *)malloc(N * sizeof(dspDouble));
    if(ret == NULL)
        return DSP_ERROR;

    for(n = 0; n < N; n++)
    {
        theta = beta * sqrt( 1 - pow( ( (2 * (dspDouble)n/(N -1)) - 1),2 ) );
        *(ret + n) = (dspDouble)besseli(0, theta, BESSELI_K_LENGTH) / besseli(0, beta, BESSELI_K_LENGTH);
    }

    *w = ret;

    return DSP_SUCESS;
}

/*
 *��������nuttalWin
 *˵��������nuttalWin������
 *���룺
 *�����
 *���أ�
 *���ã�
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = nuttalWin(99, &w); 
 */
dspErrorStatus    nuttalWin(dspUint_16 N, dspDouble **w)
{
    dspUint_16 n;
    dspDouble *ret;

    ret = (dspDouble *)malloc(N * sizeof(dspDouble));
    if(ret == NULL)
        return DSP_ERROR;

    for(n = 0; n < N; n++)
    {
        *(ret + n) =NUTTALL_A0 - NUTTALL_A1 * cos(2 * PI * (dspDouble)n / (N - 1)) +\
                                 NUTTALL_A2 * cos(4 * PI * (dspDouble)n / (N - 1)) -\
                                 NUTTALL_A3 * cos(6 * PI * (dspDouble)n / (N - 1));
                                     
    }

    *w = ret;

    return DSP_SUCESS;
}

/*
 *��������parzenWin
 *˵��������parzenWin������
 *���룺
 *�����
 *���أ�
 *���ã�
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = parzenWin(99, &w); 
 */
dspErrorStatus    parzenWin(dspUint_16 N, dspDouble **w)
{
    dspUint_16 n;
    dspDouble *ret;
    dspDouble alpha,k;

    ret = (dspDouble *)malloc(N * sizeof(dspDouble));
    if(ret == NULL)
        return DSP_ERROR;

    if(( N % 2) == 1)
    {
        for(n = 0; n < N; n++)
        {
            k = n - (N - 1) / 2;
            alpha = 2 * (dspDouble)myAbs(k) / N;
            if(myAbs(k) <= (N - 1) / 4)
            {
                *(ret + n) = 1 - 6 * pow(alpha,2) + 6 * pow(alpha, 3);
            }
            else
            {
                *(ret + n) = 2 * pow( (1 - alpha), 3 );
            }
                                     
        }
    }
    else
    {
        for(n = 0; n < N; n++)
        {
            k = n - (N - 1) / 2;
            alpha = 2 * (dspDouble)myAbs(k) / N;
            if(myAbs(k) <= (dspDouble)(N -1) / 4)
            {
                *(ret + n) = 1 - 6 * pow(alpha,2) + 6 * pow(alpha, 3);
            }
            else
            {
                *(ret + n) = 2 * pow( (1 - alpha), 3 );
            }
                                     
        }
    }



    *w = ret;

    return DSP_SUCESS;
}

/*
 *��������rectangularWin
 *˵��������rectangularWin������
 *���룺
 *�����
 *���أ�
 *���ã�
 *�������ù��Ժ���Ҫ�ֶ��ͷŵ�*w���ڴ�ռ�
 *        ����ʾ����ret = rectangularWin(99, &w); 
 */
dspErrorStatus    rectangularWin(dspUint_16 N, dspDouble **w)
{
    dspUint_16 n;
    dspDouble *ret;

    ret = (dspDouble *)malloc(N * sizeof(dspDouble));
    if(ret == NULL)
        return DSP_ERROR;

    for(n = 0; n < N; n++)
    {
        *(ret + n) = 1;                                     
    }

    *w = ret;

    return DSP_SUCESS;
}

