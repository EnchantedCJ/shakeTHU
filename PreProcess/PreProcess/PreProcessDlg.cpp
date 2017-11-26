
// PreProcessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PreProcess.h"
#include "PreProcessDlg.h"
#include "StrucData.h"
#include <fstream>
#include <iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CPreProcessDlg 对话框




CPreProcessDlg::CPreProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPreProcessDlg::IDD, pParent)
	, NoB(0)
	, LastNameSel(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPreProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_tab);
}

BEGIN_MESSAGE_MAP(CPreProcessDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CPreProcessDlg::OnTcnSelchangeTab1)
//	ON_BN_CLICKED(IDC_BUTTON1, &CPreProcessDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPreProcessDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &CPreProcessDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTONPREVIOUS, &CPreProcessDlg::OnBnClickedButtonprevious)
	ON_BN_CLICKED(IDC_BUTTONNEXT, &CPreProcessDlg::OnBnClickedButtonnext)
END_MESSAGE_MAP()


// CPreProcessDlg 消息处理程序

BOOL CPreProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_tab.InsertItem(0,_T("基本参数"));
	m_tab.InsertItem(1,_T("结构参数"));
	m_tab.InsertItem(2,_T("动力特性"));


	m_basic.Create(IDD_DIALOG1,GetDlgItem(IDC_TAB));
	m_struc.Create(IDD_DIALOG2,GetDlgItem(IDC_TAB));
	m_dyn.Create(IDD_DIALOG3,GetDlgItem(IDC_TAB));

	//获得IDC_TABTEST客户区大小 
	CRect rs; 
	m_tab.GetClientRect(&rs); 
	//调整子对话框在父窗口中的位置 
	rs.top+=22; 
	rs.bottom-=1; 
	rs.left+=1; 
	rs.right-=1; 
	 
	//设置子对话框尺寸并移动到指定位置 
	m_basic.MoveWindow(&rs); 
	m_struc.MoveWindow(&rs); 
	m_dyn.MoveWindow(&rs); 
	 
	//分别设置隐藏和显示 
	m_basic.ShowWindow(true); 
	m_struc.ShowWindow(false); 
	m_dyn.ShowWindow(false); 
	int NoB=0;
	int LastNameSel=0;
	//m_struc.CheckRadioButton(IDC_RADIO1,IDC_RADIO3,IDC_RADIO1)
	

 	hasBeenSaved=0;
	//设置对话框可以带参数启动

	int argc=0;
	LPWSTR *argv=::CommandLineToArgvW(::GetCommandLineW(),&argc);

	//根据传入的命令行参数，打开文档
	//argv[1]:工程文件夹路径
	ProjectDirectory=argv[1];
	ProjectNameNo_cal=argv[2];
//	ProjectDirectory=L"D:\\Workspace\\CAL-zx";
	openFile();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPreProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPreProcessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPreProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPreProcessDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	int CurSel = m_tab.GetCurSel(); 
	switch(CurSel) 
	{ 
		case 0: 
		m_basic.ShowWindow(true); 
		m_struc.ShowWindow(false); 
		m_dyn.ShowWindow(false); 
		break; 
		case 1: 
			m_basic.ShowWindow(false); 
			m_struc.ShowWindow(true); 
			m_dyn.ShowWindow(false); 
			break;; 
		case 2: 
			m_basic.ShowWindow(false); 
			m_struc.ShowWindow(false); 
			m_dyn.ShowWindow(true); 
			break;
		default: ;
	*pResult = 0;
	}
}

