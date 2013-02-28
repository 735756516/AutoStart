#include <Windows.h>
#include <TlHelp32.h>
#include <algorithm> 

#include <iostream>
#include <string>


#include <tchar.h>
#include <strsafe.h>

using namespace std;
using std::string;
using std::wstring;


#define Zac_Process_Name	TEXT("AutoStartExe.exe")
#define Zac_Demo_Mutex		TEXT("Global\\Zachary.XiaoZhen.Mutex")


/************************************************************************/
/* ��ӡ����ǰ��������� GetLastError ������Ĵ��󣬱�ʾʧ��
/************************************************************************/
BOOL OutputErrorMessage(LPWSTR lpszMsg)
{
	LPVOID lpszBufMsg;
	LPVOID lpszBufErrorMsg;
	DWORD dwError = GetLastError();

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&lpszBufErrorMsg, 0, NULL);

	lpszBufMsg = (LPVOID)LocalAlloc(LMEM_ZEROINIT, sizeof(TCHAR) * 256);
	StringCchPrintf((LPWSTR)lpszBufMsg, LocalSize(lpszBufMsg), lpszMsg, dwError, lpszBufErrorMsg);

	OutputDebugString((LPTSTR)lpszBufMsg);

	LocalFree(lpszBufMsg);
	LocalFree(lpszBufErrorMsg);

	return FALSE;
}


/************************************************************************/
/* ʵ�ִ�ӡһ����Ϣ��������ʾ�����ɹ�
/************************************************************************/
BOOL OutputSuccessMessage(LPWSTR lpszMsg)
{
	OutputDebugString(lpszMsg);

	return TRUE;
}

/************************************************************************/
/* �������������Ľ��� notepad.exe
/************************************************************************/
VOID CreateProtectProcess()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	si.cb = sizeof(si);
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	
	WCHAR szCmdLine[] = L"C:\\AutoStartExe.exe";

	if(CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) == 0)
	{
		OutputSuccessMessage(TEXT("�������̳ɹ�"));
	}
	else
	{
		OutputErrorMessage(TEXT("��������ʧ�ܣ�Error Code Is %d��Error Message Is %s"));
	}
}


/************************************************************************/
/* wstring ����ĸȫ��ת��Ϊ��д
/************************************************************************/
wstring WString2Upper(wstring str)
{
	wstring strResult = str;

	transform(strResult.begin(), strResult.end(), strResult.begin(), toupper); 

	return strResult;
}


/************************************************************************/
/* wstring ����ĸȫ��ת��ΪСд
/************************************************************************/
wstring WString2Lower(wstring str)
{
	wstring strResult = str;

	transform(strResult.begin(), strResult.end(), strResult.begin(), tolower); 

	return strResult;
}


/************************************************************************/
/* �жϽ����Ƿ����
/************************************************************************/
BOOL ProcessIsExist(wstring strProcessName, BOOL bCaseSensitive)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
	PROCESSENTRY32W pe; 

	pe.dwSize = sizeof(PROCESSENTRY32W); 
	if (!Process32FirstW(hSnapshot, &pe)) 
	{ 
		CloseHandle(hSnapshot);

		return FALSE; 
	} 
	while (Process32NextW(hSnapshot, &pe)) 
	{ 
		wstring strExeFile = pe.szExeFile;
		if(TRUE == bCaseSensitive)
		{
			if(strExeFile.compare(strProcessName) == 0)
			{
				CloseHandle(hSnapshot);

				return TRUE;
			}
		}
		else
		{
			wstring strProcess1 = WString2Upper(strExeFile);
			wstring strProcess2 = WString2Upper(strProcessName);
			if(strProcess1.compare(strProcess2) == 0)
			{
				CloseHandle(hSnapshot);

				return TRUE;
			}
		}
	}

	CloseHandle(hSnapshot);

	return FALSE;
}


BOOL APIENTRY DllMain( HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
			OutputSuccessMessage(TEXT("********** DLL_PROCESS_ATTACH **********"));

			wstring strProcessName = Zac_Process_Name;
			if(ProcessIsExist(strProcessName, FALSE) == FALSE)
			{
				CreateProtectProcess();
			}
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		{
			OutputSuccessMessage(TEXT("=========== DLL_PROCESS_DETACH ==========="));

			wstring strProcessName = Zac_Process_Name;
			if(ProcessIsExist(strProcessName, FALSE) == FALSE)
			{
				CreateProtectProcess();
			}

			break;
		}
	}
	return TRUE;
}