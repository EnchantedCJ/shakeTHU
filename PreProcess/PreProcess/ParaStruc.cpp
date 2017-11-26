// ParaStruc.cpp : 实现文件
//

#include "stdafx.h"
#include "PreProcess.h"
#include "ParaStruc.h"
#include "PreProcessDlg.h"
#include <math.h>


// CParaStruc 对话框

IMPLEMENT_DYNAMIC(CParaStruc, CDialog)

CParaStruc::CParaStruc(CWnd* pParent /*=NULL*/)
	: CDialog(CParaStruc::IDD, pParent)
	, m_fltProps1(0)
	, m_fltProps2(0)
	, m_fltProps3(0)
	, m_fltProps4(0)
	, m_fltProps5(0)
	, m_fltProps6(0)
	, m_fltProps7(0) 
	, m_fltProps8(0)
	, m_fltProps9(0)
	, m_fltProps10(0)
	, CurFloor(0)
	, LastFloor(0)
{

}

CParaStruc::~CParaStruc()
{
}

void CParaStruc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_luqu1, m_fltProps1);
	DDX_Text(pDX, IDC_EDIT_luqu2, m_fltProps2);
	DDX_Text(pDX, IDC_EDIT_LUQU3, m_fltProps3);
	DDX_Text(pDX, IDC_EDIT_LUQU4, m_fltProps4);
	DDX_Text(pDX, IDC_EDIT_LUQU5, m_fltProps5);
	DDX_Text(pDX, IDC_EDIT_LUQU6, m_fltProps6);
	DDX_Text(pDX, IDC_EDIT_LUQU7, m_fltProps7);
	DDX_Text(pDX, IDC_EDIT_LUQU8, m_fltProps8);
	DDX_Text(pDX, IDC_EDIT_LUQU9, m_fltProps9);
	DDX_Text(pDX, IDC_EDIT_LUQU10, m_fltProps10);
	DDX_Control(pDX, IDC_EDIT_LUQU10, m_CEProps10);
	DDX_Control(pDX, IDC_COMBO1, m_CBCurFloor);
}


BEGIN_MESSAGE_MAP(CParaStruc, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON3, &CParaStruc::OnBnClickedButtonHelp)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON4, &CParaStruc::OnBnClickedButton4)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, &CParaStruc::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CParaStruc::OnCbnSelchangeCurFloor)
	ON_CBN_SELCHANGE(IDC_BUTTONNEXT, &CParaStruc::OnCbnSelchangeButtonnext)
END_MESSAGE_MAP()


// CParaStruc 消息处理程序

void CParaStruc::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	PAINTSTRUCT ps;
	CRect rt;
	m_pDrawWnd->GetClientRect(&rt);    
	CDC* pDC=m_pDrawWnd->BeginPaint(&ps);
	DrawOnMem();
	pDC->BitBlt(0, 0, rt.Width(), rt.Height(), &m_memDC, 0, 0, SRCCOPY);    
	m_pDrawWnd->EndPaint(&ps);

	
	// 不为绘图消息调用 CDialog::OnPaint()
}

void CParaStruc::OnBnClickedButtonHelp()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL,_T("open"),_T("Background theory and step-by-step example.pdf"),NULL,NULL,SW_SHOWNORMAL);
}

HBRUSH CParaStruc::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if(pWnd->GetDlgCtrlID()==IDOK || pWnd->GetDlgCtrlID()==IDCANCEL)
	{
		pDC->SetTextColor(RGB(255,0,0));//设置文本颜色
		pDC->SetBkMode(TRANSPARENT);//文字背景设为透明
		pDC->SetBkColor(RGB(255,0,0));//设置背景色
		return m_brush;
	}


	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

BOOL CParaStruc::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_brush.CreateSolidBrush(RGB(255,0,0));//初始化画刷
	m_pDrawWnd = GetDlgItem(IDC_STATIC2);//获得静态窗口对象指针
	InitialDBB();// 初始化双缓冲相关的数据
	CurFloor=0;
	LastFloor=0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
// 初始化和双缓冲相关的要素
void CParaStruc::InitialDBB()
{
	CRect rt;
	m_pDrawWnd->GetClientRect(&rt);
	CDC* sDC = m_pDrawWnd->GetDC();
	// 为屏幕DC创建兼容的内存DC
	if(!m_memDC.CreateCompatibleDC(sDC))//
	{              
		::PostQuitMessage(0);
	} 

	// 创建位图,不能是m_memDC，否则无颜色
	m_Bmp.CreateCompatibleBitmap(sDC, rt.Width(), rt.Height());//m_memDC
	// 相当于选择画布,m_pDrawWnd->
	::SelectObject(m_memDC.GetSafeHdc(), m_Bmp); 
	m_pDrawWnd->ReleaseDC(sDC);
}