void CPreProcessDlg::openFile(void)
{

	//数据读入，存入以下临时变量中
	char Name[200];
	char StrucType[20];
	char BldFunction[20];
	float damage[4];
	float damping;
	float n;// n=T/NO of stories
	float n12;// n12=T1/T2
	int i,j;
	int Year;
	int YearType;
	int NoFloor;
	int BID;
	float FloArea;
	float FloHeight;

	//默认打开   项目名_BuildingInfo_Modified.txt文件
	CString filePath=ProjectDirectory;
	filePath=filePath+L"\\"+ProjectNameNo_cal+L"_BuildingInfo_Modified.txt";
	ifstream infile(filePath);

	//如果无法打开，则打开     项目名_BuildingInfo.txt文件（第一次运行程序时，只有该文件）
	if (!infile.good())
	{
		filePath=ProjectDirectory;
		filePath=filePath+L"\\"+ProjectNameNo_cal+L"_BuildingInfo.txt";
		infile.open(filePath);
		if (!infile.good())
		{
			CString tempStr=L"打开文件 ";
			tempStr=tempStr+filePath+L" 发生错误！程序将退出。";
			AfxMessageBox(tempStr);
			exit(0);
		}
	}
	
	
	infile>> NoB ;
	data =new StrucData* [NoB];			//开辟内存空间
	for (i=0;i<NoB;i++)
	{
		data [i]=new StrucData();
		
	}


	for (i=0;i<NoB;i++)
	{
		infile>>BID>>Name>>StrucType>>BldFunction>>Year>>YearType>>NoFloor>>FloHeight>>FloArea>>n>>n12>>damping;
		for (j=0;j<10;j++)
		{
			data[i]->props[j] = new float[NoFloor];
		}
		for (int k=0;k<NoFloor;k++)
		{
			for (j=0;j<10;j++)
			{
				infile>>data[i]->props[j][k];   //第i栋结构的第k层的第j个层间滞回参数
			}
		}
		for(j=0;j<4;j++)
			infile>>damage[j];
		data[i]->BID=BID;
		data[i]->Name=Name;
		data[i]->StrucType=StrucType;
		data[i]->BldFunction=BldFunction;
		data[i]->Year=Year;
		data[i]->YearType=YearType;
		data[i]->NoFloor=NoFloor;
		data[i]->FloHeight=FloHeight;
		data[i]->FloArea=FloArea;
		data[i]->n=n;
		data[i]->n12=n12;
		data[i]->damping=damping;
		for (j=0;j<4;j++)
		{
			data[i]->damage[j]=damage[j];
		}
		

	}
	
	//参数读取完毕

	infile.close();


	//设置首页（参数输入）的控件
	CString temp;
	int nIndex;
	temp=Name;
	m_basic.m_CBName.ResetContent();
	for (i=0;i<NoB;i++)
	{
		CString tempStr=L"";
		tempStr.Format(L"%d",i+1);
		tempStr=tempStr+L"    "+data[i]->Name;
		nIndex = m_basic.m_CBName.AddString(tempStr);
//		nIndex = m_basic.m_CBName.AddString(data[i]->Name);
	}
	
	
	m_basic.m_CBType.ResetContent();
	m_basic.m_CBType.AddString(_T("框架结构"));
	m_basic.m_CBType.AddString(_T("砖混结构"));
	m_basic.m_CBType.AddString(_T("钢结构"));
	m_basic.m_CBType.AddString(_T("框架剪力墙"));
	m_basic.m_CBType.AddString(_T("剪力墙结构"));
//	m_basic.m_CBType.AddString(data[i]->StrucType);

	


	//将从文件读入的、保存在临时变量中的数据，保存至对话框的各个控件变量中
	SaveToControl(0,0);
	
}

void CPreProcessDlg::OnBnClickedButton2()
{
	CString filePath;
	filePath=ProjectDirectory+L"\\"+ProjectNameNo_cal+L"_BuildingInfo_Modified.txt";
	int nIndex;
	int CurFloor;
	nIndex=m_basic.m_CBName.GetCurSel();
	CurFloor=m_struc.m_CBCurFloor.GetCurSel();
	SaveToData(nIndex,CurFloor);
	SaveToFile(filePath);
	hasBeenSaved=1;
}

