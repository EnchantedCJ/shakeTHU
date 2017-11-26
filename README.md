CAL项目说明
#For developers
本项目实际包含3个独立程序。本应拆分成3个不同项目，但这里放到1个项目中，分别存储在master, dev, fscmd三个分支里，如下图所示。
这3个独立程序之间有大量相同代码，所以修改了其中一处时，请同时修改其他分支上对应的代码。

![Git版本示意图](http://git.oschina.net/uploads/images/2017/0315/060710_37c207c6_1162785.png "git版本示意图")

#For users
一．	程序说明
1.	程序运行需且仅需2个文件：属性表文件和形状文件。它们都是由ArcGIS模型中导出的基本数据文件。
2.	“选择地震波”按钮打开林楷奇的程序。得到所选择的地震波以及PGA。
3.	“查看/编辑建筑模型”按钮打开熊琛的程序。
a)	点击该按钮时，先根据属性表文件，生成一个默认文件” 项目名_BuildingInfo.txt”。熊琛程序判断是否有” 项目名_BuildingInfo_modified.txt”，如有，打开之；否则打开” 项目名_BuildingInfo.txt”。
b)	熊的程序编辑之后，保存为” 项目名_BuildingInfo_modified.txt”
4.	“分析计算”按钮根据2、3两个文件，进行计算，生成计算文件。
a)	点击该按钮时，先带参数调用韩博的计算程序（前台）。
b)	然后通过形状文件生成点序列文件” 项目名_Point.txt”
5.	“查看计算结果”直接调用后处理程序。但该程序依赖工程文件.cal、计算结果文件、” 项目名_Point.txt”文件。


二．	工程目录和程序目录说明
1.	新建工程文件，该文件所在文件夹为工程目录
2.	在工程目录下，生成如下文件：
1)	工程文件名称.cal；
2)	前处理文件：项目名_BuildingInfo.txt、项目名_BuildingInfo_Modified.txt
3)	地震波文件：写到工程文件下。
4)	计算文件：
5)	结果文件：
3.	程序所在目录，即程序目录。下有如下子目录：
1)	”\PreProcessing”：熊的程序
2)	“\GIS_to_BuildingInfo”：GIS文件转换成项目名_BuildingInfo.txt所需的文件
3)	“\Earthquake”：选择地震波的程序
4)	“\Analysis”：韩博的计算程序

三．	数据交换格式与参数传递

======计算程序=======
1.  计算程序命名为Analysis.exe
2.  传给计算程序的参数为2个：第一个是工程文件夹目录  第二个是工程文件的文件名 （就是 ***.cal）
    接收参数只需在一开始添加如下代码（对于对话框，在OnInitDialog函数中添加）：
       int argc=0;
	     LPWSTR *argv=::CommandLineToArgvW(::GetCommandLineW(),&argc);
	  然后 argc保存了参数的数量，argv是一个大小为argc的数组。直接将argc[0]和argc[1]赋值给某个CString变量就能得到工程文件夹目录和工程文件的文件名。
	  
=====后处理程序=======
1.	后处理程序命名为PostProcess.exe
2.	后处理程序要打开：工程文件.cal、计算结果文件、”项目名_Point.txt”文件、”项目名_Damage.txt”文件、”项目名_Result.txt”文件
3.	工程文件.cal里面存了如下东西：
    项目名称
		总建筑数  总分析时间长度  总分析点数  分析步长
		地震波名称
		地震波路径
		地震波角度  PGA
	  基准点x 基准点y
			（按顺序，每个参数占一行）
4.	计算结果文件格式如下（二进制格式）：
建筑总数
编号1
时间点T1	1层位移d1	2层位移d2	… N层位移dN
时间点T2	……
……
时间点Tn
//重复以上单元，共NofB次，其中NofB是建筑总数


5.	项目名_Point.txt写在工程文件夹目录下，具体格式如下：
    建筑i的点的数量Ni
    该点所在建筑i的id  x坐标  y坐标   
    //....重复上一行，共Ni行
    //....重复以上单元，共NofB次，其中NofB是建筑总数
   

6.	项目名_Damage.txt写在工程文件夹目录下，具体格式如下：
    619
    1 0 0 0 0
    2 0 0
    3 0 1
    4 2 0
    5 1 0
    6 3 1
  各建筑编号和各楼层破坏情况。

7.	项目名_Result.txt写在工程文件夹目录下，

代码：
bool CAnalysisResult::Write(ofstream &ofs)
{
	ofs<<m_ID<<"	"<<m_Name<<"	"<<m_Type<<"	"<<m_Year<<"	"<<m_NumStories<<"	"<<m_StoryHeight<<"	"
		<<m_PGA<<"	"<<m_MaxInter<<"	"<<m_MaxDrift<<"	"<<m_MaxDamage<<"	"<<m_StoryMaxInter<<"	"
		<<m_TimeMaxInter<<"	"<<m_MaxAcce<<"	"<<m_StoryMaxAcce<<"	"<<m_TimeMaxAcce<<"	"<<m_DeadTime<<endl;
	return true;
}
PGA后面分别是：
最大层间位移角 最大层间位移 破坏状态 最大层间位移所在楼层 最大层间位移发生时刻 最大楼层加速度 最大楼层加速度所在楼层 最大楼层加速度发生时刻 倒塌时刻
