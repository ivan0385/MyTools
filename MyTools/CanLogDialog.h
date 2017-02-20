#pragma once

#include "ICanLogView.h"
#include "CanLogPresenter.h"
#include "afxcmn.h"
#include "afxtempl.h"
#include "afxwin.h"
#include "CustUtils.h"
// CCanLogDialog 对话框

class CCanLogDialog : public CDialogEx, ICanLogView
{
	DECLARE_DYNAMIC(CCanLogDialog)

public:
	CCanLogDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCanLogDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_CAN_BUS_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	virtual bool SetRcvMsg(const VCI_CAN_OBJ * msg, int size);
	virtual bool SetSendMsg(const VCI_CAN_OBJ * ba, int cb);

	DECLARE_MESSAGE_MAP()


private:
	CanLogPresenter * m_pCanLogPresenter;
public:
	void SendData(CString msg, int value, bool bsend = true);
	void SetCurPrj(CString vallue);
	virtual void OnOK();
	CListCtrl m_list_can_snd;
	CListCtrl m_list_can_rcv;
	virtual BOOL OnInitDialog();
private:
	CList<VCI_CAN_OBJ> m_rcvBuffer;
	CList<VCI_CAN_OBJ> m_sndBuffer;
	int m_value;
	c_critical_locker m_locker;
	c_critical_locker m_sndlocker;
protected:
	afx_msg LRESULT OnMineCanlogRcvMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMineCanlogSndMsg(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonCanlogSnd();
	afx_msg void OnBnClickedButtonCanlogSndstop();
	afx_msg void OnBnClickedButtonCanlogRefresh();
	afx_msg void OnBnClickedButtonCanLogStart();
	afx_msg void OnBnClickedButtonCanlogStop();
	CEdit m_editCanLogMsgData;
	CEdit m_editCanLogMsgID;
	CEdit m_editCanLogMsgCycle;
	CEdit m_editCanLogSigV1;
	CEdit m_editCanLogSigV2;
	CEdit m_editCanLogSigTestCycle;
	CComboBox m_cboxCanLogMsg;
	CComboBox m_cboxCanLogSig;
	afx_msg void OnCbnSelchangeComboCanlogMsg();
	CEdit m_editCanLogSndMsgId;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	bool GetAvmVideoOnOffStatus();
	bool GetPdcOnOff();
	bool GetSideViewOnOff();
	bool GetLdwOnOff();
	int GetLdwSen();
	afx_msg void OnBnClickedButtonCln1();
	afx_msg void OnBnClickedButtonCln2();
	afx_msg void OnClose();
	CEdit m_editCanLogSigStep;
	int m_valueStep;
};
