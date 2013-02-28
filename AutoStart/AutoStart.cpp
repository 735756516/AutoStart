#include "AutoStart.h"


/************************************************************************/
/* 打印出当前程序代码中 GetLastError 所代表的错误，表示失败
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
/* 实现打印一条消息，用来显示操作成功
/************************************************************************/
BOOL OutputSuccessMessage(LPWSTR lpszMsg)
{
	OutputDebugString(lpszMsg);

	return TRUE;
}


/************************************************************************/
/* 用来实现在注册表中的指定路径下创建一个子项
/************************************************************************/
BOOL CreateRegSub(HKEY hRootKey, LPCTSTR lpszRegKey)
{
	HKEY hKey = NULL;

	//if(ERROR_SUCCESS == RegCreateKeyEx(hRootKey, lpszRegKey, 0, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hKey, NULL))
	//if(ERROR_SUCCESS == RegCreateKeyEx(hRootKey, lpszRegKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_CREATE_SUB_KEY | KEY_WOW64_32KEY, NULL, &hKey, NULL))
	if(ERROR_SUCCESS == RegCreateKey(hRootKey, lpszRegKey, &hKey))
	{
		RegCloseKey(hKey);

		return TRUE;
	}
	else
	{
		OutputErrorMessage(TEXT("CreateRegSub API - RegCreateKey Failed , Error Code Is: %d , Error Message Is: %s !"));

		return FALSE;
	}
}


/************************************************************************/
/* 用来实现在指定的注册表项下创建一个类型为 String 的子键，并且给子键赋值
/************************************************************************/
BOOL CreateRegKeyString(HKEY hRootKey, LPCWSTR lpszRegKey, LPCWSTR lpszRegKeyName, LPWSTR lpszRegKeyValue)
{
	HKEY hKey = NULL;

	if(ERROR_SUCCESS == RegCreateKey(hRootKey, lpszRegKey, &hKey))
	{
		bool bResult = FALSE;
		if(ERROR_SUCCESS == RegSetValueEx(hKey, lpszRegKeyName, 0, REG_SZ, (LPBYTE)lpszRegKeyValue, (DWORD)(lstrlen(lpszRegKeyValue) + 1) * sizeof(TCHAR)))
		{
			bResult = TRUE;
		}
		else
		{
			OutputErrorMessage(TEXT("CreateRegKeyString API - RegSetValueEx Failed , Error Code Is: %d , Error Message Is: %s !"));
		}

		RegCloseKey(hKey);

		return bResult;
	}
	else
	{
		OutputErrorMessage(TEXT("CreateRegKeyString API - RegCreateKey Failed , Error Code Is: %d , Error Message Is: %s !"));

		return FALSE;
	}
}


/************************************************************************/
/* 用来实现在指定的注册表项下创建一个类型为 DWORD 的子键，并且给子键赋值
/************************************************************************/
BOOL CreateRegKeyInt32(HKEY hRootKey, LPCWSTR lpszRegKey, LPCWSTR lpszRegKeyName, DWORD dwRegKeyValue)
{
	HKEY hKey = NULL;

	if(ERROR_SUCCESS == RegCreateKey(hRootKey, lpszRegKey, &hKey))
	{
		bool bResult = FALSE;
		if(ERROR_SUCCESS == RegSetValueEx(hKey, lpszRegKeyName, 0, REG_DWORD, (LPBYTE)lpszRegKeyName, sizeof(lpszRegKeyName)))
		{
			bResult = TRUE;
		}
		else
		{
			OutputErrorMessage(TEXT("CreateRegKeyInt32 API - RegSetValueEx Failed , Error Code Is: %d , Error Message Is: %s !"));
		}

		RegCloseKey(hKey);

		return bResult;
	}
	else
	{
		OutputErrorMessage(TEXT("CreateRegKeyInt32 API - RegCreateKey Failed , Error Code Is: %d , Error Message Is: %s !"));

		return FALSE;
	}
}


