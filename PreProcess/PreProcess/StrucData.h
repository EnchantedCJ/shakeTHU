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
	CString BldFunction;	//�������ܣ��칫¥��סլ...
	int Year;
	int YearType;
	int NoFloor;
	float FloHeight;
	float FloArea;
	float n;			//һ������
	float n12;			//��������
	float damping;
	float** props;		//��ά���飬props[j][k]�ǵ�k��ĵ�j������ͻ�����
	float* damage;		//���˼�����
	
	int NoB;		//��������
	int BID;		//����ID	
};

