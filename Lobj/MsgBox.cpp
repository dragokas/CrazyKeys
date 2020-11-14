#include <windows.h>
#include <stdio.h>

#include "MsgBox.h"

static const int minBufferSize = 128;
static const int maxBufferSize = 128*128;

int MsgBox( UINT uType, const TCHAR* caption, const TCHAR* textFormat, va_list& list )
{
	for( int bufSize = minBufferSize; bufSize <= maxBufferSize; bufSize *= 2 ) {
		CString line( bufSize );
#ifdef UNICODE
		int written = _vsnwprintf( line.GetRowBuffer(), (size_t)bufSize, textFormat, list );
#else
		int written = _vsnprintf( line.GetRowBuffer(), (size_t)bufSize, textFormat, list );		
#endif
		if( 0 <= written && written < bufSize ) {
			line.GetRowBuffer()[written] = 0;
			line.ValidateStrBuffer();
			int result = MessageBox( 0, line.GetBuffer(), caption, uType );
			return result;
		}
	}
	MessageBox( 0, textFormat, T( "Message is too long" ), uType );
	return 0;	
}

int MsgBox( UINT uType, const TCHAR* caption, const TCHAR* textFormat, ... )
{
	va_list list;
	va_start( list, textFormat );
	int result = MsgBox( uType, caption, textFormat, list );
	va_end( list );
	return result;
}

void MsgBox( const TCHAR* caption, const TCHAR* textFormat, ... )
{
	va_list list;
	va_start( list, textFormat );
	MsgBox( 0, caption, textFormat, list );
	va_end( list );
}

CString GetLastErrorStr( DWORD lastError, int maxLenght )
{
	for( int bufSize = minBufferSize; 
		bufSize <= maxBufferSize && bufSize <= maxLenght; 
		bufSize = min( bufSize * 2, maxLenght ) )
	{
		CString tmpStr( bufSize );
		if( FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, 0, lastError, 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), tmpStr.GetRowBuffer()/*(LPTSTR)buffer*/, bufSize, 0 ) != 0 )
		{
			tmpStr.ValidateStrBuffer();
			return tmpStr;
		}
	}
	return EmptyStr;
}

