/************************************************************************/
/* 用来实现在指定的注册表项下查询一个类型为 String 的子键，并且返回子键的值
/************************************************************************/
BOOL QueryRegKeyString(HKEY hRootKey, LPCWSTR lpszRegKey, LPCWSTR lpszRegKeyName, LPWSTR lpszRegKeyValue, DWORD dwValueSize)
{
	HKEY hKey = NULL;
	DWORD dwType = 0;

	if(ERROR_SUCCESS == RegOpenKey(hRootKey, lpszRegKey, &hKey))
	{
		DWORD dwLen = 0;
		bool bResult = FALSE;

		if(ERROR_SUCCESS == RegQueryValueEx(hKey, (LPTSTR)lpszRegKeyName, NULL, &dwType, NULL, &dwLen))
		{
			if(dwLen > dwValueSize)
			{
				OutputDebugString(TEXT("QueryRegKeyString API - RegQueryValueEx Failed , Error Is Memory Full !"));

				bResult = FALSE;
			}
			else
			{
				if(ERROR_SUCCESS == RegQueryValueEx(hKey, lpszRegKeyName, NULL, &dwType, (LPBYTE)lpszRegKeyValue, &dwLen))
				{
					bResult = TRUE;
				}
				else
				{
					bResult = FALSE;

					OutputErrorMessage(TEXT("QueryRegKeyString API - RegQueryValueEx Failed , Error Code Is: %d , Error Message Is: %s !"));
				}
			}
		}
		else
		{
			bResult = FALSE;

			OutputErrorMessage(TEXT("QueryRegKeyString API - RegQueryValueEx Failed , Error Code Is: %d , Error Message Is: %s !"));
		}

		RegCloseKey(hKey);

		return bResult;
	}
	else
	{
		OutputErrorMessage(TEXT("QueryRegKeyString API - RegOpenKey Failed , Error Code Is: %d , Error Message Is: %s !"));

		return FALSE;
	}
}


/************************************************************************/
/* 用来实现在指定的注册表项下查询一个类型为 Int32 的子键，并且返回子键的值
/************************************************************************/
BOOL QueryRegKeyInt32(HKEY hRootKey, LPCWSTR lpszRegKey, LPCWSTR lpszRegKeyName, LPDWORD lpdwRegKeyValue)
{
	HKEY hKey = NULL;
	DWORD dwType = 0;

	if(ERROR_SUCCESS == RegOpenKey(hRootKey, lpszRegKey, &hKey))
	{
		DWORD dwLen = 0;
		bool bResult = FALSE;

		if(ERROR_SUCCESS == RegQueryValueEx(hKey, (LPTSTR)lpszRegKeyName, NULL, &dwType, NULL, &dwLen))
		{
			if(ERROR_SUCCESS == RegQueryValueEx(hKey, lpszRegKeyName, NULL, &dwType, (LPBYTE)lpdwRegKeyValue, &dwLen))
			{
				bResult = TRUE;
			}
			else
			{
				bResult = FALSE;

				OutputErrorMessage(TEXT("QueryRegKeyInt32 API - RegQueryValueEx Failed , Error Code Is: %d , Error Message Is: %s !"));
			}
		}
		else
		{
			bResult = FALSE;

			OutputErrorMessage(TEXT("QueryRegKeyInt32 API - RegQueryValueEx Failed , Error Code Is: %d , Error Message Is: %s !"));
		}

		RegCloseKey(hKey);

		return bResult;
	}
	else
	{
		OutputErrorMessage(TEXT("QueryRegKeyInt32 API - RegOpenKey Failed , Error Code Is: %d , Error Message Is: %s !"));

		return FALSE;
	}
}


/************************************************************************/
/* 用来实现在注册表中的指定路径下删除一个子项
/************************************************************************/
BOOL DeleteRegSub(HKEY hRootKey, LPCWSTR lpszRegKey)
{
	HKEY hKey = NULL;
	if(ERROR_SUCCESS == RegDeleteKey(hRootKey, lpszRegKey))
	{
		RegCloseKey(hKey);

		return TRUE;
	}
	else
	{
		OutputErrorMessage(TEXT("DeleteRegSub API - RegDeleteKey Failed , Error Code Is: %d , Error Message Is: %s !"));

		return FALSE;
	}
}


/************************************************************************/
/* 用来实现在指定的注册表项下删除一个类型为 String 的子键
/************************************************************************/
BOOL DeleteRegKeyString(HKEY hRootKey, LPCWSTR lpszRegKey, LPCWSTR lpszRegKeyName)
{
	HKEY hKey = NULL;

	if(ERROR_SUCCESS == RegOpenKey(hRootKey, lpszRegKey, &hKey))
	{
		bool bResult = FALSE;
		if(ERROR_SUCCESS == RegDeleteValue(hKey, lpszRegKeyName))
		{
			bResult = TRUE;
		}
		else
		{
			OutputErrorMessage(TEXT("DeleteRegKeyString API - RegDeleteValue Failed , Error Code Is: %d , Error Message Is: %s !"));
		}

		RegCloseKey(hKey);

		return bResult;
	}
	else
	{
		OutputErrorMessage(TEXT("DeleteRegKeyString API - RegOpenKey Failed , Error Code Is: %d , Error Message Is: %s !"));

		return FALSE;
	}
}


