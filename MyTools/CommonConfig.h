#pragma once

#include "stdafx.h"
#include <map>
#include "MyUtility.h"
#include "MyString.h"


#define FILE_NAME L"common.ini"

class CCommonConfig
{
public:

	CCommonConfig()
	{
	}

	~CCommonConfig()
	{
	}
private:
	std::map<CString, CString> m_cfg_map;
	static CCommonConfig* pCommConfig;


public:
	////////////////// interface////////////////////////
	static CCommonConfig* GetInstance()
	{
		if (pCommConfig == NULL)
			pCommConfig = new CCommonConfig;
		return pCommConfig;
	}

	bool addKey(CString key, CString value)
	{
		std::map<CString, CString >::iterator l_it;
		l_it = m_cfg_map.find(key);
		if (l_it != m_cfg_map.end())
			m_cfg_map.erase(l_it); // É¾³ýÖØ¸´Êý¾Ý
		m_cfg_map.insert(std::make_pair(key, value));
		return true;
	}

	CString getKey(CString key)
	{
		std::map<CString, CString >::iterator l_it;
		l_it = m_cfg_map.find(key);
		if (l_it == m_cfg_map.end())
			return L"";
		else
			return l_it->second;
	}

	bool saveConfig()
	{
		CStringA content;
		std::map<CString, CString >::iterator l_it;

		for (l_it = m_cfg_map.begin(); l_it != m_cfg_map.end(); l_it++)
		{
			CStringA tmpa(l_it->first);
			content.Append(tmpa);
			content.Append("=");
			CStringA tmpb(l_it->second);
			content.Append(tmpb);
			content.Append("\r\n");
		}
		return CMyUtility::FileWrite(FILE_NAME, content, content.GetLength());
	}

	bool CCommonConfig::loadConfig()
	{
		m_cfg_map.clear();
		std::vector<BYTE> file_data;
		bool ret = CMyUtility::FileRead(FILE_NAME, file_data);
		if (ret)
		{
			std::vector<BYTE>::iterator itr;
			std::string src;
			for (itr = file_data.begin(); itr != file_data.end(); itr++)
				src += *itr;

			std::vector<std::string> dest;
			std::string sep("\r\n");
			CMyString::split(src, sep, dest);

			std::vector<std::string>::iterator d_itr;
			std::string line;
			for (d_itr = dest.begin(); d_itr != dest.end(); d_itr++)
			{
				line = *d_itr;
				CString pairs(line.c_str());
				int find = pairs.Find(L"=");
				if (find>0)
					m_cfg_map.insert(std::make_pair(pairs.Left(find), pairs.Right(pairs.GetLength() - find - 1)));
			}

		}
		return ret;
	}
};


#ifdef __USE_COMM_CONFIG
CCommonConfig* CCommonConfig::pCommConfig = NULL;
#endif