
// IntegrationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Integration.h"
#include "IntegrationDlg.h"
#include "afxdialogex.h"
#include <iostream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIntegrationDlg 对话框




CIntegrationDlg::CIntegrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIntegrationDlg::IDD, pParent)
	, m_strAttrFilePath(_T(""))
	, m_strShapeFilePath(_T(""))
	, numberOfBuildings(0)
	, m_workPath(_T(""))
	, m_ProjectName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIntegrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strAttrFilePath);
	DDX_Text(pDX, IDC_EDIT2, m_strShapeFilePath);
	DDX_Text(pDX, IDC_EDIT3, m_workPath);
	DDX_Text(pDX, IDC_EDIT4, m_ProjectName);
}

BEGIN_MESSAGE_MAP(CIntegrationDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CIntegrationDlg::OnBnClickedOpenAttrFile)
	ON_EN_CHANGE(IDC_EDIT1, &CIntegrationDlg::OnEnChangeAttrFilePath)
	ON_BN_CLICKED(IDC_BUTTON2, &CIntegrationDlg::OnBnClickedOpenShapeFile)
	ON_EN_CHANGE(IDC_EDIT2, &CIntegrationDlg::OnEnChangeShapeFilePath)
	ON_BN_CLICKED(IDC_BUTTON4, &CIntegrationDlg::OnBnClickedViewOrEditBuildings)
//	ON_EN_CHANGE(IDC_EDIT3, &CIntegrationDlg::OnEnChangeWorkPath)
	ON_BN_CLICKED(IDC_BUTTON7, &CIntegrationDlg::OnBnClickedSetProjectPath)

	ON_BN_CLICKED(IDC_BUTTON3, &CIntegrationDlg::OnBnClickedGroundMotion)
	ON_BN_CLICKED(IDC_BUTTON5, &CIntegrationDlg::OnBnClickedAnalyse)
	ON_BN_CLICKED(IDC_BUTTON6, &CIntegrationDlg::OnBnClickedPostProcess)
END_MESSAGE_MAP()


// CIntegrationDlg 消息处理程序

BOOL CIntegrationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_workPath=L"";
	thisPath=GetCurrWorkingDir();	//获取当前路径，D:\a\b
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
	
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CIntegrationDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CIntegrationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//************************************************************************
//							工程文件设置
//************************************************************************


void CIntegrationDlg::OnBnClickedSetProjectPath()
{
	//新建工程文件
	CFileDialog *pDlg = new CFileDialog		
		(FALSE, _T("新建工程文件"), NULL,	OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,_T( "工程文件 (*.cal)|*.cal||"),	NULL);

	if (pDlg->DoModal() == IDCANCEL)
	{
		delete pDlg;
		return;
	}

	m_workPath = pDlg->GetPathName();
	m_ProjectName=pDlg->GetFileName();

	ofstream fout(m_workPath);
	fout.close();
	
	// 获得工程目录的路径
	int tempInt=m_workPath.GetLength()-m_ProjectName.GetLength()-1;
	ProjectDirectory=m_workPath;
	ProjectDirectory=ProjectDirectory.Left(tempInt); 
	ProjectNameNo_cal=m_ProjectName.Left(m_ProjectName.GetLength()-4);

	delete pDlg;
	UpdateData(FALSE);


	//启用各个按钮

	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);

}



//************************************************************************
//							选择数据文件
//************************************************************************


void CIntegrationDlg::OnBnClickedOpenAttrFile()
{
	
// 打开属性表文件的窗口
//	static char BASED_CODE szFilter[] = "属性表文件 (*.txt)|*.txt||";

	CFileDialog *pDlg = new CFileDialog		
		(TRUE, _T("打开属性表文件"), m_strAttrFilePath,	OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,_T( "属性表文件 (*.txt,*.csv)|*.txt;*.csv||"),	NULL);

	if (pDlg->DoModal() == IDCANCEL)
	{
		delete pDlg;
		return;
	}

	// 获得属性表文件的路径
	
	m_strAttrFilePath = pDlg->GetPathName();
	delete pDlg;

	UpdateData(FALSE);
	//启用相应按钮
	GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
	
}

