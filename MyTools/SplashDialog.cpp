// SplashDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyTools.h"
#include "SplashDialog.h"
#include "afxdialogex.h"
#include "common.h"
#include <Wininet.h>
#pragma comment(lib,"Version.lib")
// CSplashDialog �Ի���

IMPLEMENT_DYNAMIC(CSplashDialog, CDialogEx)

CSplashDialog::CSplashDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSplashDialog::IDD, pParent)
{
	HMODULE hModule = ::GetModuleHandle(NULL);
	ASSERT(hModule != 0);
	TCHAR path[MAX_PATH];
	VERIFY(::GetModuleFileName(hModule, path, MAX_PATH));
	CString executable = path;
	//AfxMessageBox(executable);
	CString exeName = executable.Mid(1 + executable.ReverseFind(_T('\\')));
}

CSplashDialog::~CSplashDialog()
{
}

void CSplashDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TXT, m_info);
}


BEGIN_MESSAGE_MAP(CSplashDialog, CDialogEx)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSplashDialog ��Ϣ�������


void CSplashDialog::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);

	switch (nIDEvent)
	{
	case TIMER_MINE_SPLASH_WND: // �ر�splash dlg
		this->GetParent()->PostMessageW(WM_MIAN_DLG_RESOTRE);
		break;
	default:
		break;
	}


	CDialogEx::OnTimer(nIDEvent);
}


BOOL CSplashDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetTimer(TIMER_MINE_SPLASH_WND, 1000, NULL);

	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP1);
	m_brBk.CreatePatternBrush(&bmp);
	bmp.DeleteObject();

	m_info.SetWindowText(L"Loading ...");

	return TRUE;  // return TRUE unless you set the focus to a control
}


HBRUSH CSplashDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd == this)
	{
		return m_brBk;
	}
	if (CTLCOLOR_STATIC == nCtlColor)  //��̬�ı��ؼ�
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,255)); //����������ɫ
		
		//HBRUSH B = CreateSolidBrush(RGB(0, 0, 255)); //������ˢ
		//return (HBRUSH)B;
	}

	if (CTLCOLOR_BTN == nCtlColor) //��ť�ؼ�
	{
		pDC->SetBkMode(TRANSPARENT);
		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return hbr;
}
