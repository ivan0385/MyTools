#ifndef __COMMM__EVENT___
#define __COMMM__EVENT___
#include <winuser.h>



#if 1 // dialogs
#include "ComLogDialog.h"
#include "SplashDialog.h"
#include "ComSettingDialog.h"
#include "ControlsDlg.h"
#include "CanSettingDialog.h"
#include "CanLogDialog.h"
#include "CanDbcSettingDialog.h"
#include "CanButtonsSetUpDialog.h"
#include "EolTestDialog.h"
#endif

#include "AccessDbDriver.h"

#define CB_PROPERTY(varType, varName, varValue)\
public: virtual int get##varName(void){return m_; \
public: virtual void set##varName(varType varValue);



enum
{
	TIMER_MINE_TEST = 100,
	TIMER_MINE_CAN_SND_CYCLE_TEST,
	TIMER_MINE_CAN_CONTROL_CYCLE_QUERY,
	TIMER_MINE_CAN_CAPTURE_SCREEN,
	TIMER_MINE_SPLASH_WND,
	TIMER_MINE_COM_LOGIN_A6,
	TIMER_MINE_COM_LOGIN_J6,
	TIMER_MINE_EOL_STEP_IN,
	TIMER_MINE_EOL_STEP_CHECKING,
	TIMER_MINE_EOL_STEP_TIMEOUT,
};

// 
#define	WM_MIAN_DLG_RESOTRE		WM_USER + 100
enum
{
	WM_MINE_MAIN_COM_CONNECT=WM_MIAN_DLG_RESOTRE+1,
	WM_MINE_MAIN_COM_DISCONNECT,
	WM_MINE_MAIN_CAN_CONNECT,
	WM_MINE_MAIN_CAN_DISCONNECT,
	WM_MINE_CANLOG_RCV_MSG,
	WM_MINE_CANLOG_SND_MSG,
};


#ifdef _DEBUG
#define debug_out(x) printf##x
#else
#define debug_out(x) 
#endif

#endif