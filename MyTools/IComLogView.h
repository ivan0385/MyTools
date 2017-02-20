#pragma once
class IComLogView
{
public:
	IComLogView() {};
	virtual ~IComLogView() {};

	virtual bool SetRcvMsg(const unsigned char * msg,int size) = 0;
	virtual bool SetRcvMsg(CString msg) = 0;
	virtual bool SetSndMsg(CString msg) { return false; }
};

