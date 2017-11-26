
#pragma once

#include "stdafx.h"
using namespace std;

//enum	StruType		{钢筋混凝土框架, 框架剪力墙, 砖混结构, 钢结构};	//结构类型
//enum	ConstrucYear	{before_1979, from_1979_to_1989, after_1989};	//建造时间所属的时间阶段


struct bldtyp
{							//用户存储建筑类型及相关参数
	char name[5];			//如  C2L
	int lowlmt,highlmt;		//该状态适用的最低、最高楼层
	double Props[10];		//10参数层间滞回模型，与熊的一一对应。但是前两个参数要重新计算
	double state[4];		//震害损伤状态
	double T1,T2,damp;		//从PreCode等文件中读进来的是：T1=T0/N  (T0：基本周期，N：建筑层数)，T2=0.333
							//程序中要重新计算，使得T1、T2分别为结构的真实的前两阶周期
	
	double hos;				//层高
	int year;				//建造年代
};




//程序用到的几个类的定义，用于存储建筑信息

class BuildingInfo {		//用于存储建筑的各种信息

public:
	
	//从GIS数据表中直接读出的信息
	
	CString			name;					//名称
	int				constructionTime;		//建造时间
	int				nofStory;				//层数
	//StruType		struType;				//结构类型
	CString			struType;				//结构类型
	double			storyHeight;			//层高
	CString			descriptions;			//特殊情况


		//暂时没用到的属性：
		//int				FID;	
		//int				ID;
		//double			buildingArea;			//建筑面积
		double			floorArea;				//占地面积
		//CString			structureType;			//结构类型
		//ConstrucYear		constructionYear;		//建造时间所属的时间阶段
		CString			function;				//结构功能
		//double			columnRatio;			//柱率
		//double			longitudinalBarRatio;	//纵筋配筋率
		//double			hoopRatio;				//箍筋配筋率
		//CString			concreteGrade			//混凝土标号
		//CString			rebarGrade				//钢筋标号
		//double			wallRatioX				//X方向墙率
		//double			wallRatioY				//Y方向墙率
		//CString			brickGrade				//砌块标号
		//CString			mortarGrade				//砂浆标号
		//CString			steelGrade				//钢材标号


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