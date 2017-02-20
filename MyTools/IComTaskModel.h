#pragma once
#include "IComPortStatusListener.h"
#include "ComPort.h"
#include "DataRcvIf.h"
class IComTaskModel
{
public:
	IComTaskModel() {};
	virtual ~IComTaskModel() {}; // ����Ϊ��������������֤����������鹹������
	virtual int OpenComport(ComPort port, IComPortStatusListener * lis) = 0;
	virtual int CloseComport() = 0;
	virtual bool GetComportList(CArray<CString> *list) = 0;
	virtual ComPort GetSavedComportInfo() = 0;
	virtual bool SetDataReceiver(i_data_receiver * rcv) = 0;
	virtual bool SendData(unsigned char *msg,int size) = 0;
	virtual bool IsDeviceOpen() = 0;
};

