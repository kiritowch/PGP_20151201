
// PGPDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CPGPDlg �Ի���
class CPGPDlg : public CDialogEx
{
// ����
public:
	CPGPDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PGP_DIALOG };
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
	afx_msg void OnBnClickedOk();
	CEdit m_Edit2;
	afx_msg void OnBnClickedButton1();
	CEdit m_Edit1;

	char priKey1[128] = { 0 };
	char pubKey1[128] = { 0 };
	char priKey2[128] = { 0 };
	char pubKey2[128] = { 0 };
	char seed2[1024] = { 0 };
	char seed1[1024] = { 0 };

	CEdit m_Edit3;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
