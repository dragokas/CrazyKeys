#include <windows.h>
#include <stdio.h>

#include "presume.h"

#pragma warning( disable : 4996 )//fopen, printf...

template<typename T1, typename T2>
void char2char( const T1* source, T2* target ) 
{
	int i;
	for( i = 0; source[i] != 0; i++ ) {
		target[i] = source[i];
	}
	target[i] = 0;
}

#ifdef _DEBUG 
void BreakPoint() { /*__asm { int 3 }*/ }
#else
void BreakPoint() {}
#endif

#ifdef _UNICODE
void ShowAssertMessage( wchar_t* caption, wchar_t* exp, const char *file, int line )
{
	wchar_t fname[260];
	char2char( file, fname );
	wchar_t text[500];
	swprintf(text, L"Expretion \n( %s ) \nfieled in file \n'%s'\nin line %d", exp, fname, line);
	MessageBox( 0, text, caption, 0);
	BreakPoint();
}
#else
void ShowAssertMessage( char* caption, char* exp, const char *file, int line )
{
	char fname[260];
	char2char( file, fname );
	char text[500];
	sprintf(text, "Expretion \n( %s ) \nfieled in file \n'%s'\nin line %d", exp, fname, line);
	MessageBox( 0, text, caption, 0);
	BreakPoint();
}
#endif

#ifdef _UNICODE
void ShowLastErrorMessage( wchar_t* exp, const char *file, int line )
{
	wchar_t fname[260];
	char2char( file, fname );
	wchar_t erMsgBuf[260] = L"GetLastError() == ERROR_SUCCESS\n";
	DWORD lastError = GetLastError();
	if( lastError != ERROR_SUCCESS ) {
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, 0, lastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &erMsgBuf, 259, 0 );
	}
	wchar_t text[500];
	swprintf( text, L"Expretion:\n( %s )\nfieled with Error Code:\n%d - %s\n in file:\n%s\nin line : %d\n", exp, lastError, erMsgBuf, fname, line );
	MessageBox( 0, text, L"ShowLastError", 0 );
	BreakPoint();
}
#else
void ShowLastErrorMessage( char* exp, const char *file, int line )
{
	char erMsgBuf[260] = "GetLastError() == ERROR_SUCCESS\n";
	DWORD lastError = GetLastError();
	if( lastError != ERROR_SUCCESS ) {
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, 0, lastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &erMsgBuf, 259, 0 );
	}
	char text[500];
	sprintf( text, "Expretion:\n( %s )\nfieled with Error Code:\n%d - %s\nin file:\n%s in line : %d\n", exp, lastError, erMsgBuf, file, line );
	MessageBox( 0, text, "ShowLastError", 0 );
	BreakPoint();
}
#endif