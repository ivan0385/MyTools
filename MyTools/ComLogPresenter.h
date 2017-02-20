#pragma once
#include "IComLogView.h"
#include "IComTaskModel.h"
#include "ComTaskModel.h"


class ComLogPresenter : i_data_receiver
{
public:
	ComLogPresenter(IComLogView * view)
	{
		m_pComLogDlg = view;
		m_pComTaskModel = ComTaskModel::GetInstance();
		m_pComTaskModel->SetDataReceiver(this);
	}
	~ComLogPresenter()
	{
	}

private:
	IComLogView* m_pComLogDlg;
	IComTaskModel * m_pComTaskModel;

public:
	virtual void receive(const BYTE* ba, int size)
	{
		m_pComLogDlg->SetRcvMsg(ba, size);
	}

	int SendAsciiData(CString inputStr)
	{
		CStringA msg(inputStr);
		int nSize = msg.GetLength() + 2;
		BYTE *Send_buff;
		Send_buff = new BYTE[nSize];
		for (int i = 0; i < nSize - 2; i++)
		{
			Send_buff[i] = (BYTE)msg[i];
		};
		Send_buff[nSize - 2] = 0x0d;
		Send_buff[nSize - 1] = 0x0a;
		m_pComTaskModel->SendData(Send_buff, nSize);
		
		delete[] Send_buff;
		return nSize;
	}


	int SendHexData(CString inputStr)
	{
		if (inputStr.IsEmpty())
		{
			return -1;
		}

		int bufPos = 0;
		int datasize, bufsize, i;
		BYTE *Send_buff, byHigh, byLow;
		inputStr.Replace(L" ", L"");
		inputStr.Replace(L"\r\n", L"");
		datasize = inputStr.GetLength();

		if (datasize % 2 == 0)
		{
			bufsize = datasize;
		}
		else
		{
			bufsize = datasize - 1;
		}

		Send_buff = new BYTE[bufsize];
		for (i = 0; i < bufsize; i += 2)
		{
			byHigh = ConvertHexChar((char)inputStr[i]);
			byLow = ConvertHexChar((char)inputStr[i + 1]);
			Send_buff[bufPos++] = (byHigh << 4) | byLow;
		}
		if (bufPos != 0)
		{
			m_pComTaskModel->SendData(Send_buff, bufPos);
		}
		delete[] Send_buff;
		return bufPos;
	}

private:
	int ConvertHexChar(char ch)
	{
		if ((ch >= '0') && (ch <= '9'))
			return ch - 0x30;
		else if ((ch >= 'A') && (ch <= 'F'))
			return ch - 'A' + 10;
		else if ((ch >= 'a') && (ch <= 'f'))
			return ch - 'a' + 10;
		else
			return (-1);
	}
};

