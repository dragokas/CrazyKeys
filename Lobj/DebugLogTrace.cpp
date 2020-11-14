#include <stdio.h>

#include "DebugLogTrace.h"

#include "Str.h"
#include "Queue.h"
#include "WinMsgName.h"

CList<CStr<char> > FileNames;//список имён файлов

#ifdef _DEBUG
void LogTrace( const char* fileName, const char* format, ... )
{
	va_list list;
	va_start( list, format );
	
	if( fileName[0] == 0 ) {
		fileName = "!defaultLogFileName2.txt";
	}

	if( FileNames.Find( fileName ) == -1 ) {
		DeleteFileA( fileName );//удаляем старый файл
		FileNames.PushToHead( fileName );
	}

	FILE* fLog = fopen( fileName, "a+" );
	if( fLog != 0 ) {
		for( int bufSize = 2; bufSize < 1025; bufSize *= 2 ) {
			CStr<char> line( bufSize );
			int written = _vsnprintf( line.GetRowBuffer(), (size_t)bufSize, format, list );
			if( 0 <= written && written < bufSize ) {
				line.GetRowBuffer()[written] = 0;
				line.ValidateStrBuffer();
				int res = fprintf( fLog, "%s", line.GetBuffer() );
				presume( 0 <= res );
				break;
			}
		}
		fclose( fLog );
	}
	
	va_end( list );
}
#else
void LogTrace( const char* , const char* , ... ) {}
#endif

void LogTraceWM( const char* fileName, unsigned int uMsg )
{
	LogTrace( fileName, "%s  : %u\n", GetWinMsgNameA( uMsg ).GetBuffer(), uMsg ); 
}