//将保存在内存临时变量中的数据，保存至对话框的各个控件变量中
void CPreProcessDlg::ReCalcData(int BuildingNO)
{
	int nIndex;
	CString temp;
	
	//GetDlgItem(IDC_EDIT_LUQU10)->EnableWindow(FALSE);
	//设置对话框初始数据
	//m_basic类
	
	m_basic.m_CBName.SetCurSel(BuildingNO);
	m_basic.m_intNoF=data[BuildingNO]->NoFloor;

	temp=data[BuildingNO]->StrucType.TrimLeft();
	temp=temp.TrimRight();
	m_basic.m_strType=data[BuildingNO]->StrucType;
	//nIndex=m_basic.m_CBType.FindStringExact(-1,temp);
	//m_basic.m_CBType.SetCurSel(nIndex);
	m_basic.m_intYear= data[BuildingNO]->Year;
	m_basic.m_fltArea = data[BuildingNO]->FloArea;

	//数据更新了，需要重新计算该结构类型、结构层数等对应的变量。

	{
		const int n=39;
		const double pi=3.14159265358979;
		//	const int bldn=4255;

		bldtyp typ[4][n];
		string tempString;
		CString typPath=ProjectDirectory+L"\\"+L"typ.typ";
		ifstream typIn(typPath);
		getline(typIn,tempString);
		for(int code=0; code<4; code++)
		{
			for (int line=0; line<n;line++)
			{
				typIn>>typ[code][line].name>>typ[code][line].lowlmt>>typ[code][line].highlmt;
				for (int i=0;i<10;i++)
				{
					typIn>>typ[code][line].Props[i];
				}
				for (int i=0;i<4;i++)
				{
					typIn>>typ[code][line].state[i];
				}
				typIn>>typ[code][line].T1>>typ[code][line].T2>>typ[code][line].damp>>typ[code][line].hos;
			}
		}
		typIn.close();

	int						code;
	int						year;						//建造年代
	CString					struType;					//结构类型
	int						nofStory;					//建筑层数
	int						typ1;						//结构类型在36个类型中的索引
	double					T0;							//一阶周期
	//double					K0;							//初始刚度
	//double					Vy;							//当前层设计抗剪强度
	double					m;							//该建筑每层的质量
	/*double					lambda[30]={1,2.618,5.0489,8.2909,12.3435,17.2069,22.8808,29.3653,36.6604,44.7661,
						53.6823,63.4091,73.9465,85.2945,97.453,110.4221,124.2018,138.7921,154.1929,170.4043,
						187.4262,205.2588,223.9018,243.3555,263.6197,284.6946,306.5799,329.2759,352.7824,377.0995};*/

		struType=m_basic.m_strType;
		nofStory=m_basic.m_intNoF;
		year=m_basic.m_intYear;
		if(year<1978) code=3;
		else if(year>=1978 && year<1989) code=2;
		else if(year>=1989) code=1;

		//不同层数的“钢筋混凝土框架”（“框架结构”）、“框架剪力墙”、“砖混结构”、“钢结构”、“剪力墙”
		//对应的结构类型
		if (!struType.CompareNoCase(L"钢筋混凝土框架")||!struType.CompareNoCase(L"框架结构")) 
		{
			if(nofStory>7)							typ1=18;
			else if(nofStory<7&&nofStory>=4)		typ1=17;
			else									typ1=16;
		}
		else if (!struType.CompareNoCase(L"钢结构"))
		{
			if(nofStory>7)							typ1=5;
			else if(nofStory<7&&nofStory>=4)		typ1=4;
			else									typ1=3;
		}
		else if (!struType.CompareNoCase(L"框架剪力墙"))
		{
			if(nofStory>7)							typ1=39;
			else if(nofStory<7&&nofStory>=4)		typ1=38;
			else									typ1=37;
		}
		else if (!struType.CompareNoCase(L"砖混结构"))
		{
			if(nofStory>2)					typ1=35;
			else							typ1=34;
		}
		else if (!struType.CompareNoCase(L"剪力墙")||!struType.CompareNoCase(L"剪力墙结构"))
		{
			if(nofStory>7)							typ1=21;
			else if(nofStory<7&&nofStory>=4)		typ1=20;
			else									typ1=19;
		}
		typ1--;
		/*if (nofStory>typ[code][typ1].highlmt) typ1++;
		if (nofStory>typ[code][typ1].highlmt) typ1++;*/
		

		T0=nofStory*typ[code][typ1].T1;
		
		data[BuildingNO]->n=T0;
		data[BuildingNO]->n12=T0*typ[code][typ1].T2;
		data[BuildingNO]->damping=typ[code][typ1].damp;

		m=data[BuildingNO]->FloArea*1000.0;			//建筑物质量：1吨每平米

		for (int i=0;i<nofStory;i++)
		{
			data[BuildingNO]->props[0][i]=4.0*pi*pi*lambda(nofStory)*m/T0/T0;
			double Gamma = 1.0-double(i+1)*double(i)/double(nofStory)/double(nofStory+1);
			data[BuildingNO]->props[1][i]=typ[code][typ1].Props[1]*m*9.8*double(nofStory)*Gamma;

			for (int j=2; j<10;j++)
			{
				data[BuildingNO]->props[j][i]=typ[code][typ1].Props[j];
			}

		}
		for (int j=0; j<4;j++)
		{
			data[BuildingNO]->damage[j]=typ[code][typ1].state[j];
		}

	}


	m_basic.m_fltGood=data[BuildingNO]->damage[0];
	m_basic.m_fltSlightly=data[BuildingNO]->damage[1];
	m_basic.m_fltModerate=data[BuildingNO]->damage[2];
	m_basic.m_fltSerious=data[BuildingNO]->damage[3];

	m_basic.UpdateData(FALSE);


	//m_Struc类
	int NoFloor;
	NoFloor=data[BuildingNO]->NoFloor;
	m_struc.m_CBCurFloor.ResetContent();
	for (int k=0;k<NoFloor;k++)
	{
		temp.Format(_T("Floor %d"),k+1);
		m_struc.m_CBCurFloor.AddString(temp);
	}

	m_struc.m_CBCurFloor.SetCurSel(0);

	m_struc.m_fltProps1=data[BuildingNO]->props[0][0];
	m_struc.m_fltProps2=data[BuildingNO]->props[1][0];
	m_struc.m_fltProps3=data[BuildingNO]->props[2][0];
	m_struc.m_fltProps4=data[BuildingNO]->props[3][0];
	m_struc.m_fltProps5=data[BuildingNO]->props[4][0];
	m_struc.m_fltProps6=data[BuildingNO]->props[5][0];
	m_struc.m_fltProps7=data[BuildingNO]->props[6][0];
	m_struc.m_fltProps8=data[BuildingNO]->props[7][0];
	m_struc.m_fltProps9=data[BuildingNO]->props[8][0];
	m_struc.m_fltProps10=data[BuildingNO]->props[9][0];

	m_struc.UpdateData(FALSE);

	//m_Dyn类
	m_dyn.damping1=data[BuildingNO]->damping;
	m_dyn.FreqRatio=data[BuildingNO]->n12;
	m_dyn.UpdateData(FALSE);



	nIndex=m_basic.m_CBType.GetCurSel();
	m_basic.m_CBType.GetLBText(nIndex,temp);
	temp=temp.TrimLeft();
	temp=temp.TrimRight();
	if (temp==_T("钢结构"))
	{
		//m_struc.GetDlgItem(IDC_EDIT_LUQU9)->EnableWindow(FALSE);
		m_struc.GetDlgItem(IDC_EDIT_LUQU10)->EnableWindow(FALSE);
		m_struc.GetDlgItem(IDC_EDIT_LUQU5)->EnableWindow(FALSE);
	}
	else if (temp==_T("钢筋混凝土框架")||temp==_T("框架结构"))
	{
		m_struc.GetDlgItem(IDC_EDIT_LUQU9)->EnableWindow(TRUE);
		m_struc.GetDlgItem(IDC_EDIT_LUQU10)->EnableWindow(TRUE);
		m_struc.GetDlgItem(IDC_EDIT_LUQU5)->EnableWindow(TRUE);
	}
	else if (temp==_T("砖混结构"))
	{
		m_struc.GetDlgItem(IDC_EDIT_LUQU9)->EnableWindow(TRUE);
		m_struc.GetDlgItem(IDC_EDIT_LUQU10)->EnableWindow(TRUE);
		m_struc.GetDlgItem(IDC_EDIT_LUQU5)->EnableWindow(TRUE);
	}

}


