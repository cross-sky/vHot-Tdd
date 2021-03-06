#include "adc_filter.h"

/*
***************************************************************************************************
* 函数名称: search_data()
* 输　  入: search_value  查找基准值
* 输　  出:
* 作　  者:
* 日　  期: 2010.08.11
* 功能描述: 二分查找adc采样值
*--------------------------------------------------------------------------------------------------
*注     意:
*    1.    返回值为查找表格中第一个小于查找值(search_value)的下标；
*    2.    查找值(search_value)大于查找表格中最大值时，返回1
*          查找值(search_value)小于查找表格中最小值时，返回查找表格数量，即最大下标+1
***************************************************************************************************
*/
uint16_t uADCSearchData10K(uint16_t searchValue)
{
	uint16_t max=ADC10K_ARRAY_NUM-1;
	uint16_t min=1;
	uint16_t mid;

	while(min<=max)
	{
		mid=(max+min)>>1;		//mean (max+min)/2
		if (searchValue > ntcAdc10K[mid])
		{
			min=mid+1;
		}
		else
			max=mid-1;
	}

	return min;
}

int16_t iADCTemperCalc10K(uint16_t index,uint16_t adcValue)
{
	int16_t t; 
	if (index >= ADC10K_INDEX_MAX )
	{
		//@@@@@@return err
		t=10*((int16_t)ADC10K_INDEX_MAX + (int16_t)ADC10K_TEMPER_REF -1);
		return t;
	}
	else if(index <= ADC10K_INDEX_MIN)
	{
		return 10*(ADC10K_INDEX_MIN + ADC10K_TEMPER_REF -1);
	}
	else{
		t = 10*(ntcAdc10K[index-1] - adcValue);
		t /=(ntcAdc10K[index-1] - ntcAdc10K[index]);
		t +=10*(index + ADC10K_TEMPER_REF -1);
		return t;
	}
}

//用于排气检测--基准2k
uint16_t uADCSearchData10KV2(uint16_t searchValue)
{
	uint16_t max=ADC10KV2_INDEX_MAX;
	uint16_t min=1;
	uint16_t mid;

	while(min<=max)
	{
		mid=(max+min)>>1;		//mean (max+min)/2
		if (searchValue > ntcAdc10KV2[mid])
		{
			min=mid+1;
		}
		else
			max=mid-1;
	}

	return min;
}
/*
***************************************************************************************************
* 函数名称: temper_calc()
* 输　  入: index       数组下标
*           adc0_value  ad滤波后的采样值
* 输　  出: ntc温度测量值
* 作　  者:
* 日　  期: 2010.10.26
* 功能描述: 温度计算(分段线性插值算法)
***************************************************************************************************
*/
/*************************************************
=== 直线近似计算 ===
y1 = kx1+b       x <-> adc_value
y2 = kx2+b       y <-> temper
yn = kxn+b
      y2 - y1     yn - y1
k  = --------- = ---------
      x2 - x1     xn - x1
      (y2 - y1)*(xn - x1)
yn = --------------------- + y1
            (x2 - x1)
y2 - y1 = 1 ,以1为段高
*************************************************/
int16_t iADCTemperCalc10KV2(uint16_t index,uint16_t adcValue)
{
	int16_t t; 
	if (index >= ADC10KV2_INDEX_MAX )
	{
		//@@@@@@return err
		t=10*((int16_t)ADC10KV2_INDEX_MAX + (int16_t)ADC10KV2_TEMPER_REF -1);
		return t;
	}
	else if(index <= ADC10KV2_INDEX_MIN)
	{
		return 10*(ADC10KV2_INDEX_MIN + ADC10KV2_TEMPER_REF -1);
	}
	else{
		t = 10*(ntcAdc10KV2[index-1] - adcValue);
		t /=(ntcAdc10KV2[index-1] - ntcAdc10KV2[index]);
		t +=10*(index + ADC10KV2_TEMPER_REF -1);
		return t;
	}
}


uint16_t uADCSearchData5K(uint16_t searchValue)
{
	uint16_t max = ADC5K_ARRAY_NUM - 1;
	uint16_t min=1;
	uint16_t mid;

	while(min <= max)
	{
		mid = (max + min) >> 1;		//mean (max+min)/2
		if (searchValue > ntcAdc5K[mid])
		{
			min=mid+1;
		}
		else
			max=mid-1;
	}

	return min;
}

int16_t iADCTemperCalc5K(uint16_t index,uint16_t adcValue)
{
	int16_t t; 
	if (index >= ADC5K_INDEX_MAX )
	{
		//@@@@@@return err
		t = 10*((int16_t)ADC5K_INDEX_MAX + (int16_t)ADC5K_TEMPER_REF -1);
		return t;
	}
	else if(index <= ADC5K_INDEX_MIN)
	{
		return 10 * (ADC5K_INDEX_MIN + ADC5K_TEMPER_REF -1);
	}
	else{
		t = 10*(ntcAdc5K[index-1] - adcValue);
		t /= (ntcAdc5K[index-1] - ntcAdc5K[index]);
		t += 10*(index - 1 + ADC5K_TEMPER_REF );
		return t;
	}
}

