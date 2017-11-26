#pragma once
#include "afxwin.h"


// CParaDyn �Ի���

class CParaDyn : public CDialog
{
	DECLARE_DYNAMIC(CParaDyn)

public:
	CParaDyn(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CParaDyn();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
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
	//������ɫ����Ϣ��Ӧ����
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