/************************************************************************/
/* 用来实现在指定的注册表项下删除一个类型为 Int32 的子键
/************************************************************************/
BOOL DeleteRegKeyInt32(HKEY hRootKey, LPCWSTR lpszRegKey, LPCWSTR lpszRegKeyName)
{
	HKEY hKey = NULL;

	if(ERROR_SUCCESS == RegOpenKey(hRootKey, lpszRegKey, &hKey))
	{
		bool bResult = FALSE;
		if(ERROR_SUCCESS == RegDeleteValue(hKey, lpszRegKeyName))
		{
			bResult = TRUE;
		}
		else
		{
			OutputErrorMessage(TEXT("DeleteRegKeyInt32 API - RegDeleteValue Failed , Error Code Is: %d , Error Message Is: %s !"));
		}

		RegCloseKey(hKey);

		return bResult;
	}
	else
	{
		OutputErrorMessage(TEXT("DeleteRegKeyInt32 API - RegOpenKey Failed , Error Code Is: %d , Error Message Is: %s !"));

		return FALSE;
	}
}


//LPCWSTR lpszCLSID1 = TEXT("SOFTWARE\\Classes\\CLSID\\{881EF26E-B879-4B82-AFC8-5C85F2792266}");
//LPCWSTR lpszCLSID2 = TEXT("SOFTWARE\\Classes\\CLSID\\{881EF26E-B879-4B82-AFC8-5C85F2792266}\\InProcServer32");
//LPCWSTR lpszCLSID3 = TEXT("SOFTWARE\\Classes\\CLSID\\{881EF26E-B879-4B82-AFC8-5C85F2792266}\\shellex");
//LPCWSTR lpszCLSID4 = TEXT("SOFTWARE\\Classes\\CLSID\\{881EF26E-B879-4B82-AFC8-5C85F2792266}\\shellex\\NoRecentDocs");


const DWORD REGKEY_ARRAY_LEN = 19;
LPWSTR lpszRegKeyArray[REGKEY_ARRAY_LEN] = 
{
	TEXT("SOFTWARE\\Classes\\*\\shellex\\ContextMenuHandlers\\"), 
	TEXT("SOFTWARE\\Classes\\*\\shellex\\DragDropHandlers\\"),
	TEXT("SOFTWARE\\Classes\\*\\shellex\\PropertySheetHandlers\\"),

	TEXT("SOFTWARE\\Classes\\Directory\\Background\\shellex\\ContextMenuHandlers\\"),
	TEXT("SOFTWARE\\Classes\\Directory\\shellex\\ContextMenuHandlers\\"),
	TEXT("SOFTWARE\\Classes\\Directory\\shellex\\CopyHookHandlers\\"),
	TEXT("SOFTWARE\\Classes\\Directory\\shellex\\DragDropHandlers\\"),
	TEXT("SOFTWARE\\Classes\\Directory\\shellex\\PropertySheetHandlers\\"),

	TEXT("SOFTWARE\\Classes\\Drive\\shellex\\ContextMenuHandlers\\"),
	TEXT("SOFTWARE\\Classes\\Drive\\shellex\\DragDropHandlers\\"),
	TEXT("SOFTWARE\\Classes\\Drive\\shellex\\FolderExtensions\\"),
	TEXT("SOFTWARE\\Classes\\Drive\\shellex\\PropertySheetHandlers\\"),

	TEXT("SOFTWARE\\Classes\\Folder\\shellex\\ContextMenuHandlers\\"),
	TEXT("SOFTWARE\\Classes\\Folder\\shellex\\DragDropHandlers\\"),
	TEXT("SOFTWARE\\Classes\\Folder\\shellex\\PropertySheetHandlers\\"),

	TEXT("SOFTWARE\\Classes\\lnkfile\\shellex\\ContextMenuHandlers\\"),
	TEXT("SOFTWARE\\Classes\\LibraryLocation\\shellex\\ContextMenuHandlers\\"),
	TEXT("SOFTWARE\\Classes\\LibraryFolder\\background\\shellex\\ContextMenuHandlers\\"),
	TEXT("SOFTWARE\\Classes\\InternetShortcut\\shellex\\ContextMenuHandlers\\")
};


//LPCWSTR lpszRegKey1 = TEXT("SOFTWARE\\Classes\\Folder\\shellex\\ColumnHandlers\\{881EF26E-B879-4B82-AFC8-5C85F2792266}");