void CIntegrationDlg::OnEnChangeAttrFilePath()
{
	UpdateData(TRUE);
	//启用相应按钮
	GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
}



void CIntegrationDlg::OnBnClickedOpenShapeFile()
{
	// 打开形状文件的窗口
	//	static char BASED_CODE szFilter[] = "形状文件 (*.txt)|*.txt||";

	CFileDialog *pDlg = new CFileDialog		
		(TRUE, _T("打开形状文件"), m_strShapeFilePath,	OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,_T( "形状文件 (*.txt)|*.txt||"),	NULL);

	if (pDlg->DoModal() == IDCANCEL)
	{
		delete pDlg;
		return;
	}

	// 获得形状文件的路径

	m_strShapeFilePath = pDlg->GetPathName();
	delete pDlg;

	UpdateData(FALSE);
}

void CIntegrationDlg::OnEnChangeShapeFilePath()
{
	UpdateData(TRUE); 
}


//************************************************************************
//								前处理
//************************************************************************


void CIntegrationDlg::OnBnClickedViewOrEditBuildings()	//查看、修改各建筑的滞回曲线等信息
{

//========================================================================================================
//		 1. 打开从GIS中导出的数据表文件，从中读取建筑的GIS信息，存储到 BuildingInfo类中
//========================================================================================================

	ifstream inAttrFile(m_strAttrFilePath);		//打开从GIS中导出的数据表文件

	if (!inAttrFile.good())
	{
		AfxMessageBox(L"“属性表文件”打开失败，请重新指定文件路径！");
		return;
	}

	string temp;
	BuildingInfo tempBuilding;
	getline(inAttrFile,temp);
	buildings.clear();

	//逐行读取数据，保存至buildings中
	char c_judge;	//处理最后一行重复读取的问题
	//inAttrFile.get(c_judge);	

	while (/*c_judge!='\n'&&c_judge!=NULL&&*/!inAttrFile.eof())
	{
		//inAttrFile.putback(c_judge);
		if (inAttrFile.peek() == EOF)
		{
			cout << "file is empty."<<endl;
			break;
		}


		getline(inAttrFile,temp,',');
		getline(inAttrFile,temp,',');					//名称
			tempBuilding.name=temp.c_str();
			tempBuilding.name.TrimLeft();
			tempBuilding.name.TrimRight();
			tempBuilding.name.Replace(' ','_');
			if (!tempBuilding.name.CompareNoCase(L""))
				tempBuilding.name=L"无名建筑";

	//	getline(inAttrFile,temp,'\"');
		inAttrFile>>tempBuilding.constructionTime;		//年代
		getline(inAttrFile,temp,',');					
		getline(inAttrFile,temp,',');					//此处跳过了建筑面积
		inAttrFile>>tempBuilding.floorArea;				//占地面积
		getline(inAttrFile,temp,',');					
		inAttrFile>>tempBuilding.nofStory;				//层数
	//	getline(inAttrFile,temp,'\"');
		getline(inAttrFile,temp,',');
		getline(inAttrFile,temp,',');					//结构类型
		tempBuilding.struType=temp.c_str();
		if (!tempBuilding.struType.CompareNoCase(L""))
			tempBuilding.struType=L"砖混结构";			//默认砖混结构类型
		tempBuilding.struType.TrimLeft();
		tempBuilding.struType.TrimRight();
			if (!tempBuilding.struType.CompareNoCase(L"钢筋混凝土框架")||!tempBuilding.struType.CompareNoCase(L"框架")||!tempBuilding.struType.CompareNoCase(L"內浇外砌"))
				tempBuilding.struType=L"框架结构";
			else if (!tempBuilding.struType.CompareNoCase(L"框架剪力墙结构"))
				tempBuilding.struType=L"框架剪力墙";
			else if (!tempBuilding.struType.CompareNoCase(L"剪力墙"))
				tempBuilding.struType=L"剪力墙结构";
			else if (!tempBuilding.struType.CompareNoCase(L"混合")||!tempBuilding.struType.CompareNoCase(L"混合结构")||!tempBuilding.struType.CompareNoCase(L"砖混"))
				tempBuilding.struType=L"砖混结构";

		//getline(inAttrFile,temp,',');
		inAttrFile>>tempBuilding.storyHeight;			//层高
		getline(inAttrFile,temp,',');
		getline(inAttrFile,temp,',');					//特殊情况
		tempBuilding.descriptions=temp.c_str();
		//getline(inAttrFile,temp,'\"');
		getline(inAttrFile,temp);					//结构功能
		tempBuilding.function=temp.c_str();
		if (!tempBuilding.function.CompareNoCase(L""))
			tempBuilding.function=L"其他";			//默认其他类型
		tempBuilding.function.TrimLeft();
		tempBuilding.function.TrimRight();
			if (!tempBuilding.function.CompareNoCase(L"公寓")||!tempBuilding.function.CompareNoCase(L"家属区")||!tempBuilding.function.CompareNoCase(L"职工家属住宅")||!tempBuilding.function.CompareNoCase(L"教工宿舍")||!tempBuilding.function.CompareNoCase(L"居民楼")||!tempBuilding.function.CompareNoCase(L"宿舍"))
				tempBuilding.function=L"住宅";
			else if (!tempBuilding.function.CompareNoCase(L"出版社")||!tempBuilding.function.CompareNoCase(L"服务类建筑")||!tempBuilding.function.CompareNoCase(L"公共设施"))
				tempBuilding.function=L"办公楼";
			else if (!tempBuilding.function.CompareNoCase(L"教室"))
				tempBuilding.function=L"教学楼";
			else if (!tempBuilding.function.CompareNoCase(L"购物")||!tempBuilding.function.CompareNoCase(L"商业楼")||!tempBuilding.function.CompareNoCase(L""))
				tempBuilding.function=L"商店";
			else if (!tempBuilding.function.CompareNoCase(L"科技楼")||!tempBuilding.function.CompareNoCase(L"科研")||!tempBuilding.function.CompareNoCase(L"汽车房")||!tempBuilding.function.CompareNoCase(L"设计楼")||!tempBuilding.function.CompareNoCase(L"实验室")||!tempBuilding.function.CompareNoCase(L"实验楼"))
				tempBuilding.function=L"研究所";
			/*else if (!tempBuilding.function.CompareNoCase(L"教室"))
			tempBuilding.function=L"教学楼";*/
			else if (!tempBuilding.function.CompareNoCase(L"学生食堂"))
				tempBuilding.function=L"食堂";
			else if (!tempBuilding.function.CompareNoCase(L"招待所")||!tempBuilding.function.CompareNoCase(L"住宿"))
				tempBuilding.function=L"宾馆";

	//	getline(inAttrFile,temp);						//此处省略了其余参数的读取
		buildings.push_back(tempBuilding);				//将该建筑保存至动态数组“buildings”中


	}
	numberOfBuildings=buildings.size();
	inAttrFile.close();

//========================================================================================================
//		 2.  将GIS信息转换成前处理文件  项目名_BuildingInfo.txt
//========================================================================================================

	const int n=39;
	const double pi=3.14159265358979;
//	const int bldn=4255;

	bldtyp typ[4][n];
	int tempInt;
	//int code;
	int c=0;
	CString codePath;
	//将HighCode、LowCode等文件读进内存
	codePath=thisPath+L"\\GIS_to_BuildingInfo\\HighCode.txt";
	ifstream iptH(codePath);

	for (int i=0;i<n;i++)
	{
		iptH>>tempInt>>typ[c][i].name>>typ[c][i].lowlmt>>typ[c][i].highlmt;
		for (int j=0;j<10;j++)
		{
			iptH>>typ[c][i].Props[j];
		}
		for (int j=0;j<4;j++)
		{
			iptH>>typ[c][i].state[j];
		}
		iptH>>typ[c][i].T1>>typ[c][i].T2>>typ[c][i].hos>>typ[c][i].damp;
	}
	iptH.close();

	c=1;
	codePath=thisPath+L"\\GIS_to_BuildingInfo\\ModerateCode.txt";
	ifstream iptM(codePath);
	for (int i=0;i<n;i++)
	{
		iptM>>tempInt>>typ[c][i].name>>typ[c][i].lowlmt>>typ[c][i].highlmt;
		for (int j=0;j<10;j++)
		{
			iptM>>typ[c][i].Props[j];
		}
		for (int j=0;j<4;j++)
		{
			iptM>>typ[c][i].state[j];
		}
		iptM>>typ[c][i].T1>>typ[c][i].T2>>typ[c][i].hos>>typ[c][i].damp;
	}
	iptM.close();
	c=2;
	codePath=thisPath+L"\\GIS_to_BuildingInfo\\LowCode.txt";
	ifstream iptL(codePath);
	for (int i=0;i<n;i++)
	{
		iptL>>tempInt>>typ[c][i].name>>typ[c][i].lowlmt>>typ[c][i].highlmt;
		for (int j=0;j<10;j++)
		{
			iptL>>typ[c][i].Props[j];
		}
		for (int j=0;j<4;j++)
		{
			iptL>>typ[c][i].state[j];
		}
		iptL>>typ[c][i].T1>>typ[c][i].T2>>typ[c][i].hos>>typ[c][i].damp;
	}
	iptL.close();
	c=3;
	codePath=thisPath+L"\\GIS_to_BuildingInfo\\PreCode.txt";
	ifstream iptP(codePath);
	for (int i=0;i<n;i++)
	{
		iptP>>tempInt>>typ[c][i].name>>typ[c][i].lowlmt>>typ[c][i].highlmt;
		for (int j=0;j<10;j++)
		{
			iptP>>typ[c][i].Props[j];
		}
		for (int j=0;j<4;j++)
		{
			iptP>>typ[c][i].state[j];
		}
		iptP>>typ[c][i].T1>>typ[c][i].T2>>typ[c][i].hos>>typ[c][i].damp;
	}
	iptP.close();

	//输出typ数组，供“PreProcess”程序读取

	CString typPath=ProjectDirectory+L"\\"+L"typ.typ";
	ofstream typOut(typPath);
	typOut<<"name"<<'\t'<<"lowlmt"<<'\t'<<"high"<<'\t'<<"Props[10]"<<'\t'<<"damageState[4]"<<'\t'
		  <<"T1"<<'\t'<<"T2"<<'\t'<<"damp"<<'\t'<<"storyHeigt"<<'\n';
	for(int icode=0; icode<4; icode++)
	{
		for (int line=0; line<n;line++)
		{
			typOut<<typ[icode][line].name<<'\t'<<typ[icode][line].lowlmt<<'\t'<<typ[icode][line].highlmt<<'\t';
			for (int i=0;i<10;i++)
			{
				typOut<<typ[icode][line].Props[i]<<'\t';
			}
			for (int i=0;i<4;i++)
			{
				typOut<<typ[icode][line].state[i]<<'\t';
			}
			typOut<<typ[icode][line].T1<<'\t'<<typ[icode][line].T2<<'\t'<<typ[icode][line].damp<<'\t'
				  <<typ[icode][line].hos<<endl;
		}
	}
	typOut.close();
	
	CString filePath=ProjectDirectory+L"\\"+ProjectNameNo_cal+L"_BuildingInfo.txt";
	ofstream fout(filePath);
	
	int						year;						//建造年代
	CString					struType;					//结构类型
	int						nofStory;					//建筑层数
	int						typ1;						//结构类型在36个类型中的索引
	double					T0;							//一阶周期
	double					K0;							//初始刚度
	double					Vy;							//当前层设计抗剪强度
	double					m;							//该建筑每层的质量
	/*double	 lambda[30]={1,2.618,5.0489,8.2909,12.3435,17.2069,22.8808,29.3653,36.6604,44.7661,
						53.6823,63.4091,73.9465,85.2945,97.453,110.4221,124.2018,138.7921,154.1929,170.4043,
						187.4262,205.2588,223.9018,243.3555,263.6197,284.6946,306.5799,329.2759,352.7824,377.0995};*/
	int code;
	fout<<numberOfBuildings<<endl;

	int level=8;	//设防等级 6度到9度：6 7 75 8 85 9
	
	{
		ifstream fin("level");
		fin>>level;
		CString str;
		str.Format(_T("%d"), level);
		//AfxMessageBox(str);
		
		fin.close();
	}
	//while (level!=6&&level!=7&&level!=75&&level!=8&&level!=85&&level!=9)
	//{
	//	cout<<"输入设防等级：（6 7 75 8 85 9）\n ";
	//	cin>>level;
	//}
	

	for (int i=0;i<numberOfBuildings;i++)
	{	
		struType=buildings[i].struType;
		nofStory=buildings[i].nofStory;
		year=buildings[i].constructionTime;

		if(year<=1979)
		{
			code=3;
		}
		else if(year>1979 && year<=1989)
		{
			switch (level)
			{
			case 6:
			case 7:
				code=3;
				break;
			case 75:
			case 8:
				code=2;
				break;
			default:
				code=1;
				break;
			}
		}
		else if(year>1989)
		{
			switch (level)
			{
			case 6:
				code=3;
				break;
			case 7:
			case 75:
				code=2;
				break;
			case 8:
			case 85:
				code=1;
				break;
			default:
				code=0;
				break;
			}
		}

		CStringA stra(buildings[i].name.GetBuffer(0));
		buildings[i].name.ReleaseBuffer();
		string tempString1=stra.GetBuffer(0);
		stra.ReleaseBuffer();

		CStringA strb(struType.GetBuffer(0));
		struType.ReleaseBuffer();
		string tempString2=strb.GetBuffer(0);
		strb.ReleaseBuffer();

		CStringA strc(buildings[i].function.GetBuffer(0));
		buildings[i].function.ReleaseBuffer();
		string tempString3=strc.GetBuffer(0);
		strc.ReleaseBuffer();

		fout<<i+1<<'\t'<<tempString1<<'\t'<<tempString2<<'\t'<<tempString3<<'\t'<<year<<'\t'<<code<<'\t'
			<<nofStory<<'\t'<<buildings[i].storyHeight<<'\t'<<buildings[i].floorArea<<'\t';

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
			if(nofStory>7)							typ1=33;
			else if(nofStory<7&&nofStory>=4)		typ1=32;
			else									typ1=31;
		}
		else if (!struType.CompareNoCase(L"未设防砌体"))
		{
			if(nofStory>2)							typ1=35;
			else									typ1=34;
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
		fout<<T0<<'\t'<<T0*typ[code][typ1].T2<<'\t'<<typ[code][typ1].damp<<endl;
		m=buildings[i].floorArea*1000.0;
		K0=4.0*pi*pi*lambda(nofStory)*m/T0/T0;
		
		for (int currFloor=0;currFloor<nofStory;currFloor++)
		{
			
			double Gamma = 1.0-double(currFloor+1)*double(currFloor)/double(nofStory)/double(nofStory+1);
			Vy=typ[code][typ1].Props[1]*m*9.8*double(nofStory)*Gamma;

			fout<<'\t'<<K0<<'\t'<<Vy;
			for (int j=2; j<10;j++)
			{
				fout<<'\t'<<typ[code][typ1].Props[j];
			}

			fout<<endl;	
		}
		for (int j=0; j<4;j++)
		{
			fout<<'\t'<<typ[code][typ1].state[j];
		}
		fout<<endl;	
	}

	fout.close();
//========================================================================================================
//		 3.  打开PreProcess.exe，并将工程目录、不带后缀的工程文件名作为参数传给该程序
//========================================================================================================

	SHELLEXECUTEINFO shell = { sizeof(shell) };

	shell.fMask = SEE_MASK_FLAG_DDEWAIT;
	shell.lpVerb = L"open";
//	shell.lpFile = L"D:\\Workspace\\PreProcess-v1.0\\Debug\\PreProcess.exe";
	CString tempPath1=thisPath+L"\\PreProcessing\\PreProcess.exe";
//	shell.lpFile = L"PreProcess.exe";
	shell.lpFile = tempPath1;
	shell.nShow = SW_SHOWNORMAL;
//	shell.lpDirectory=L"PreProcessing";
	CString tempPath2=thisPath+L"\\PreProcessing";
	shell.lpDirectory=tempPath2;
	CString para=L"\""+ProjectDirectory+L"\""+L" "+L"\""+ProjectNameNo_cal+L"\"";
	shell.lpParameters=para;
	BOOL ret = ShellExecuteEx(&shell);

	//启用相应按钮
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
}

