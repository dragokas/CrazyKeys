#pragma once
#include <windows.h>
#include "Str.h"

union UBinRegValue {
	UBinRegValue( DWORD defaultValue = 0 ) : dwVal( defaultValue ) {}
	DWORD dwVal;
	WORD wVal[2];
	BYTE bVal[4];
};


bool GetStrFromRegistry( HKEY hKeyRoot, CString subKey, CString hotKey, CString& output );
bool SetStrToRegistry( HKEY hKeyRoot, CString subKey, CString hotKey, CString input );

bool GetBinFromRegistry( HKEY hKeyRoot, CString subKey, CString hotKey, DWORD& output );

bool GetIntFromRegistry( HKEY hKeyRoot, CString subKey, CString hotKey, int& output );

bool GetBinFromRegistry( HKEY hKeyRoot, CString subKey, CString hotKey, UBinRegValue& output );

