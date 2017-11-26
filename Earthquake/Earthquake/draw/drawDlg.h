// drawDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CdrawDlg �Ի���
class CdrawDlg : public CDialog
{
// ����
public:
	//�Ի���ȫ�ֱ���
	CString  workPath;		//����·��
	TCHAR  thisPath[MAX_PATH];		//��ǰ�����·��
	CString  EQname;		//��ǰ��������
	void	CdrawDlg::OnCancel();



	CdrawDlg(CWnd* pParent = NULL);	// ��׼���캯��
	CBrush m_brush;//�����Զ�����ɫ
    CDC drawDC,drawDCspe;//��ĻDC���ݵ��ڴ�DC
    CBitmap m_Bmp;//λͼ
	CBitmap m_Bmpspe;//λͼ
    CWnd* m_pDrawWnd;//���ڱ��澲̬�ı���Ķ���ָ��
	CWnd* m_Drawspe;//���ڱ��澲̬�ı���Ķ���ָ��
	int nIndex;
	int EQfilenum;
	int SPEfilenum;
	int adiIndex;
	COLORREF color;
	CStdioFile initialfile;
	CString strcolor;
	CString   RECpathname;
     // ��ʼ����˫������ص�����
    void InitialDBB();
    //˫�����ڴ��ϻ�ͼ
	void InitialDBBspe();
    //˫�����ڴ��ϻ�ͼ
    void DrawOnMem(CString Earname);
    //˫�����ڴ��ϳ�ʼ��ͼ��
	void DrawChart();
    //˫�����ڴ��ϳ�ʼ��ͼ��
	void DrawChartspe();
    //�ھ�̬����ͼ
	void Drawspe(CString maxminname,CString filename);
    //�ھ�̬����ͼ
    void DrawOnStaticArea(CString Earname);
	void DrawOnStaticAreaspe(CString maxminname,CString filename);
	void StoreData(CString filename,CString outname,CString maxminname,COLORREF color,int EQSPE);
    //������ɫ����Ϣ��Ӧ����
     afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);

// �Ի�������
	enum { IDD = IDD_DRAW_DIALOG };

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButton2();
	bool m_ifexpand;
	CRect winrect;
	
private:
	// �ж��Ƿ���չ�Ի���
	// //����������ߺ���չ��ʾ����
public:
	afx_msg void OnBnClickedButton3();
	CButton viewbtn;
	afx_msg void OnBnClickedButton4();
	afx_msg void SCGFFYP();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	// ѡ�����
	CComboBox Choseear;
	CEdit AngleControl;
	// ��������Ƕȣ���x�ļн�
	double Angle;
	// �ж��Ƿ�������𶯽Ƕ�
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
	// �������
	CComboBox CDLB;
	// �������
	CComboBox DZFZ;
	// ����Ӱ��
	CComboBox DZYX;
	// ��������
	CButton TypeEQ;
	// ���ƹ淶��Ӧ��
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
