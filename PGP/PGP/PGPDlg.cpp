
// PGPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PGP.h"
#include "PGPDlg.h"
#include "afxdialogex.h"
#include "AES.h"
#include "MD5.h"
#include "RSA.h"
#include "zip_w.h"//自定义的函数，声明函数
#include "base64.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern BYTE key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];

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


// CPGPDlg 对话框



CPGPDlg::CPGPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PGP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPGPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_EDIT3, m_Edit3);
}

BEGIN_MESSAGE_MAP(CPGPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CPGPDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CPGPDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPGPDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPGPDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CPGPDlg 消息处理程序

BOOL CPGPDlg::OnInitDialog()
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

void CPGPDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPGPDlg::OnPaint()
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
HCURSOR CPGPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//#include <atlconv.h>



void CPGPDlg::OnBnClickedOk()
{
	string encryptedText_B;//代码多的话，变量要申请到前面，否则报错。。。。。。坑
	string decryptedText_B;
	string key_temp;
	key_temp = "1234567890123456";//默认的key1234567890123456
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();

	//AES加密过程
	CString plaintext_AES;  //待接受字符串的变量
	GetDlgItem(IDC_EDIT1)->GetWindowText(plaintext_AES);  
	string text = "";
	//转换LPCTSTR到string，便于MFC中调用c++中的函数
	USES_CONVERSION;
	text = W2A(plaintext_AES.GetBuffer(0));
	//MD5过程=（得到数据的散列值）================================================================
	string text_MD5;
	text_MD5 = md5_string(text);
	//text += md5_string(text);

	//RSA过程（加密散列值）=======================================================================
	//两对秘钥文件在下面的按钮处产生
	// RSA 加解密
	//priKey_A私钥加密，公钥解密
	string encryptedText_A = RSAEncryptString(priKey1, seed1, text_MD5.c_str());  // RSA 私钥（名义）加密，作认证用
	//string decryptedTex_A = RSADecryptString(pubKey1, encryptedText_A.c_str());  // RSA 解密
	text = text + "'" + encryptedText_A;
	//text = compression(text.c_str());
	//AES过程（加密整个数据）=======================================================================
	initKV(key_temp);
	string cipherHex_AES = encrypt(text);
	//转换string到LPCTSTR，便于MFC中调用c++中的函数
	//GetDlgItem(IDC_EDIT2)->SetWindowText((LPCTSTR)text.c_str()); //此函数仍输出乱码
	//SetWindowTextA(m_Edit2, (LPCSTR)cipherHex_AES.c_str());

	//RSA过程（加密AES秘钥）==========================================================
	//pubKey_B公钥加密，私钥解密
	encryptedText_B = RSAEncryptString(pubKey2, seed2, key_temp.c_str());  // RSA 加密
	//decryptedText_B = RSADecryptString(priKey2, encryptedText_B.c_str());  // RSA 解密

	//cipherHex_AES += encryptedText_B;//..已自动写入
	cipherHex_AES = cipherHex_AES + "''" + encryptedText_B;
	//base64编码
	cipherHex_AES = base64_encode(reinterpret_cast<const unsigned char*>(cipherHex_AES.c_str()), cipherHex_AES.length());
	SetWindowTextA(m_Edit2, (LPCSTR)cipherHex_AES.c_str());//cipherHex_AES
	writeCipher(cipherHex_AES);//写入文件

	

	
	//解密过程（解密AES，解压缩，解密MD5，验证签名，得到原数据）--------------------------------
	//MessageBox(_T("AES加密完成，现在开始解密！"));
	string text_pla = read();
	text_pla = base64_decode(text_pla);//base64解码
	string encryptedText_B_cry = text_pla.substr(text_pla.find("''")+2 , text_pla.length());//取到加密的AES密钥
	text_pla = text_pla.substr(0, text_pla.find("''"));
	decryptedText_B = RSADecryptString(priKey2, encryptedText_B_cry.c_str());  // RSA 解密 得到AES
	writeCipher(text_pla);

	initKV(decryptedText_B);
	string text_cry = readCipher();
	//text_cry.find("'");
	//text_cry = uncompression(text_cry.c_str());
	string encryptedText_A_cry = text_cry.substr(text_cry.find("'")+1, text_cry.length());//取到加密的MD5值
	text_cry = text_cry.substr(0, text_cry.find("'"));
    //SetWindowTextA(m_Edit2, (LPCSTR)text_cry.c_str());
	string decryptedTex_A = RSADecryptString(pubKey1, encryptedText_A_cry.c_str());  // RSA公钥（名义）解密验证
	string text_MD5_cry;//新验证的MD5
	text_MD5_cry = md5_string(text_cry);
	if (decryptedTex_A == text_MD5_cry)
	{
		MessageBox(_T("签名验证成功,文档已解密到本文件夹内！"));
		SetWindowTextA(m_Edit3, (LPCSTR)text_cry.c_str());
	}
	else
		MessageBox(_T("签名验证失败！"));
	//文档使用的是ANSI编码，这里需要用SetWindowTextA，强制输出ANSI，否则会乱码
	//SetWindowTextA(m_Edit2,(LPCSTR)text_cry.c_str());
}

void CPGPDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	// 生成 RSA 密钥对，文件路径设置为固定位置D盘，相对位置老是错，日了狗了
	strcpy_s(priKey1, "d:\\pri1");  // 生成的私钥文件
	strcpy_s(pubKey1, "d:\\pub1");  // 生成的公钥文件
	strcpy_s(seed1, "d:\\seed1");
	GenerateRSAKey(1024, pubKey1, priKey1, seed1);
	strcpy_s(priKey2, "d:\\pri2");  // 生成的私钥文件
	strcpy_s(pubKey2, "d:\\pub2");  // 生成的公钥文件
	strcpy_s(seed2, "d:\\seed2");
	GenerateRSAKey(1024, priKey2, pubKey2, seed2);
}


void CPGPDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//string temp = "123";
	CString plaintext_AES;  //待接受字符串的变量
	GetDlgItem(IDC_EDIT1)->GetWindowText(plaintext_AES);
	string temp = "";
	//转换LPCTSTR到string，便于MFC中调用c++中的函数
	USES_CONVERSION;
	temp = W2A(plaintext_AES.GetBuffer(0));
	string temp_1;
	string temp_2;
	temp_1 = compression(temp.c_str());
	temp_2 = uncompression(temp_1.c_str());
//	temp_2 = uncompression(temp_1);
	MessageBox((LPCTSTR)temp.c_str());
	MessageBox((LPCTSTR)temp_1.c_str());
	MessageBox((LPCTSTR)temp_2.c_str());
	SetWindowTextA(m_Edit2, (LPCSTR)temp.c_str());
	SetWindowTextA(m_Edit2, (LPCSTR)temp_1.c_str());
	SetWindowTextA(m_Edit3, (LPCSTR)temp_2.c_str());
}


void CPGPDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	//string temp = "123";
	CString plaintext_AES;  //待接受字符串的变量
	GetDlgItem(IDC_EDIT1)->GetWindowText(plaintext_AES);
	string temp = "";
	//转换LPCTSTR到string，便于MFC中调用c++中的函数
	USES_CONVERSION;
	temp = W2A(plaintext_AES.GetBuffer(0));
	string temp_1;
	string temp_2;
	temp_1 = base64_encode(reinterpret_cast<const unsigned char*>(temp.c_str()), temp.length());
	temp_2 = base64_decode(temp_1);
	//	temp_2 = uncompression(temp_1);
	MessageBox((LPCTSTR)temp.c_str());
	MessageBox((LPCTSTR)temp_1.c_str());
	MessageBox((LPCTSTR)temp_2.c_str());
	SetWindowTextA(m_Edit2, (LPCSTR)temp.c_str());
	SetWindowTextA(m_Edit2, (LPCSTR)temp_1.c_str());
	SetWindowTextA(m_Edit3, (LPCSTR)temp_2.c_str());
}
