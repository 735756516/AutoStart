#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>

#include <iostream>
#include <string>

using namespace std;
using std::wstring;


BOOL OutputErrorMessage(LPWSTR lpszMsg);

BOOL OutputSuccessMessage(LPWSTR lpszMsg);

BOOL CreateRegSub(HKEY hRootKey, LPCTSTR lpszRegKey);

BOOL CreateRegKeyString(HKEY hRootKey, LPCWSTR lpszRegKey, LPCWSTR lpszRegKeyName, LPWSTR lpszRegKeyValue);

BOOL CreateRegKeyInt32(HKEY hRootKey, LPCWSTR lpszRegKey, LPCWSTR lpszRegKeyName, DWORD dwRegKeyValue);

BOOL QueryRegKeyString(HKEY hRootKey, LPCWSTR lpszRegKey, LPCWSTR lpszRegKeyName, LPWSTR lpszRegKeyValue, DWORD dwValueSize);

BOOL QueryRegKeyInt32(HKEY hRootKey, LPCWSTR lpszRegKey, LPCWSTR lpszRegKeyName, LPDWORD lpdwRegKeyValue);

BOOL DeleteRegSub(HKEY hRootKey, LPCWSTR lpszRegKey);

BOOL DeleteRegKeyString(HKEY hRootKey, LPCWSTR lpszRegKey, LPCWSTR lpszRegKeyName);

BOOL DeleteRegKeyInt32(HKEY hRootKey, LPCWSTR lpszRegKey, LPCWSTR lpszRegKeyName);