#include "Registry.h"

bool GetStrFromRegistry( HKEY hKeyRoot, CString subKey, CString hotKey, CString& output )
{
	HKEY hKey = 0;
	if( RegOpenKey( hKeyRoot, subKey.GetBuffer(), &hKey ) == ERROR_SUCCESS ) {
		DWORD dw = 256;
		BYTE outStr[256]; ZeroMemory( outStr, 256 );
		DWORD type = REG_SZ;
		DWORD erCode = RegQueryValueEx( hKey, hotKey.GetBuffer(), 0, &type, outStr, &dw );
		if( erCode == ERROR_SUCCESS ) {
			output = (TCHAR *)outStr;
			RegCloseKey( hKey );
			return true;
		}
		RegCloseKey( hKey );
	}
	return false;
}

bool SetStrToRegistry( HKEY hKeyRoot, CString subKey, CString hotKey, CString input )
{
	HKEY hKey = 0;
	if( RegOpenKey( hKeyRoot, subKey.GetBuffer(), &hKey ) == ERROR_SUCCESS ) {
		DWORD erCode = RegSetValueEx( hKey, hotKey.GetBuffer(), 0, REG_SZ, (const BYTE*)input.GetBuffer(), input.Length() );
		RegCloseKey( hKey );
		return ( erCode == ERROR_SUCCESS );
	}
	return false;
}

bool GetBinFromRegistry( HKEY hKeyRoot, CString subKey, CString hotKey, DWORD& output )
{
	HKEY hKey = 0;
	if( RegOpenKey( hKeyRoot, subKey.GetBuffer(), &hKey ) == ERROR_SUCCESS ) {
		DWORD dw = 256;
		BYTE outStr[256]; 
		ZeroMemory( outStr, 256 );
		if( RegQueryValueEx( hKey, hotKey.GetBuffer(), 0, 0, outStr, &dw ) == ERROR_SUCCESS ) {
			output = Char2Int( outStr, 16 );
			RegCloseKey( hKey );
			return true;
		}
		RegCloseKey( hKey );
	}	
	return false;
}

bool GetBinFromRegistry( HKEY hKeyRoot, CString subKey, CString hotKey, UBinRegValue& output )
{
	HKEY hKey = 0;
	if( RegOpenKey( hKeyRoot, subKey.GetBuffer(), &hKey ) == ERROR_SUCCESS ) {
		DWORD dw = 256;
		BYTE outStr[512]; 
		ZeroMemory( outStr, 256 );
		DWORD type = REG_BINARY;
		if( RegQueryValueEx( hKey, hotKey.GetBuffer(), 0, &type, outStr, &dw ) == ERROR_SUCCESS ) {
			output.dwVal = Char2Int( (TCHAR* )outStr, 16 );
			RegCloseKey( hKey );
			return true;
		}
		RegCloseKey( hKey );
	}
	return false;
}

bool GetIntFromRegistry( HKEY hKeyRoot, CString subKey, CString hotKey, int& output )
{
	CString outputStr;
	if( GetStrFromRegistry( hKeyRoot, subKey, hotKey, outputStr ) ) {
		output = Str2Int( outputStr );
		return true;
	}
	return false;
}



