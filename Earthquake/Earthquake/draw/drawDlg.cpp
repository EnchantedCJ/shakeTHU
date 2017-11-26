// drawDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "draw.h"
#include "drawDlg.h"
#include "math.h"
#include <string>
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CdrawDlg �Ի���




CdrawDlg::CdrawDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CdrawDlg::IDD, pParent)
	, m_ifexpand(false)
	, Angle(0)
	, PGA(0)
	, newEQ(_T(""))
	, m_AnalyzeTime(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON3, viewbtn);
	DDX_Control(pDX, IDC_COMBO1, Choseear);
	DDX_Control(pDX, IDC_EDIT1, AngleControl);
	DDX_Text(pDX, IDC_EDIT1, Angle);
	DDV_MinMaxDouble(pDX, Angle, 0, 360);
	DDX_Control(pDX, IDC_CHECK1, InAngle);
	DDX_Control(pDX, IDC_BUTTON10, CXbtn);
	DDX_Control(pDX, IDC_BUTTON6, CYbtn);
	DDX_Control(pDX, IDC_BUTTON9, CDbtn);
	DDX_Control(pDX, IDC_BUTTON4, Drawspe1);
	DDX_Control(pDX, IDC_CHECK2, PGAedit);
	DDX_Control(pDX, IDC_EDIT2, InPGA);
	DDX_Text(pDX, IDC_EDIT2, PGA);
	DDV_MinMaxInt(pDX, PGA, 1, 10000);
	DDX_Control(pDX, IDC_COMBO3, CDLB);
	DDX_Control(pDX, IDC_COMBO4, DZFZ);
	DDX_Control(pDX, IDC_COMBO5, DZYX);
	DDX_Control(pDX, IDC_RADIO1, TypeEQ);
	DDX_Control(pDX, IDC_BUTTON5, HZGFFYP);
	DDX_Text(pDX, IDC_EDIT3, newEQ);
	DDX_Control(pDX, IDC_EDIT3, newEQname);
	DDX_Text(pDX, IDC_EDIT4, m_AnalyzeTime);
	DDV_MinMaxDouble(pDX, m_AnalyzeTime, 0, 1000);
	DDX_Control(pDX, IDC_EDIT4, m_AnalyzeTimeCtrl);
}

BEGIN_MESSAGE_MAP(CdrawDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CdrawDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CdrawDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CdrawDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CdrawDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CdrawDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON7, &CdrawDlg::SCGFFYP)
	ON_BN_CLICKED(IDC_CHECK1, &CdrawDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON9, &CdrawDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CdrawDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON6, &CdrawDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON11, &CdrawDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CdrawDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_CHECK2, &CdrawDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON5, &CdrawDlg::OnBnClickedButton5)
	ON_BN_CLICKED(ADDEQ, &CdrawDlg::OnBnClickedAddeq)
	ON_BN_CLICKED(IDC_INFO, &CdrawDlg::OnBnClickedInfo)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CdrawDlg::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_EDIT2, &CdrawDlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CdrawDlg ��Ϣ�������

BOOL CdrawDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    m_brush.CreateSolidBrush(RGB(255,0,0));//��ʼ����ˢ
    m_pDrawWnd = GetDlgItem(IDC_STATIC);//��þ�̬���ڶ���ָ��
	m_Drawspe =  GetDlgItem(IDC_SPE); //��û��Ʒ�Ӧ�״��ڵ�ָ��
    InitialDBB();// ��ʼ��˫������ص�����
	InitialDBBspe();// ��ʼ��˫������ص�����
	m_ifexpand = TRUE;
	GetWindowRect(winrect);
	SetWindowPos(&wndTop,0,0,winrect.Width(),winrect.Height()/4.25,SWP_NOMOVE|SWP_NOZORDER);
	Choseear.AddString(_T("2008�� �й��봨���� 8.0��"));
	Choseear.AddString(_T("2011�� �ձ����� 8.9��"));
	Choseear.AddString(_T("2011�� ���������е��� 6.3��"));
	Choseear.AddString(_T("1995�� �ձ�kobe���� 6.9��"));
	Choseear.AddString(_T("1999�� �й�̨�弯������ 7.6��"));
	Choseear.AddString(_T("1994�� ����Northridge���� 6.7��"));
	Choseear.AddString(_T("1999�� ������Kocaeli���� 7.5��"));
	Choseear.AddString(_T("1990�� ����Manji���� 7.4��"));
	Choseear.AddString(_T("1940�� ����EL-Centro���� 7.1��"));
	Choseear.AddString(_T("1999�� ������Duzce���� 7.1��"));
	Choseear.AddString(_T("1999�� ����HectorMine���� 7.1��"));
	Choseear.AddString(_T("1992�� ����Landers Coolwater���� 7.3��"));
	Choseear.AddString(_T("1989�� ����LomaPrieta���� 6.9��"));
	Choseear.AddString(_T("1971�� ����SanFernando���� 6.6��"));
	Choseear.SetWindowTextW(_T("2008�� �й��봨���� 8.0��"));
	newEQname.SetWindowText(_T("���ڴ������¼ӵĵ�������!"));
	adiIndex=0;
	color = RGB(255,180,0);
	CDLB.AddString(_T("I0"));
	CDLB.AddString(_T("I1"));
	CDLB.AddString(_T("II"));
	CDLB.AddString(_T("��"));
	CDLB.AddString(_T("��"));
	CDLB.SetWindowTextW(_T("I0"));
	DZFZ.AddString(_T("��һ��"));
	DZFZ.AddString(_T("�ڶ���"));
	DZFZ.AddString(_T("������"));
	DZFZ.SetWindowTextW(_T("��һ��"));
	DZYX.AddString(_T("6��"));
	DZYX.AddString(_T("7��"));
	DZYX.AddString(_T("7�Ȱ�"));
	DZYX.AddString(_T("8��"));
	DZYX.AddString(_T("8�Ȱ�"));
	DZYX.AddString(_T("9��"));
	DZYX.SetWindowTextW(_T("6��"));
	AngleControl.SetWindowTextW(_T("0"));
	InPGA.SetWindowTextW(_T("539"));
	AngleControl.EnableWindow(FALSE);
	InAngle.SetCheck(FALSE);
	CXbtn.EnableWindow(FALSE);
	CYbtn.EnableWindow(FALSE);
	CDbtn.EnableWindow(FALSE);
	Drawspe1.EnableWindow(FALSE);
	InPGA.EnableWindow(FALSE);
	HZGFFYP.EnableWindow(FALSE);
	TypeEQ.SetCheck(TRUE);
	EQfilenum=0;
	SPEfilenum=0;
	PGA=539;
	EQname=_T("EQWenchuan.rec");
	initialfile.Open(_T("max_min.txt"),CFile::modeCreate|CFile::modeReadWrite);
	initialfile.WriteString(_T("0.0\n0.0\n0.0\n0.0\n0"));
	initialfile.Close();
	initialfile.Open(_T("max_minspe.txt"),CFile::modeCreate|CFile::modeReadWrite);
	initialfile.WriteString(_T("0.0\n0.0\n0.0\n0.0\n0"));
	initialfile.Close();	
	initialfile.Open(_T("SJmax_min.txt"),CFile::modeCreate|CFile::modeReadWrite);
	initialfile.WriteString(_T("0.0\n0.0\n0.0\n0.0\n0"));
	initialfile.Close();
	initialfile.Open(_T("STOREEQ.txt"),CFile::modeCreate|CFile::modeReadWrite);
	initialfile.Close();
	initialfile.Open(_T("EQ01.txt"),CFile::modeCreate|CFile::modeReadWrite);
	initialfile.Close();
	initialfile.Open(_T("EQX.txt"),CFile::modeCreate|CFile::modeReadWrite);
	initialfile.Close();
	initialfile.Open(_T("EQY.txt"),CFile::modeCreate|CFile::modeReadWrite);
	initialfile.Close();
	initialfile.Open(_T("STORESPE.txt"),CFile::modeReadWrite|CFile::modeCreate);
	initialfile.Close();
	initialfile.Open(_T("newrec.txt"),CFile::modeReadWrite|CFile::modeCreate);
	initialfile.Close();
	initialfile.Open(_T("colorinfo.txt"),CFile::modeReadWrite|CFile::modeCreate);
	strcolor.Format(_T("%d"),GetRValue(color));
	initialfile.WriteString(strcolor);
	initialfile.WriteString(_T("\n"));
	strcolor.Format(_T("%d"),GetBValue(color));
	initialfile.WriteString(strcolor);
	initialfile.WriteString(_T("\n"));
	strcolor.Format(_T("%d"),GetBValue(color));
	initialfile.WriteString(strcolor);
	initialfile.WriteString(_T("\n"));
	initialfile.Close();	
	UpdateData(TRUE);

	int argc=0;
	LPWSTR *argv=::CommandLineToArgvW(::GetCommandLineW(),&argc);

	//���ݴ���������в��������ĵ�
	//argv[1]:�����ļ���·��
	workPath=argv[1];
	ZeroMemory(thisPath,MAX_PATH);
	GetCurrentDirectory(MAX_PATH,thisPath);
	m_AnalyzeTimeCtrl.SetWindowTextW(_T("40.00"));
	m_AnalyzeTime=40.00;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CdrawDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CdrawDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		PAINTSTRUCT ps;
		CRect rt;
		m_pDrawWnd->GetClientRect(&rt);    
		CDC* pDC=m_pDrawWnd->BeginPaint(&ps);
		//DrawOnMem();
		pDC->BitBlt(0, 0, rt.Width(), rt.Height(), &drawDC, 0, 0, SRCCOPY);    
		m_pDrawWnd->EndPaint(&ps);
		
		CRect rtspe;
		m_Drawspe->GetClientRect(&rtspe);    
		CDC* pDCspe=m_Drawspe->BeginPaint(&ps);
		//DrawOnMem();
		pDC->BitBlt(0, 0, rtspe.Width(), rtspe.Height(), &drawDCspe, 0, 0, SRCCOPY);    
		m_Drawspe->EndPaint(&ps);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CdrawDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CdrawDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
    HBRUSH hbr=CDialog::OnCtlColor(pDC,pWnd,nCtlColor);
    //
    if(pWnd->GetDlgCtrlID()==IDOK || pWnd->GetDlgCtrlID()==IDCANCEL)
    {
         pDC->SetTextColor(RGB(255,0,0));//�����ı���ɫ
         pDC->SetBkMode(TRANSPARENT);//���ֱ�����Ϊ͸��
         pDC->SetBkColor(RGB(255,0,0));//���ñ���ɫ
        return m_brush;
    }
    //
    return hbr;//m_brush;///hbr
}

