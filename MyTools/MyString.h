#pragma once
#include "stdafx.h"

#include<string>
#include<vector>
#include <iostream>
#include <sstream>

class CMyString
{
public:
	CMyString(void) {};
	~CMyString(void) {};



	static CString CMyString::GetAsciiFromString(CString rcv_total, CString str)
	{
		CString mm;
		CString nn = L"";

		int iFirst = rcv_total.Find(str);
		BYTE *nu = new BYTE[256];
		String2Hex(str.Mid(3, 2), nu);
		int length = nu[0] >> 3;
		if (iFirst >= 0)
		{
			mm = rcv_total.Right(rcv_total.GetLength() - (iFirst + str.GetLength()));
		}

		mm.Replace(L" ", L"");
		mm.Replace(L"\r\n", L"");
		String2Hex(mm, nu);

		CString tt;
		for (int i = 0; i<length - 1; i++)
		{
			tt.Format(L"%c", nu[i]);
			nn += tt;
		}

		delete[] nu;

		return nn;
	}


	static int String2Hex(CString inputStr, BYTE * output)
	{
		int bufPos = 0;
		int datasize, bufsize, i;
		BYTE byHigh, byLow;

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

		for (i = 0; i < bufsize; i += 2)
		{
			byHigh = ConvertHexChar((char)inputStr[i]);
			byLow = ConvertHexChar((char)inputStr[i + 1]);
			output[bufPos++] = (byHigh << 4) | byLow;
		}
		return bufPos;
	}


	static int ConvertHexChar(char ch)
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


	static CString GetNowTime(void)
	{
		SYSTEMTIME st;
		CString strDate, strTime;
		GetLocalTime(&st);
		strTime.Format(L"%02d:%02d:%02d.%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		return strTime;
	}


	static CString GetNowDate(void)
	{
		SYSTEMTIME st;
		CString strDate, strTime;
		GetLocalTime(&st);
		strDate.Format(L"%4d-%2d-%2d", st.wYear, st.wMonth, st.wDay);
		return strDate;
	}


	static CString GetNowDateTime(void)
	{
		SYSTEMTIME st;
		CString strDate, strTime;
		GetLocalTime(&st);
		strTime.Format(L"%2d-%2d %02d:%02d:%02d", st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		return strTime;
	}


	static void split(std::string & src, std::string separator, std::vector<std::string> & dest)
	{
		std::string str = src;
		std::string substring;
		std::string::size_type start = 0, index;

		do
		{
			index = str.find(separator, start);
			if (index != std::string::npos)
			{
				substring = str.substr(start, index - start);
				dest.push_back(substring);
				start = separator.length() + index;
				if (start == std::string::npos) return;
			}
		} while (index != std::string::npos);

		//the last token
		substring = str.substr(start);
		dest.push_back(substring);
	}

	static bool split_first_of(std::string & str, std::vector<std::string> & separators, std::string & dest)
	{
		std::string::size_type start = 0, index = std::string::npos, temp;
		std::string sep;
		str = trim(str);

		do
		{
			std::vector<std::string>::iterator separator;
			for (separator = separators.begin(); separator != separators.end(); separator++)
			{
				temp = str.find(*separator, 0);
				if (temp != std::string::npos)
				{
					if (index == std::string::npos)
					{
						sep = *separator;
						index = temp;
					}
					else if (temp < index)
					{
						sep = *separator;
						index = temp;
					}
				}
			}

			if (index != std::string::npos)
			{
				dest = str.substr(0, index);
				start = sep.length() + index;
				if (start != std::string::npos && str.length() - start>0)
					str = str.substr(start, str.length() - start);
				return true;
			}
		} while (index != std::string::npos);
		return false;
	}

	static bool split_first_of(std::string & str, std::string insep, std::string & dest)
	{
		std::vector<std::string> separators;
		separators.push_back(insep);
		return split_first_of(str, separators, dest);
		
	}

	template <class Type>
	static Type stringToNum(const std::string& str)
	{
		std::istringstream iss(str);
		Type num;
		iss >> num;
		return num;
	}

	static std::string& trim(std::string &s)
	{
		if (s.empty())
		{
			return s;
		}
		s.erase(0, s.find_first_not_of(" "));
		s.erase(s.find_last_not_of(" \r\n") + 1);
		return s;
	}

	static int ustr_astr(WCHAR * unicodestr, char *ansistr)
	{
		int result = 0;
		try
		{
			int needlen = WideCharToMultiByte(CP_ACP, 0, unicodestr, -1, NULL, 0, NULL, NULL);
			if (needlen < 0)
			{
				return needlen;
			}
			result = WideCharToMultiByte(CP_ACP, 0, unicodestr, -1, ansistr, needlen + 1, NULL, NULL);
			if (result < 0)
			{
				return result;
			}
			return strlen(ansistr);
		}
		catch (...)
		{
		}
		return result;
	}

	static std::string toString(CString cs)
	{
#ifdef _UNICODE  
		//如果是unicode工程  
		USES_CONVERSION;
		std::string str(W2A(cs));
		return str;
#else  
		//如果是多字节工程   
		std::string str(cs.GetBuffer());
		cs.ReleaseBuffer();
		return str;
#endif // _UNICODE   
	}


	static CString toCString(std::string str)
	{
#ifdef _UNICODE  
		//如果是unicode工程  
		USES_CONVERSION; 
		CString s(str.c_str());
		CString ans(str.c_str());
		return ans;
#else  
		//如果是多字节工程   
		//string 转 CString  
		CString ans;
		ans.Format("%s", str.c_str());
		return ans;
#endif // _UNICODE    
	}

	static int toInt(CString &str)
	{
		return _ttol(str);
	}
	static double toFloat(CString &str)
	{
		return _ttof(str);
	}

	static long toHex(std::string &str)
	{
		long hex;
		std::istringstream(str) >> std::hex >> hex;
		return hex;
	}

};

