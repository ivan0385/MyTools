#pragma once
class ComPort
{
public:
	ComPort();
	~ComPort();
private:
	CString m_PortName;
	CString m_BaudRate;
	CString m_DataBit;
	CString m_StopBit;
	CString m_Parity;
public:
	CString GetPortName();
	void SetPortName(CString str);
	CString GetBaudRate();
	void SetBaudRate(CString str);
	CString GetDataBit();
	void SetDataBit(CString str);
	CString GetStopBit();
	void SetStopBit(CString str);
	CString GetParity();
	void SetParity(CString str);
};

