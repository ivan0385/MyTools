#pragma once
#include "IComLogView.h"
#include "ComLogPresenter.h"
#include "afxwin.h"
#include "resource.h"
#include "RichEditCtrlEx.h"
#include "atltypes.h"
// CComLogDialog 对话框

class CComLogDialog : public CDialogEx, IComLogView
{
	DECLARE_DYNAMIC(CComLogDialog)

public:
	CComLogDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComLogDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_COM_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()



public:
	virtual bool SetRcvMsg(const unsigned char * msg, int size);
	virtual bool SetRcvMsg(CString msg);
	virtual BOOL OnInitDialog();
private:
	ComLogPresenter* m_pComLogPresenter;
public:
	afx_msg void OnBnClickedButtonComTest();
	void SendData(CString msg);
	CRichEditCtrlEx m_editComLog;
	CEdit m_edit_com_snd1;
	CEdit m_edit_com_snd2;
	CEdit m_edit_com_snd3;
	CEdit m_edit_com_snd4;
	CEdit m_edit_com_snd5;
	CEdit m_edit_com_snd6;
	CEdit m_edit_com_snd7;
	CEdit m_edit_com_snd8;
	CEdit m_edit_com_snd9;
	CEdit m_edit_com_snd10;
	CEdit m_edit_com_snd11;
	CEdit m_edit_com_snd12;
	CEdit m_edit_com_snd13;
	CEdit m_edit_com_snd14;
	CEdit m_edit_com_snd15;
	CEdit m_edit_com_snd16;
	CEdit m_edit_com_snd17;
	CEdit m_edit_com_snd18;
	afx_msg void OnBnClickedButtonF1();
	afx_msg void OnBnClickedButtonF2();
	afx_msg void OnBnClickedButtonF3();
	afx_msg void OnBnClickedButtonF4();
	afx_msg void OnBnClickedButtonF5();
	afx_msg void OnBnClickedButtonF6();
	afx_msg void OnBnClickedButtonF7();
	afx_msg void OnBnClickedButtonF8();
	afx_msg void OnBnClickedButtonF9();
	afx_msg void OnBnClickedButtonF10();
	afx_msg void OnBnClickedButtonF11();
	afx_msg void OnBnClickedButtonF12();
	afx_msg void OnBnClickedButtonF13();
	afx_msg void OnBnClickedButtonF14();
	afx_msg void OnBnClickedButtonF15();
	afx_msg void OnBnClickedButtonF16();
	afx_msg void OnBnClickedButtonF17();
	afx_msg void OnBnClickedButtonF18();


private:
	void ButtonsClick(int id);

public:
	CRichEditCtrlEx m_editComSnd;
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	std::list<CString> m_ComMsgSndList;
	unsigned int m_IdxOfComMsgSnd;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CRect m_rect;
	void ChangeSize(UINT nID, int x, int y);
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButtonComLogClean();
	afx_msg void OnBnClickedButtonComLogCopy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_bPlatform;
	afx_msg void OnBnClickedButtonComPause();
	bool m_bRcv;
	afx_msg void OnClose();
};
