
// IntegrationDlg.h : ͷ�ļ�
//

#pragma once
#include "stdafx.h"
#include "DataStructure.h"
#include "afxwin.h"
using namespace std;

// CIntegrationDlg �Ի���
class CIntegrationDlg : public CDialogEx
{
// ����
public:
	CIntegrationDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_INTEGRATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	//�Ի���ı���
	int							numberOfBuildings;				// ����������
	vector<BuildingInfo>		buildings;						// ���ڴ洢���н�����
	multimap<int,Point>			points;							// ���ڴ洢���н����Ľǵ�

	afx_msg void OnBnClickedSetProjectPath();				//���ù���Ŀ¼������ȡ��·��
	CString m_workPath;										//�����ļ�����·��
	CString ProjectDirectory;								//����Ŀ¼
	CString m_ProjectName;									//�����ļ�����
	CString ProjectNameNo_cal;								//û��.cal�Ĺ����ļ���
	CString thisPath;										//��ǰ����Ĺ���·��


	//ѡ�������ļ����ֵı����Ϳؼ�
	afx_msg void OnBnClickedOpenAttrFile();			// �����Ա��ļ�����������Ա��ļ���·��
	CString m_strAttrFilePath;
	afx_msg void OnEnChangeAttrFilePath();
	afx_msg void OnBnClickedOpenShapeFile();		// ����״�ļ����������״�ļ���·��
	CString m_strShapeFilePath;
	afx_msg void OnEnChangeShapeFilePath();


	//ǰ�����ֵı����Ϳؼ�
	afx_msg void OnBnClickedViewOrEditBuildings();	//�鿴���޸ĸ��������ͻ����ߵ���Ϣ
	afx_msg void OnBnClickedGroundMotion();			//ѡ����𲨡�PGA�����𶯽ǶȵȲ���

	//��������ͺ����ֵı����Ϳؼ�

	afx_msg void OnBnClickedAnalyse();				//���ü��������м���
	afx_msg void OnBnClickedPostProcess();			//���ú�����򣬲鿴�������

	//��ȡ����ǰ·��
	CString CIntegrationDlg::GetCurrWorkingDir();

	double lambda(int n) { return 0.4053*(double)(n*n)+0.405*(double)(n)+0.1869;};

};
