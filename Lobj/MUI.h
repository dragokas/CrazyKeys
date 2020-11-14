#pragma once
#include "Vector.h"

void SetDictionary( const CStrVector& source, const CStrVector& target );
CString TranslateStr( const CString& str );
void TranslateDlg( HWND hDlg );
void TranslateMenu( HMENU hMenu );