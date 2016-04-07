
// testDlg.h : 头文件
//

#pragma once
#include "mscomm1.h"
#include "afxwin.h"


// CtestDlg 对话框
class CtestDlg : public CDialogEx
{
// 构造
public:
	CtestDlg(CWnd* pParent = NULL);	// 标准构造函数

	//*****************************************************************
	int gllen;//定义整型标量gllen，用于记录接收数据的个数
	CProgressCtrl * pbar; //指向进度条的指针，用于操作进度条
	CString strRXDdata; //编辑框显示的文本，记录历次转换值
	//*****************************************************************


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnEnChangeEdit1();
	CMscomm1 m_comm1;
	CButton m_serial;
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
};
