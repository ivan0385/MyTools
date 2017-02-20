#pragma once
#include "stdafx.h"
#include <vector>

class CMyUtility
{
public:
	CMyUtility(void) {}
	~CMyUtility(void) {}

	/**
	Get folder that module locate, with backslash('\') append \n
	hModule - NULL to get current process folder.
	*/
	static CString GetModuleFolder(HMODULE hModule = NULL)
	{
		TCHAR   s[MAX_PATH] = { 0 };
		GetModuleFileName(hModule, s, MAX_PATH);
		PathRemoveFileSpec(s);
		PathAddBackslash(s);
		return s;
	}

	static CString GetModuleName(HMODULE hModule = NULL)
	{
		TCHAR   s[MAX_PATH] = { 0 };
		GetModuleFileName(hModule, s, MAX_PATH);
		CString name(s);
		name = name.Mid(1 + name.ReverseFind(L'\\'));
		return name;
	}

	static CString GetExcutableFullPath(HMODULE hModule = NULL)
	{
		TCHAR   s[MAX_PATH] = { 0 };
		GetModuleFileName(hModule, s, MAX_PATH);

		CString name(s);
		return name;
	}

	// Get folder that file locate, with backslash('\') append.
	static CString GetFileFolder(LPCTSTR strFilename)
	{
		TCHAR   s[MAX_PATH] = { 0 };
		lstrcpyn(s, strFilename, MAX_PATH);
		PathRemoveFileSpec(s);
		PathAddBackslash(s);
		return s;
	}

	/// Get file's size.
	static INT64 GetFileSize(LPCTSTR strFilename)
	{
		WIN32_FILE_ATTRIBUTE_DATA   fd;
		if (GetFileAttributesEx(strFilename, GetFileExInfoStandard, &fd))
		{
			LARGE_INTEGER   n;
			n.LowPart = fd.nFileSizeLow;
			n.HighPart = (LONG)fd.nFileSizeHigh;
			return n.QuadPart;
		}
		return 0;
	}

	/// Get file's extension name, not including leading period.
	static CString GetFileExtName(LPCTSTR strFilename)
	{
		CString   s(strFilename);
		int   n = s.ReverseFind('.');
		if (n != -1)
		{
			return s.Mid(n + 1);
		}
		return _T("");
	}


	static CString QueryTempFilePath(void)
	{
		TCHAR   szTmpPath[MAX_PATH],
			szFilename[MAX_PATH];
		::GetTempPath(MAX_PATH, szTmpPath);
		::GetTempFileName(szTmpPath, _T("t"), 0, szFilename);
		return szFilename;
	}

	/// Read file to memory.
	static bool FileRead(LPCTSTR strFilename, std::vector<BYTE>& file_data)
	{
		file_data.clear();

		bool     b = false;
		HANDLE   f = CreateFile(strFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (f != INVALID_HANDLE_VALUE)
		{
			DWORD   nLength = ::GetFileSize(f, NULL);
			if (nLength)
			{
				file_data.resize(nLength);

				DWORD   dwRead;
				::ReadFile(f, &file_data[0], nLength, &dwRead, NULL);
				b = (dwRead == nLength);
			}
			CloseHandle(f);
		}
		return b;
	}

	// Write buffer to file, if file already exist, it will be delete before write.
	static bool FileWrite(LPCTSTR strFilename, const void* p, int nLength)
	{
		SetFileAttributes(strFilename, FILE_ATTRIBUTE_NORMAL);

		bool     b = false;
		HANDLE   f = CreateFile(strFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (f != INVALID_HANDLE_VALUE)
		{
			DWORD   nWrite;
			::WriteFile(f, p, nLength, &nWrite, NULL);
			b = ((int)nWrite == nLength);
			CloseHandle(f);
		}
		return b;
	}



};