//LPCWSTR lpszRegKey1 = TEXT("SOFTWARE\\Classes\\*\\shellex\\ContextMenuHandlers\\Zachary.XiaoZhen");
//LPCWSTR lpszRegKey2 = TEXT("SOFTWARE\\Classes\\*\\shellex\\DragDropHandlers\\Zachary.XiaoZhen");
//LPCWSTR lpszRegKey3 = TEXT("SOFTWARE\\Classes\\*\\shellex\\PropertySheetHandlers\\Zachary.XiaoZhen");
//
//LPCWSTR lpszRegKey4 = TEXT("SOFTWARE\\Classes\\Directory\\Background\\shellex\\ContextMenuHandlers\\Zachary.XiaoZhen");
//LPCWSTR lpszRegKey5 = TEXT("SOFTWARE\\Classes\\Directory\\shellex\\ContextMenuHandlers\\Zachary.XiaoZhen");
//LPCWSTR lpszRegKey6 = TEXT("SOFTWARE\\Classes\\Directory\\shellex\\CopyHookHandlers\\Zachary.XiaoZhen");
//LPCWSTR lpszRegKey7 = TEXT("SOFTWARE\\Classes\\Directory\\shellex\\DragDropHandlers\\Zachary.XiaoZhen");
//LPCWSTR lpszRegKey8 = TEXT("SOFTWARE\\Classes\\Directory\\shellex\\PropertySheetHandlers\\Zachary.XiaoZhen");
//
//LPCWSTR lpszRegKey9 = TEXT("SOFTWARE\\Classes\\Drive\\shellex\\ContextMenuHandlers\\Zachary.XiaoZhen");
//LPCWSTR lpszRegKey10 = TEXT("SOFTWARE\\Classes\\Drive\\shellex\\DragDropHandlers\\Zachary.XiaoZhen");
//LPCWSTR lpszRegKey11 = TEXT("SOFTWARE\\Classes\\Drive\\shellex\\FolderExtensions\\Zachary.XiaoZhen");
//LPCWSTR lpszRegKey12 = TEXT("SOFTWARE\\Classes\\Drive\\shellex\\PropertySheetHandlers\\Zachary.XiaoZhen");
//
//LPCWSTR lpszRegKey13 = TEXT("SOFTWARE\\Classes\\Folder\\shellex\\ContextMenuHandlers\\Zachary.XiaoZhen");
//LPCWSTR lpszRegKey14 = TEXT("SOFTWARE\\Classes\\Folder\\shellex\\DragDropHandlers\\Zachary.XiaoZhen");
//LPCWSTR lpszRegKey15 = TEXT("SOFTWARE\\Classes\\Folder\\shellex\\PropertySheetHandlers\\Zachary.XiaoZhen");


/************************************************************************/
/* 注册 DLL 自动启动
/************************************************************************/
BOOL RegisterDllAutoStart(LPWSTR lpszExeName, LPWSTR lpszGUID, LPWSTR lpszDllPath)
{
	wstring strCLSID1;
	wstring strCLSID2;
	wstring strCLSID3;
	wstring strCLSID4;

	wstring strRegKey1;

	strCLSID1 = TEXT("SOFTWARE\\Classes\\CLSID\\");
	strCLSID1 += lpszGUID;

	strCLSID2 = TEXT("SOFTWARE\\Classes\\CLSID\\");
	strCLSID2 += lpszGUID;
	strCLSID2 += TEXT("\\InProcServer32");

	strCLSID3 = TEXT("SOFTWARE\\Classes\\CLSID\\{881EF26E-B879-4B82-AFC8-5C85F2792266}\\shellex");
	strCLSID3 += lpszGUID;
	strCLSID3 += TEXT("\\shellex");

	strCLSID4 = TEXT("SOFTWARE\\Classes\\CLSID\\{881EF26E-B879-4B82-AFC8-5C85F2792266}\\shellex\\NoRecentDocs");
	strCLSID4 += lpszGUID;
	strCLSID4 += TEXT("\\shellex\\NoRecentDocs");

	strRegKey1 = TEXT("SOFTWARE\\Classes\\Folder\\shellex\\ColumnHandlers\\");
	strRegKey1 == lpszGUID;

	if (CreateRegSub(HKEY_LOCAL_MACHINE, strCLSID1.c_str()) == FALSE || 
		CreateRegSub(HKEY_LOCAL_MACHINE, strCLSID2.c_str()) == FALSE || 
		CreateRegSub(HKEY_LOCAL_MACHINE, strCLSID3.c_str()) == FALSE || 
		CreateRegSub(HKEY_LOCAL_MACHINE, strCLSID4.c_str()) == FALSE || 
		CreateRegKeyString(HKEY_LOCAL_MACHINE, strCLSID1.c_str(), NULL, lpszExeName) == FALSE || 
		CreateRegKeyString(HKEY_LOCAL_MACHINE, strCLSID2.c_str(), NULL, lpszDllPath) == FALSE || 
		CreateRegKeyString(HKEY_LOCAL_MACHINE, strCLSID2.c_str(), TEXT("ThreadingModel"), TEXT("Apartment")) == FALSE)
	{
		OutputErrorMessage(TEXT("Error Code Is % d，Error Message Is %s"));

		return FALSE;
	}
	else
	{
		if(CreateRegSub(HKEY_LOCAL_MACHINE, strRegKey1.c_str()) == FALSE)
		{
			OutputErrorMessage(TEXT("Error Code Is % d，Error Message Is %s"));

			return FALSE;
		}

		wstring strTmp;
		for(int i = 0; i < REGKEY_ARRAY_LEN; i++)
		{
			strTmp = lpszRegKeyArray[i];
			strTmp += lpszExeName;
			if(CreateRegKeyString(HKEY_LOCAL_MACHINE, strTmp.c_str(), NULL, lpszGUID) == FALSE)
			{
				OutputErrorMessage(TEXT("Error Code Is % d，Error Message Is %s"));

				return FALSE;
			}
		}

		return TRUE;
	}
}


