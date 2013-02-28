#include <Windows.h>
#include <TlHelp32.h>
#include <algorithm> 

#include <iostream>
#include <string>

using namespace std;
using std::string;
using std::wstring;


#define Zac_Demo_Mutex		TEXT("Global\\Zachary.XiaoZhen.Mutex")
#define Zac_Process_Name	TEXT("AutoStartExe.exe")


/************************************************************************/
/* wstring 中字母全部转换为大写
/************************************************************************/
wstring WString2Upper(wstring str)
{
	wstring strResult = str;

	transform(strResult.begin(), strResult.end(), strResult.begin(), toupper); 

	return strResult;
}


/************************************************************************/
/* wstring 中字母全部转换为小写
/************************************************************************/
wstring WString2Lower(wstring str)
{
	wstring strResult = str;

	transform(strResult.begin(), strResult.end(), strResult.begin(), tolower); 

	return strResult;
}


/************************************************************************/
/* 判断进程是否存在
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


int main()
{
	while(1)
	{
		DWORD dwIn = 0;
		cin>>dwIn;

		if(dwIn == 1)
		{
			break;
		}
		else
		{
			cout<<"//\\//\\//\\HaHa，This Program Is Auto Start//\\//\\//\\"<<endl;
		}
	}
}

