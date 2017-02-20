#pragma once
#include "stdafx.h"

class CMyQueue
{
public:

	CMyQueue()
	{
		Clear();
	}

	~CMyQueue()
	{
	}
private:
	BYTE buff[4096];
	int m_iHead, m_iTail;
public:
	void Clear()
	{
		m_iHead = m_iTail = 0;
		memset(buff, 0, 4096);
	}
	int GetSize()
	{
		return (m_iHead - m_iTail + 4096) % 4096;
	}
	bool PutByte(BYTE b)
	{
		if (GetSize() == (4096 - 1)) return false;
		buff[m_iHead++] = b;
		m_iHead %= 4096;
		return true;
	}
	bool GetByte(BYTE *pb)
	{
		if (GetSize() == 0) return false;
		*pb = buff[m_iTail++];
		m_iTail %= 4096;
		return true;
	}

};

