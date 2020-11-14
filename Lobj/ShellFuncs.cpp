#include "ShellFuncs.h"
#include "ShellPathFuncs.h"

CString GetUserName()
{
	TCHAR buf[257] = T("\0\0");
	unsigned long nSize = 256;
	CheckZero( GetUserName( buf, &nSize ) );
	return CString( buf );
}

bool ChooseFileName( CString& fileName, HWND hOwner, CString title, 
					CString currentDir, TCHAR* filter, DWORD flags )
{
	presume( ( flags & OFN_ALLOWMULTISELECT ) == 0 );
	TCHAR fileNameBuffer[MAX_PATH * 2] = {0};
	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hOwner;
	ofn.lpstrTitle = title;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileNameBuffer;
	ofn.nMaxFile = 256; 
	ofn.lpstrInitialDir = currentDir;
	ofn.Flags = flags;

	if( GetOpenFileName( &ofn ) != 0 ) {
		fileName = fileNameBuffer;
		return true;
	}
	return false;
}

bool ChooseFileName( CString& fileName )
{
	return ChooseFileName( fileName, 0, T(""), GetCurrentDirectory(), T("*.*\0"), 
		OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST );
}

bool ChooseColor( COLORREF& color, HWND hOwner, DWORD flags, COLORREF pCustColors[16] )
{
	static COLORREF acrCustClr[16] = {0};
	CHOOSECOLOR cc = {0};
	cc.lStructSize = sizeof( CHOOSECOLOR );
	cc.hwndOwner = hOwner;
	cc.rgbResult = color;
	cc.lpCustColors = ( pCustColors != 0 ) ? pCustColors : acrCustClr;
	cc.Flags = flags;
	if( ::ChooseColor( &cc ) != 0 ) {
		color = (COLORREF)cc.rgbResult;
		return true;
	}
	return false;
}

bool ChooseFont( LOGFONT& logFont, HWND hOwner, DWORD flags, COLORREF color /*= 0*/ )
{
	LOGFONT lf = logFont;
	CHOOSEFONT cf = {0};
	cf.lStructSize = sizeof( CHOOSEFONT );
	cf.hwndOwner = hOwner;
	cf.lpLogFont = &lf;
	cf.rgbColors = color;
	cf.Flags = flags;
	if( ::ChooseFont( &cf ) != 0 ) {
		logFont = lf;
		return true;
	}
	return false;
}


























