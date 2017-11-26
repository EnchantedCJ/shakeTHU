// drawDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CdrawDlg 对话框
class CdrawDlg : public CDialog
{
// 构造
public:
	//对话框全局变量
	CString  workPath;		//工程路径
	TCHAR  thisPath[MAX_PATH];		//当前程序的路径
	CString  EQname;		//当前地震波名称
	void	CdrawDlg::OnCancel();



	CdrawDlg(CWnd* pParent = NULL);	// 标准构造函数
	CBrush m_brush;//用于自定义颜色
    CDC drawDC,drawDCspe;//屏幕DC兼容的内存DC
    CBitmap m_Bmp;//位图
	CBitmap m_Bmpspe;//位图
    CWnd* m_pDrawWnd;//用于保存静态文本框的对象指针
	CWnd* m_Drawspe;//用于保存静态文本框的对象指针
	int nIndex;
	int EQfilenum;
	int SPEfilenum;
	int adiIndex;
	COLORREF color;
	CStdioFile initialfile;
	CString strcolor;
	CString   RECpathname;
     // 初始化和双缓冲相关的数据
    void InitialDBB();
    //双缓冲内存上绘图
	void InitialDBBspe();
    //双缓冲内存上绘图
    void DrawOnMem(CString Earname);
    //双缓冲内存上初始化图表
	void DrawChart();
    //双缓冲内存上初始化图表
	void DrawChartspe();
    //在静态区域画图
	void Drawspe(CString maxminname,CString filename);
    //在静态区域画图
    void DrawOnStaticArea(CString Earname);
	void DrawOnStaticAreaspe(CString maxminname,CString filename);
	void StoreData(CString filename,CString outname,CString maxminname,COLORREF color,int EQSPE);
    //更改颜色的消息响应函数
     afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);

// 对话框数据
	enum { IDD = IDD_DRAW_DIALOG };

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButton2();
	bool m_ifexpand;
	CRect winrect;
	
private:
	// 判断是否扩展对话框
	// //点击绘制曲线后扩展显示窗口
public:
	afx_msg void OnBnClickedButton3();
	CButton viewbtn;
	afx_msg void OnBnClickedButton4();
	afx_msg void SCGFFYP();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	// 选择地震波
	CComboBox Choseear;
	CEdit AngleControl;
	// 地震动输入角度，与x的夹角
	double Angle;
	// 判断是否输入地震动角度
	CButton InAngle;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton11();
	CButton CXbtn;
	CButton CYbtn;
	CButton CDbtn;
	afx_msg void OnBnClickedButton12();
	CButton Drawspe1;
	CButton PGAedit;
	CEdit InPGA;
	double PGA;
	afx_msg void OnBnClickedCheck2();
	// 场地类别
	CComboBox CDLB;
	// 地震分组
	CComboBox DZFZ;
	// 地震影响
	CComboBox DZYX;
	// 地震动类型
	CButton TypeEQ;
	// 绘制规范反应谱
	CButton HZGFFYP;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedAddeq();
	CString newEQ;
	CEdit newEQname;
	afx_msg void OnBnClickedInfo();
	double m_AnalyzeTime;
	CEdit m_AnalyzeTimeCtrl;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnEnChangeEdit2();
};
