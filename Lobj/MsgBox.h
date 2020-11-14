#pragma once
#include <windows.h>
#include "Str.h"

int MsgBox( UINT uType, const TCHAR* caption, const TCHAR* textFormat, ... );
void MsgBox( const TCHAR* caption, const TCHAR* textFormat, ... );

CString GetLastErrorStr( DWORD lastError, int maxLenght = 512 );