
// PreProcessDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "parabasic.h"
#include "parastruc.h"
#include "paradyn.h"
#include "strucdata.h"
#include "string"

// CPreProcessDlg �Ի���
class CPreProcessDlg : public CDialog
{
// ����
public:
	CPreProcessDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PREPROCESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	void ReCalcData(int BuildingNO);		//�����Ľṹ���͡�������ʱ��Ӧ����Ӧ�����¼�����������
	void SaveToFile(CString str);
	StrucData** data;					//һ��StrucData*���͵����飬�洢���нṹ
	void SaveToData(int nIndex,int Floor);		//���ؼ��е�ֵ���浽data������
	void SaveToControl(int nIndex,int CurFloor);	//���¿ؼ���ʾ
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
	double lambda(int n) { return 0.4053*(double)(n*n)+0.405*(double)(n)+0.1869;};
};