// ��ʼ����˫������ص�Ҫ��
void CdrawDlg::InitialDBBspe()
{
	CRect rtspe;
	m_Drawspe->GetClientRect(&rtspe);
	CDC* sDCspe = m_Drawspe->GetDC();
	if(!drawDCspe.CreateCompatibleDC(sDCspe))//
	{              
		::PostQuitMessage(0);
	} 
	// ����λͼ,������drawDCspe����������ɫ
	m_Bmpspe.CreateCompatibleBitmap(sDCspe, rtspe.Width(), rtspe.Height());//drawDC
	drawDCspe.SelectObject(&m_Bmpspe);
	drawDCspe.FillSolidRect(&rtspe,RGB(255,255,255));
	//�ػ�m_Bmp
	::SelectObject(drawDCspe.GetSafeHdc(), m_Bmpspe);

	m_Drawspe->ReleaseDC(sDCspe);
}
void CdrawDlg::InitialDBB()
{
	CRect rt;

	//color = RGB(255,120, 0);      // ��ɫ��ʼ��

    m_pDrawWnd->GetClientRect(&rt);
    CDC* sDC = m_pDrawWnd->GetDC();

	// Ϊ��ĻDC�������ݵ��ڴ�DC
    if(!drawDC.CreateCompatibleDC(sDC))//
	{              
		::PostQuitMessage(0);
	} 
	
    m_Bmp.CreateCompatibleBitmap(sDC, rt.Width(), rt.Height());//drawDC

	// �൱��ѡ�񻭲�,m_pDrawWnd->
    drawDC.SelectObject(&m_Bmp);
	drawDC.FillSolidRect(&rt,RGB(255,255,255));
	//�ػ�m_Bmp
	::SelectObject(drawDC.GetSafeHdc(), m_Bmp);
	m_pDrawWnd->ReleaseDC(sDC);
	
}


//˫�����ڴ��ϻ�ͼ
void CdrawDlg::DrawOnMem(CString Earname)
{
    CRect rect;
    m_pDrawWnd->GetClientRect(&rect);
	int i,ii=1,e,jj=1,kk,num;
	int k=0,l=0,flag=0,m=0,dotnum;
	double time,accel,maxtime,mintime,maxaccel,minaccel,temptime,tempaccel,daccel;
	double h=double(rect.top)-double(rect.bottom);
	double w=double(rect.right)-double(rect.left);
	double left;
	CString str,TOstr,TOstr2,temp1,temp2;
	CStdioFile infile,outfile,infile2;
	infile2.Open(_T("max_min.txt"),CFile::modeRead);
	infile2.ReadString(str);
	infile2.ReadString(str);
	infile2.ReadString(str);
	maxaccel=_tcstod(str,NULL);
	str.Format(_T("%.0lf"),maxaccel);
	left=rect.left+str.GetLength()*5+10;
	infile2.Close();
	w=99*w/100-(str.GetLength()*5+10);
	h=199*h/200;
	
    //COLORREF crl = GetSysColor(COLOR_3DFACE);
    //drawDC.FillSolidRect(rect, crl); 
    //drawDC.FillSolidRect(&rect, 0x00FFFFFF);// ��ɫ���, ע��,����ǻ����ڴ��豸������
    CPen solidpen(PS_SOLID,1,RGB(0,0,0));
    drawDC.SelectObject(&solidpen);
	drawDC.MoveTo(left,rect.top);
	//drawDC.LineTo(left/*+w*399/400*/,rect.top/*-h/200*/);
	drawDC.LineTo(left+w,rect.top);
	//drawDC.MoveTo(rect.right,rect.top);
	drawDC.LineTo(left+w,rect.top-h);
	//drawDC.MoveTo(rect.right,rect.top-h);
	drawDC.LineTo(left,rect.top-h);
	//drawDC.MoveTo(left,rect.top-h);
	drawDC.LineTo(left,rect.top);
	drawDC.MoveTo(left,rect.top-h/2);
	drawDC.LineTo(left+w,rect.top-h/2);
	CPen dashpen(PS_DOT,1,RGB(0,0,0));
    drawDC.SelectObject(&dashpen);
	for(i=1;i<=3;i++)
	{
		drawDC.MoveTo(left,rect.top-double(h/8.0*i));
		drawDC.LineTo(left+w,rect.top-double(h/8.0*i));
		drawDC.MoveTo(left,rect.top-double(h/8.0*i)-double(h/2.0));
		drawDC.LineTo(left+w,rect.top-double(h/8.0*i)-double(h/2.0));
	}
	for(i=1;i<=9;i++)
	{
		drawDC.MoveTo(left+double(w/10.0*i),rect.top);
		drawDC.LineTo(left+double(w/10.0*i),rect.top-h);
	}

	infile2.Open(_T("max_min.txt"),CFile::modeRead);
	infile2.ReadString(str);
	maxtime=_tcstod(str,NULL);
	infile2.ReadString(str);
	mintime=_tcstod(str,NULL);
	infile2.ReadString(str);
	maxaccel=_tcstod(str,NULL);
	infile2.ReadString(str);
	minaccel=_tcstod(str,NULL);
	infile2.ReadString(str);
	dotnum=_tcstod(str,NULL);
	temptime=0;
	tempaccel=0;
	daccel=maxaccel-minaccel;
	int Rvalue,Gvalue,Bvalue;
	infile.Open(Earname,CFile::modeRead);

	for(num=0;num<EQfilenum;num++)
	{

	infile.ReadString(str);
	dotnum=_tcstod(str,NULL);
	infile.ReadString(str);
	Rvalue=_tcstod(str,NULL);
	infile.ReadString(str);
	Gvalue=_tcstod(str,NULL);
	infile.ReadString(str);
	Bvalue=_tcstod(str,NULL);
	color=RGB(Rvalue,Gvalue,Bvalue);
	
    CPen solidpen2(PS_SOLID,1,color);
    drawDC.SelectObject(&solidpen2);
	time=0;
	accel=0;
	tempaccel=0;
	temptime=0;
	for(kk=1;kk<dotnum;kk++)
	{
	infile.ReadString(str);
	k=0;
	jj=1;
	while(_T("") != (TOstr = str.Tokenize(_T(" "), k)))
	{
		if(jj==1)
		{
			flag=TOstr.Find(_T("E"),l);
			if(flag == -1)
			{
				time=_tcstod(TOstr,NULL);
			}
			else
			//��ȡ��ѧ����������
			{
				m=0;
				ii=1;
				while(_T("") != (TOstr2 = TOstr.Tokenize(_T("E"),m)))
				{
					//outfile.WriteString(TOstr2);
					if(ii==1)
					{
						time=_tcstod(TOstr2,NULL);
					}
					else
					{
						e=_tcstod(TOstr2,NULL);
					}
					ii++;
				}
				time=time*pow(10.0,e);
			}
		}
		else
		{
			flag=TOstr.Find(_T("E"),l);
			if(flag == -1)
			{
				accel=_tcstod(TOstr,NULL);
			}
			else
			//��ȡ��ѧ����������
			{
				m=0;
				ii=1;
				while(_T("") != (TOstr2 = TOstr.Tokenize(_T("E"),m)))
				{
					//outfile.WriteString(TOstr2);
					if(ii==1)
					{
						accel=_tcstod(TOstr2,NULL);
					}
					else
					{
						e=_tcstod(TOstr2,NULL);
					}
					ii++;
				}
				accel=accel*pow(10.0,e);
			}
		}
		jj++;
	}
	drawDC.MoveTo(int(time/maxtime*w+left),int(rect.top-h/2-accel/(daccel)*h));
	drawDC.LineTo(int(temptime/maxtime*w+left),int(rect.top-h/2-tempaccel/(daccel)*h));
	temptime=time;
	tempaccel=accel;
	
	}
	}
	infile.Close();
	CFont *MyFont=new CFont;
	MyFont->CreateFontW(15,0,0,0,FW_MEDIUM,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("TIMES NEW ROMAN"));
	drawDC.SelectObject(MyFont);
	drawDC.SetBkMode(TRANSPARENT);
	drawDC.TextOutW(rect.right-50,rect.top-h/2.0+h/12.0,_T("ʱ�� (s)"));
	//drawDC.TextOutW(left+5,rect.top,_T("��ֵ���ٶ� (gal)"));
	drawDC.TextOutW(left-10,rect.top-h/2.0,_T("0"));
	str.Format(_T("%.1lf"),maxtime);
	drawDC.TextOutW(rect.right-str.GetLength()*8,rect.top-h/2.0,str,str.GetLength());
	str.Format(_T("%.1lf"),maxtime/2);
	drawDC.TextOutW(rect.right-20-w/2.0,rect.top-h/2.0,str,str.GetLength());
	str.Format(_T("%.0lf"),maxaccel);
	drawDC.TextOutW(rect.left+5,rect.top,str,str.GetLength());	
	str.Format(_T("%.0lf"),minaccel);
	drawDC.TextOutW(rect.left,rect.bottom-15,str,str.GetLength());	
	delete MyFont;
}
//�ھ�̬����ͼ
void CdrawDlg::DrawOnStaticArea(CString Earname)
{
   	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CRect rt;
    m_pDrawWnd->GetClientRect(&rt);
	// �൱��ѡ�񻭲�,m_pDrawWnd->
	drawDC.FillSolidRect(&rt,RGB(255,255,255));
	
	CWnd* pWnd = GetDlgItem(IDC_STATIC);//��þ�̬�ı���Ĵ��ڶ���
    CRect rect;
    pWnd->GetClientRect(&rect);    
    //
    CDC* pDC = pWnd->GetDC();//
    //
    DrawOnMem(Earname);
    // һ���ԵĽ��ڴ��豸�����ϻ�����ϵ�ͼ��"��"����Ļ��
    pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &drawDC, 0, 0, SRCCOPY);
    pWnd->ReleaseDC(pDC);//


    //
}

