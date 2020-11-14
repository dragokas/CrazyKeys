#pragma once
#include <Windows.h>
#include "Str.h"


CString GetUserName();

bool ChooseFileName( CString& fileName );

bool ChooseFileName( CString& fileName, HWND hOwner, CString title, 
					CString currentDir, TCHAR* filter, DWORD flags );


bool ChooseColor( COLORREF& color, HWND hOwner, DWORD flags, COLORREF pCustColors[16] = 0 );

bool ChooseFont( LOGFONT& logFont, HWND hOwner, DWORD flags, COLORREF color = 0 );


