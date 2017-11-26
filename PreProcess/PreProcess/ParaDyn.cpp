// ParaDyn.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PreProcess.h"
#include "ParaDyn.h"
#include "PreProcessDlg.h"


#include <math.h>
#include <iostream>
#include <stdio.h>
#include "dsygv.h"

// CParaDyn �Ի���

IMPLEMENT_DYNAMIC(CParaDyn, CDialog)

CParaDyn::CParaDyn(CWnd* pParent /*=NULL*/)
	: CDialog(CParaDyn::IDD, pParent)
	
	
	, vector(NULL)
	, flag(0)
	, selecteditem(NULL)
	, NOselected(0)
	, damping1(0)
	, FreqRatio(0)
	, Evals(NULL)
	, Evecs(NULL)
	, NoF(0)
{

}

CParaDyn::~CParaDyn()
{
	delete selecteditem;
	delete Evals;
	for (int i=0;i<100;i++)
	{
		delete vector[i];
	}
	delete vector;

	//for (int i=0;i<n;i++)
	//{
	//	delete Evecs[i];
	//}
	//delete Evecs;
}

void CParaDyn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctlFreq);
	DDX_Text(pDX, IDC_EDIT2, damping1);
	DDV_MinMaxFloat(pDX, damping1, 0, 1);
	DDX_Text(pDX, IDC_EDIT3, FreqRatio);
//	DDV_MinMaxFloat(pDX, FreqRatio, 0, 1);
}


BEGIN_MESSAGE_MAP(CParaDyn, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CParaDyn::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CParaDyn::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CParaDyn::OnBnClickedButton2)
END_MESSAGE_MAP()


// CParaDyn ��Ϣ�������

HBRUSH CParaDyn::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	
	if(pWnd->GetDlgCtrlID()==IDOK || pWnd->GetDlgCtrlID()==IDCANCEL)
	{
		pDC->SetTextColor(RGB(255,0,0));//�����ı���ɫ
		pDC->SetBkMode(TRANSPARENT);//���ֱ�����Ϊ͸��
		pDC->SetBkColor(RGB(255,0,0));//���ñ���ɫ
		return m_brush;
	}
	//

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CParaDyn::InitialDBB()
{
	CRect rt;
	m_pDrawWnd->GetClientRect(&rt);
	CDC* sDC = m_pDrawWnd->GetDC();
	// Ϊ��ĻDC�������ݵ��ڴ�DC
	if(!m_memDC.CreateCompatibleDC(sDC))//
	{              
		::PostQuitMessage(0);
	} 

	// ����λͼ,������m_memDC����������ɫ
	m_Bmp.CreateCompatibleBitmap(sDC, rt.Width(), rt.Height());//m_memDC
	// �൱��ѡ�񻭲�,m_pDrawWnd->
	::SelectObject(m_memDC.GetSafeHdc(), m_Bmp); 
	m_pDrawWnd->ReleaseDC(sDC);
}