//双缓冲内存上绘图
void CParaStruc::DrawOnMem()
{
	int i,j,nIndex;
	CRect rect;
	CString StrucType;
	m_pDrawWnd->GetClientRect(&rect);
	//COLORREF crl = GetSysColor(COLOR_3DFACE);
	//m_memDC.FillSolidRect(rect, crl); 
	//m_memDC.FillSolidRect(&rect, 0x00FFFFFF);// 白色填充, 注意,这次是画在内存设备环境上
	CPen pen(PS_SOLID,1,RGB(0,0,0));
	CPen pen2(PS_SOLID,2,RGB(255,0,0));
	CPen pen3(PS_SOLID,1,RGB(0,0,0));
	m_memDC.SelectObject(&pen);
	m_memDC.Rectangle(rect);
	int start_x,start_y,end_x,end_y,center_x,center_y,height,width;
	int margin=rect.right/50;
	float point_x[20],point_y[20],pointx,pointy,temp1,temp2,Eh1,Ehc,dplus;
	start_x=margin;
	start_y=margin;
	float ratio_x,ratio_y;
	ratio_x=2;
	ratio_y=2;
	end_x=rect.right-margin;
	end_y=rect.bottom-margin;
	center_x=end_x/ratio_x;
	center_y=end_y/ratio_y;
	height=-(end_y/ratio_y-2*margin);
	width=end_x-end_x/ratio_x-2*margin;

	//画图部分
	//画坐标轴；
	m_memDC.MoveTo(end_x,end_y/ratio_y);
	m_memDC.LineTo(start_x,end_y/ratio_y);
	m_memDC.MoveTo(end_x,end_y/ratio_y);
	m_memDC.LineTo(end_x-margin*2,end_y/ratio_y+margin/2);
	m_memDC.MoveTo(end_x,end_y/ratio_y);
	m_memDC.LineTo(end_x-margin*2,end_y/ratio_y-margin/2);
	
	m_memDC.MoveTo(end_x/ratio_x,start_y);
	m_memDC.LineTo(end_x/ratio_x,end_y);
	m_memDC.MoveTo(end_x/ratio_x,start_y);
	m_memDC.LineTo(end_x/ratio_x+margin/2,start_y+margin*2);
	m_memDC.MoveTo(end_x/ratio_x,start_y);
	m_memDC.LineTo(end_x/ratio_x-margin/2,start_y+margin*2);

	double k,Eta,fy,EtaSoft,k_unload,C,Gamma,Alpha,Beta,a_k,Omega;
	CWnd* pWnd=AfxGetApp()->GetMainWnd();
	CPreProcessDlg* dlg;
	dlg= (CPreProcessDlg*)pWnd;
	k=dlg->m_struc.m_fltProps1;
	fy=dlg->m_struc.m_fltProps2;
	Eta=dlg->m_struc.m_fltProps3;
	C=dlg->m_struc.m_fltProps4;
	Gamma=dlg->m_struc.m_fltProps5;
	EtaSoft=dlg->m_struc.m_fltProps6;
	Alpha=dlg->m_struc.m_fltProps7;
	Beta=dlg->m_struc.m_fltProps8;
	a_k=dlg->m_struc.m_fltProps9;
	Omega=dlg->m_struc.m_fltProps10;

	if (k!=0)
	{
		//计算关键点
		
		nIndex=dlg->m_basic.m_CBType.GetCurSel();
		dlg->m_basic.m_CBType.GetLBText(nIndex,StrucType);
		StrucType=StrucType.TrimRight();
		if (StrucType==_T("钢结构"))
		{
				Gamma=1;
		}
		for(i=0;i<20;i++ )
		{
			point_x[i]=0;
			point_y[i]=0;
		}
		point_x[0]=fy/k;
		point_y[0]=fy;
		point_y[1]=point_y[0]*Alpha;
		point_x[1]=point_x[0]+(point_y[1]-point_y[0])/(k*Eta);
		point_x[2]=2*point_x[0]+point_x[1];
		point_y[2]=point_y[1]+(point_x[2]-point_x[1])*EtaSoft*k;

		point_x[10]=-Beta*point_x[0];
		point_y[10]=-Beta*point_y[0];
		point_y[11]=point_y[10]*Alpha;
		point_x[11]=point_x[10]+(point_y[11]-point_y[10])/(k*Eta);
		point_x[12]=point_x[10]+point_x[11];;
		point_y[12]=point_y[11]+(point_x[12]-point_x[11])*EtaSoft*k;

		k_unload=k*pow((double)(abs(point_x[2]/point_x[0])),(double)(-a_k));
		point_x[3]=point_x[2]-point_y[2]/k_unload;
		point_y[3]=0;
		point_x[5]=(point_x[11]+point_x[12])/2;
		point_y[5]=(point_y[11]+point_y[12])/2;
		point_x[6]=point_x[5]-point_y[5]/k_unload;
		point_y[6]=0;
		/*temp1=point_x[6]+(point_x[5]-point_x[6])*Gamma;
		temp2=point_y[5]*Gamma;*/
		/*point_x[4]=temp1*Omega;
		point_y[4]=temp2-temp2*(temp1*Omega-temp1)/(point_x[3]-temp1);*/

		temp1=point_x[3]+point_y[5]/k_unload;
		temp2=point_y[5];

		point_x[4]=point_x[6]+Gamma*(temp1-point_x[6]);
		point_y[4]=Gamma*temp2;

		//temp1=point_x[3]+(point_x[2]-point_x[3])*Gamma;
		//temp2=point_y[2]*Gamma;

	/*	point_x[7]=temp1*Omega;
		point_y[7]=temp2-temp2*(temp1*Omega-temp1)/(point_x[6]-temp1);*/


		Eh1=(point_y[0]/2.0+point_y[1]+point_y[2]/2.0)*(point_x[2]-point_x[0]);
		Ehc=Eh1*pow((point_x[2]/(point_x[0]*10.0)),2.0);
		dplus=1.0-Ehc/(3.0*C*fy*point_x[0]);

		point_y[8]=point_y[2]*dplus;
		point_x[8]=point_x[3]+dplus*(point_x[2]-point_x[3]);

		temp1=point_x[6]+point_y[8]/k_unload;
		temp2=point_y[8];

		point_x[7]=point_x[3]-Gamma*(point_x[3]-temp1);
		point_y[7]=Gamma*temp2;

		temp1=(point_y[2]-point_y[7])/(point_x[2]-point_x[7]);
		temp2=(1.0-dplus)*point_y[2];
		//point_x[8]=point_x[2]-(1.0-dplus)*point_y[2]*(point_x[2]-point_x[7])/(point_y[2]-point_y[7]);
		point_x[9]=point_x[8]*1.2;
		point_y[9]=point_y[8]+(point_x[9]-point_x[8])*EtaSoft*k;


		//缩放图形，使得曲线不跑出框外
		//pointx=point_x[2];
		//pointy=point_y[1];

		pointx=0;
		pointy=0;
		for (i=0;i<20;i++)
		{
			if (pointx<abs(point_x[i]))
			{
				pointx=abs(point_x[i]);
			}
			if (pointy<abs(point_y[i]))
			{
				pointy=abs(point_y[i]);
			}
		}

		for(i=0;i<20;i++ )
		{
			point_x[i]=point_x[i]/pointx;
			point_y[i]=point_y[i]/pointy;
		}

		m_memDC.SelectObject(&pen2);
		m_memDC.MoveTo(center_x,center_y);
		for (i=0;i<10;i++)
		{
			m_memDC.LineTo(center_x+point_x[i]*width,center_y+point_y[i]*height);
		}

		m_memDC.SelectObject(&pen3);
		m_memDC.MoveTo(center_x,center_y);
		for (i=10;i<13;i++)
		{
			m_memDC.LineTo(center_x+point_x[i]*width,center_y+point_y[i]*height);
		}
	}
	
	

	
}
//
//在静态区域画图
void CParaStruc::DrawOnStaticArea()
{
	CWnd* pWnd = GetDlgItem(IDC_STATIC2);//获得静态文本框的窗口对象
	CRect rect;
	pWnd->GetClientRect(&rect);    
	//
	CDC* pDC = pWnd->GetDC();//
	//
	DrawOnMem();
	// 一次性的将内存设备环境上绘制完毕的图形"贴"到屏幕上
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_memDC, 0, 0, SRCCOPY);
	pWnd->ReleaseDC(pDC);//
	//
}

