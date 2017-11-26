#pragma once
#include "afxwin.h"


// CParaBasic �Ի���

class CParaBasic : public CDialog
{
	DECLARE_DYNAMIC(CParaBasic)

public:
	CParaBasic(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CParaBasic();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	CComboBox m_CBName;
	int m_intNoF;
	CString m_strType;
	CComboBox m_CBType;
	int m_intYear;
	float m_fltArea;
	float m_fltGood;
	float m_fltSlightly;
	float m_fltModerate;
	float m_fltSerious;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCbnSelchangeBuildingName();
	afx_msg void OnCbnSelchangeBuildingType();
	afx_msg void OnEnChangeFloorNo();
//	afx_msg void OnEnKillfocusYear();
public:
	float m_styHeight;
	CComboBox m_bldFunctionCtrl;
	CString m_BldFunctionValue;
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit3();
};
