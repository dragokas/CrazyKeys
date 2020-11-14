#pragma once
#include <windows.h>
#include "Presume.h"

class CGdiObj {
protected:
	CGdiObj( HDC hDC, HGDIOBJ hNewObj ) : hdc( hDC ), hObj( hNewObj ) {}
	HDC hdc;
	HGDIOBJ hObj;
	HGDIOBJ hOldObj;
	void DeleteSelectedObject() {
		HGDIOBJ hTmpObj = SelectObject( hdc, hOldObj );
		presume( hTmpObj == hObj );
		DeleteObject( hObj );
	}
};

class CSelectPen : public CGdiObj {
public:
	CSelectPen( HDC hDC, COLORREF color, int width = 1, DWORD fnPenStyle = PS_ENDCAP_SQUARE | PS_SOLID ) 
		: CGdiObj( hDC, createPen( color, width, fnPenStyle ) )	
	{
		hOldObj = SelectObject( hdc, hObj );
	}
	~CSelectPen() { DeleteSelectedObject(); }

private:
	static HPEN createPen( COLORREF color, int width, DWORD fnPenStyle = PS_ENDCAP_SQUARE | PS_SOLID ) {
		fnPenStyle |= ( width == 1 ) ? PS_COSMETIC : PS_GEOMETRIC; 
		LOGBRUSH logBrush = {0};
		logBrush.lbColor = color;
		logBrush.lbStyle = BS_SOLID;
		return ExtCreatePen( fnPenStyle, width, &logBrush, 0, 0 );
	}
};

class CSelectBrush : public CGdiObj {
public:
	CSelectBrush( HDC hDC, COLORREF color )
		: CGdiObj( hDC, CreateSolidBrush( color ) ) {
		hOldObj = SelectObject( hdc, hObj );
	}
	~CSelectBrush() { DeleteSelectedObject(); }
};

class CSelectFont : public CGdiObj {
public:
	CSelectFont( HDC hDC, const LOGFONT* logFont )
		: CGdiObj( hDC, CreateFontIndirect( logFont ) ) {
		hOldObj = SelectObject( hdc, hObj );
	}
	~CSelectFont() { DeleteSelectedObject(); }
};

//////////////////////////////////////////////////////////////////////////\

// class CSelectGdiObj : public CGdiObj {
// public:
// 	CSelectGdiObj( HDC hDC, HGDIOBJ hNewObj ) : CGdiObj( hDC, hNewObj ) {
// 		presume( hObj != 0 );
// 		hOldObj = SelectObject( hdc, hObj );
// 	}
// 	~CSelectGdiObj() {
// 		HGDIOBJ hTmpObj = SelectObject( hdc, hOldObj );
// 		presume( hTmpObj == hObj );
// 	}
// };

//////////////////////////////////////////////////////////////////////////

struct CPoint : POINT  {
	CPoint() {}
	CPoint( int _x, int _y ) { x = _x; y = _y; }
	bool operator==( const CPoint& other ) const { return ( x == other.x && y == other.y ); }
	bool operator!=( const CPoint& other ) const { return !( *this == other ); }
};

struct CSize : SIZE {
	CSize() {}
	CSize( int _cx, int _cy ) { cx = _cx; cy = _cy; }
	bool operator==( SIZE other ) const { return ( cx == other.cx && cy == other.cy ); }
};

struct CRect : RECT {
	CRect() {}
	CRect( const RECT& other ) { left = other.left; top = other.top; right = other.right; bottom = other.bottom; }
	CRect( const CPoint& pt, const CSize& size ) 
	{ left = pt.x; top = pt.y; right = left + size.cx; bottom = top + size.cy; }
	CRect( int x, int y, const CSize& size )
	{ left = x; top = y; right = left + size.cx; bottom = top + size.cy; }
	CRect( int x1, int y1, int x2, int y2 )
	{ left = x1; top = y1; right = x2; bottom = y2; }
	
	int CX() const { return ( right - left ); }
	int CY() const { return ( bottom - top ); }
	CSize GetSize() const { return CSize( CX(), CY() ); }
	CPoint GetCentre() const { return CPoint( ( left + right ) / 2, ( top + bottom ) / 2 ); }

	CRect& operator+=( const CRect& other ) 
	{ left += other.left; top += other.top; right += other.right; bottom += other.bottom; return *this; }
	
	CRect& operator-=( const CRect& other ) 
	{ left -= other.left; top -= other.top; right -= other.right; bottom -= other.bottom; return *this; }
	
	bool operator==( const CRect& other ) const 
	{ return ( left == other.left && right == other.right && top == other.top && bottom == other.bottom ); }
	
	bool operator!=( const CRect& other ) const { return !( *this == other ); }

	void MulDiv( const CSize& multiplier, const CSize& divisor ) { 
		presume( divisor.cx != 0 && divisor.cy != 0 );
		left = ::MulDiv( left, multiplier.cx, divisor.cx );
		top = ::MulDiv( top, multiplier.cy, divisor.cy );
		right = ::MulDiv( right, multiplier.cx, divisor.cx );
		bottom = ::MulDiv( bottom, multiplier.cy, divisor.cy );
	}
};

//////////////////////////////////////////////////////////////////////////

class CBitmapDC {
public:
	CBitmapDC() : hBmpDC(0), hBitmap(0), hOldBitmap(0) {
		hDC = GetDC( 0 );
		bmpSize.cx = bmpSize.cy = 0;
		hBmpDC = CreateCompatibleDC( hDC ); 
	}
	~CBitmapDC() { DestroyBitmap(); DeleteDC( hBmpDC ); ReleaseDC( 0 , hDC ); }

	void CreateBitmap( const CSize& size ) { CreateBitmap( size.cx, size.cy ); }
	void CreateBitmap( int cx, int cy ) {
		if( hBitmap != 0 ) {
			DestroyBitmap();
		}
		hBitmap = CreateCompatibleBitmap( hDC, cx, cy );
		hOldBitmap = (HBITMAP)SelectObject( hBmpDC, (HBITMAP)hBitmap );
		bmpSize.cx = cx;
		bmpSize.cy = cy;
	}
	
	HDC GetBmpDC() const { presume( hBitmap != 0 ); return hBmpDC; }
	CSize GetBmpSize() const { return bmpSize; }
	HBITMAP GetBitmap() const { return hBitmap; }


	void DestroyBitmap() {
		if( hBitmap != 0 ) {
			SelectObject( hBmpDC, (HBITMAP)hOldBitmap );
			DeleteObject( hBitmap );
			hBitmap = 0;
		}
	}

private:

	HDC hDC;//оригинальный 
	HDC hBmpDC;//с загруженной битмапкой
	CSize bmpSize;//размер битмапки
	HBITMAP hBitmap;//сама битмапка
	HBITMAP hOldBitmap;//старая битмапка 1х1
};

//////////////////////////////////////////////////////////////////////////
