void CdrawDlg::DrawOnStaticAreaspe(CString maxminname,CString filename)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CRect rt;
    m_Drawspe->GetClientRect(&rt);
	// �൱��ѡ�񻭲�,m_pDrawWnd->
	drawDCspe.FillSolidRect(&rt,RGB(255,255,255));

    CWnd* pWnd = GetDlgItem(IDC_SPE);//��þ�̬�ı���Ĵ��ڶ���
    CRect rect;
    pWnd->GetClientRect(&rect);    
    //
    CDC* pDC = pWnd->GetDC();//
    //
    Drawspe(maxminname,filename);
    // һ���ԵĽ��ڴ��豸�����ϻ�����ϵ�ͼ��"��"����Ļ��
    pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &drawDCspe, 0, 0, SRCCOPY);
    pWnd->ReleaseDC(pDC);//
   //
}

//˫�����ڴ��ϻ�ͼ
void CdrawDlg::Drawspe(CString maxminname,CString filename)
{
  CRect rect;
  m_Drawspe->GetClientRect(&rect);
	int i,ii=1,e,jj=1,kk;
	int k=0,l=0,flag=0,m=0,dotnum,num;
	double time,accel,maxtime,mintime,maxaccel,minaccel,temptime,tempaccel,daccel;
	double h=double(rect.top)-double(rect.bottom);
	double w=double(rect.right)-double(rect.left);
	double left,bottom;
	CString str,TOstr,TOstr2,temp1,temp2;
	CStdioFile infile,outfile,infile2;
	infile2.Open(maxminname,CFile::modeRead);
	infile2.ReadString(str);
	infile2.ReadString(str);
	infile2.ReadString(str);
	maxaccel=_tcstod(str,NULL);
	str.Format(_T("%.2lf"),maxaccel);

	left=rect.left+str.GetLength()*5+2;
	infile2.Close();
	bottom=rect.bottom-20;
	w=99*w/100-(str.GetLength()*5+2);
	h=199*h/200+20;
	
    //COLORREF crl = GetSysColor(COLOR_3DFACE);
    //drawDCspe.FillSolidRect(rect, crl); 
    //drawDCspe.FillSolidRect(&rect, 0x00FFFFFF);// ��ɫ���, ע��,����ǻ����ڴ��豸������
    CPen solidpen(PS_SOLID,1,RGB(0,0,0));
    drawDCspe.SelectObject(&solidpen);
	drawDCspe.MoveTo(left,rect.top);
	//drawDCspe.LineTo(left/*+w*399/400*/,rect.top/*-h/200*/);
	drawDCspe.LineTo(left+w,rect.top);
	//drawDCspe.MoveTo(rect.right,rect.top);
	drawDCspe.LineTo(left+w,bottom);
	//drawDCspe.MoveTo(rect.right,rect.top-h);
	drawDCspe.LineTo(left,bottom);
	//drawDCspe.MoveTo(left,rect.top-h);
	drawDCspe.LineTo(left,rect.top);
	CPen dashpen(PS_DOT,1,RGB(0,0,0));
    drawDCspe.SelectObject(&dashpen);
	drawDCspe.MoveTo(left,rect.top-h/2);
	drawDCspe.LineTo(left+w,rect.top-h/2);
	for(i=1;i<=3;i++)
	{
		drawDCspe.MoveTo(left,rect.top-double(h/8.0*i));
		drawDCspe.LineTo(left+w,rect.top-double(h/8.0*i));
		drawDCspe.MoveTo(left,rect.top-double(h/8.0*i)-double(h/2.0));
		drawDCspe.LineTo(left+w,rect.top-double(h/8.0*i)-double(h/2.0));
	}
	for(i=1;i<=9;i++)
	{
		drawDCspe.MoveTo(left+double(w/10.0*i),rect.top);
		drawDCspe.LineTo(left+double(w/10.0*i),rect.top-h);
	}


	//outfile.Open(_T("test.txt"),CFile::modeReadWrite|CFile::modeCreate);
	infile2.Open(maxminname,CFile::modeRead);
	infile2.ReadString(str);
	maxtime=_tcstod(str,NULL);
	infile2.ReadString(str);
	mintime=_tcstod(str,NULL);
	infile2.ReadString(str);
	maxaccel=_tcstod(str,NULL);
	infile2.ReadString(str);
	minaccel=_tcstod(str,NULL);

	temptime=0;
	tempaccel=0;
	daccel=maxaccel-minaccel;
	int Rvalue,Gvalue,Bvalue;
	infile.Open(filename,CFile::modeRead);
	for(num=0;num<SPEfilenum;num++)
	{

		infile.ReadString(str);
		dotnum=_tcstod(str,NULL);
		infile.ReadString(str);
		Rvalue=_tcstod(str,NULL);
		infile.ReadString(str);
		Gvalue=_tcstod(str,NULL);
		infile.ReadString(str);
		Bvalue=_tcstod(str,NULL);
		color=RGB(Rvalue,Gvalue,Bvalue);
		time=0;
		accel=0;
		tempaccel=0;
		temptime=0;
    CPen solidpen2(PS_SOLID,1,color);
    drawDCspe.SelectObject(&solidpen2);
	for(kk=1;kk<dotnum;kk++)
	{
	infile.ReadString(str);
	k=0;
	jj=1;
	while(_T("") != (TOstr = str.Tokenize(_T(" "), k)))
	{
		if(jj==1)
		{
			//outfile.WriteString(TOstr);
			//outfile.WriteString(_T("\n"));
			flag=TOstr.Find(_T("E"),l);
			if(flag == -1)
			{
				time=_tcstod(TOstr,NULL);
			}
			else
			//��ȡ��ѧ����������
			{
				m=0;
				ii=1;
				while(_T("") != (TOstr2 = TOstr.Tokenize(_T("E"),m)))
				{
					//outfile.WriteString(TOstr2);
					if(ii==1)
					{
						time=_tcstod(TOstr2,NULL);
					}
					else
					{
						e=_tcstod(TOstr2,NULL);
					}
					ii++;
				}
				time=time*pow(10.0,e);
			}
		}
		else
		{
			//outfile.WriteString(TOstr);
			//outfile.WriteString(_T("\n"));
			flag=TOstr.Find(_T("E"),l);
			if(flag == -1)
			{
				accel=_tcstod(TOstr,NULL);
			}
			else
			//��ȡ��ѧ����������
			{
				m=0;
				ii=1;
				while(_T("") != (TOstr2 = TOstr.Tokenize(_T("E"),m)))
				{
					//outfile.WriteString(TOstr2);
					if(ii==1)
					{
						accel=_tcstod(TOstr2,NULL);
					}
					else
					{
						e=_tcstod(TOstr2,NULL);
					}
					ii++;
				}
				accel=accel*pow(10.0,e);
			}
		}
		jj++;
	}
	drawDCspe.MoveTo(int(time/maxtime*w+left),int(bottom+accel/(daccel)*h));
	drawDCspe.LineTo(int(temptime/maxtime*w+left),int(bottom+tempaccel/(daccel)*h));
	temptime=time;
	tempaccel=accel;
	}
	}
	infile.Close();
	CFont *MyFont=new CFont;
	MyFont->CreateFontW(15,0,0,0,FW_MEDIUM,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("TIMES NEW ROMAN"));
	drawDCspe.SelectObject(MyFont);
	drawDCspe.SetBkMode(TRANSPARENT);
	drawDCspe.TextOutW(rect.right-50,bottom+h/12.0,_T("���� (s)"));
	str.Format(_T("%.1lf"),maxtime);
	drawDCspe.TextOutW(rect.right-20,bottom,str,str.GetLength());
	str.Format(_T("%.1lf"),maxtime/2);
	drawDCspe.TextOutW(rect.right-20-w/2.0,bottom,str,str.GetLength());
	str.Format(_T("%.2lf"),maxaccel);
	drawDCspe.TextOutW(rect.left,rect.top,str,str.GetLength());	
	str.Format(_T("%.0lf"),minaccel);
	drawDCspe.TextOutW(rect.left+10,bottom,str,str.GetLength());	
	delete MyFont;
}