void CIntegrationDlg::OnBnClickedGroundMotion()
{

	// 1.  打开draw.exe，并将工程目录作为参数传给该程序
	SHELLEXECUTEINFO shell = { sizeof(shell) };

	shell.fMask = SEE_MASK_FLAG_DDEWAIT;
	shell.lpVerb = L"open";

	CString tempPath1=thisPath+L"\\Earthquake\\draw.exe";
	//shell.lpFile = L"draw.exe";
	shell.lpFile = tempPath1;
	shell.nShow = SW_SHOWNORMAL;
	CString tempPath2=thisPath+L"\\Earthquake";
	shell.lpDirectory=tempPath2;
//	shell.lpDirectory=L"Earthquake";
	CString tempPath3=L"\""+m_workPath+L"\"";
	shell.lpParameters=tempPath3;
	BOOL ret = ShellExecuteEx(&shell);

	//启用相应按钮
	GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
}



//************************************************************************
//							分析计算及结果查看
//************************************************************************


void CIntegrationDlg::OnBnClickedAnalyse()
{

//========================================================================================================
//		0. 往工程文件.cal中写入数据，
//========================================================================================================
	{
		//这里，调用OnBnClickedGroundMotion()函数时，外部程序先在.cal文件里写入了部分数据，
		//但本主程序还要做一些数据补充，因此先用ifstream读取，然后再覆盖写入
		string tempStr0,tempStr1,tempStr2,tempStr3;
		ifstream fin(m_workPath);
		getline(fin,tempStr0);
		getline(fin,tempStr1);
		getline(fin,tempStr2);
		getline(fin,tempStr3);
		fin.close();

		ofstream fout(m_workPath);
		//每行依次保存:
		/*		项目名称
				总建筑数，总分析时间长度，总分析点数，分析步长
				地震波名称
				地震波路径
				地震波角度，PGA
				基准点x坐标，基准点y坐标
		*/

			CStringA stra(m_ProjectName.GetBuffer(0));
			m_ProjectName.ReleaseBuffer();
			string tempString1=stra.GetBuffer(0);
			stra.ReleaseBuffer();

		fout<<tempString1<<'\n'<<numberOfBuildings<<'\t'<<tempStr0.c_str()<<'\n'
			<<tempStr1.c_str()<<'\n'
			<<tempStr2.c_str()<<'\n'
			<<tempStr3.c_str()<<'\n';
		   // <<12521000<<'\t'<<4212700<<'\n';
			//<<497000<<'\t'<<315000<<'\n';
		fout.close();
	}
	
//========================================================================================================
//			 1.  打开Analysis.exe，并将工程目录作为参数传给该程序
//========================================================================================================
	SHELLEXECUTEINFO shell = { sizeof(shell) };
	CString para;
	para=L"\""+ProjectDirectory+L"\""+L' '+L"\""+m_ProjectName+L"\"";
	shell.fMask = SEE_MASK_FLAG_DDEWAIT;
	shell.lpVerb = L"open";

	CString tempPath1=thisPath+L"\\Analysis\\Analysis.exe";
	//	shell.lpFile = L"Analysis.exe";
	shell.lpFile = tempPath1;
	shell.nShow = SW_SHOWNORMAL;
	//	shell.lpDirectory=L"Analysis";
	CString tempPath2=thisPath+L"\\Analysis";
	shell.lpDirectory=tempPath2;

	shell.lpParameters=para;
	BOOL ret = ShellExecuteEx(&shell);
//========================================================================================================
//		 2.  根据形状文件，读取点坐标
//========================================================================================================
	points.clear();
	ifstream fin(m_strShapeFilePath);		//打开从GIS中导出的形状文件
	if (!fin.good())
	{
		AfxMessageBox(L"“形状文件”打开失败，请重新指定文件路径！");
		return;
	}

	string temp;
	int tempID=0;
	double tempID2=0.0;
	Point tempPoint;

	getline(fin,temp);
	//temp=fin.get();
	while (!fin.eof())
	{		
		//if (temp=="")
		//	break;
		fin>>tempID2;		//这里要将流数据存入double型变量中，若直接存入int型变量，会直接读到小数点截止，导致后面的读取全乱了
		tempID=(int)tempID2;

			getline(fin,temp,',');

		fin>>tempPoint.x;
		temp=fin.get();
		fin>>tempPoint.y;
		points.insert(make_pair(tempID,tempPoint));
		//temp=fin.get();
	}
	fin.close();

//========================================================================================================
//	 3.  创建点序列文件  ”项目名_Point.txt”，供后处理程序调用
//========================================================================================================

	CString filePath=ProjectDirectory+L"\\"+ProjectNameNo_cal+L"_Point.txt";
	ofstream fout(filePath);
	for (multimap<int, Point>::iterator it=points.begin(); it!=points.end(); ++it)
	{
		int total=points.count(it->first)-1;			//删去同一ID中最后一个点
		fout<<total<<'\n';
		for (int i=0;i<total;i++)
		{
			fout<<it->first<<'\t';
			fout.precision(12);
			fout<<it->second.x<<'\t'<<it->second.y<<'\n';
			++it;
		}
//		it--;
	}
	fout.close();


	{
		ofstream foutCAL(m_workPath,ios_base::app);
		foutCAL.precision(12);
		//foutCAL<<points.begin()->second.x<<'\t'<<points.begin()->second.y<<'\n';
		foutCAL<<497000<<'\t'<<315000<<'\n';
		foutCAL.close();
	}


	// 4.  创建三角划分文件”项目名_Point.txt”，供后处理程序调用



	//启用各个按钮
	GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);

}


