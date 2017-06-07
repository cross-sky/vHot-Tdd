#include "adc_filter.h"

/*
***************************************************************************************************
* ��������: search_data()
* �䡡  ��: search_value  ���һ�׼ֵ
* �䡡  ��:
* ����  ��:
* �ա�  ��: 2010.08.11
* ��������: ���ֲ���adc����ֵ
*--------------------------------------------------------------------------------------------------
*ע     ��:
*    1.    ����ֵΪ���ұ���е�һ��С�ڲ���ֵ(search_value)���±ꣻ
*    2.    ����ֵ(search_value)���ڲ��ұ�������ֵʱ������1
*          ����ֵ(search_value)С�ڲ��ұ������Сֵʱ�����ز��ұ��������������±�+1
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

//�����������--��׼2k
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
* ��������: temper_calc()
* �䡡  ��: index       �����±�
*           adc0_value  ad�˲���Ĳ���ֵ
* �䡡  ��: ntc�¶Ȳ���ֵ
* ����  ��:
* �ա�  ��: 2010.10.26
* ��������: �¶ȼ���(�ֶ����Բ�ֵ�㷨)
***************************************************************************************************
*/
/*************************************************
=== ֱ�߽��Ƽ��� ===
y1 = kx1+b       x <-> adc_value
y2 = kx2+b       y <-> temper
yn = kxn+b
      y2 - y1     yn - y1
k  = --------- = ---------
      x2 - x1     xn - x1
      (y2 - y1)*(xn - x1)
yn = --------------------- + y1
            (x2 - x1)
y2 - y1 = 1 ,��1Ϊ�θ�
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

