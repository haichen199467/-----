
// testDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CtestDlg 对话框



CtestDlg::CtestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_comm1);
	DDX_Control(pDX, IDC_BUTTON2, m_serial);
}

BEGIN_MESSAGE_MAP(CtestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CtestDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT1, &CtestDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CtestDlg 消息处理程序

BOOL CtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestDlg::OnPaint()
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
HCURSOR CtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtestDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CtestDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
BEGIN_EVENTSINK_MAP(CtestDlg, CDialogEx)
	ON_EVENT(CtestDlg, IDC_MSCOMM1, 1, CtestDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()


void CtestDlg::OnCommMscomm1()
{
	VARIANT variant1;//定义VARIANT型变量，用于存放接收到的数据
	COleSafeArray safearray;//定义safearray型变量
	LONG len, k;//定义长整型变量len,k
	BYTE rxdata[2048];//定义BYTE型数组
	CString stremp1, stremp2;//定义两个字符串
	if (m_comm1.get_CommEvent() == 2) //判断引起OnComm时间的原因
	{//如果是接收到特定个字节数，则读取接收到的数据
		variant1 = m_comm1.GetInput();//把接收到的数据存放到VARIANT型变量里
		safearray = variant1;//VARIANT型变量转换为ColeSafeArray型变量
		len = safearray.GetOneDimSize();
		for (k = 0;k<len;k++)
		{
			safearray.GetElement(&k, rxdata + k); //得到接接收到的数据放到BYTE型数组rxdata里
		}
		for (k = 0;k<len;k++)
		{
			BYTE bt = (*(unsigned char*)(rxdata + k)); //读取AD转换的高字节
			if ((k % 2) == 0)
				if ((k + 1)<len)
				{
					gllen++;//全局的变量，对接收到的转换结果的个数进行计算
					stremp2.Format("第%d次转换结果：", gllen);//显示第几次转换
					int temp = bt * 4 + ((*(unsigned char *)(rxdata + k + 1)) >> 6); //高低字节合并成实际的转换结果，注意转换结果是左对齐
					stremp1.Format("%2.2f", (2.56*temp / 1024));//计算成实际电压值
					SetDlgItemText(IDC_STATIC, ("当前电压值为： " + stremp1 + " V")); //更新静态文本控件
					pbar->SetPos(temp);//更新进度条的当前位置
					strRXDdata += stremp2;//把新的数据放到全局的字符串里  
					strRXDdata += stremp1;
					strRXDdata += " V\r\n";//字符串加单位V后换行
				}
		}
	}
	SetDlgItemText(IDC_EDIT1, strRXDdata);//更新文本控件的显示

}
