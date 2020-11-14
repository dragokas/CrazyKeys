#pragma once
#include "Enumeration.h"
#include "GdiObjects.h"

inline void FillRect( HDC hDC, CRect& rect, COLORREF color )
{
	HBRUSH hBrush = CreateSolidBrush( color );
	FillRect( hDC, &rect, hBrush );
	DeleteObject( hBrush );
}

inline CRect GetClientRect( HWND hWnd )
{
	CRect rect;
	GetClientRect( hWnd, &rect );
	return rect;
}

inline COLORREF MixColor( COLORREF c1, COLORREF c2, int mul, int div ) 
{
	int r = MulDiv( GetRValue( c1 ), div - mul, div ) + MulDiv( GetRValue( c2 ), mul, div );
	int g = MulDiv( GetGValue( c1 ), div - mul, div ) + MulDiv( GetGValue( c2 ), mul, div );
	int b = MulDiv( GetBValue( c1 ), div - mul, div ) + MulDiv( GetBValue( c2 ), mul, div );
	return RGB( MinMax( 0, r, 255 ), MinMax( 0, g, 255 ), MinMax( 0, b, 255 ) );
}

inline COLORREF MixColor( COLORREF c1, COLORREF c2, int percent )
{
	return MixColor( c1, c2, percent, 100 );
}

inline COLORREF GetMaskColor( const CVector<COLORREF>& takenColors )
{
	for( CCombination c( 3, 256 ); !c.WasLast(); c.GetNext() ) {
		COLORREF color = RGB( c[0], c[1], c[2] );
		bool isTaken = false;
		for( int i = 0; !isTaken && i < takenColors.Size(); i++ ) {
			isTaken = ( color == takenColors[i] );
		}
		if( !isTaken ) {
			return color;
		}
	}
    presumeFalse;
	return RGB( 1, 1, 1 );
}

inline COLORREF InvertColor( COLORREF color ) 
{
	return RGB( ~GetRValue( color ), ~GetGValue( color ), ~GetBValue( color ) ) ;
}

//////////////////////////////////////////////////////////////////////////

inline int GetWcharWidth( HDC hDC, TCHAR letter )
{
	SIZE size = {0};
	GetTextExtentPoint32( hDC, &letter, 1, &size );
	return size.cx;
}

inline int GetTextWidth( HDC hDC, const TCHAR* text, int length )
{
	SIZE size = {0};
	GetTextExtentPoint32( hDC, text, length, &size );
	return size.cx;
}

inline int GetMaxWcharWidth( HDC hDC, const TCHAR* text, int length )
{
	int maxWidth = 0;
	for( int i = 0; i < length; i++ ) {
		int tmpWidth = GetWcharWidth( hDC, text[i] );
		if( maxWidth < tmpWidth ) {
			maxWidth = tmpWidth;
		}
	}
	return maxWidth;
}