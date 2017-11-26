
#pragma once

#include "stdafx.h"
using namespace std;

//enum	StruType		{�ֽ���������, ��ܼ���ǽ, ש��ṹ, �ֽṹ};	//�ṹ����
//enum	ConstrucYear	{before_1979, from_1979_to_1989, after_1989};	//����ʱ��������ʱ��׶�


struct bldtyp
{							//�û��洢�������ͼ���ز���
	char name[5];			//��  C2L
	int lowlmt,highlmt;		//��״̬���õ���͡����¥��
	double Props[10];		//10��������ͻ�ģ�ͣ����ܵ�һһ��Ӧ������ǰ��������Ҫ���¼���
	double state[4];		//������״̬
	double T1,T2,damp;		//��PreCode���ļ��ж��������ǣ�T1=T0/N  (T0���������ڣ�N����������)��T2=0.333
							//������Ҫ���¼��㣬ʹ��T1��T2�ֱ�Ϊ�ṹ����ʵ��ǰ��������
	
	double hos;				//���
	int year;				//�������
};




//�����õ��ļ�����Ķ��壬���ڴ洢������Ϣ

class BuildingInfo {		//���ڴ洢�����ĸ�����Ϣ

public:
	
	//��GIS���ݱ���ֱ�Ӷ�������Ϣ
	
	CString			name;					//����
	int				constructionTime;		//����ʱ��
	int				nofStory;				//����
	//StruType		struType;				//�ṹ����
	CString			struType;				//�ṹ����
	double			storyHeight;			//���
	CString			descriptions;			//�������


		//��ʱû�õ������ԣ�
		//int				FID;	
		//int				ID;
		//double			buildingArea;			//�������
		double			floorArea;				//ռ�����
		//CString			structureType;			//�ṹ����
		//ConstrucYear		constructionYear;		//����ʱ��������ʱ��׶�
		CString			function;				//�ṹ����
		//double			columnRatio;			//����
		//double			longitudinalBarRatio;	//�ݽ������
		//double			hoopRatio;				//���������
		//CString			concreteGrade			//���������
		//CString			rebarGrade				//�ֽ���
		//double			wallRatioX				//X����ǽ��
		//double			wallRatioY				//Y����ǽ��
		//CString			brickGrade				//������
		//CString			mortarGrade				//ɰ�����
		//CString			steelGrade				//�ֲı��


	BuildingInfo(void);	
							
};

class Point {
public:
	double	x;
	double	y;

	Point()
	{
		x=0.0;
		y=0.0;
	}


};