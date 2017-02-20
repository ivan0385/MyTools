#pragma once
#include "IComLogView.h"
#include "IComTaskModel.h"
#include "ComTaskModel.h"
#include "stdafx.h"

#include "MyString.h"
#include <vector>

class EolTestPresenter : i_data_receiver
{
public:

	EolTestPresenter(IComLogView * view)
	{
		m_pEolTestDlg = view;
		m_pComTaskModel = ComTaskModel::GetInstance();
		m_pComTaskModel->SetDataReceiver(this);
		m_bEolConnected = 0;
	}

	~EolTestPresenter()
	{
	}

private:
	IComLogView* m_pEolTestDlg;
	IComTaskModel * m_pComTaskModel;
	bool m_bEolConnected;
	CString rcv_total;

	void _ParseMsg(CString msg)
	{
#if 0
		if (msg.Find(L"05 FF 00 FF FF FA") >= 0)
		{
			SendEolMsg(L"FF 01 FF FF");
		}
#endif
	}

public:
	virtual void receive(const BYTE* ba, int size)
	{

		static int step = 0;
		static int length = 0;
		static CString msg;
		CString tmp;

		for (int i = 0; i < size;)
		{
			switch (step)
			{
			case 0:
				if (ba[i] == 0xff || ba[i]<5)
				{
					msg = L"";
					step = 0;
				}
				else
				{
					tmp.Format(L"%02X ", ba[i]);
					msg.Append(tmp);
					step++;
					length = ba[i];
				}
				break;

			case 1:
				if (ba[i] == 0xff)
				{
					step++;
					tmp.Format(L"%02X ", ba[i]);
					msg.Append(tmp);
				}
				else
				{
					step = 0;
					msg = L"";
					continue;
				}
				break;

			case 3:
			case 4:
				if (ba[i] == 0xff)
				{
					step++;
					tmp.Format(L"%02X ", ba[i]);
					msg.Append(tmp);
				}
				else
				{
					step = 0;
					msg = L"";
					continue;
				}
				break;

			default:
				tmp.Format(L"%02X ", ba[i]);
				msg.Append(tmp);
				if (step == length)
				{
					rcv_total.Append(msg);
					m_pEolTestDlg->SetRcvMsg(msg);
					_ParseMsg(msg);
					step = 0;
					msg = L"";
				}
				else
					step++;
				break;
			}

			i++;
		}

	}

	bool IsComPortOpened()
	{
		return m_pComTaskModel->IsDeviceOpen();
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

	int SendEolMsg(CString inputStr)
	{
		if (inputStr.IsEmpty())
		{
			return -1;
		}

		BYTE *Send_buff=NULL;
		int length = 0;
		int cs = 0;
		inputStr.Replace(L"\r\n", L"");
		std::vector<std::string> tmp;
		CMyString::split(CMyString::toString(inputStr), " ", tmp);

		CString snd,tmpStr;

		length = tmp.size() + 2;
		if (length > 0)
		{
			Send_buff = new BYTE[length];

			Send_buff[0] = tmp.size() + 1;
			snd.Format(L"%02X ", Send_buff[0]);
			snd.Append(inputStr);

			cs = Send_buff[0];
			for (unsigned int i = 0; i < tmp.size(); i++)
			{
				int hex;
				std::istringstream(tmp[i]) >> std::hex >> hex;

				Send_buff[i + 1] = (BYTE)hex;
				cs ^= Send_buff[i + 1];
			}
			Send_buff[length-1] = cs;
			tmpStr.Format(L" %02X", cs);
			snd.Append(tmpStr);
			m_pEolTestDlg->SetSndMsg(snd);
			m_pComTaskModel->SendData(Send_buff, length);
			delete[] Send_buff;
		}
		return length;
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


