#pragma once
#include "MyString.h"
#include "MyUtility.h"
#include "AccessDbDriver.h"
class CustSetup
{
public:

	CustSetup()
	{
		m_dbAddr = CMyUtility::GetModuleFolder() + L"MyCanDb.accdb";
	}

	~CustSetup()
	{
	}


	void InsertUserButtonDef(CString prj, CString cap, CString msg, CString id, CString sig, CString val)
	{
		CString sql;
		sql.Format(L"insert into VIEW_BUTTONS(prj,caption,msg,msgid,sig,sigval) values('%s','%s','%s','%s','%s','%s')", prj, cap, msg, id, sig, val);

		AccessDbDriver adb;
		adb.OpenDb(m_dbAddr);
		adb.Insertdb(sql, L"VIEW_BUTTONS", L"caption", cap, L"prj", prj);
		adb.CloseDb();
	}

	void GetSavedUserButtonDef(CString prj, std::vector<std::vector<CString>>& rec)
	{
		CString sql;
		sql.Format(L"select caption,msg,msgid,sig,sigval from VIEW_BUTTONS where prj like '%s'",prj);

		AccessDbDriver adb;
		adb.OpenDb(m_dbAddr);
		adb.ReadDb(sql, rec);
		adb.CloseDb();
	}

private:
	CString m_dbAddr;
};

