#pragma once
#include "IComLogView.h"
#include "EolTestPresenter.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "afxbutton.h"

// CEolTestDialog 对话框

class CEolTestDialog : public CDialogEx, IComLogView
{
	DECLARE_DYNAMIC(CEolTestDialog)

public:
	CEolTestDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEolTestDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_EOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual bool SetRcvMsg(CString msg);
	virtual bool SetRcvMsg(const unsigned char * msg, int size);
	virtual bool SetSndMsg(CString msg);
private:
	EolTestPresenter* m_pPresenter;
	unsigned int m_TestStep;
	std::vector<CString> m_msglist;

	CFont * m_pFont;

	typedef struct
	{
		CString testName;
		CString sndCmd;
		CString checkCmd;
		int bNeedStr; // -1:just go, 0:status,1:hex,2:ascii
		int waitTime;
		int stillTime;
	}MY_EOL_TC;

	std::vector<MY_EOL_TC> tcList;

	bool findStr(CString &dest, CString &which);
	void _NextStep();
	void _StartStep(bool bReset);
public:
	CEdit m_edit_log;
	virtual BOOL OnInitDialog();
	CProgressCtrl m_progress;
	CListCtrl m_list;
	CEdit m_edit_cur;
	CEdit m_edit_result;
	afx_msg void OnBnClickedButtonEolStart();
	afx_msg void OnBnClickedButtonEolPause();
	afx_msg void OnBnClickedButtonEolStop();
	void initTcList();
	int ssut(CString Search_STR, long Search_Time, long delay_time, CString &output);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
	bool m_bRuning;
	CEdit m_edit_snd;
	CMFCButton m_btn_eol_snd;
	afx_msg void OnBnClickedMfcbuttonEolSnd();
	afx_msg void OnEnSetfocusEditEolSnd();
};
