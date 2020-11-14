#include "TextFileFuncs.h"
#include "common.h"
#include "Vector.h"

void RecieveStandartLineBreaks( CVector<CStr<char> >& lineBreaks )
{
	lineBreaks.SetBufferSize( 3 );
	lineBreaks.Append( "\x0D\x0A" );
	lineBreaks.Append( "\x0D" );
	lineBreaks.Append( "\x0A" );
}

void RecieveStandartLineBreaks( CVector<CStr<wchar_t> >& lineBreaks )
{
	lineBreaks.SetBufferSize( 4 );
	lineBreaks.Append( L"\x0D\x0A" );
	lineBreaks.Append( L"\x0D" );
	lineBreaks.Append( L"\x0A" );
	lineBreaks.Append( L"\x2028" );
}



