void CdrawDlg::DrawChart()
{
	CRect rect;
    m_pDrawWnd->GetClientRect(&rect);
	int i;
	double h=double(rect.top)-double(rect.bottom);
	double w=double(rect.right)-double(rect.left);
	double left;
	CString str;
	CStdioFile infile2;
	double maxaccel;
	infile2.Open(_T("max_min.txt"),CFile::modeRead);
	infile2.ReadString(str);
	infile2.ReadString(str);
	infile2.ReadString(str);
	maxaccel=_tcstod(str,NULL);
	str.Format(_T("%.0lf"),maxaccel);
	left=rect.left+(str.GetLength()*5+10);
	infile2.Close();
	w=99*w/100-(str.GetLength()*5+10);
	h=199*h/200;
	
    CPen solidpen(PS_SOLID,1,RGB(0,0,0));
    drawDC.SelectObject(&solidpen);
	drawDC.MoveTo(left,rect.top);
	//drawDC.LineTo(left/*+w*399/400*/,rect.top/*-h/200*/);
	drawDC.LineTo(left+w,rect.top);
	//drawDC.MoveTo(rect.right,rect.top);
	drawDC.LineTo(left+w,rect.top-h);
	//drawDC.MoveTo(rect.right,rect.top-h);
	drawDC.LineTo(left,rect.top-h);
	//drawDC.MoveTo(left,rect.top-h);
	drawDC.LineTo(left,rect.top);
	drawDC.MoveTo(left,rect.top-h/2);
	drawDC.LineTo(left+w,rect.top-h/2);
	CPen dashpen(PS_DOT,1,RGB(0,0,0));
    drawDC.SelectObject(&dashpen);
	for(i=1;i<=3;i++)
	{
		drawDC.MoveTo(left,rect.top-double(h/8.0*i));
		drawDC.LineTo(left+w,rect.top-double(h/8.0*i));
		drawDC.MoveTo(left,rect.top-double(h/8.0*i)-double(h/2.0));
		drawDC.LineTo(left+w,rect.top-double(h/8.0*i)-double(h/2.0));
	}
	for(i=1;i<=9;i++)
	{
		drawDC.MoveTo(left+double(w/10.0*i),rect.top);
		drawDC.LineTo(left+double(w/10.0*i),rect.top-h);
	}
}
void CdrawDlg::DrawChartspe()
{
	CRect rect;
	m_Drawspe->GetClientRect(&rect);
	int i;
	double h=double(rect.top)-double(rect.bottom),maxaccel;
	double w=double(rect.right)-double(rect.left);
	double left,bottom;
	bottom=rect.bottom-20;
	CString str;
	CStdioFile infile2;
	infile2.Open(_T("max_minspe.txt"),CFile::modeRead);
	infile2.ReadString(str);
	infile2.ReadString(str);
	infile2.ReadString(str);
	maxaccel=_tcstod(str,NULL);
	str.Format(_T("%.2lf"),maxaccel);
	left=rect.left+str.GetLength()*5+2;
	infile2.Close();
	w=99*w/100-(str.GetLength()*5+2);
	h=199*h/200+20;

    CPen solidpen(PS_SOLID,1,RGB(0,0,0));
    drawDCspe.SelectObject(&solidpen);
	drawDCspe.MoveTo(left,rect.top);
	//drawDCspe.LineTo(left/*+w*399/400*/,rect.top/*-h/200*/);
	drawDCspe.LineTo(left+w,rect.top);
	//drawDCspe.MoveTo(rect.right,rect.top);
	drawDCspe.LineTo(left+w,bottom);
	//drawDCspe.MoveTo(rect.right,rect.top-h);
	drawDCspe.LineTo(left,bottom);
	//drawDCspe.MoveTo(left,rect.top-h);
	drawDCspe.LineTo(left,rect.top);
	CPen dashpen(PS_DOT,1,RGB(0,0,0));
    drawDCspe.SelectObject(&dashpen);
	drawDCspe.MoveTo(left,rect.top-h/2);
	drawDCspe.LineTo(left+w,rect.top-h/2);
	for(i=1;i<=3;i++)
	{
		drawDCspe.MoveTo(left,rect.top-double(h/8.0*i));
		drawDCspe.LineTo(left+w,rect.top-double(h/8.0*i));
		drawDCspe.MoveTo(left,rect.top-double(h/8.0*i)-double(h/2.0));
		drawDCspe.LineTo(left+w,rect.top-double(h/8.0*i)-double(h/2.0));
	}
	for(i=1;i<=9;i++)
	{
		drawDCspe.MoveTo(left+double(w/10.0*i),rect.top);
		drawDCspe.LineTo(left+double(w/10.0*i),rect.top-h);
	}
}

void CdrawDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	nIndex=Choseear.GetCurSel();
	if(nIndex==-1)
	{
		nIndex=0;
	}

	switch(nIndex)
	{
	case 0: 
		EQname=_T("EQWenchuan.rec");
		break;
	case 1:
		EQname=_T("EQTohoku.rec");

		break;
	case 2: 
		EQname=_T("EQNZ.rec");
		break;
	case 3: 
		EQname=_T("EQKobe.rec");
		break;
	case 4: 
		EQname=_T("EQChichi.rec");
		break;
	case 5: 
		EQname=_T("EQNorth.rec");
		break;
	case 6: 
		EQname=_T("EQKocaeli.rec");
		break;
	case 7: 
		EQname=_T("EQManjil.rec");
		break;
	case 8:
		EQname=_T("EQElCentro.rec");			
		break;
	case 9:
		EQname=_T("EQDuzce.rec");			
		break;
	case 10:
		EQname=_T("EQHectorMine.rec");			
		break;
	case 11:
		EQname=_T("EQLanders.rec");			
		break;
	case 12:
		EQname=_T("EQLomaPrieta.rec");			
		break;
	case 13:
		EQname=_T("EQSanFernando.rec");			
		break;
	default:
		break;
	}
	
	ofstream fout;
//	CString path=workPath+L"\\Tsinghua_GroundMotion.txt";

	CStringA stra(EQname.GetBuffer(0));
	EQname.ReleaseBuffer();
	string tempStr=stra.GetBuffer(0);
	stra.ReleaseBuffer();

	CString tempStr2=thisPath;

	CStringA strb(tempStr2.GetBuffer(0));
	tempStr2.ReleaseBuffer();
	string tempStr3=strb.GetBuffer(0);
	strb.ReleaseBuffer();

	CString tempStr4;
	((CComboBox*)GetDlgItem(IDC_COMBO1))->GetWindowTextW(tempStr4);
	CStringA strc(tempStr4.GetBuffer(0));
	tempStr4.ReleaseBuffer();
	string tempStr5=strc.GetBuffer(0);
	strc.ReleaseBuffer();
	
	CString recordPath=thisPath+(CString)_T("\\")+EQname;
	double dT;
	ifstream getDT(recordPath);
	string buffer;
	getline(getDT,buffer);
	getline(getDT,buffer);
	getDT>>dT>>dT;
	
	
	int state=MessageBox(L"ȷ�����²���������ȷ�����𲨡����𶯽Ƕȡ���ֵ���ٶ�(PGA)������ʱ�䣿",L"ȷ���˳�",MB_OKCANCEL|MB_ICONEXCLAMATION);
	switch (state)
	{
	case 1:			//ȷ���������˳�

//		fout.open(workPath,ios_base::app);
		fout.open(workPath);
		//ÿ�����α���:
		/*		��Ŀ����
				�ܽ��������ܷ���ʱ�䳤�ȣ��ܷ�����������������
				��������
				����·��
				���𲨽Ƕȣ�PGA
				��׼��x���꣬��׼��y����
		*/

		fout<<m_AnalyzeTime<<'\t'<<m_AnalyzeTime/dT<<'\t'<<dT<<'\n'
			<<tempStr5.c_str()<<'\n'
			<<tempStr3.c_str()<<"\\"<<tempStr.c_str()<<'\n'
			<<Angle<<'\t'<<PGA<<endl;
		
		fout.close();
		CDialog::OnOK();
		break;
	default:
		//ȡ��
		break;
	}
}

void CdrawDlg::OnBnClickedButton1()
{
	CStdioFile outfile;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	nIndex=Choseear.GetCurSel();
	if(nIndex==-1)
	{
		nIndex=0;
	}
	
	switch(nIndex)
	{
	case 0: 
		EQname=_T("EQWenchuan.rec");
		break;
	case 1:
		EQname=_T("EQTohoku.rec");

		break;
	case 2: 
		EQname=_T("EQNZ.rec");
		break;
	case 3: 
		EQname=_T("EQKobe.rec");
		break;
	case 4: 
		EQname=_T("EQChichi.rec");
		break;
	case 5: 
		EQname=_T("EQNorth.rec");
		break;
	case 6: 
		EQname=_T("EQKocaeli.rec");
		break;
	case 7: 
		EQname=_T("EQManjil.rec");
		break;
	case 8:
		EQname=_T("EQElCentro.rec");			
		break;
	case 9:
		EQname=_T("EQDuzce.rec");			
		break;
	case 10:
		EQname=_T("EQHectorMine.rec");			
		break;
	case 11:
		EQname=_T("EQLanders.rec");			
		break;
	case 12:
		EQname=_T("EQLomaPrieta.rec");			
		break;
	case 13:
		EQname=_T("EQSanFernando.rec");			
		break;
	default:
		break;
	}
	int ii,jj;
	CStdioFile infile3;
	CString str2;
	for(ii=1;ii<=adiIndex;ii++)
	{
		if (nIndex==8+ii)
		{
			infile3.Open(_T("newrec.txt"),CFile::modeRead);
			for(jj=0;jj<ii;jj++)
			{
				infile3.ReadString(str2);
			}
			EQname=str2;
		}
	}
	outfile.Open(_T("test.txt"),CFile::modeReadWrite|CFile::modeCreate);
	outfile.WriteString(EQname);
	outfile.Close();
	SHELLEXECUTEINFO sei; 
	memset(&sei, 0, sizeof(SHELLEXECUTEINFO)); 
	sei.cbSize = sizeof(SHELLEXECUTEINFO); 
	sei.fMask = SEE_MASK_NOCLOSEPROCESS; 
	sei.lpVerb = _T("open"); 
	sei.lpFile = _T("pre.exe"); 
	sei.nShow = SW_HIDE; 
	ShellExecuteEx(&sei);
	WaitForSingleObject(sei.hProcess, INFINITE); 
	CloseHandle(sei.hProcess);
	StoreData(_T("EQ01.txt"),_T("STOREEQ.txt"),_T("max_min.txt"),color,0);
	int RR=GetRValue(color);
	int GG=GetGValue(color);
	int BB=GetBValue(color);
	DrawOnStaticArea(_T("STOREEQ.txt"));
}

void CdrawDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CColorDialog colorDlg(color);         // ������ɫ�Ի��򣬴����ʼ��ɫֵ
	if (IDOK == colorDlg.DoModal())       // ��ʾ��ɫ�Ի��򣬲��ж��Ƿ����ˡ�ȷ���� 
	{   
        color = colorDlg.GetColor();      // ��ȡ��ɫ�Ի�����ѡ�����ɫֵ   
	}
	initialfile.Open(_T("colorinfo.txt"),CFile::modeReadWrite|CFile::modeCreate);
	strcolor.Format(_T("%d"),GetRValue(color));
	initialfile.WriteString(strcolor);
	initialfile.WriteString(_T("\n"));
	strcolor.Format(_T("%d"),GetBValue(color));
	initialfile.WriteString(strcolor);
	initialfile.WriteString(_T("\n"));
	strcolor.Format(_T("%d"),GetBValue(color));
	initialfile.WriteString(strcolor);
	initialfile.WriteString(_T("\n"));
	initialfile.Close();	

}

int CdrawDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect rect=winrect;
	SetWindowPos(&wndTop,0,0,rect.Width(),rect.Height(),SWP_NOMOVE|SWP_NOZORDER);

	return 0;
}
void CdrawDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CRect rect=winrect;
	
	if(m_ifexpand)
	{
		SetWindowPos(&wndTop,0,0,rect.Width(),rect.Height(),SWP_NOMOVE|SWP_NOZORDER);
		viewbtn.SetWindowTextW(_T("�鿴���ߡ�"));
		m_ifexpand = FALSE;
	}
	else
	{
		SetWindowPos(&wndTop,0,0,rect.Width(),rect.Height()/4.25,SWP_NOMOVE|SWP_NOZORDER);
		viewbtn.SetWindowTextW(_T("�鿴���ߡ�"));
		m_ifexpand = TRUE;
	}
}

void CdrawDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString maxminname=_T("max_minspe.txt"),filename=_T("xMAG.txt");
	CRect rtspe;
	m_Drawspe->GetClientRect(&rtspe);
	drawDCspe.FillSolidRect(&rtspe,RGB(255,255,255));
	CWnd* pWnd = GetDlgItem(IDC_SPE);//��þ�̬�ı���Ĵ��ڶ���
    CRect rectspe;
    pWnd->GetClientRect(&rectspe);    
    CDC* pDC = pWnd->GetDC();//
	DrawChartspe();
    // һ���ԵĽ��ڴ��豸�����ϻ�����ϵ�ͼ��"��"����Ļ��
    pDC->BitBlt(0, 0, rectspe.Width(), rectspe.Height(), &drawDCspe, 0, 0, SRCCOPY);
    pWnd->ReleaseDC(pDC);//

	SHELLEXECUTEINFO sei; 
	memset(&sei, 0, sizeof(SHELLEXECUTEINFO)); 
	sei.cbSize = sizeof(SHELLEXECUTEINFO); 
	sei.fMask = SEE_MASK_NOCLOSEPROCESS; 
	sei.lpVerb = _T("open"); 
	sei.lpFile = _T("pre3.exe"); 
	sei.nShow = SW_HIDE; 
	ShellExecuteEx(&sei);
	WaitForSingleObject(sei.hProcess, INFINITE); 
	CloseHandle(sei.hProcess);
	StoreData(_T("xMAG.txt"),_T("STORESPE.txt"),_T("max_minspe.txt"),color,1);
	DrawOnStaticAreaspe(maxminname,_T("STORESPE.txt"));
}

void CdrawDlg::SCGFFYP()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int IndexCDLB=0,IndexDZFZ=0,IndexDZYX=0;
	CString Tg=_T("0.20"),amax=_T("0.04");
	IndexCDLB=CDLB.GetCurSel();
	IndexDZFZ=DZFZ.GetCurSel();
	IndexDZYX=DZYX.GetCurSel();
	if (IndexDZYX==-1)
	{
		IndexDZYX=0;
	}
	if (IndexCDLB==-1)
	{
		IndexCDLB=0;
	}
	if (IndexDZFZ==-1)
	{
		IndexDZFZ=0;
	}
	switch(IndexCDLB)
	{
	case 0:
		switch(IndexDZFZ)
		{
		case 0:
			Tg="0.20";
			break;
		case 1:
			Tg="0.25";
			break;
		default:
			Tg="0.30";
			break;
		}
	break;
	case 1:
		switch(IndexDZFZ)
		{
		case 0:
			Tg="0.25";
			break;
		case 1:
			Tg="0.30";
			break;
		default:
			Tg="0.35";
			break;
		}
	break;
	case 2:
		switch(IndexDZFZ)
		{
		case 0:
			Tg="0.35";
			break;
		case 1:
			Tg="0.40";
			break;
		default:
			Tg="0.45";
			break;
		}
	break;
	case 3:
		switch(IndexDZFZ)
		{
		case 0:
			Tg="0.45";
			break;
		case 1:
			Tg="0.55";
			break;
		default:
			Tg="0.65";
			break;
		}
	break;
	default:
		switch(IndexDZFZ)
		{
		case 0:
			Tg="0.65";
			break;
		case 1:
			Tg="0.75";
			break;
		default:
			Tg="0.90";
			break;
		}
	break;
	}
	if(TypeEQ.GetCheck())
	{
		switch(IndexDZYX)
		{
		case 0:
			amax="0.04";
			break;
		case 1:
			amax="0.08";
			break;
		case 2:
			amax="0.12";
			break;
		case 3:
			amax="0.16";
			break;
		case 4:
			amax="0.24";
			break;
		default:
			amax="0.32";
			break;
		}
	}
	else
	{
		switch(IndexDZYX)
		{
		case 0:
			amax="0.28";
			break;
		case 1:
			amax="0.50";
			break;
		case 2:
			amax="0.72";
			break;
		case 3:
			amax="0.90";
			break;
		case 4:
			amax="1.20";
			break;
		default:
			amax="1.24";
			break;
		}
	}
	CStdioFile outfile;
	outfile.Open(_T("SJFYP.txt"),CFile::modeReadWrite|CFile::modeCreate);
	outfile.WriteString(Tg);
	outfile.WriteString(_T("\n"));
	outfile.WriteString(amax);
	outfile.Close();
	SHELLEXECUTEINFO sei; 
	memset(&sei, 0, sizeof(SHELLEXECUTEINFO)); 
	sei.cbSize = sizeof(SHELLEXECUTEINFO); 
	sei.fMask = SEE_MASK_NOCLOSEPROCESS; 
	sei.lpVerb = _T("open"); 
	sei.lpFile = _T("SJFYP.exe"); 
	sei.nShow = SW_HIDE; 
	ShellExecuteEx(&sei);
	WaitForSingleObject(sei.hProcess, INFINITE); 
	CloseHandle(sei.hProcess);
	HZGFFYP.EnableWindow(TRUE);
}

void CdrawDlg::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(InAngle.GetCheck())
	{
		AngleControl.EnableWindow(TRUE);
		CXbtn.EnableWindow(TRUE);
		CYbtn.EnableWindow(TRUE);
		CDbtn.EnableWindow(TRUE);
	}
	else
	{
		AngleControl.EnableWindow(FALSE);
		CXbtn.EnableWindow(FALSE);
		CYbtn.EnableWindow(FALSE);
		CDbtn.EnableWindow(FALSE);
	}
}

void CdrawDlg::OnBnClickedButton9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(InAngle.GetCheck())
	{
	CStdioFile outfile;
	CString str;
	str.Format(_T("%lf"),Angle);
	outfile.Open(_T("angle.txt"),CFile::modeReadWrite|CFile::modeCreate);
	outfile.WriteString(str);
	outfile.Close();
	SHELLEXECUTEINFO sei; 
	memset(&sei, 0, sizeof(SHELLEXECUTEINFO)); 
	sei.cbSize = sizeof(SHELLEXECUTEINFO); 
	sei.fMask = SEE_MASK_NOCLOSEPROCESS; 
	sei.lpVerb = _T("open"); 
	sei.lpFile = _T("pre2.exe"); 
	sei.nShow = SW_HIDE; 
	ShellExecuteEx(&sei);
	WaitForSingleObject(sei.hProcess, INFINITE); 
	CloseHandle(sei.hProcess);
	}
	else
	{
		MessageBox(_T("��ѡ���������Ƕȣ�"),_T("WARNING"), MB_OKCANCEL ); 
	}

}

void CdrawDlg::OnBnClickedButton10()
{
	CStdioFile outfile;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	nIndex=Choseear.GetCurSel();
	if(nIndex==-1)
	{
		nIndex=0;
	}
	CString EQname;
	switch(nIndex)
	{
	case 0: 
		EQname=_T("EQWenchuan.rec");
		break;
	case 1:
		EQname=_T("EQTohoku.rec");

		break;
	case 2: 
		EQname=_T("EQNZ.rec");
		break;
	case 3: 
		EQname=_T("EQKobe.rec");
		break;
	case 4: 
		EQname=_T("EQChichi.rec");
		break;
	case 5: 
		EQname=_T("EQNorth.rec");
		break;
	case 6: 
		EQname=_T("EQKocaeli.rec");
		break;
	case 7: 
		EQname=_T("EQManjil.rec");
		break;
	case 8:
		EQname=_T("EQElCentro.rec");			
		break;
	case 9:
		EQname=_T("EQDuzce.rec");			
		break;
	case 10:
		EQname=_T("EQHectorMine.rec");			
		break;
	case 11:
		EQname=_T("EQLanders.rec");			
		break;
	case 12:
		EQname=_T("EQLomaPrieta.rec");			
		break;
	case 13:
		EQname=_T("EQSanFernando.rec");			
		break;
	default:
		break;
	}
	int ii,jj;
	CStdioFile infile3;
	CString str2;
	for(ii=1;ii<=adiIndex;ii++)
	{
		if (nIndex==8+ii)
		{
			infile3.Open(_T("newrec.txt"),CFile::modeRead);
			for(jj=0;jj<ii;jj++)
			{
				infile3.ReadString(str2);
			}
			EQname=str2;
		}
	}
	outfile.Open(_T("test.txt"),CFile::modeReadWrite|CFile::modeCreate);
	outfile.WriteString(EQname);
	outfile.Close();
	SHELLEXECUTEINFO sei; 
	memset(&sei, 0, sizeof(SHELLEXECUTEINFO)); 
	sei.cbSize = sizeof(SHELLEXECUTEINFO); 
	sei.fMask = SEE_MASK_NOCLOSEPROCESS; 
	sei.lpVerb = _T("open"); 
	sei.lpFile = _T("pre.exe"); 
	sei.nShow = SW_HIDE; 
	ShellExecuteEx(&sei);
	WaitForSingleObject(sei.hProcess, INFINITE); 
	CloseHandle(sei.hProcess);

	StoreData(_T("EQX.txt"),_T("STOREEQ.txt"),_T("max_min.txt"),color,0);
	DrawOnStaticArea(_T("STOREEQ.txt"));
}

