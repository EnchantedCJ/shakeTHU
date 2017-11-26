#pragma once
#include "afxwin.h"


// CParaDyn 对话框

class CParaDyn : public CDialog
{
	DECLARE_DYNAMIC(CParaDyn)

public:
	CParaDyn(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParaDyn();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CBrush m_brush;//用于自定义颜色
	CDC m_memDC;//屏幕DC兼容的内存DC
	CBitmap m_Bmp;//位图
	CWnd* m_pDrawWnd;//用于保存静态文本框的对象指针
	//
	// 初始化和双缓冲相关的数据
	void InitialDBB();
	//双缓冲内存上绘图
	void DrawOnMem();
	//在静态区域画图
	void DrawOnStaticArea();
	//更改颜色的消息响应函数
	virtual BOOL OnInitDialog();
	
	float** vector;
	afx_msg void OnBnClickedButton1();
	CListBox m_ctlFreq;
	int flag;
	afx_msg void OnLbnSelchangeList1();
	int* selecteditem;
	int NOselected;
	float damping1;
	float FreqRatio;
	afx_msg void OnBnClickedButton2();
	void ModalCal(void);
	double* Evals;
	double** Evecs;
	int NoF;
};
