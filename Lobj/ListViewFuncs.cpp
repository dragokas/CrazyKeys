#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <CommCtrl.h>

#include "ListViewFuncs.h"

void SetListViewItemSelected( HWND hList, int index )
{
	//ListView_SetSelectionMark( hList, index );
	ListView_SetItemState( hList, index, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED );
}


void InitListViewHeader( HWND hList, const CStrVector& headerWords, bool countScrollWidth )
{//������ ����� �����
	RECT rect;
	GetClientRect( hList, &rect );
	if( countScrollWidth ) {//���� ���� ������� ��� ����� �����, ������� � ������� �� �����
		rect.right -= min( rect.right, GetSystemMetrics( SM_CYHSCROLL ) );
	}

	LVCOLUMN colum;
	ZeroMemory( &colum, sizeof( LVCOLUMN ) );
	colum.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	colum.fmt = LVCFMT_LEFT;

	for( int i = 0; i < headerWords.Size(); i++ ) {
		colum.pszText = (TCHAR*)headerWords[i].GetBuffer(); 
		colum.cx = rect.right / headerWords.Size();
		SendMessage( (HWND) hList,(UINT) LVM_INSERTCOLUMN, (WPARAM)i, (LPARAM) &colum );
	}
}

void FillListViewItems( HWND hList, const CStrMatrix& items )
{//��������� ������ �����
	int selectedIndex = ListView_GetSelectionMark( hList );//���������� ��������
	LVITEM item; 
	ZeroMemory( &item, sizeof( LVITEM ) );
	int nListViewItemCount = ListView_GetItemCount( hList );
	while( nListViewItemCount < items.GetRowCount() ) {//��������� �����
		item.iItem = nListViewItemCount++;
		ListView_InsertItem( hList, &item );
	}
	for( ; items.GetRowCount() < nListViewItemCount; nListViewItemCount-- ) {//������� ������
		ListView_DeleteItem( hList, nListViewItemCount - 1 );
	}
	for( int i = 0; i < items.GetRowCount(); i++ ) {//��������� ����
		for( int j = 0; j < items.GetColumnCount(); j++ ) {
			CString tmp = items[i][j];
			ListView_SetItemText( hList, i, j, tmp.GetRowBuffer() );
		}
	}
	ListView_SetItemState( hList, -1, 0, LVIS_SELECTED);
	if( selectedIndex != -1 && 0 < nListViewItemCount ) {//�������������� ��������
		selectedIndex = max( 0, min( nListViewItemCount - 1, selectedIndex ) ); 
		ListView_SetItemState( hList, selectedIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED );
		ListView_SetSelectionMark( hList, selectedIndex );
		ListView_EnsureVisible( hList, selectedIndex, TRUE);
	}
}

//////////////////////////////////////////////////////////////////////////

void InitListViewHeader( HWND hList, const CString& headerWord, bool countScrollWidth )
{//������ ����� �����
	RECT rect;
	GetClientRect( hList, &rect );
	if( countScrollWidth ) {//���� ���� ������� ��� ����� �����, ������� � ������� �� �����
		rect.right -= min( rect.right, GetSystemMetrics( SM_CYHSCROLL ) );
	}

	LVCOLUMN colum;
	ZeroMemory( &colum, sizeof( LVCOLUMN ) );
	colum.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	colum.fmt = LVCFMT_LEFT;

	colum.pszText = (TCHAR*)headerWord.GetBuffer(); 
	colum.cx = rect.right;
	SendMessage( (HWND) hList,(UINT) LVM_INSERTCOLUMN, (WPARAM)0, (LPARAM) &colum );
}


void FillListViewItems( HWND hList, const CStrVector& items )
{//��������� ������ �����
	int selectedIndex = ListView_GetSelectionMark( hList );//���������� ��������
	LVITEM item; 
	ZeroMemory( &item, sizeof( LVITEM ) );
	int nListViewItemCount = ListView_GetItemCount( hList );
	while( nListViewItemCount < items.Size() ) {//��������� �����
		item.iItem = nListViewItemCount++;
		ListView_InsertItem( hList, &item );
	}
	for( ; items.Size() < nListViewItemCount; nListViewItemCount-- ) {//������� ������
		ListView_DeleteItem( hList, nListViewItemCount - 1 );
	}
	for( int i = 0; i < items.Size() ; i++ ) {//��������� ����
        CString tmp = items[i];
		ListView_SetItemText( hList, i, 0, tmp.GetRowBuffer() );
	}
	ListView_SetItemState( hList, -1, 0, LVIS_SELECTED);
	if( selectedIndex != -1 && 0 < nListViewItemCount ) {//�������������� ��������
		selectedIndex = max( 0, min( nListViewItemCount - 1, selectedIndex ) ); 
		ListView_SetItemState( hList, selectedIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED );
		ListView_SetSelectionMark( hList, selectedIndex );
		ListView_EnsureVisible( hList, selectedIndex, TRUE);
	}
}


















