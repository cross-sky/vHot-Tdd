#include "pressure.h"

uint16_t uPresLSearchData(uint16_t searchValue)
{
	uint16_t max=PRES_L_ARRAY_NUM-1;
	uint16_t min=1;
	uint16_t mid;

	while(min<=max)
	{
		mid=(max+min)>>1;		//mean (max+min)/2
		if (searchValue > Pressure_L[mid])
		{
			min=mid+1;
		}
		else
			max=mid-1;
	}

	return min;
}

int16_t iPresLCalc(uint16_t index,uint16_t adcValue)
{
	int16_t t; 
	if (index >= PRES_L_INDEX_MAX )
	{
		//@@@@@@return err
		t=10*((int16_t)PRES_L_INDEX_MAX + (int16_t)PRES_L_TEMPER_REF -1);
		return t;
	}
	else if(index <= PRES_L_INDEX_MIN)
	{
		return 10*(PRES_L_INDEX_MIN + PRES_L_TEMPER_REF -1);
	}
	else{
		t = 10*(Pressure_L[index-1] - adcValue);
		t /=(Pressure_L[index-1] - Pressure_L[index]);
		t +=10*(index + PRES_L_TEMPER_REF -1);
		return t;
	}
}


/*calc H pressure*/
uint16_t uPresHSearchData(uint16_t searchValue)
{
	uint16_t max=PRES_H_ARRAY_NUM-1;
	uint16_t min=1;
	uint16_t mid;

	while(min<=max)
	{
		mid=(max+min)>>1;		//mean (max+min)/2
		if (searchValue > Pressure_H[mid])
		{
			min=mid+1;
		}
		else
			max=mid-1;
	}

	return min;
}

int16_t iPresHCalc(uint16_t index,uint16_t adcValue)
{
	int16_t t; 
	if (index >= PRES_H_INDEX_MAX )
	{
		//@@@@@@return err
		t=10*((int16_t)PRES_H_INDEX_MAX + (int16_t)PRES_H_TEMPER_REF -1);
		return t;
	}
	else if(index <= PRES_H_INDEX_MIN)
	{
		return 10*(PRES_H_INDEX_MIN + PRES_H_TEMPER_REF -1);
	}
	else{
		t = 10*(Pressure_H[index-1] - adcValue);
		t /=(Pressure_H[index-1] - Pressure_H[index]);
		t +=10*(index + PRES_H_TEMPER_REF -1);
		return t;
	}
}