//˫�����ڴ��ϻ�ͼ
void CParaDyn::DrawOnMem()
{
	int i,j;
	int n;
	float MaxDisp,Divider;
	Divider=2.5;
	MaxDisp=1;
	n=NoF;
	CRect rect;
	float FloArea,mass,StiffCoef,stiff;
	
	

	m_pDrawWnd->GetClientRect(&rect);
	//COLORREF crl = GetSysColor(COLOR_3DFACE);
	//m_memDC.FillSolidRect(rect, crl); 
	m_memDC.FillSolidRect(&rect, RGB(255,255,255));// ��ɫ���, ע��,����ǻ����ڴ��豸������
	CPen pen(PS_SOLID,1,RGB(0,0,0));
	CPen pen2(PS_SOLID,1,RGB(255,0,0));
	CPen pen3(PS_DASH,1,RGB(100,100,100));
	m_memDC.SelectObject(&pen);


	for (i=0;i<NOselected;i++)	//�ж��������ֵ����ֹ�����ڻ���֮��
	{
		for (j=0;j<NoF;j++)
		{
			if(abs(Evecs[j][selecteditem[i]])>MaxDisp)
				MaxDisp=abs(Evecs[j][selecteditem[i]]);
		}
	}
	Divider=MaxDisp*2.5;

	//�ж��������ֵ����ֹ�����ڻ���֮��

	//��ͼ����
	CFont *MyFont= new CFont;
	CBrush brush,*oldbrush;
	MyFont->CreateFont(15,0,0,0,FW_MEDIUM,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("TIME NEW ROMAN"));
	m_memDC.SelectObject(MyFont);
	m_memDC.Rectangle(rect);

	m_memDC.SelectObject(&pen3);
	m_memDC.MoveTo(rect.right/2,rect.top);
	m_memDC.LineTo(rect.right/2,rect.bottom);
	m_memDC.SelectObject(&pen);
	m_memDC.MoveTo(rect.right/2,rect.bottom);
	m_memDC.LineTo(rect.right/2,rect.bottom-8);
	m_memDC.MoveTo(rect.right/2-rect.right/Divider,rect.bottom);
	m_memDC.LineTo(rect.right/2-rect.right/Divider,rect.bottom-8);
	m_memDC.MoveTo(rect.right/2+rect.right/Divider,rect.bottom);
	m_memDC.LineTo(rect.right/2+rect.right/Divider,rect.bottom-8);

	m_memDC.SelectObject(&pen2);
	brush.CreateSolidBrush(RGB(255,0,0));
	if (NoF!=0)
	{
		
		
		char Freqstr[20];
		CString Freq;
		
		//printf(Freqstr,_T("%10.3f"),10.1);
		//Freq=Freqstr;
		if(Evals!=0x00000000)
		{
			m_ctlFreq.ResetContent();
			for (i=0;i<n;i++)
			{
				Freq.Format(_T("%s%d%10.3f%s"),_T("Mod"),i+1,Evals[i],_T("Hz"));
				m_ctlFreq.AddString(Freq);
				
			}
		}
		
		



		for (j=0;j<NOselected;j++)
		{
			m_memDC.MoveTo(rect.right/2,rect.bottom);
			
			for (i=0;i<NoF;i++)
			{

				m_memDC.LineTo(rect.right/2+(rect.right/Divider)*Evecs[i][selecteditem[j]],rect.bottom-(i+1)*rect.bottom/NoF);
				m_memDC.MoveTo(rect.right/2+(rect.right/Divider)*Evecs[i][selecteditem[j]],rect.bottom-(i+1)*rect.bottom/NoF);
				oldbrush=m_memDC.SelectObject(&brush);
				m_memDC.Ellipse(rect.right/2+(rect.right/Divider)*Evecs[i][selecteditem[j]]-3,rect.bottom-(i+1)*rect.bottom/NoF-3,rect.right/2+(rect.right/Divider)*Evecs[i][selecteditem[j]]+3,rect.bottom-(i+1)*rect.bottom/NoF+3);
				m_memDC.SelectObject(oldbrush);
			}
		}

	}
	
	m_memDC.TextOut(rect.right/2+5,rect.bottom-20,_T("0"));
	m_memDC.TextOut(rect.right/2-rect.right/Divider,rect.bottom-20,_T("-1"));
	m_memDC.TextOut(rect.right/2+rect.right/Divider,rect.bottom-20,_T("1"));


	NOselected=0;

	delete MyFont;
	
	
	
	
}
//
//�ھ�̬����ͼ
void CParaDyn::DrawOnStaticArea()
{
	CWnd* pWnd = GetDlgItem(IDC_STATIC1);//��þ�̬�ı���Ĵ��ڶ���
	CRect rect;
	pWnd->GetClientRect(&rect);    
	//
	CDC* pDC = pWnd->GetDC();//
	//
	DrawOnMem();
	// һ���ԵĽ��ڴ��豸�����ϻ�����ϵ�ͼ��"��"����Ļ��
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_memDC, 0, 0, SRCCOPY);
	pWnd->ReleaseDC(pDC);//
	//
}


void CParaDyn::OnPaint()
{
	
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	//�ػ�ͼ������
	PAINTSTRUCT ps;
	CRect rt;
	m_pDrawWnd->GetClientRect(&rt);    
	CDC* pDC=m_pDrawWnd->BeginPaint(&ps);
	if (flag)
	{
		DrawOnMem();
		flag--;
	}
	
	pDC->BitBlt(0, 0, rt.Width(), rt.Height(), &m_memDC, 0, 0, SRCCOPY);    
	m_pDrawWnd->EndPaint(&ps);

}

BOOL CParaDyn::OnInitDialog()
{
	CDialog::OnInitDialog();
	int i,j;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_brush.CreateSolidBrush(RGB(255,0,0));//��ʼ����ˢ
	m_pDrawWnd = GetDlgItem(IDC_STATIC1);//��þ�̬���ڶ���ָ��
	InitialDBB();// ��ʼ��˫������ص�����
	vector = new float*[100];
	for (i=0; i<100; i++) vector[i] = new float[100];
	
	for (i=0;i<100;i++)
	{
		for (j=0;j<100;j++)
		{
			vector[i][j]=0;

		}
	}
	vector[0][0]=0.1;
	vector[1][0]=0.3;
	vector[2][0]=0.5;
	vector[3][0]=0.7;
	vector[4][0]=1.0;

	vector[0][1]=0.2;
	vector[1][1]=0.4;
	vector[2][1]=0.6;
	vector[3][1]=0.8;
	vector[4][1]=1.0;

	vector[0][2]=-0.1;
	vector[1][2]=-0.3;
	vector[2][2]=-0.5;
	vector[3][2]=0.3;
	vector[4][2]=1.0;
	flag=10;
	selecteditem= new int[100];
	for (i=0;i<100;i++)
		selecteditem[i]=0;
	NOselected=-1;

	CWnd* pWnd=AfxGetApp()->GetMainWnd();
	CPreProcessDlg* dlg;
	dlg= (CPreProcessDlg*)pWnd;
	//dlg->m_basic.UpdateData(TRUE);
	NoF=dlg->m_basic.m_intNoF;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CParaDyn::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (NOselected!=0)
	{
		DrawOnStaticArea();
	}
	else if(NOselected=-1)
		MessageBox(_T("��ѡ������һ������"));
	/*else if(NOselected=-1)
	MessageBox(_T("���ȼ�������"));*/
		
	
}

