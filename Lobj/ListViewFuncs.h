#pragma once
#include <windows.h>
#include "Queue.h"

#include "Vector.h"
#include "Matrix.h"

void SetListViewItemSelected( HWND hList, int index );

void InitListViewHeader( HWND hList, const CStrVector& headerWords, bool countScrollWidth );
void FillListViewItems( HWND hList, const CStrMatrix& items );

void InitListViewHeader( HWND hList, const CString& headerWord, bool countScrollWidth );
void FillListViewItems( HWND hList, const CStrVector& items );
