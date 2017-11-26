
// IntegrationDlg.h : 头文件
//

#pragma once
#include "stdafx.h"
#include "DataStructure.h"
#include "afxwin.h"
using namespace std;

// CIntegrationDlg 对话框
class CIntegrationDlg : public CDialogEx
{
// 构造
public:
	CIntegrationDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_INTEGRATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	//对话框的变量
	int							numberOfBuildings;				// 建筑物总数
	vector<BuildingInfo>		buildings;						// 用于存储所有建筑物
	multimap<int,Point>			points;							// 用于存储所有建筑的角点

	afx_msg void OnBnClickedSetProjectPath();				//设置工程目录，并获取该路径
	CString m_workPath;										//工程文件完整路径
	CString ProjectDirectory;								//工作目录
	CString m_ProjectName;									//工程文件名称
	CString ProjectNameNo_cal;								//没有.cal的工程文件名
	CString thisPath;										//当前程序的工作路径


	//选择数据文件部分的变量和控件
	afx_msg void OnBnClickedOpenAttrFile();			// 打开属性表文件，并获得属性表文件的路径
	CString m_strAttrFilePath;
	afx_msg void OnEnChangeAttrFilePath();
	afx_msg void OnBnClickedOpenShapeFile();		// 打开形状文件，并获得形状文件的路径
	CString m_strShapeFilePath;
	afx_msg void OnEnChangeShapeFilePath();


	//前处理部分的变量和控件
	afx_msg void OnBnClickedViewOrEditBuildings();	//查看、修改各建筑的滞回曲线等信息
	afx_msg void OnBnClickedGroundMotion();			//选择地震波、PGA、地震动角度等参数

	//分析计算和后处理部分的变量和控件

	afx_msg void OnBnClickedAnalyse();				//调用计算程序进行计算
	afx_msg void OnBnClickedPostProcess();			//调用后处理程序，查看分析结果

	//获取程序当前路径
	CString CIntegrationDlg::GetCurrWorkingDir();

	double lambda(int n) { return 0.4053*(double)(n*n)+0.405*(double)(n)+0.1869;};

};