//将修改后的参数保存在文件里
void CPreProcessDlg::SaveToFile(CString str)
{
	int i;
	ofstream outfile(str);
	outfile<<NoB<<endl;
	for (i=0;i<NoB;i++)
	{
		//设置对话框初始数据
		//m_basic类
		char Name[200],StrucType[20] , BldFunction[20];
		int nIndex;

		int nLength = data[i]->Name.GetLength(); //Cstring to char
		int nBytes = WideCharToMultiByte(CP_ACP,0,data[i]->Name,nLength,NULL,0,NULL,NULL);
		memset(Name,0,nLength + 1);
		WideCharToMultiByte(CP_OEMCP, 0,data[i]->Name, nLength, Name, nBytes, NULL, NULL); 
		Name[nBytes] = 0;

		nLength = data[i]->StrucType.GetLength(); //Cstring to char
		nBytes = WideCharToMultiByte(CP_ACP,0,data[i]->StrucType,nLength,NULL,0,NULL,NULL);
		memset(StrucType,0,nLength + 1);
		WideCharToMultiByte(CP_OEMCP, 0, data[i]->StrucType, nLength, StrucType, nBytes, NULL, NULL); 
		StrucType[nBytes] = 0;

		nLength = data[i]->BldFunction.GetLength(); //Cstring to char
		nBytes = WideCharToMultiByte(CP_ACP,0,data[i]->BldFunction,nLength,NULL,0,NULL,NULL);
		memset(BldFunction,0,nLength + 1);
		WideCharToMultiByte(CP_OEMCP, 0, data[i]->BldFunction, nLength, BldFunction, nBytes, NULL, NULL); 
		BldFunction[nBytes] = 0;

		int YearType;
		if (data[i]->Year>=2001)
		{
			YearType=1;
		}
		else if (data[i]->Year>=1989&&data[i]->Year<2001)
		{
			YearType=2;
		}
		else if (data[i]->Year>=1979&&data[i]->Year<1989)
		{
			YearType=3;
		}
		else if (data[i]->Year<1979)
		{
			YearType=4;
		}
		

		outfile<<i+1<<"\t"<<Name<<"\t"<<StrucType<<"\t"<<BldFunction<<"\t"<<data[i]->Year<<"\t"<<YearType<<"\t"<<data[i]->NoFloor<<"\t"<<data[i]->FloHeight<<"\t"<<data[i]->FloArea<<"	"<<data[i]->n<<"\t"<<data[i]->n12<<"\t"<<data[i]->damping<<endl;
		
		if (data[i]->StrucType==_T("钢结构"))
		{
			for (int k=0;k<data[i]->NoFloor;k++)
			{
				for(int j=0;j<10;j++)
					if (j==9)
					{
						outfile<<"\t"<<-1;
					}
					else
					{
						outfile<<"\t"<<data[i]->props[j][k];
					}
					
				outfile<<endl;
			}
		}
		else
		{
			for (int k=0;k<data[i]->NoFloor;k++)
			{
				for(int j=0;j<10;j++)
					outfile<<"\t"<<data[i]->props[j][k];
				outfile<<endl;
			}
		}
			
			for(int j=0;j<4;j++)
				outfile<<"\t"<<data[i]->damage[j];

		outfile<<endl;

		
		}
	outfile.close();//设置对话框初始数据
	
}


