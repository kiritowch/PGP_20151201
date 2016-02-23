
// PGPDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CPGPDlg 对话框
class CPGPDlg : public CDialogEx
{
// 构造
public:
	CPGPDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PGP_DIALOG };
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
