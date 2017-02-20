#include "stdafx.h"
#include "ComPort.h"


ComPort::ComPort()
{
	m_PortName = _T("");
	m_BaudRate = _T("");
	m_DataBit = _T("");
	m_StopBit = _T("");
	m_Parity = _T("");
}


ComPort::~ComPort()
{
}


CString ComPort::GetPortName()
{
	return m_PortName;
}
void ComPort::SetPortName(CString str)
{
	m_PortName = str;
}

CString ComPort::GetBaudRate()
{
	return m_BaudRate;
}
void ComPort::SetBaudRate(CString str)
{
	m_BaudRate = str;
}

CString ComPort::GetDataBit()
{
	return m_DataBit;
}
void ComPort::SetDataBit(CString str)
{
	m_DataBit = str;
}

CString ComPort::GetStopBit()
{
	return m_StopBit;
}
void ComPort::SetStopBit(CString str)
{
	m_StopBit = str;
}

CString ComPort::GetParity()
{
	return m_Parity;
}
void ComPort::SetParity(CString str)
{
	m_Parity = str;
}