//将控件中的当前值保存在内存中
void CPreProcessDlg::SaveToData(int nIndex,int Floor)
{
	
	
	m_basic.UpdateData(TRUE);
	
	//m_basic.m_CBName.GetLBText(nIndex,data[nIndex]->Name);



	data[nIndex]->NoFloor=m_basic.m_intNoF;


	int Index=m_basic.m_CBType.GetCurSel();
	m_basic.m_CBType.GetLBText(Index,data[nIndex]->StrucType);
	data[nIndex]->Year=m_basic.m_intYear;
	data[nIndex]->FloArea=m_basic.m_fltArea;
	data[nIndex]->FloHeight=m_basic.m_styHeight;



	data[nIndex]->damage[0]=m_basic.m_fltGood;
	data[nIndex]->damage[1]=m_basic.m_fltSlightly;
	data[nIndex]->damage[2]=m_basic.m_fltModerate;
	data[nIndex]->damage[3]=m_basic.m_fltSerious;



	//m_Struc类
	m_struc.UpdateData(TRUE);
	data[nIndex]->props[0][Floor]=m_struc.m_fltProps1;
	data[nIndex]->props[1][Floor]=m_struc.m_fltProps2;
	data[nIndex]->props[2][Floor]=m_struc.m_fltProps3;
	data[nIndex]->props[3][Floor]=m_struc.m_fltProps4;
	data[nIndex]->props[4][Floor]=m_struc.m_fltProps5;
	data[nIndex]->props[5][Floor]=m_struc.m_fltProps6;
	data[nIndex]->props[6][Floor]=m_struc.m_fltProps7;
	data[nIndex]->props[7][Floor]=m_struc.m_fltProps8;
	data[nIndex]->props[8][Floor]=m_struc.m_fltProps9;
	data[nIndex]->props[9][Floor]=m_struc.m_fltProps10;



	//m_Dyn类
	m_dyn.UpdateData(TRUE);
	data[nIndex]->damping=m_dyn.damping1;
//	data[nIndex]->n12=m_dyn.FreqRatio;

}


