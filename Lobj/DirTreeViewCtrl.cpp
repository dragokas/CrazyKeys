#include <shlobj.h>

#include "DirTreeViewCtrl.h"
#include "Path.h"

void CDirTreeViewCtrl::Init( HWND hTreeView, const CStrList& list )
{
	hTree = hTreeView;
	setSelectedList( list );
	HIMAGELIST hImageList32;
	Shell_GetImageLists( &hImageList32, &hImageList16 );
	TreeView_SetImageList( hTree, hImageList16, TVSIL_NORMAL );
	TreeView_DeleteAllItems( hTree );
	TCHAR drives[500];
	GetLogicalDriveStrings( 500, drives );
	for( TCHAR* pos = drives; *pos != 0; pos += 4 ) {
		CString text( pos, 2 );
		if( text.FindOneOf( T("AaBb") ) == -1 ) {//не флопик чтобы его не дёргать
			addChildItem( NULL, text );
		}
	}
}

void CDirTreeViewCtrl::OnNotify( WPARAM /*wParam*/, LPARAM lParam )
{
	NMHDR* pNMHDR = (NMHDR*)lParam;
	if( pNMHDR->hwndFrom == hTree ) {	
		switch( pNMHDR->code ) {
			case NM_CLICK: onClick(); break;
			case TVN_KEYDOWN: onKeyDown( lParam ); break;
			case TVN_ITEMEXPANDING: onExpand( lParam );	break;
			case TVN_GETDISPINFO: onGetDispInfo( lParam ); break;
		}
	}
}

void CDirTreeViewCtrl::onGetDispInfo( LPARAM lParam )
{
	NMTVDISPINFO* pDI = (NMTVDISPINFO*)lParam;
	pDI->item.cChildren = ( hasSubDirs( getFullPath( pDI->item.hItem ) ) ? 1 : 0 );
}

void CDirTreeViewCtrl::onExpand( LPARAM lParam )
{
	NMTREEVIEW* pTV = (NMTREEVIEW*)lParam;
    if( pTV->action == TVE_EXPAND ) {
		HTREEITEM hItem = pTV->itemNew.hItem;
		for( HTREEITEM hRemove = TreeView_GetChild( hTree, hItem );
			hRemove != 0; hRemove = TreeView_GetChild( hTree, hItem ) )
		{
			TreeView_DeleteItem( hTree, hRemove ); 
		}
		CStrList list;
		ReceivePathList( getFullPath( hItem ), list, FILE_ATTRIBUTE_DIRECTORY, false, 1 );
		for( int i = 0; i < list.GetCount(); i++ ) {
			addChildItem( hItem, list[i] );
		}
	}
}

void CDirTreeViewCtrl::onClick()
{
	TVHITTESTINFO ht = {0};
	ZeroData( ht );
	GetCursorPos( &ht.pt );
	ScreenToClient( hTree, &ht.pt );
	HTREEITEM hItem = TreeView_HitTest( hTree, &ht );
	if( ( ht.flags & TVHT_ONITEMSTATEICON ) != 0 ) {
		onCheckStateChange( hItem );
	}
	TreeView_SelectItem( hTree, hItem );
}

void CDirTreeViewCtrl::onKeyDown( LPARAM lParam )
{
	NMTVKEYDOWN* pTVKD = (NMTVKEYDOWN*)lParam;
	if( pTVKD->wVKey == VK_SPACE ) {
		onCheckStateChange( TreeView_GetSelection( hTree ) );
	}
}

void CDirTreeViewCtrl::onCheckStateChange( HTREEITEM hItem )
{
	if( hItem != 0 ) {	
		bool isCheckAdd = ( TreeView_GetCheckState( hTree, hItem ) == 0 );
		if( isCheckAdd ) {
			addToSelected( getFullPath( hItem ) );
		} else {
			removeFromSelected( getFullPath( hItem ) );
		}
		TreeView_SetItemState( hTree, hItem, isCheckAdd ? TVIS_BOLD : 0, TVIS_BOLD );
		updateParentsState( hItem );
		updateChildrenState( hItem );
	}
}

HTREEITEM CDirTreeViewCtrl::addChildItem( HTREEITEM hRoot, CString text )
{
	TV_INSERTSTRUCT tvins = {0};
	ZeroData( tvins );
	tvins.hParent = hRoot;
	tvins.hInsertAfter = ( hRoot == 0 ) ? TVI_LAST : TVI_SORT;
	tvins.item.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvins.item.stateMask = TVIS_BOLD | TVIS_STATEIMAGEMASK;
	tvins.item.cchTextMax = MAX_PATH;
	tvins.item.pszText = const_cast<LPTSTR>((LPCTSTR)text.GetBuffer());
	CString fullPathText = getFullPath( hRoot ) + text + T("\\");
	fillIconIndex( tvins, fullPathText );
	tvins.item.cChildren = I_CHILDRENCALLBACK;//hasSubDirs( fullPathText ) ? 1 : 0;
	DWORD boldState = isBold( fullPathText ) ? TVIS_BOLD : 0;
	DWORD checkState = INDEXTOSTATEIMAGEMASK( (isParentSelected( fullPathText ) ? 2 : 1) );
	tvins.item.state = boldState | checkState;
	return TreeView_InsertItem( hTree, &tvins );
}

