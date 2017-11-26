// ParaBasic.cpp : 实现文件
//

#include "stdafx.h"
#include "PreProcess.h"
#include "ParaBasic.h"
#include "PreProcessDlg.h"
#include "StrucData.h"


// CParaBasic 对话框

IMPLEMENT_DYNAMIC(CParaBasic, CDialog)

CParaBasic::CParaBasic(CWnd* pParent /*=NULL*/)
	: CDialog(CParaBasic::IDD, pParent)
	, m_intNoF(0)
	, m_strType(_T(""))
	, m_intYear(0)
	, m_fltArea(0)
	, m_fltGood(0)
	, m_fltSlightly(0)
	, m_fltModerate(0)
	, m_fltSerious(0)
	, m_styHeight(0)
	, m_BldFunctionValue(_T(""))
{

}

CParaBasic::~CParaBasic()
{
}

void CParaBasic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_CBName);
	DDX_Text(pDX, IDC_EDIT2, m_intNoF);
	DDV_MinMaxInt(pDX, m_intNoF, 1, 100);
	DDX_CBString(pDX, IDC_COMBO2, m_strType);
	DDX_Control(pDX, IDC_COMBO2, m_CBType);
	DDX_Text(pDX, IDC_EDIT3, m_intYear);
	DDX_Text(pDX, IDC_EDIT4, m_fltArea);
	DDX_Text(pDX, IDC_EDIT5, m_fltGood);
	DDX_Text(pDX, IDC_EDIT6, m_fltSlightly);
	DDX_Text(pDX, IDC_EDIT7, m_fltModerate);
	DDX_Text(pDX, IDC_EDIT8, m_fltSerious);
	DDX_Text(pDX, IDC_EDIT9, m_styHeight);
	DDX_Control(pDX, IDC_COMBO3, m_bldFunctionCtrl);
	DDX_CBString(pDX, IDC_COMBO3, m_BldFunctionValue);
}



BEGIN_MESSAGE_MAP(CParaBasic, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CParaBasic::OnCbnSelchangeBuildingName)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CParaBasic::OnCbnSelchangeBuildingType)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CParaBasic::OnEnChangeFloorNo)
//	ON_EN_KILLFOCUS(IDC_EDIT3, &CParaBasic::OnEnKillfocusYear)
	ON_EN_CHANGE(IDC_EDIT4, &CParaBasic::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT3, &CParaBasic::OnEnChangeEdit3)
END_MESSAGE_MAP()


void CParaBasic::OnCbnSelchangeBuildingName()
{
	// TODO: 在此添加控件通知处理程序
	int nIndex;
	int LastSel;
	int CurFloor;
	CWnd* pWnd=AfxGetApp()->GetMainWnd();
	CPreProcessDlg* dlg;
	dlg= (CPreProcessDlg*)pWnd;
	nIndex=m_CBName.GetCurSel();
	LastSel=dlg->LastNameSel;
	CurFloor=dlg->m_struc.m_CBCurFloor.GetCurSel();
	dlg->SaveToData(LastSel,CurFloor);
	dlg->LastNameSel=nIndex;
	dlg->SaveToControl(nIndex,0);
	
}




void CParaBasic::OnCbnSelchangeBuildingType()
{

	int BuildingNo, FloorNo;

	CWnd* pWnd= AfxGetApp()->GetMainWnd();
	CPreProcessDlg* dlg;
	dlg=(CPreProcessDlg*)pWnd;
	BuildingNo=dlg->m_basic.m_CBName.GetCurSel();
	FloorNo=dlg->m_struc.m_CBCurFloor.GetCurSel();
	dlg->SaveToData(BuildingNo,FloorNo);
	dlg->ReCalcData(BuildingNo);

}


void CParaBasic::OnEnChangeFloorNo()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	int BuildingNo,i,j,k;
	CWnd* pWnd=AfxGetApp()->GetMainWnd();
	//StrucData** data1;
	//StrucData** data2;
	CPreProcessDlg* dlg;
	dlg=(CPreProcessDlg*)pWnd;
	BuildingNo=dlg->m_basic.m_CBName.GetCurSel();
	int CurFloorNo,OriFloorNo;
	OriFloorNo=sizeof(dlg->data);
	UpdateData(TRUE);
	CurFloorNo=m_intNoF;
	dlg->data[BuildingNo]->NoFloor=CurFloorNo;
		
	for (int i=0;i<10;i++)
	{
		delete dlg->data[BuildingNo]->props[i];
		dlg->data[BuildingNo]->props[i]=new float[CurFloorNo]();

	}
		dlg->ReCalcData(BuildingNo);
	
}


//void CParaBasic::OnEnKillfocusYear()	//当输入焦点离开“建造年代”编辑框时
//{
//
//
//}


void CParaBasic::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	int BuildingNo, FloorNo;

	CWnd* pWnd= AfxGetApp()->GetMainWnd();
	CPreProcessDlg* dlg;
	dlg=(CPreProcessDlg*)pWnd;
	BuildingNo=dlg->m_basic.m_CBName.GetCurSel();
	FloorNo=dlg->m_struc.m_CBCurFloor.GetCurSel();
	dlg->SaveToData(BuildingNo,FloorNo);
	dlg->ReCalcData(BuildingNo);
}


void CParaBasic::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	int BuildingNo, FloorNo;

	CWnd* pWnd= AfxGetApp()->GetMainWnd();
	CPreProcessDlg* dlg;
	dlg=(CPreProcessDlg*)pWnd;
	BuildingNo=dlg->m_basic.m_CBName.GetCurSel();
	FloorNo=dlg->m_struc.m_CBCurFloor.GetCurSel();
	dlg->SaveToData(BuildingNo,FloorNo);
	dlg->ReCalcData(BuildingNo);
}