void CPreProcessDlg::OnBnClickedOk()
{
	if (!hasBeenSaved)
	{
		OnBnClickedButton2();
		for (int i=0; i<NoB;i++)
		{
			delete data[i];
		}
		delete data;
		exit(0);
	} 
	
	int state=MessageBox(L"是否保存修改？",NULL,MB_YESNOCANCEL|MB_ICONEXCLAMATION);
	switch (state)
	{
		case 6:			//是，保存退出
			
			OnBnClickedButton2();
			for (int i=0; i<NoB;i++)
			{
				delete data[i];
			}
			delete data;
			CDialog::OnOK();
			break;

		case 7:			//否，不保存退出
			CDialog::OnOK();
			for (int i=0; i<NoB;i++)
			{
				delete data[i];
			}
			delete data;
			break;

		default:
				//取消
			break;
	}
	

}

void	CPreProcessDlg::OnCancel()
{
	OnBnClickedOk();
}

void CPreProcessDlg::OnBnClickedButtonprevious()
{
	int nIndex;
	int LastSel;
	int CurFloor;
	//CWnd* pWnd=AfxGetApp()->GetMainWnd();
	//CPreProcessDlg* dlg;
	//dlg= (CPreProcessDlg*)pWnd;

	nIndex=m_basic.m_CBName.GetCurSel()-1;
	if (nIndex<0)
	{
		nIndex=0;
	}
	LastSel=LastNameSel;
	CurFloor=m_struc.m_CBCurFloor.GetCurSel();
	SaveToData(LastSel,CurFloor);
	LastNameSel=nIndex;
	SaveToControl(nIndex,0);
}


void CPreProcessDlg::OnBnClickedButtonnext()
{
	int nIndex;
	int LastSel;
	int CurFloor;
	//CWnd* pWnd=AfxGetApp()->GetMainWnd();
	//CPreProcessDlg* dlg;
	//dlg= (CPreProcessDlg*)pWnd;

	nIndex=m_basic.m_CBName.GetCurSel()+1;
	if (nIndex>m_basic.m_CBName.GetCount()-1)
	{
		nIndex--;
	}
	LastSel=LastNameSel;
	CurFloor=m_struc.m_CBCurFloor.GetCurSel();
	SaveToData(LastSel,CurFloor);
	LastNameSel=nIndex;
	SaveToControl(nIndex,0);

}

