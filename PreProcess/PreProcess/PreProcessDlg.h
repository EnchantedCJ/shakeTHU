
// PreProcessDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "parabasic.h"
#include "parastruc.h"
#include "paradyn.h"
#include "strucdata.h"
#include "string"

// CPreProcessDlg 对话框
class CPreProcessDlg : public CDialog
{
// 构造
public:
	CPreProcessDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PREPROCESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_tab;
	CParaBasic m_basic;
	CParaStruc m_struc;
	CParaDyn m_dyn;
//	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void ReCalcData(int BuildingNO);		//当更改结构类型、层数等时，应该相应地重新计算其他数据
	void SaveToFile(CString str);
	StrucData** data;					//一个StrucData*类型的数组，存储所有结构
	void SaveToData(int nIndex,int Floor);		//将控件中的值保存到data数组中
	void SaveToControl(int nIndex,int CurFloor);	//更新控件显示
	int NoB;
	int LastNameSel;

	void openFile(void);
	bool hasBeenSaved;
	CString ProjectDirectory;
	CString ProjectNameNo_cal;
	afx_msg void OnBnClickedOk();

	void	CPreProcessDlg::OnCancel();
	afx_msg void OnBnClickedButtonprevious();
	afx_msg void OnBnClickedButtonnext();


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
	double lambda(int n) { return 0.4053*(double)(n*n)+0.405*(double)(n)+0.1869;};
};



