#pragma once
#include "afxwin.h"


// CParaStruc �Ի���

class CParaStruc : public CDialog
{
	DECLARE_DYNAMIC(CParaStruc)

public:
	CParaStruc(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CParaStruc();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	CBrush m_brush;//�����Զ�����ɫ
	CDC m_memDC;//��ĻDC���ݵ��ڴ�DC
	CBitmap m_Bmp;//λͼ
	CWnd* m_pDrawWnd;//���ڱ��澲̬�ı���Ķ���ָ��
	// 
	// ��ʼ����˫������ص�����
	void InitialDBB();
	//˫�����ڴ��ϻ�ͼ
	void DrawOnMem();
	//�ھ�̬����ͼ
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