void CParaStruc::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	double k,Eta,fy,EtaSoft,k_unload,C,Gamma,Alpha,Beta,a_k,Omega;
	CWnd* pWnd=AfxGetApp()->GetMainWnd();
	CPreProcessDlg* dlg;
	dlg= (CPreProcessDlg*)pWnd;
	dlg->m_struc.UpdateData();
	DrawOnStaticArea();

}



BOOL CParaStruc::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == VK_RETURN)//当按下键为回车键时
		{
			OnBnClickedButton4();//调用按钮函数
			return TRUE;
		}
	}


	return CDialog::PreTranslateMessage(pMsg);
}


void CParaStruc::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CParaStruc::OnCbnSelchangeCurFloor()
{
	// TODO: 在此添加控件通知处理程序代码
	int CurBuilding;
	CurFloor=m_CBCurFloor.GetCurSel();
	CWnd* pWnd=AfxGetApp()->GetMainWnd();
	CPreProcessDlg* dlg;
	dlg=(CPreProcessDlg*)pWnd;
	CurBuilding=dlg->m_basic.m_CBName.GetCurSel();
	
	dlg->SaveToData(CurBuilding,LastFloor);
	dlg->SaveToControl(CurBuilding,CurFloor);
	OnBnClickedButton4();
	LastFloor=CurFloor;

}


void CParaStruc::OnCbnSelchangeButtonnext()
{
	// TODO: 在此添加控件通知处理程序代码
}