void CdrawDlg::OnBnClickedButton6()
{
	CStdioFile outfile;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	nIndex=Choseear.GetCurSel();
	if(nIndex==-1)
	{
		nIndex=0;
	}
	CString EQname;
	switch(nIndex)
	{
	case 0: 
		EQname=_T("EQWenchuan.rec");
		break;
	case 1:
		EQname=_T("EQTohoku.rec");

		break;
	case 2: 
		EQname=_T("EQNZ.rec");
		break;
	case 3: 
		EQname=_T("EQKobe.rec");
		break;
	case 4: 
		EQname=_T("EQChichi.rec");
		break;
	case 5: 
		EQname=_T("EQNorth.rec");
		break;
	case 6: 
		EQname=_T("EQKocaeli.rec");
		break;
	case 7: 
		EQname=_T("EQManjil.rec");
		break;
	case 8:
		EQname=_T("EQElCentro.rec");			
		break;
	case 9:
		EQname=_T("EQDuzce.rec");			
		break;
	case 10:
		EQname=_T("EQHectorMine.rec");			
		break;
	case 11:
		EQname=_T("EQLanders.rec");			
		break;
	case 12:
		EQname=_T("EQLomaPrieta.rec");			
		break;
	case 13:
		EQname=_T("EQSanFernando.rec");			
		break;
	default:
		break;
	}
	int ii,jj;
	CStdioFile infile3;
	CString str2;
	for(ii=1;ii<=adiIndex;ii++)
	{
		if (nIndex==8+ii)
		{
			infile3.Open(_T("newrec.txt"),CFile::modeRead);
			for(jj=0;jj<ii;jj++)
			{
				infile3.ReadString(str2);
			}
			EQname=str2;
		}
	}
	outfile.Open(_T("test.txt"),CFile::modeReadWrite|CFile::modeCreate);
	outfile.WriteString(EQname);
	outfile.Close();
	SHELLEXECUTEINFO sei; 
	memset(&sei, 0, sizeof(SHELLEXECUTEINFO)); 
	sei.cbSize = sizeof(SHELLEXECUTEINFO); 
	sei.fMask = SEE_MASK_NOCLOSEPROCESS; 
	sei.lpVerb = _T("open"); 
	sei.lpFile = _T("pre.exe"); 
	sei.nShow = SW_HIDE; 
	ShellExecuteEx(&sei);
	WaitForSingleObject(sei.hProcess, INFINITE); 
	CloseHandle(sei.hProcess);

	StoreData(_T("EQY.txt"),_T("STOREEQ.txt"),_T("max_min.txt"),color,0);
	DrawOnStaticArea(_T("STOREEQ.txt"));
}

void CdrawDlg::OnBnClickedButton11()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CStdioFile outfile;
	outfile.Open(_T("STOREEQ.txt"),CFile::modeReadWrite|CFile::modeCreate);
	outfile.Close();
	outfile.Open(_T("STORESPE.txt"),CFile::modeReadWrite|CFile::modeCreate);
	outfile.Close();
	outfile.Open(_T("EQ01.txt"),CFile::modeReadWrite|CFile::modeCreate);
	outfile.Close();
	outfile.Open(_T("xMAG.txt"),CFile::modeReadWrite|CFile::modeCreate);
	outfile.Close();
	outfile.Open(_T("EQX.txt"),CFile::modeReadWrite|CFile::modeCreate);
	outfile.Close();
	outfile.Open(_T("EQY.txt"),CFile::modeReadWrite|CFile::modeCreate);
	outfile.Close();
	initialfile.Open(_T("max_min.txt"),CFile::modeCreate|CFile::modeReadWrite);
	initialfile.WriteString(_T("0.0\n0.0\n0.0\n0.0\n0"));
	initialfile.Close();
	initialfile.Open(_T("max_minspe.txt"),CFile::modeCreate|CFile::modeReadWrite);
	initialfile.WriteString(_T("0.0\n0.0\n0.0\n0.0\n0"));
	initialfile.Close();	
	initialfile.Open(_T("SJmax_min.txt"),CFile::modeCreate|CFile::modeReadWrite);
	initialfile.WriteString(_T("0.0\n0.0\n0.0\n0.0\n0"));
	initialfile.Close();
	AngleControl.EnableWindow(FALSE);
	InAngle.SetCheck(FALSE);
	CXbtn.EnableWindow(FALSE);
	CYbtn.EnableWindow(FALSE);
	CDbtn.EnableWindow(FALSE);
	Drawspe1.EnableWindow(FALSE);
	InPGA.EnableWindow(FALSE);
	HZGFFYP.EnableWindow(FALSE);
	EQfilenum=0;
	SPEfilenum=0;
	CRect rt;
    m_pDrawWnd->GetClientRect(&rt);
	// �൱��ѡ�񻭲�,m_pDrawWnd->
	drawDC.FillSolidRect(&rt,RGB(255,255,255));

	CRect rtspe;
	m_Drawspe->GetClientRect(&rtspe);
	drawDCspe.FillSolidRect(&rtspe,RGB(255,255,255));

    CWnd* pWnd = GetDlgItem(IDC_STATIC);//��þ�̬�ı���Ĵ��ڶ���
    CRect rect;
    pWnd->GetClientRect(&rect);    
    CDC* pDC = pWnd->GetDC();//
	DrawChart();
    // һ���ԵĽ��ڴ��豸�����ϻ�����ϵ�ͼ��"��"����Ļ��
    pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &drawDC, 0, 0, SRCCOPY);


	pWnd = GetDlgItem(IDC_SPE);//��þ�̬�ı���Ĵ��ڶ���
    CRect rectspe;
    pWnd->GetClientRect(&rectspe);    
    pDC = pWnd->GetDC();//
	DrawChartspe();

    // һ���ԵĽ��ڴ��豸�����ϻ�����ϵ�ͼ��"��"����Ļ��
    pDC->BitBlt(0, 0, rectspe.Width(), rectspe.Height(), &drawDCspe, 0, 0, SRCCOPY);
    pWnd->ReleaseDC(pDC);//

}

void CdrawDlg::OnBnClickedButton12()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	Drawspe1.EnableWindow(TRUE);
	CStdioFile outfile;
	CString PGAstr;
	nIndex=Choseear.GetCurSel();
	if(nIndex==-1)
	{
		nIndex=0;
	}
	CString EQname;
	switch(nIndex)
	{
	case 0: 
		EQname=_T("EQWenchuan.rec");
		break;
	case 1:
		EQname=_T("EQTohoku.rec");

		break;
	case 2: 
		EQname=_T("EQNZ.rec");
		break;
	case 3: 
		EQname=_T("EQKobe.rec");
		break;
	case 4: 
		EQname=_T("EQChichi.rec");
		break;
	case 5: 
		EQname=_T("EQNorth.rec");
		break;
	case 6: 
		EQname=_T("EQKocaeli.rec");
		break;
	case 7: 
		EQname=_T("EQManjil.rec");
		break;
	case 8:
		EQname=_T("EQElCentro.rec");			
		break;
	case 9:
		EQname=_T("EQDuzce.rec");			
		break;
	case 10:
		EQname=_T("EQHectorMine.rec");			
		break;
	case 11:
		EQname=_T("EQLanders.rec");			
		break;
	case 12:
		EQname=_T("EQLomaPrieta.rec");			
		break;
	case 13:
		EQname=_T("EQSanFernando.rec");			
		break;
	default:
		break;
	}
	int ii,jj;
	CStdioFile infile3;
	CString str2;
	for(ii=1;ii<=adiIndex;ii++)
	{
		if (nIndex==8+ii)
		{
			infile3.Open(_T("newrec.txt"),CFile::modeRead);
			for(jj=0;jj<ii;jj++)
			{
				infile3.ReadString(str2);
			}
			EQname=str2;
		}
	}
	outfile.Open(_T("test.txt"),CFile::modeReadWrite|CFile::modeCreate);
	outfile.WriteString(EQname);
	outfile.Close();
	PGAstr.Format(_T("%lf"),PGA);
	outfile.Open(_T("PGA.txt"),CFile::modeReadWrite|CFile::modeCreate);
	outfile.WriteString(PGAstr);
	outfile.Close();

	SHELLEXECUTEINFO sei; 
	memset(&sei, 0, sizeof(SHELLEXECUTEINFO)); 
	sei.cbSize = sizeof(SHELLEXECUTEINFO); 
	sei.fMask = SEE_MASK_NOCLOSEPROCESS; 
	sei.lpVerb = _T("open"); 
	sei.lpFile = _T("Specturm.exe"); 
	sei.nShow = SW_SHOW;
	ShellExecuteEx(&sei);
	WaitForSingleObject(sei.hProcess, INFINITE); 
	CloseHandle(sei.hProcess);
	
	memset(&sei, 0, sizeof(SHELLEXECUTEINFO)); 
	sei.cbSize = sizeof(SHELLEXECUTEINFO); 
	sei.fMask = SEE_MASK_NOCLOSEPROCESS; 
	sei.lpVerb = _T("open"); 
	sei.lpFile = _T("pre3.exe"); 
	sei.nShow = SW_HIDE;
	ShellExecuteEx(&sei);
	WaitForSingleObject(sei.hProcess, INFINITE); 
	CloseHandle(sei.hProcess);
	
}

