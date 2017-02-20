#pragma once

#include "stdafx.h"
#include <afxdb.h>
class AccessDbDriver
{
public:

	AccessDbDriver(void)
	{
	}

	~AccessDbDriver(void)
	{
	}


	CDatabase m_db;
	void OpenDb(CString addr)
	{
		BOOL result = TRUE;
		CString lpszConnect = L"Driver={Microsoft Access Driver (*.mdb, *.accdb)};DSN='';DBQ=" + addr+L"; PWD=aa; ";
		//AfxMessageBox(lpszConnect);
		try
		{
			result = m_db.OpenEx(lpszConnect, CDatabase::noOdbcDialog);
		}
		catch (CDBException * e)
		{
			AfxMessageBox(e->m_strError);
		}
	}

	bool ReadDb(CString query, std::vector<std::vector<CString>> &output)
	{
		BOOL result = TRUE;
		CDBVariant var;
		CString value;
		
		output.clear();

		CRecordset rs(&m_db);
		try
		{
			result = rs.Open(CRecordset::dynaset, query, CRecordset::none);
			if (TRUE == result)
			{
				CODBCFieldInfo fInfo;
				short sFieldCount = rs.GetODBCFieldCount();
				int rowCount = 0;
				while (!rs.IsEOF())
				{
					std::vector<CString> row;

					for (short column = 0; column < sFieldCount; column++)
					{
						rs.GetFieldValue(column, var);
						switch (var.m_dwType)
						{
						case DBVT_BOOL:
							value.Format(_T("%d"), var.m_boolVal);
							break;
						case DBVT_UCHAR:
							value.Format(_T("%d"), var.m_chVal);
							break;
						case DBVT_SHORT:
							value.Format(_T("%d"), var.m_iVal);
							break;
						case DBVT_LONG:
							value.Format(_T("%ld"), var.m_lVal);
							break;
						case DBVT_DOUBLE:
							value.Format(_T("%f"), var.m_dblVal);
							break;
						case DBVT_SINGLE:
							value.Format(_T("%f"), var.m_fltVal);
							break;
						case DBVT_STRING:
							value.Format(_T("%s"), var.m_pstring->GetBuffer(var.m_pstring->GetLength()));
							break;
						case DBVT_ASTRING:
							value.Format(_T("%s"), var.m_pstringA->GetBuffer(var.m_pstringA->GetLength()));
							break;
						case DBVT_WSTRING:
							value.Format(_T("%s"), var.m_pstringW->GetBuffer(var.m_pstringW->GetLength()));
							break;
						case DBVT_BINARY:
							//value.Format(_T("%d"), var.m_pbinary);
							break;
						case DBVT_DATE:
							value.Format(_T("%d-%d-%d %d:%d:%d"), var.m_pdate->year, var.m_pdate->month, var.m_pdate->day, var.m_pdate->hour, var.m_pdate->minute, var.m_pdate->second);
							break;
						default:
							value = _T("");
						}
						row.push_back(value);
					}
					rowCount++;
					rs.MoveNext();
					output.push_back(row);
				}
			}
		}
		catch (CDBException * e)
		{
			AfxMessageBox(e->m_strError);
			return false;
		}

		return true;
	}

	
	bool ExcuteSql(CString &sql)
	{
		try
		{
			m_db.ExecuteSQL(sql);
		}
		catch (CDBException *e)
		{
			AfxMessageBox(e->m_strError);
			return false;
		}
		return true;
	}

	bool Insertdb(CString &sql, CString table, CString keyname, CString & key, CString keyname1, unsigned int key1)
	{
		CString del;
		del.Format(L"delete from %s where %s like '%s' and %s=%d", table, keyname,key,keyname1,key1);
		//AfxMessageBox(del);
		ExcuteSql(del);

		//AfxMessageBox(sql);
		return ExcuteSql(sql);
	}

	bool Insertdb(CString &sql, CString table, CString keyname, CString & key, CString keyname1, CString & key1)
	{
		CString del;
		del.Format(L"delete from %s where %s like '%s' and %s like '%s'", table, keyname, key, keyname1, key1);
		//AfxMessageBox(del);
		ExcuteSql(del);

		//AfxMessageBox(sql);
		return ExcuteSql(sql);
	}

	bool Insertdb(CString &sql, CString table, CString keyname, CString & key)
	{
		CString del;
		del.Format(L"delete from %s where %s like '%s' ", table, keyname, key);
		//AfxMessageBox(del);
		ExcuteSql(del);

		//AfxMessageBox(sql);
		return ExcuteSql(sql);
	}

	bool Insertdb(CString &sql, CString table, CString keyname, unsigned int key, CString keyname1, unsigned int key1)
	{
		CString del;
		del.Format(L"delete from %s where %s=%d and %s=%d", table, keyname, key, keyname1, key1);

		//AfxMessageBox(del);
		ExcuteSql(del);

		//AfxMessageBox(sql);
		return ExcuteSql(sql);
	}

	void CloseDb()
	{
		try
		{
			m_db.Close();
		}
		catch (CDBException * e)
		{
			AfxMessageBox(e->m_strError);
		}
	}


};