void CIntegrationDlg::OnBnClickedPostProcess()
{
	// 1.  打开PostProcess.exe，并将工程目录作为参数传给该程序
	SHELLEXECUTEINFO shell = { sizeof(shell) };
	shell.fMask = SEE_MASK_FLAG_DDEWAIT;
	shell.lpVerb = L"open";

	CString tempPath1=thisPath+L"\\PostProcess\\PostProcess.exe";
	//	shell.lpFile = L"PostProcess.exe";
	shell.lpFile = tempPath1;
	shell.nShow = SW_SHOWNORMAL;
	//	shell.lpDirectory=L"PostProcess";
	CString tempPath2=thisPath+L"\\PostProcess";
	shell.lpDirectory=tempPath2;

	shell.lpParameters=NULL;
	BOOL ret = ShellExecuteEx(&shell);
}

CString CIntegrationDlg::GetCurrWorkingDir()
{
	//这部分是动态获取程序运行时所在路径（即debug文件）

	//CString strPath;
	//TCHAR szFull[_MAX_PATH];
	//TCHAR szDrive[_MAX_DRIVE];
	//TCHAR szDir[_MAX_DIR];
	//::GetModuleFileName(NULL, szFull, sizeof(szFull)/sizeof(TCHAR));
	//_tsplitpath(szFull, szDrive, szDir, NULL, NULL);
	//_tcscpy(szFull, szDrive);
	//_tcscat(szFull, szDir);
	//strPath = CString(szFull);
	//return strPath;

		//这部分是常规路径
		CString strPath;
		GetCurrentDirectory(MAX_PATH,strPath.GetBuffer(MAX_PATH));
		strPath.ReleaseBuffer();
		return strPath;
	
}
