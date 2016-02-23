
// PGPDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PGP.h"
#include "PGPDlg.h"
#include "afxdialogex.h"
#include "AES.h"
#include "MD5.h"
#include "RSA.h"
#include "zip_w.h"//�Զ���ĺ�������������
#include "base64.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern BYTE key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];

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


// CPGPDlg �Ի���



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


// CPGPDlg ��Ϣ�������

BOOL CPGPDlg::OnInitDialog()
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPGPDlg::OnPaint()
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
HCURSOR CPGPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//#include <atlconv.h>



void CPGPDlg::OnBnClickedOk()
{
	string encryptedText_B;//�����Ļ�������Ҫ���뵽ǰ�棬���򱨴�������������
	string decryptedText_B;
	string key_temp;
	key_temp = "1234567890123456";//Ĭ�ϵ�key1234567890123456
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();

	//AES���ܹ���
	CString plaintext_AES;  //�������ַ����ı���
	GetDlgItem(IDC_EDIT1)->GetWindowText(plaintext_AES);  
	string text = "";
	//ת��LPCTSTR��string������MFC�е���c++�еĺ���
	USES_CONVERSION;
	text = W2A(plaintext_AES.GetBuffer(0));
	//MD5����=���õ����ݵ�ɢ��ֵ��================================================================
	string text_MD5;
	text_MD5 = md5_string(text);
	//text += md5_string(text);

	//RSA���̣�����ɢ��ֵ��=======================================================================
	//������Կ�ļ�������İ�ť������
	// RSA �ӽ���
	//priKey_A˽Կ���ܣ���Կ����
	string encryptedText_A = RSAEncryptString(priKey1, seed1, text_MD5.c_str());  // RSA ˽Կ�����壩���ܣ�����֤��
	//string decryptedTex_A = RSADecryptString(pubKey1, encryptedText_A.c_str());  // RSA ����
	text = text + "'" + encryptedText_A;
	//text = compression(text.c_str());
	//AES���̣������������ݣ�=======================================================================
	initKV(key_temp);
	string cipherHex_AES = encrypt(text);
	//ת��string��LPCTSTR������MFC�е���c++�еĺ���
	//GetDlgItem(IDC_EDIT2)->SetWindowText((LPCTSTR)text.c_str()); //�˺������������
	//SetWindowTextA(m_Edit2, (LPCSTR)cipherHex_AES.c_str());

	//RSA���̣�����AES��Կ��==========================================================
	//pubKey_B��Կ���ܣ�˽Կ����
	encryptedText_B = RSAEncryptString(pubKey2, seed2, key_temp.c_str());  // RSA ����
	//decryptedText_B = RSADecryptString(priKey2, encryptedText_B.c_str());  // RSA ����

	//cipherHex_AES += encryptedText_B;//..���Զ�д��
	cipherHex_AES = cipherHex_AES + "''" + encryptedText_B;
	//base64����
	cipherHex_AES = base64_encode(reinterpret_cast<const unsigned char*>(cipherHex_AES.c_str()), cipherHex_AES.length());
	SetWindowTextA(m_Edit2, (LPCSTR)cipherHex_AES.c_str());//cipherHex_AES
	writeCipher(cipherHex_AES);//д���ļ�

	

	
	//���ܹ��̣�����AES����ѹ��������MD5����֤ǩ�����õ�ԭ���ݣ�--------------------------------
	//MessageBox(_T("AES������ɣ����ڿ�ʼ���ܣ�"));
	string text_pla = read();
	text_pla = base64_decode(text_pla);//base64����
	string encryptedText_B_cry = text_pla.substr(text_pla.find("''")+2 , text_pla.length());//ȡ�����ܵ�AES��Կ
	text_pla = text_pla.substr(0, text_pla.find("''"));
	decryptedText_B = RSADecryptString(priKey2, encryptedText_B_cry.c_str());  // RSA ���� �õ�AES
	writeCipher(text_pla);

	initKV(decryptedText_B);
	string text_cry = readCipher();
	//text_cry.find("'");
	//text_cry = uncompression(text_cry.c_str());
	string encryptedText_A_cry = text_cry.substr(text_cry.find("'")+1, text_cry.length());//ȡ�����ܵ�MD5ֵ
	text_cry = text_cry.substr(0, text_cry.find("'"));
    //SetWindowTextA(m_Edit2, (LPCSTR)text_cry.c_str());
	string decryptedTex_A = RSADecryptString(pubKey1, encryptedText_A_cry.c_str());  // RSA��Կ�����壩������֤
	string text_MD5_cry;//����֤��MD5
	text_MD5_cry = md5_string(text_cry);
	if (decryptedTex_A == text_MD5_cry)
	{
		MessageBox(_T("ǩ����֤�ɹ�,�ĵ��ѽ��ܵ����ļ����ڣ�"));
		SetWindowTextA(m_Edit3, (LPCSTR)text_cry.c_str());
	}
	else
		MessageBox(_T("ǩ����֤ʧ�ܣ�"));
	//�ĵ�ʹ�õ���ANSI���룬������Ҫ��SetWindowTextA��ǿ�����ANSI�����������
	//SetWindowTextA(m_Edit2,(LPCSTR)text_cry.c_str());
}

void CPGPDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ���� RSA ��Կ�ԣ��ļ�·������Ϊ�̶�λ��D�̣����λ�����Ǵ����˹���
	strcpy_s(priKey1, "d:\\pri1");  // ���ɵ�˽Կ�ļ�
	strcpy_s(pubKey1, "d:\\pub1");  // ���ɵĹ�Կ�ļ�
	strcpy_s(seed1, "d:\\seed1");
	GenerateRSAKey(1024, pubKey1, priKey1, seed1);
	strcpy_s(priKey2, "d:\\pri2");  // ���ɵ�˽Կ�ļ�
	strcpy_s(pubKey2, "d:\\pub2");  // ���ɵĹ�Կ�ļ�
	strcpy_s(seed2, "d:\\seed2");
	GenerateRSAKey(1024, priKey2, pubKey2, seed2);
}


void CPGPDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//string temp = "123";
	CString plaintext_AES;  //�������ַ����ı���
	GetDlgItem(IDC_EDIT1)->GetWindowText(plaintext_AES);
	string temp = "";
	//ת��LPCTSTR��string������MFC�е���c++�еĺ���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//string temp = "123";
	CString plaintext_AES;  //�������ַ����ı���
	GetDlgItem(IDC_EDIT1)->GetWindowText(plaintext_AES);
	string temp = "";
	//ת��LPCTSTR��string������MFC�е���c++�еĺ���
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
