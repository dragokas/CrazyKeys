#pragma once
#include <windows.h>
#include <commctrl.h>
#include "Str.h"
#include "Queue.h"

class CDirTreeViewCtrl {
public:
	void Init( HWND hTree, const CStrList& list );
	void OnNotify( WPARAM wParam, LPARAM lParam );
	const CStrList& GetSelectedList() const { return selectedList; }

private:
	void onGetDispInfo( LPARAM lParam );
	void onExpand( LPARAM lParam );
	void onClick();
	void onKeyDown( LPARAM lParam );
	void onCheckStateChange( HTREEITEM hItem );

	HTREEITEM addChildItem( HTREEITEM hRoot, CString text );
	void fillIconIndex( TV_INSERTSTRUCT& tvins, CString fullPathText ); 
	bool hasSubDirs(CString strDir);
	CString getFullPath( HTREEITEM hItem, bool needSlash = true );
	
	void updateParentsState( HTREEITEM hItem );
	void updateChildrenState( HTREEITEM hItem );

	void setSelectedList( const CStrList& list );
	bool isParentSelected( CString path );
	bool isBold( CString path );

	void addToSelected( CString path );
	void removeFromSelected( CString path );

	HWND hTree;
	HIMAGELIST hImageList16;
	CStrList selectedList;
};

CString GetTreeViewItemText( HWND hTree, HTREEITEM hItem, int bufSize = 512  );
























































