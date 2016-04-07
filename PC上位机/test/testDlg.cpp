
// testDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CtestDlg �Ի���



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


// CtestDlg ��Ϣ�������

BOOL CtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CtestDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtestDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CtestDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
BEGIN_EVENTSINK_MAP(CtestDlg, CDialogEx)
	ON_EVENT(CtestDlg, IDC_MSCOMM1, 1, CtestDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()


void CtestDlg::OnCommMscomm1()
{
	VARIANT variant1;//����VARIANT�ͱ��������ڴ�Ž��յ�������
	COleSafeArray safearray;//����safearray�ͱ���
	LONG len, k;//���峤���ͱ���len,k
	BYTE rxdata[2048];//����BYTE������
	CString stremp1, stremp2;//���������ַ���
	if (m_comm1.get_CommEvent() == 2) //�ж�����OnCommʱ���ԭ��
	{//����ǽ��յ��ض����ֽ��������ȡ���յ�������
		variant1 = m_comm1.GetInput();//�ѽ��յ������ݴ�ŵ�VARIANT�ͱ�����
		safearray = variant1;//VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���
		len = safearray.GetOneDimSize();
		for (k = 0;k<len;k++)
		{
			safearray.GetElement(&k, rxdata + k); //�õ��ӽ��յ������ݷŵ�BYTE������rxdata��
		}
		for (k = 0;k<len;k++)
		{
			BYTE bt = (*(unsigned char*)(rxdata + k)); //��ȡADת���ĸ��ֽ�
			if ((k % 2) == 0)
				if ((k + 1)<len)
				{
					gllen++;//ȫ�ֵı������Խ��յ���ת������ĸ������м���
					stremp2.Format("��%d��ת�������", gllen);//��ʾ�ڼ���ת��
					int temp = bt * 4 + ((*(unsigned char *)(rxdata + k + 1)) >> 6); //�ߵ��ֽںϲ���ʵ�ʵ�ת�������ע��ת������������
					stremp1.Format("%2.2f", (2.56*temp / 1024));//�����ʵ�ʵ�ѹֵ
					SetDlgItemText(IDC_STATIC, ("��ǰ��ѹֵΪ�� " + stremp1 + " V")); //���¾�̬�ı��ؼ�
					pbar->SetPos(temp);//���½������ĵ�ǰλ��
					strRXDdata += stremp2;//���µ����ݷŵ�ȫ�ֵ��ַ�����  
					strRXDdata += stremp1;
					strRXDdata += " V\r\n";//�ַ����ӵ�λV����
				}
		}
	}
	SetDlgItemText(IDC_EDIT1, strRXDdata);//�����ı��ؼ�����ʾ

}