void CDirTreeViewCtrl::fillIconIndex( TV_INSERTSTRUCT& tvins, CString fullPathText )
{
	SHFILEINFO shFinfo;
	if( SHGetFileInfo( fullPathText, 0, &shFinfo, sizeof(shFinfo), SHGFI_ICON | SHGFI_SMALLICON ) != 0 ) {
        tvins.itemex.iImage = shFinfo.iIcon;
		DestroyIcon( shFinfo.hIcon );
	}
	if( SHGetFileInfo( fullPathText, 0, &shFinfo, sizeof(shFinfo), SHGFI_ICON | SHGFI_OPENICON | SHGFI_SMALLICON ) != 0 ) {
		tvins.itemex.iSelectedImage = shFinfo.iIcon;
		DestroyIcon( shFinfo.hIcon );
	}
}

CString CDirTreeViewCtrl::getFullPath( HTREEITEM hItem, bool needSlash )
{
	CString fullPath;
	for( ; hItem != 0; hItem = TreeView_GetParent( hTree, hItem ) ) {
		fullPath.Insert( 0, T("\\") );
		fullPath.Insert( 0, GetTreeViewItemText( hTree, hItem ) );
	}
	return ( !fullPath.IsEmpty() && fullPath.Last() != T('\\') && needSlash ) ? fullPath + T("\\") : fullPath;
}

bool CDirTreeViewCtrl::hasSubDirs( CString strDir )
{
	if( strDir.Last() != T('\\') ) {
		strDir += T("\\");
	}
	WIN32_FIND_DATA findData;
	CString searchPath = strDir + T("*.*");
	HANDLE handle = FindFirstFile( searchPath.GetBuffer(), &findData );
	if( handle != INVALID_HANDLE_VALUE ) {
		do {
			CString tmpStr = findData.cFileName;
			if( tmpStr != T(".") && tmpStr != T("..") && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 ) {
				FindClose(handle);
				return true;
			}
		} while( FindNextFile( handle, &findData ) );
	}
	FindClose(handle);
	return false;
}

void CDirTreeViewCtrl::updateParentsState( HTREEITEM hItem )
{
	for( hItem = TreeView_GetParent( hTree, hItem ); hItem != 0; hItem = TreeView_GetParent( hTree, hItem ) ) {
		CString fullPath = getFullPath( hItem );
		DWORD boldState = isBold( fullPath ) ? TVIS_BOLD : 0;
		DWORD checkState = INDEXTOSTATEIMAGEMASK( (isParentSelected( fullPath ) ? 2 : 1) );
		TreeView_SetItemState( hTree, hItem, boldState | checkState, TVIS_BOLD | TVIS_STATEIMAGEMASK );
	}
}

void CDirTreeViewCtrl::updateChildrenState( HTREEITEM hItem )
{
	for( hItem = TreeView_GetChild( hTree, hItem ); hItem != 0; hItem = TreeView_GetNextSibling( hTree, hItem ) ) {
		CString fullPath = getFullPath( hItem );
		DWORD boldState = isBold( fullPath ) ? TVIS_BOLD : 0;
		DWORD checkState = INDEXTOSTATEIMAGEMASK( (isParentSelected( fullPath ) ? 2 : 1) );
		TreeView_SetItemState( hTree, hItem, boldState | checkState, TVIS_BOLD | TVIS_STATEIMAGEMASK );
		updateChildrenState( hItem );
	}
}

void CDirTreeViewCtrl::setSelectedList( const CStrList& list )
{
	for( int i = 0; i < list.GetCount(); i++ ) {
		CString tmp = list[i];
		tmp.ToLower();
		selectedList.PushToTail( tmp );
	}
}

bool CDirTreeViewCtrl::isBold( CString path )
{
	path.ToLower();
	for( int i = 0; i < selectedList.GetCount(); i++ ) {
		if( selectedList[i].Find( path ) == 0 || path.Find( selectedList[i] ) == 0 ) {
			return true;
		}
	}
	return false;
}

bool CDirTreeViewCtrl::isParentSelected( CString path )
{
	path.ToLower();
	for( int i = 0; i < selectedList.GetCount(); i++ ) {
		if( path.Find( selectedList[i] ) == 0 ) {
			return true;
		}
	}
	return false;
}

void CDirTreeViewCtrl::addToSelected( CString path )
{
	path.ToLower();
	for( int i = selectedList.GetCount() - 1; 0 <= i; i-- ) {
		if( path.Find( selectedList[i] ) == 0 ) {
			return;
		}
		if( selectedList[i].Find( path ) == 0 ) {
			selectedList.PopByIndex( i );
		}		
	}
	selectedList.PushToTail( path );
}

void CDirTreeViewCtrl::removeFromSelected( CString path )
{
	path.ToLower();
	for( int i = selectedList.GetCount() - 1; 0 <= i; i-- ) {
		if( path.Find( selectedList[i] ) == 0 ) {
			CString parentPath = selectedList.PopByIndex( i );
			if( parentPath != path ) {
				CStrList list;
				ReceivePathList( parentPath, list, FILE_ATTRIBUTE_DIRECTORY, true, 1 );
				for( int j = 0; j < list.GetCount(); j++ ) {
					selectedList.PushToTail( list[j].ToLower() + T("\\") );
				}
				removeFromSelected( path );
			}

		}		
	}
}

CString GetTreeViewItemText( HWND hTree, HTREEITEM hItem, int bufSize )
{
	CString tmpStr( bufSize );
	TVITEM tvItem = {0};
	ZeroData( tvItem );
	tvItem.hItem = hItem;
	tvItem.mask = TVIF_TEXT;
	tvItem.cchTextMax = bufSize;
	tvItem.pszText = tmpStr.GetRowBuffer();//buffer;
	TreeView_GetItem( hTree, &tvItem );
	tmpStr.ValidateStrBuffer();
	return tmpStr;
}















































