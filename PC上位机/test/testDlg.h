
// testDlg.h : ͷ�ļ�
//

#pragma once
#include "mscomm1.h"
#include "afxwin.h"


// CtestDlg �Ի���
class CtestDlg : public CDialogEx
{
// ����
public:
	CtestDlg(CWnd* pParent = NULL);	// ��׼���캯��

	//*****************************************************************
	int gllen;//�������ͱ���gllen�����ڼ�¼�������ݵĸ���
	CProgressCtrl * pbar; //ָ���������ָ�룬���ڲ���������
	CString strRXDdata; //�༭����ʾ���ı�����¼����ת��ֵ
	//*****************************************************************


// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
