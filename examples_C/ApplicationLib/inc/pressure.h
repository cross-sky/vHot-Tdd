#ifndef __PRESSUREA_H
#define __PRESSUREA_H

#include "commdata.h"
//low pressure 
static const uint16_t Pressure_L[]={
	860 ,	878 ,	898 ,	919 ,	940 ,	957 ,	980 ,
	1004, 	1024, 	1050, 	1072, 	1104,	1122, 	1152, 
	1176, 	1201, 	1235, 	1262, 	1291,	1321, 	1358, 
	1389, 	1422, 	1457, 	1491, 	1527,	1565, 	1602, 
	1642, 	1683, 	1725, 	1758, 	1804,	1850, 	1889, 
	1945, 	1986, 	2038, 	2081, 	2130,	2195, 	2228, 
	2294, 	2343, 	2408, 	2458, 	2507,	2572, 	2621, 
	2703, 	2769, 	2818, 	2900, 	2966,	3031, 	3097, 
	3162, 	3244, 	3326, 	3391, 	3490,	3539, 	3621, 
	3736, 	3817, 	3899
};

#define PRES_L_ARRAY_NUM			(sizeof(Pressure_L)/sizeof(uint16_t))
#define PRES_L_INDEX_MAX			(PRES_L_ARRAY_NUM-1)		// 数组下标的最大可用取值，对应 35℃采样值下标
#define PRES_L_INDEX_MIN			(1)						//数组下标的最小可用取值，对应-30℃采样值下标
#define PRES_L_TEMPER_REF			(-30)					//温度基准点，即下标为0时对应的温度
#define PRES_L_TEMP_LAST			35						//温度最后值，即数组最后一个对应的温度


static const uint16_t Pressure_H[]={
	605 ,	613 ,	622 ,	631 ,	640 ,	648 ,	657 ,
	668 ,	677 ,	688 ,	697 ,	712 ,	719 ,	732 ,
	743 ,	754 ,	769 ,	780 ,	793 ,	806 ,	822 ,
	836 ,	850 ,	865 ,	880 ,	895 ,	912 ,	928 ,
	945 ,	963 ,	982 ,	996 ,	1016, 	1036, 	1053, 
	1077, 	1095, 	1118, 	1136, 	1158, 	1186, 	1200, 
	1229, 	1250, 	1279, 	1300, 	1321, 	1350, 	1371, 
	1407, 	1435, 	1457, 	1492, 	1521, 	1549, 	1578, 
	1606, 	1642, 	1678, 	1706, 	1749, 	1770, 	1806, 
	1856, 	1891, 	1927, 	1962, 	2005, 	2041, 	2084, 
	2126, 	2162, 	2198, 	2247, 	2297, 	2340, 	2383, 
	2433, 	2482, 	2532, 	2582, 	2618, 	2675, 	2725, 
	2775, 	2832, 	2889, 	2931, 	2995, 	3059, 	3102, 
	3188, 	3238 					
};
#define PRES_H_ARRAY_NUM			(sizeof(Pressure_H)/sizeof(uint16_t))
#define PRES_H_INDEX_MAX			(PRES_H_ARRAY_NUM-1)		// 数组下标的最大可用取值，对应 35℃采样值下标
#define PRES_H_INDEX_MIN			(1)						//数组下标的最小可用取值，对应-30℃采样值下标
#define PRES_H_TEMPER_REF			(-30)					//温度基准点，即下标为0时对应的温度
#define PRES_H_TEMP_LAST			62						//温度最后值，即数组最后一个对应的温度

uint16_t uPresLSearchData(uint16_t searchValue);
int16_t iPresLCalc(uint16_t index,uint16_t adcValue);
uint16_t uPresHSearchData(uint16_t searchValue);
int16_t iPresHCalc(uint16_t index,uint16_t adcValue);

#endif