/************************************************************************/
/* 取消 DLL 自动启动注册
/************************************************************************/
BOOL UnRegisterDllAutoStart(LPWSTR lpszExeName, LPWSTR lpszGUID)
{
	wstring strCLSID1;
	wstring strCLSID2;
	wstring strCLSID3;
	wstring strCLSID4;

	wstring strRegKey1;

	strCLSID1 = TEXT("SOFTWARE\\Classes\\CLSID\\");
	strCLSID1 += lpszGUID;

	strCLSID2 = TEXT("SOFTWARE\\Classes\\CLSID\\");
	strCLSID2 += lpszGUID;
	strCLSID2 += TEXT("\\InProcServer32");

	strCLSID3 = TEXT("SOFTWARE\\Classes\\CLSID\\{881EF26E-B879-4B82-AFC8-5C85F2792266}\\shellex");
	strCLSID3 += lpszGUID;
	strCLSID3 += TEXT("\\shellex");

	strCLSID4 = TEXT("SOFTWARE\\Classes\\CLSID\\{881EF26E-B879-4B82-AFC8-5C85F2792266}\\shellex\\NoRecentDocs");
	strCLSID4 += lpszGUID;
	strCLSID4 += TEXT("\\shellex\\NoRecentDocs");

	strRegKey1 = TEXT("SOFTWARE\\Classes\\Folder\\shellex\\ColumnHandlers\\");
	strRegKey1 == lpszGUID;

	DeleteRegSub(HKEY_LOCAL_MACHINE, strRegKey1.c_str());

	wstring strTmp;
	for(int i = 0; i < REGKEY_ARRAY_LEN; i++)
	{
		strTmp = lpszRegKeyArray[i];
		strTmp += lpszExeName;
		DeleteRegSub(HKEY_LOCAL_MACHINE, strTmp.c_str());
	}

	DeleteRegKeyString(HKEY_LOCAL_MACHINE, strCLSID2.c_str(), TEXT("ThreadingModel"));
	DeleteRegSub(HKEY_LOCAL_MACHINE, strCLSID4.c_str());
	DeleteRegSub(HKEY_LOCAL_MACHINE, strCLSID3.c_str());
	DeleteRegSub(HKEY_LOCAL_MACHINE, strCLSID2.c_str());
	DeleteRegSub(HKEY_LOCAL_MACHINE, strCLSID1.c_str());

	return TRUE;
}


int main()
{
	LPWSTR lpszExeName = TEXT("Zachary.XiaoZhen");
	LPWSTR lpszGUID = TEXT("{881EF26E-B879-4B82-AFC8-5C85F2792266}");
	LPWSTR lpszDllPath = TEXT("C:\\AutoStartDLL.dll");

	cout<<"Input 1 : Register Dll Auto Start !"<<endl;
	cout<<"Input 2 : UnRegister Dll Auto Start !"<<endl;
	
	DWORD dwSelected = 0;
	cin>>dwSelected;
	if(dwSelected == 1)
	{
		RegisterDllAutoStart(lpszExeName, lpszGUID, lpszDllPath);
		cout<<"Register Dll Auto Start Succeed !"<<endl;
	}
	else if(dwSelected == 2)
	{
		UnRegisterDllAutoStart(lpszExeName, lpszGUID);
		cout<<"UnRegister Dll Auto Start Succeed !"<<endl;
	}
	else
	{
		cout<<"Input Invalid Data"<<endl;
	}

	system("pause");

	return 0;
}

