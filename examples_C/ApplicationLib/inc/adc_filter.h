#ifndef __ADC_FILTER_H
#define __ADC_FILTER_H

#include "commdata.h"

//@@@@@@@@@@@ ��ҪУ��
/*
***************************************************************************************************
*                                       ntc��Ӧ������ѹֵ����
*--------------------------------------------------------------------------------------------------
* ע�͵�һ�У���Ӧ�¶�
*--------------------------------------------------------------------------------------------------
* �����¶ȷ�Χ��Ϊ-20��-80��(���ܻ�����㼸�����ݳ���),10k��׼����
***************************************************************************************************
*/
//-20 - +80
static const uint16_t	ntcAdc10K[]={
	389	,	409	,	431	,	453	,	476	,	500	,	525	,
	550	,	577	,	604	,	632	,	662	,	692	,	723	,
	755	,	787	,	821	,	855	,	891	,	927	,	964	,
	1001,	1040,	1079,	1119,	1160,	1201,	1242,
	1285,	1328,	1371,	1415,	1459,	1504,	1548,
	1593,	1639,	1684,	1730,	1775,	1821,	1867,
	1912,	1958,	2003,	2048,	2093,	2137,	2182,
	2225,	2269,	2312,	2354,	2396,	2438,	2479,
	2519,	2559,	2598,	2636,	2674,	2711,	2748,
	2784,	2819,	2853,	2887,	2920,	2952,	2984,
	3015,	3045,	3074,	3103,	3131,	3158,	3185,
	3211,	3236,	3261,	3285,	3308,	3331,	3353,
	3374,	3395,	3415,	3435,	3454,	3473,	3491,
	3508,	3525,	3542,	3558,	3574,	3589,	3603,
	3618,	3631,	3645				
};

#define ADC10K_ARRAY_NUM	(sizeof(ntcAdc10K)/sizeof(uint16_t))
#define ADC10K_INDEX_MAX			(ADC10K_ARRAY_NUM-1)	// �����±��������ȡֵ����Ӧ 80�����ֵ�±�
#define ADC10K_INDEX_MIN			(1)						//�����±����С����ȡֵ����Ӧ-10�����ֵ�±�
#define ADC10K_TEMPER_REF			(-20)					//�¶Ȼ�׼�㣬���±�Ϊ0ʱ��Ӧ���¶�
#define ADC10K_TEMP_LAST			80						//�¶����ֵ�����������һ����Ӧ���¶�

//-20 - +120 2k��׼����
static const uint16_t	ntcAdc10KV2[]={
	84,		89,		94,		 99,	105,	111,	117,
	123,	130,	137,	144,	152,	160,	168,
	177,	186,	196,	205,	216,	226,	237,
	249,	261,	273,	286,	300,	314,	328,
	343,	359,	374,	391,	408,	426,	444,
	463,	482,	502,	522,	544,	565,	588,
	610,	634,	658,	683,	708,	734,	760,
	787,	815,	843,	872,	901,	931,	961,
	992,	1023,	1055,	1087,	1120,	1153,	1186,
	1220,	1254,	1289,	1324,	1359,	1395,	1430,
	1466,	1502,	1539,	1575,	1612,	1649,	1685,
	1722,	1759,	1796,	1833,	1869,	1906,	1943,
	1979,	2016,	2052,	2088,	2123,	2159,	2194,
	2229,	2264,	2298,	2333,	2366,	2400,	2433,
	2466,	2498,	2526,	2558,	2588,	2619,	2649,
	2678,	2707,	2735,	2763,	2790,	2817,	2843,
	2869,	2895,	2920,	2945,	2970,	2993,	3017,
	3040,	3063,	3085,	3106,	3127,	3148,	3168,
	3188,	3208,	3228,	3247,	3266,	3285,	3303,
	3321,	3338,	3356,	3373,	3389,	3406,	3421,
	3437
};

#define ADC10KV2_ARRAY_NUM	(sizeof(ntcAdc10KV2)/sizeof(uint16_t))
#define ADC10KV2_INDEX_MAX			(ADC10KV2_ARRAY_NUM-1)	// �����±��������ȡֵ����Ӧ 80�����ֵ�±�
#define ADC10KV2_INDEX_MIN			(1)						//�����±����С����ȡֵ����Ӧ-10�����ֵ�±�
#define ADC10KV2_TEMPER_REF			(-20)					//�¶Ȼ�׼�㣬���±�Ϊ0ʱ��Ӧ���¶�
#define ADC10KV2_TEMP_LAST			120						//�¶����ֵ�����������һ����Ӧ���¶�

uint16_t uADCSearchData10K(uint16_t searchValue);
int16_t iADCTemperCalc10K(uint16_t index,uint16_t adcValue);

uint16_t uADCSearchData10KV2(uint16_t searchValue);
int16_t iADCTemperCalc10KV2(uint16_t index,uint16_t adcValue);

#endif