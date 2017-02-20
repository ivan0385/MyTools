
// MyToolsDlg.h : ͷ�ļ�
//

#pragma once
#include "afxext.h"
#include "common.h"
#include "atltypes.h"
#include "afxcmn.h"
#include "afxbutton.h"
#include "afxwin.h"

// CMyToolsDlg �Ի���
class CMyToolsDlg : public CDialogEx
{
// ����
public:
	CMyToolsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYTOOLS_DIALOG };

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
	CToolBar m_MainToolBar; 
	CImageList m_ilToolBar;//ʹ������ͼ����ʾ256ɫ

	void HideMainWnd(void);
	void RestoreMainWnd(void);

	afx_msg BOOL OnToolTipText(UINT NID, NMHDR* pNMHDR, LRESULT* pResult);//ʹ��������ʾ��ʾ��Ϣ
private:
	CSplashDialog *m_splashDlg;
protected:
	afx_msg LRESULT OnMianDlgResotre(WPARAM wParam, LPARAM lParam);




////////////////////////dialogs///////////////////////////
public:
	CComSettingDialog *m_pComSettingDlg;	
	CComLogDialog *m_pComLogDlg;
	void InitAllDialogs();
	int TestFunc();
	afx_msg void OnToolbarButtonComSetting();
	afx_msg void OnToolbarButtonComconnect();
	afx_msg void OnMenuDeviceCom();
	afx_msg void OnMenuDeviceCan();
	afx_msg void OnMenuViewCom();
	afx_msg void OnMenuViewCan();
	void DeleteAllDialogs();
	virtual BOOL DestroyWindow();
	CRect m_rect;
	CTabCtrl m_tab_main;
	void InitControls();
	CControlsDlg *m_pControlsDlg;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ChangeSize(UINT nID, int x, int y);
	CRect m_initWinRect;
	CCanSettingDialog *m_pCanSettingDlg;
	CCanLogDialog *m_pCanLogDlg;
	afx_msg void OnToolbarButtonComdisconnect();
	CCanDbcSettingDialog* m_pCanDbcSettingDlg;
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	int m_mainTabSelInx;
	afx_msg void OnDbcOpen();
	afx_msg void OnButtonToolCansetting();
	afx_msg void OnButtonCanconnect();
	afx_msg void OnButtonCandisconnect();
	//CMFCButton m_btn_can_conn;
	//CMFCButton m_btn_com_conn;
	afx_msg void OnBnClickedMfcbuttonComConn();
	afx_msg void OnBnClickedMfcbuttonComLog();
	afx_msg void OnBnClickedMfcbuttonCanConn();
	afx_msg void OnBnClickedMfcbuttonCanLog();
	CMFCButton m_btn_com_conn;
	CMFCButton m_btn_can_conn;
	CComboBox m_avtive_candb;
	afx_msg void OnCbnSelchangedComboActiveCan();
	void SetMainTabSel(int idx);
	void SetMainTabVisible(bool vis);
private:
	std::vector<CString> m_prjList;
protected:
	afx_msg LRESULT OnMineMainComConnect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMineMainComDisconnect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMineMainCanConnect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMineMainCanDisconnect(WPARAM wParam, LPARAM lParam);
public:
	CCanButtonsSetUpDialog *m_pCanBtnSetupDlg;
	afx_msg void OnNMClickTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_CurSelPrj;
	CEolTestDialog *m_pEolDlg;
};