void CParaDyn::OnLbnSelchangeList1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int i;
	//for (int i = 0; i < m_ctlFreq.GetSelCount(); i++) 
	//{
	m_ctlFreq.GetSelItems(10,selecteditem);
	NOselected=m_ctlFreq.GetSelCount();
	//}

}

void CParaDyn::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CWnd* pWnd=AfxGetApp()->GetMainWnd();
	CPreProcessDlg* dlg;
	dlg= (CPreProcessDlg*)pWnd;
	dlg->m_basic.UpdateData(TRUE);
	NoF=dlg->m_basic.m_intNoF;
	if (NoF!=0)
	{
		
		ModalCal();
		DrawOnStaticArea();
	}
	else
		MessageBox(_T("���ȵ��뽨�������ļ�"));
	
}

void CParaDyn::ModalCal(void)
{
	int i,j,n,BuildingNo,FloorNo;
	float FloArea,mass;
	float* StiffCoef;
	float* stiff;
	CWnd* pWnd=AfxGetApp()->GetMainWnd();
	CPreProcessDlg* dlg;
	dlg= (CPreProcessDlg*)pWnd;
	n=dlg->m_basic.m_intNoF;
	FloArea=dlg->m_basic.m_fltArea;
	StiffCoef = new float[n];
	stiff = new float[n];
	BuildingNo=dlg->m_basic.m_CBName.GetCurSel();
	FloorNo=dlg->m_struc.m_CBCurFloor.GetCurSel();
	mass=1000*FloArea;
	dlg->SaveToData(BuildingNo,FloorNo);
	for (i=0;i<n;i++)
	{
		StiffCoef[i]=dlg->data[BuildingNo]->props[0][i];
		//stiff[i]=mass*9.8*NoF*StiffCoef[i];
		stiff[i]=StiffCoef[i];
	}
	

	dlg->m_basic.UpdateData(TRUE);
	dlg->m_struc.UpdateData(TRUE);
	//ģ̬����**********************************
	double **K, **M;
	int  k;


	// Generate the matrix for my equation

	K = new double*[n];
	for (i=0; i<n; i++) K[i] = new double[n];
	M = new double*[n];
	for (i=0; i<n; i++) M[i] = new double[n];

	//�����ʼ��
	for(i=0;i<n;i++)
	{
		for (j=0;j<n;j++)
		{
			K[i][j]=0;
			M[i][j]=0;
		}
	}
	for (i=0; i<n-1; i++)
		K[i][i] = stiff[i]+stiff[i+1];
	K[n-1][n-1]=stiff[n-1];
	for (i=0; i<n-1; i++)
		K[i+1][i] = -stiff[i+1];
	for (i=0; i<n-1; i++)
		K[i][i+1] = -stiff[i+1];

	for (i=0; i<n; i++) 
		M[i][i] = mass;


	delete stiff;
	delete StiffCoef;

	// Call the LAPACK solver

	Evals = new double[n]; 
	Evecs = new double*[n];
	for (i=0; i<n; i++) Evecs[i] = new double[n];



	dsygv(K, M, n, Evals, Evecs);
	for (i=0;i<n;i++)
	{
		for (j=0;j<n;j++)
		{
			Evecs[j][i]=Evecs[j][i]/Evecs[n-1][i];
		}
		Evals[i]=abs(sqrt(Evals[i]))/2.0/3.1415926;
	}

	//ģ̬����**********************************

	//����1����2��Ƶ�ʴ洢����Ӧ�ı�����
	dlg->data[BuildingNo]->n=1.0/Evals[0];
	if (n<2)
	{
		dlg->data[BuildingNo]->n12=0;
	} 
	else
	{
		dlg->data[BuildingNo]->n12=1.0/Evals[1];
	}
	FreqRatio=dlg->data[BuildingNo]->n12;
	UpdateData(FALSE);


	for (int i=0;i<n;i++)
	{
		delete K[i];
		delete M[i];
	}
	delete K;
	delete M;



}
