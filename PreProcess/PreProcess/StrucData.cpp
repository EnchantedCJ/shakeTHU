#include "StdAfx.h"
#include "StrucData.h"


//StrucData::StrucData(void)
//	/*: Name(_T(""))
//	, StrucType(_T(""))
//	, Year(0)
//	, YearType(0)
//	, NoFloor(0)
//	, FloHeight(0)
//	, FloArea(0)
//	, n(0)
//	, n12(0)
//	, damping(0)*/
//{
//}
//
//
StrucData::~StrucData(void)
{
	//float** props;		//��ά���飬props[j][k]�ǵ�j��ĵ�k������ͻ�����
	//float* damage;
	//props = new float* [10]; 
	//damage = new float [10];
	delete damage;
	for (int i=0;i<10;i++)
	{
		delete props[i];
	}
	delete props;

}