void CdrawDlg::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(PGAedit.GetCheck())
	{
		InPGA.EnableWindow(TRUE);
	}
	else
	{
		InPGA.EnableWindow(FALSE);
	}
}

void CdrawDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString maxminname=_T("max_minspe.txt"),filename=_T("SJFYPOUT.txt");
	CRect rtspe;
	m_Drawspe->GetClientRect(&rtspe);
	drawDCspe.FillSolidRect(&rtspe,RGB(255,255,255));
	CWnd* pWnd = GetDlgItem(IDC_SPE);//��þ�̬�ı���Ĵ��ڶ���
	CRect rectspe;
	pWnd->GetClientRect(&rectspe);    
	CDC* pDC = pWnd->GetDC();//
	DrawChartspe();
	// һ���ԵĽ��ڴ��豸�����ϻ�����ϵ�ͼ��"��"����Ļ��
	pDC->BitBlt(0, 0, rectspe.Width(), rectspe.Height(), &drawDCspe, 0, 0, SRCCOPY);
	pWnd->ReleaseDC(pDC);//

	StoreData(_T("SJFYPOUT.txt"),_T("STORESPE.txt"),_T("max_minspe.txt"),color,1);
	DrawOnStaticAreaspe(maxminname,_T("STORESPE.txt"));
}
void CdrawDlg::StoreData(CString filename,CString outname,CString maxminname,COLORREF color,int EQSPE)
{
	CStdioFile infile,outfile,infile2;
	int kk,dotnum,jj,ii,e;
	int k=0,l=0,flag=0,m=0;
	CString str,TOstr,TOstr2;
	double maxtime,mintime,minaccel,maxaccel,time,accel;
	infile.Open(filename,CFile::modeRead);
	infile2.Open(maxminname,CFile::modeRead);
	outfile.Open(outname,CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
	outfile.SeekToEnd();
	if(EQSPE==0)
	{
		EQfilenum++;
	}
	else
	{
		SPEfilenum++;
	}
	infile2.ReadString(str);
	maxtime=_tcstod(str,NULL);
	infile2.ReadString(str);
	mintime=_tcstod(str,NULL);
	infile2.ReadString(str);
	maxaccel=_tcstod(str,NULL);
	infile2.ReadString(str);
	minaccel=_tcstod(str,NULL);
	infile2.ReadString(str);
	//д����ĸ���
	outfile.WriteString(str);
	outfile.WriteString(_T("\n"));
	dotnum=_tcstod(str,NULL);
	//д������ɫ����
	str.Format(_T("%d"),GetRValue(color));
	outfile.WriteString(str);
	outfile.WriteString(_T("\n"));
	str.Format(_T("%d"),GetGValue(color));
	outfile.WriteString(str);
	outfile.WriteString(_T("\n"));
	str.Format(_T("%d"),GetBValue(color));
	outfile.WriteString(str);
	outfile.WriteString(_T("\n"));

	for(kk=1;kk<dotnum;kk++)
	{
		infile.ReadString(str);
		k=0;
		jj=1;
		while(_T("") != (TOstr = str.Tokenize(_T(" "), k)))
		{
			if(jj==1)
			{
				flag=TOstr.Find(_T("E"),l);
				if(flag == -1)
				{
					time=_tcstod(TOstr,NULL);
				}
				else
				//��ȡ��ѧ����������
				{
					m=0;
					ii=1;
					while(_T("") != (TOstr2 = TOstr.Tokenize(_T("E"),m)))
					{
						//outfile.WriteString(TOstr2);
						if(ii==1)
						{
							time=_tcstod(TOstr2,NULL);
						}
						else
						{
							e=_tcstod(TOstr2,NULL);
						}
						ii++;
					}
					time=time*pow(10.0,e);
				}
			}
			else
			{
				flag=TOstr.Find(_T("E"),l);
				if(flag == -1)
				{
					accel=_tcstod(TOstr,NULL);
				}
				else
					//��ȡ��ѧ����������
				{
					m=0;
					ii=1;
					while(_T("") != (TOstr2 = TOstr.Tokenize(_T("E"),m)))
					{
						//outfile.WriteString(TOstr2);
						if(ii==1)
						{
							accel=_tcstod(TOstr2,NULL);
						}
						else
						{
							e=_tcstod(TOstr2,NULL);
						}
						ii++;
					}
					accel=accel*pow(10.0,e);
				}
			}
			jj++;
		}
		str.Format(_T("%.5lf"),time);
		outfile.WriteString(str);
		outfile.WriteString(_T(" "));
		str.Format(_T("%.5lf"),accel);
		outfile.WriteString(str);
		outfile.WriteString(_T("\n"));
	}

	infile.Close();
	infile2.Close();
	outfile.Close();
}
void CdrawDlg::OnBnClickedAddeq()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog *pDlg = new CFileDialog(TRUE,_T("�򿪵��������ļ�"), NULL,	OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,_T("EQ (*.rec)|*.rec|"),	NULL);
	if (pDlg->DoModal() == IDCANCEL)
	{
		delete pDlg;
		return;
	}
	// ���IFC�ļ���·��
	RECpathname = pDlg->GetPathName();
	initialfile.Open(_T("newrec.txt"),CFile::modeReadWrite|CFile::modeNoTruncate|CFile::modeCreate);
	initialfile.SeekToEnd();
	initialfile.WriteString(RECpathname);
	initialfile.WriteString(_T("\n"));
	initialfile.Close();
	UpdateData(TRUE);
	adiIndex++;
	Choseear.InsertString(8+adiIndex,newEQ);
	delete pDlg;
}

void CdrawDlg::OnBnClickedInfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAboutDlg dlg;
	dlg.DoModal();
}

void	CdrawDlg::OnCancel()
{
	OnBnClickedOk();
}

void CdrawDlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	nIndex=Choseear.GetCurSel();
	if(nIndex==-1)
	{
		nIndex=0;
	}

	switch(nIndex)
	{
	case 0: 
		PGA=539;
		InPGA.SetWindowTextW(_T("539"));
		break;
	case 1:
		PGA=2871;
		InPGA.SetWindowTextW(_T("2871"));
		break;
	case 2: 
		PGA=460;
		InPGA.SetWindowTextW(_T("460"));
		break;
	case 3: 
		PGA=500;
		InPGA.SetWindowTextW(_T("500"));
		break;
	case 4: 
		PGA=431;
		InPGA.SetWindowTextW(_T("431"));
		break;
	case 5: 
		PGA=510;
		InPGA.SetWindowTextW(_T("510"));
		break;
	case 6: 
		PGA=353;
		InPGA.SetWindowTextW(_T("353"));
		break;
	case 7: 
		PGA=500;
		InPGA.SetWindowTextW(_T("500"));
		break;
	case 8:
		PGA=342;
		InPGA.SetWindowTextW(_T("342"));
		break;
	case 9:
		PGA=806;
		InPGA.SetWindowTextW(_T("806"));
		break;
	case 10:
		PGA=330;
		InPGA.SetWindowTextW(_T("330"));
		break;
	case 11:
		PGA=400;
		InPGA.SetWindowTextW(_T("400"));
		break;
	case 12:
		PGA=540;
		InPGA.SetWindowTextW(_T("540"));
		break;
	case 13:
		PGA=206;
		InPGA.SetWindowTextW(_T("206"));
		break;
	default:
		break;
	}
}


void CdrawDlg::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
