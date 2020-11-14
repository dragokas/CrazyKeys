#include "Path.h"

#include "ShellPathFuncs.h"

#include "StartDebugNew.h"

CPath::CPath( CString _path, CPath* _parent )
: path( _path ), parent ( _parent )
{
	WIN32_FILE_ATTRIBUTE_DATA fd;
	ZeroData( fd );
	GetFileAttributesEx( path.GetBuffer(), GetFileExInfoStandard, &fd );
	attributes = fd.dwFileAttributes;
	creationTime = fd.ftCreationTime;
	lastAccessTime = fd.ftLastAccessTime;
	lastWriteTime = fd.ftLastWriteTime;
	size.HighPart = fd.nFileSizeHigh;
	size.LowPart = fd.nFileSizeLow;
}

CString CPath::GetName() const
{
	int i = path.Length() - 2;
	for( i; path[i] != L'\\' && 0 < i; i-- );
	if( path[i] == T('\\') ) {
		i++;
	}
	CString name( path.GetBuffer() + i );
	if( name.Last() == L'\\' ) {
		name.CutEnd( 1 );
	}
	return name;
}

void CPath::Dettach()
{
	if( parent == 0 ) {
		return;
	}
	CDirPath* parentDir = static_cast<CDirPath*>(parent);
	int pos = parentDir->Find( this );
	if( pos == -1 ) {
		return;
	}
	parentDir->PopByIndex( pos );
	parentDir->size.QuadPart -= size.QuadPart;
}

CString CPath::GetExtension( const CString& fileName )
{
	int i = fileName.Length() - 1;
	for( i; fileName[i] != L'.' && 0 < i; i-- ) {
		if( fileName[i] == L'\\' ) {
			return EmptyStr;
		}
	}
	CString name( fileName.GetBuffer() + i + 1 );
	name.ToLower();
	return name;
}

CString CPath::GetName( const CString& fileName )
{
	int i = fileName.Length() - 1;
	for( i; 0 < i && fileName[i] != L'.'; i-- ) {
		if( fileName[i] == L'\\' ) {
			return EmptyStr;
		}
	}
	int j = i - 1;
	for( j; 0 < j && fileName[j] != L'\\'; j-- ) {
	}
	CString name( fileName.GetBuffer() + j + 1, i - j - 1 );
	return name;
}

CDirPath::~CDirPath()
{
	while( 0 < GetCount() ) {
		CPath* child = PopFromHead();
		delete child;
	}
}

void CDirPath::BuildTree() 
{
	WIN32_FIND_DATA findData;
	presume( path.Last() != T('\\') );
	//CString searchPath = path + T("*.*");
	CString searchPath = MakePath( path, T("*.*") );
	HANDLE handle = FindFirstFile( searchPath.GetBuffer(), &findData );
	if( handle != INVALID_HANDLE_VALUE ) {
		size.QuadPart = 0;
		do {
			CString tmpStr = findData.cFileName;
			if( tmpStr != T(".") && tmpStr != T("..") ) {
				CPath* child;
				if( ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ) {
					//child = new CDirPath( path + tmpStr + T("\\"), this );
					child = new CDirPath( MakePath( path, tmpStr ), this );
					child->BuildTree();
				} else {
					//child = new CFilePath( path + tmpStr, this );
					child = new CFilePath( MakePath( path, tmpStr ), this );
				}
				size.QuadPart += child->GetSize();
				PushToTail( child );
			}
		} while( FindNextFile( handle, &findData ) );
	}
	FindClose(handle);
}

void CDirPath::SortTree( int (*cmpFunc)( CPath* const &, CPath * const & ) ) 
{
	if( IsDirectory() ) {
		QuickSort( cmpFunc );
		for( int i = 0; i < GetCount(); i++ ) {
			(*this)[i]->SortTree( cmpFunc );
		}
	}
}

//------------------------

bool ExistPath( CString path )
{
	WIN32_FILE_ATTRIBUTE_DATA fd;
	ZeroData( fd );
	BOOL res = GetFileAttributesEx( path.GetBuffer(), GetFileExInfoStandard, &fd );
	return ( res != 0 );
}

//------------------------

void ReceivePathList( CString path, CStrList& list, DWORD attributes, bool needFullPath, int depht )
{
	if( depht == 0 ) {
		return;
	}
	WIN32_FIND_DATA findData;
	CString searchPath = path + T("*.*");
	HANDLE handle = FindFirstFile( searchPath.GetBuffer(), &findData );
	if( handle != INVALID_HANDLE_VALUE ) {
		do {
			CString tmpStr( MAX_PATH, findData.cFileName );
			if( tmpStr != T(".") && tmpStr != T("..") ) {
				if( ( findData.dwFileAttributes & attributes) != 0 ) {
					list.PushToTail( needFullPath ? path + tmpStr : tmpStr );
				}
				if( ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ) {
					CString dirName( MAX_PATH, (path + tmpStr + T("\\")) );
					ReceivePathList( dirName, list, attributes, needFullPath, depht - 1 );
				} 
			}
		} while( FindNextFile( handle, &findData ) );
	}
	FindClose(handle);
}


void ReceivePathList( const CPath* path, CList<const CPath*>& list )
{
	if( !path->IsDirectory() ) {
		list.PushToTail( static_cast<const CFilePath*>( path ) );
	} else {
		const CDirPath* dirPath = static_cast<const CDirPath*>( path );
		list.PushToTail( dirPath );
		for( int i = 0; i < dirPath->GetCount(); i++ ) {
			ReceivePathList( (*dirPath)[i], list );
		}
	}
}

//------------------------

