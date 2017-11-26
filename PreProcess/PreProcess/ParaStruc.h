#pragma once
#include "afxwin.h"


// CParaStruc 对话框

class CParaStruc : public CDialog
{
	DECLARE_DYNAMIC(CParaStruc)

public:
	CParaStruc(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParaStruc();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float m_fltProps1;
	float m_fltProps2;
	float m_fltProps3;
	float m_fltProps4;
	float m_fltProps5;
	float m_fltProps6;
	float m_fltProps7;
	float m_fltProps8;
	float m_fltProps9;
	float m_fltProps10;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonHelp();
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

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog(); 
	afx_msg void OnBnClickedButton4();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEdit m_CEProps10;
	CComboBox m_CBCurFloor;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeCurFloor();
	int CurFloor;
	int LastFloor;
	afx_msg void OnCbnSelchangeButtonnext();
};
