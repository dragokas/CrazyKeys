#pragma once
#include "Str.h"

CStr<char> GetWinMsgNameA( UINT uMsg );
CStr<wchar_t> GetWinMsgNameW( UINT uMsg );

#ifdef _UNICODE
#define GetWinMsgName GetWinMsgNameW
#else
#define GetWinMsgName GetWinMsgNameA
#endif


