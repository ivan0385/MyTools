#pragma once
#include "stdafx.h"
class CanBus
{
public:
	CanBus() 
		: m_ChannelNo(_T(""))
		, m_BitRate(_T(""))
	{
	}

	~CanBus() {};

	void SetChannelNo(CString value)
	{
		m_ChannelNo = value;
	}
	void SetBitRate(CString value)
	{
		m_BitRate = value;
	}
	CString GetChannelNo()
	{
		return m_ChannelNo;
	}
	CString GetBitRate()
	{
		return m_BitRate;
	}

private:
	CString m_ChannelNo;
	CString m_BitRate;
};