void CPreProcessDlg::SaveToControl(int nIndex,int CurFloor)
{
	CString temp;
	//GetDlgItem(IDC_EDIT_LUQU10)->EnableWindow(FALSE);
	//设置对话框初始数据
	//m_basic类

	m_basic.m_CBName.SetCurSel(nIndex);
	m_basic.m_intNoF=data[nIndex]->NoFloor;
	m_basic.m_strType=data[nIndex]->StrucType;
	m_basic.m_intYear= data[nIndex]->Year;
	m_basic.m_fltArea = data[nIndex]->FloArea;
	m_basic.m_styHeight = data[nIndex]->FloHeight;
	m_basic.m_BldFunctionValue = data[nIndex]->BldFunction;

	m_basic.m_fltGood=data[nIndex]->damage[0];
	m_basic.m_fltSlightly=data[nIndex]->damage[1];
	m_basic.m_fltModerate=data[nIndex]->damage[2];
	m_basic.m_fltSerious=data[nIndex]->damage[3];
	
	m_basic.UpdateData(FALSE);

	//m_Struc类
	int NoFloor;
	NoFloor=data[nIndex]->NoFloor;
	m_struc.m_CBCurFloor.ResetContent();
	for (int k=0;k<NoFloor;k++)
	{
		temp.Format(_T("Floor %d"),k+1);
		m_struc.m_CBCurFloor.AddString(temp);
	}
	m_struc.m_CBCurFloor.SetCurSel(CurFloor);

	m_struc.m_fltProps1=data[nIndex]->props[0][CurFloor];
	m_struc.m_fltProps2=data[nIndex]->props[1][CurFloor];
	m_struc.m_fltProps3=data[nIndex]->props[2][CurFloor];
	m_struc.m_fltProps4=data[nIndex]->props[3][CurFloor];
	m_struc.m_fltProps5=data[nIndex]->props[4][CurFloor];
	m_struc.m_fltProps6=data[nIndex]->props[5][CurFloor];
	m_struc.m_fltProps7=data[nIndex]->props[6][CurFloor];
	m_struc.m_fltProps8=data[nIndex]->props[7][CurFloor];
	m_struc.m_fltProps9=data[nIndex]->props[8][CurFloor];
	m_struc.m_fltProps10=data[nIndex]->props[9][CurFloor];

	m_struc.UpdateData(FALSE);

	//m_Dyn类
	m_dyn.damping1=data[nIndex]->damping;
	m_dyn.FreqRatio=data[nIndex]->n12;
	m_dyn.UpdateData(FALSE);



	nIndex=m_basic.m_CBType.GetCurSel();
	m_basic.m_CBType.GetLBText(nIndex,temp);
	temp=temp.TrimLeft();
	temp=temp.TrimRight();
	if (temp==_T("钢结构"))
	{
		//m_struc.GetDlgItem(IDC_EDIT_LUQU9)->EnableWindow(FALSE);
		m_struc.GetDlgItem(IDC_EDIT_LUQU10)->EnableWindow(FALSE);
		m_struc.GetDlgItem(IDC_EDIT_LUQU5)->EnableWindow(FALSE);
	}
	if (temp==_T("框架结构"))
	{
		m_struc.GetDlgItem(IDC_EDIT_LUQU9)->EnableWindow(TRUE);
		m_struc.GetDlgItem(IDC_EDIT_LUQU10)->EnableWindow(TRUE);
		m_struc.GetDlgItem(IDC_EDIT_LUQU5)->EnableWindow(TRUE);
	}
	if (temp==_T("砖混结构"))
	{
		m_struc.GetDlgItem(IDC_EDIT_LUQU9)->EnableWindow(TRUE);
		m_struc.GetDlgItem(IDC_EDIT_LUQU10)->EnableWindow(TRUE);
		m_struc.GetDlgItem(IDC_EDIT_LUQU5)->EnableWindow(TRUE);
	}


}