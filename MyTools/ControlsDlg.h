#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "CanLogDialog.h"
#include "ComLogDialog.h"
// CControlsDlg 对话框

class CControlsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlsDlg)

public:
	CControlsDlg(CComLogDialog* pcom = NULL, CCanLogDialog *pcan = NULL, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CControlsDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONTROLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_edit_value_can_radar_level_fl;
	afx_msg void OnDeltaposSpinCanRadarLevelFl(NMHDR *pNMHDR, LRESULT *pResult);
	void _SetRadarLevelString(int which, int add);
	CString m_edit_value_can_radar_level_fml;
	CString m_edit_value_can_radar_level_fmr;
	CString m_edit_value_can_radar_level_fr;
	CString m_edit_value_can_radar_level_rl;
	CString m_edit_value_can_radar_level_rml;
	CString m_edit_value_can_radar_level_rmr;
	CString m_edit_value_can_radar_level_rr;
	afx_msg void OnDeltaposSpinCanRadarLevelFml(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCanRadarLevelFmr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCanRadarLevelFr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCanRadarLevelRl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCanRadarLevelRml(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCanRadarLevelRmr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCanRadarLevelRr(NMHDR *pNMHDR, LRESULT *pResult);
	int _InitRadarLevelString();
	virtual BOOL OnInitDialog(); CComboBox m_cbox_radar_number;
	CComboBox m_cbox_radar_min;
	CString m_edit_value_can_angle;
	CString m_edit_value_can_speed;
	afx_msg void OnBnClickedRadioCanIgn();
	afx_msg void OnBnClickedRadioCanLamp();
	afx_msg void OnBnClickedRadioCanRervse();
	afx_msg void OnBnClickedRadioCanScreen();
	afx_msg void OnBnClickedRadioCan3dside();
	afx_msg void OnBnClickedRadioCanPdc();
	afx_msg void OnBnClickedRadioCanTraj();
	afx_msg void OnBnClickedButtonCanCalib();
	afx_msg void OnBnClickedButtonCanAvm();
	afx_msg void OnEnChangeEditCanSpeed();
	afx_msg void OnEnChangeEditCanAngle();
	afx_msg void OnCbnSelchangeComboCanRadarNumber();
	afx_msg void OnCbnSelchangeComboCanRadarMin();
	afx_msg void OnEnChangeEditcanRadarLevelFl();
	afx_msg void OnEnChangeEditcanRadarLevelFml();
	afx_msg void OnEnChangeEditcanRadarLevelFmr();
	afx_msg void OnEnChangeEditcanRadarLevelFr();
	afx_msg void OnEnChangeEditcanRadarLevelRl();
	afx_msg void OnEnChangeEditcanRadarLevelRml();
	afx_msg void OnEnChangeEditcanRadarLevelRmr();
	afx_msg void OnEnChangeEditcanRadarLevelRr();
	afx_msg void OnBnClickedRadioCanLdw();
	afx_msg void OnBnClickedRadioCanLdwSen();
	afx_msg void OnBnClickedCheckCanRadarErrFl();
	afx_msg void OnBnClickedCheckCanRadarErrFml();
	afx_msg void OnBnClickedCheckCanRadarErrRl();
	afx_msg void OnBnClickedCheckCanRadarErrRml();
	afx_msg void OnBnClickedCheckCanRadarErrFmr();
	afx_msg void OnBnClickedCheckCanRadarRmr();
	afx_msg void OnBnClickedCheckCanRadarErrFr();
	afx_msg void OnBnClickedCheckCanRadarRr();
	afx_msg void OnBnClickedCheckCanRadarErrSys();
	CSliderCtrl m_slider_can_speed;
	CSliderCtrl m_slider_can_angle;
	afx_msg void OnBnClickedButtonComEnvSetup();
	CComLogDialog *m_pComLogDlg;
	CCanLogDialog *m_pCanLogDlg;
	afx_msg void OnBnClickedButtonComReboot();
	afx_msg void OnBnClickedButtonComCpbin();
	afx_msg void OnBnClickedButtonComCplib();
	afx_msg void OnBnClickedButtonComTarsvp();
	afx_msg void OnBnClickedButtonComErasecalib();
	afx_msg void OnBnClickedButtonComEraseCache();
	afx_msg void OnBnClickedButtonComEraseData();
	afx_msg void OnBnClickedButtonComEraseNor();
	afx_msg void OnBnClickedButtonComNfs();
	afx_msg void OnBnClickedButtonComNfssvp();
	afx_msg void OnBnClickedButtonComCpCalibtousb();
	afx_msg void OnBnClickedButtonComCpCalibtousb3();
	CComboBox m_cbox_can_color;
	bool m_bInited;
	afx_msg void OnCbnSelchangeComboCanColor();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnOK();
	afx_msg void OnNMCustomdrawSliderCanSpeed(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_edit_canSpd;
	afx_msg void OnNMCustomdrawSliderCanAngle(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_edit_canAngle;

	CSliderCtrl m_slider_src_x;
	CSliderCtrl m_slider_src_y;
	CEdit m_edit_src_x;
	CEdit m_edit_src_y;
	afx_msg void OnBnClickedButtonCanScrn();
	afx_msg void OnNMCustomdrawSliderCanScrnX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderCanScrnY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCapture();
	afx_msg void OnBnClickedButtonCanDvr();
};
