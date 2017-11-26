#pragma once
class StrucData
{
public:
	StrucData(void)
		: NoB(0)
		, BID(0)
	{
		props = new float* [10]; 
		damage = new float [10];
	}
	
	~StrucData(void);
	CString Name;
	CString StrucType;
	CString BldFunction;	//建筑功能：办公楼、住宅...
	int Year;
	int YearType;
	int NoFloor;
	float FloHeight;
	float FloArea;
	float n;			//一阶周期
	float n12;			//二阶周期
	float damping;
	float** props;		//二维数组，props[j][k]是第k层的第j个层间滞回属性
	float* damage;		//损伤级别定义
	
	int NoB;		//建筑总数
	int BID;		//建筑ID	
};

