#include "ShellPathFuncs.h"

CString GetCurrentDirectory()
{
	TCHAR curDir[MAX_PATH];
	CheckZero( GetCurrentDirectory( MAX_PATH, curDir ) );
	return curDir;
}

CString GetCurrentFileName()
{
	TCHAR fileName[MAX_PATH];
	CheckZero( GetModuleFileName( 0, fileName, MAX_PATH ) ); 
	return fileName;
}

CString GetFolderPath( int nFolder )
{
	TCHAR path[MAX_PATH];
	HRESULT hRes = SHGetFolderPath( 0, nFolder | CSIDL_FLAG_CREATE, 0, 0, path );
	assert( SUCCEEDED( hRes ) );
	return path;
}

bool DeleteDirectory( const CString& dirName, bool deleteSubDirs )
{
	if( !deleteSubDirs ) {
		return ( RemoveDirectory( dirName.GetBuffer() ) != 0 );
	} else if( RemoveDirectory( dirName.GetBuffer() ) == 0 ) {
		SHFILEOPSTRUCT sh;
		ZeroData( sh ); 
		sh.wFunc = FO_DELETE;		
		TCHAR dName[MAX_PATH];
		ZeroData( dName );//необходимо, инече не работает
		dirName.CopyTo( dName, ( dirName.Last() == T('\\') ) ? dirName.Length() - 1 : dirName.Length() );		
		sh.pFrom = dName;		
		sh.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
		return ( SHFileOperation(&sh) == 0 );
	}
	return true;
}

void SetDirectoryCompress( const CString& dirName )
{
	HANDLE hDir = CreateFile( dirName.GetBuffer(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0 ); 
	USHORT c = COMPRESSION_FORMAT_DEFAULT; 
	DWORD dw = 0; 
	DeviceIoControl( hDir, FSCTL_SET_COMPRESSION, &c, sizeof(c), 0, 0, &dw, 0 );
	CloseHandle( hDir );
}

CString GetFolderName( HWND hParent, CString title, UINT flags /*= ( BIF_NEWDIALOGSTYLE | BIF_NONEWFOLDERBUTTON )*/)
{
	BROWSEINFO bi;
	ZeroMemory( &bi, sizeof( BROWSEINFO ) );
	TCHAR buffer[MAX_PATH];
	bi.hwndOwner = hParent;
	bi.pszDisplayName = buffer;
	bi.lpszTitle = title;//.GetBuffer();
	bi.ulFlags = flags;
	CoInitialize( 0 );//без неё BIF_NEWDIALOGSTYLE не работает
	LPITEMIDLIST result = SHBrowseForFolder( &bi );
	if( result != 0 && SHGetPathFromIDList( result, buffer ) == TRUE ) {
		CString resStr( MAX_PATH + 1, buffer );
		if( resStr.Last() != T('\\') ) {
			resStr += T("\\");
		}
		return resStr;
	}
	return EmptyStr;
}

//////////////////////////////////////////////////////////////////////////

CString MakePath( CString path1, CString path2 )
{
	presume( 0 < path1.Length() && 0 < path2.Length() );
	presume( path1.Last() != T('\\') );
	presume( path2.First() != T('\\') );
	presume( path2.Last() != T('\\') );
	if( path1.Last() == T('\\') ) {
		path1.CutEnd( 1 );
	}
	if( path2.First() == T('\\') ) {
		return path1 + path2;
	}
	return path1 + T('\\') + path2;
}

CString MakePath( const CString& path1, const CString& path2, const CString& path3 )
{
	return MakePath( MakePath( path1, path2 ), path3 );
}

CString MakePath( const CString& path1, const CString& path2, const CString& path3, const CString& path4 )
{
	return MakePath( MakePath( path1, path2, path3 ), path4 );
}

CString MakeFullPath( const CString& path )
{
	return MakePath( GetCurrentDirectory(), path );
}

CString MakeFullPath( const CString& path1, const CString& path2 )
{
	return MakePath( GetCurrentDirectory(), path1, path2 );
}

//////////////////////////////////////////////////////////////////////////

CString GetFileExt( const CString& fullFilePath )
{
	int i = fullFilePath.Length() - 1;
	for( i; fullFilePath[i] != L'.' && 0 < i; i-- ) {
		if( fullFilePath[i] == L'\\' ) {
			return EmptyStr;
		}
	}
	CString ext( fullFilePath.GetBuffer() + i + 1 );
	ext.ToLower();
	return ext;
}

CString GetFileName( const CString& fullFilePath )
{
	int i = fullFilePath.Length() - 1;
	for( i; 0 < i && fullFilePath[i] != T('.'); i-- ) {
		if( fullFilePath[i] == T('\\') ) {
			CString fileName( fullFilePath.GetBuffer() + i + 1, fullFilePath.Length() - i );
			return fileName;
		}
	}
	int j = i - 1;
	for( j; 0 < j && fullFilePath[j] != T('\\'); j-- ) {
	}
	CString fileName( fullFilePath.GetBuffer() + j + 1, i - j - 1 );
	return fileName;
}

CString GetFileNameExt( const CString& fullFilePath )
{
	int i = fullFilePath.Length() - 1;
	for( i; 0 < i && fullFilePath[i] != T('\\'); i-- ) {
	}
	CString fileName( fullFilePath.GetBuffer() + i + 1, fullFilePath.Length() - i );
	return fileName;
}

CString GetFilePath( const CString& fullFilePath )
{
	int i = fullFilePath.Length() - 1;
	for( i; 0 < i && fullFilePath[i] != T('\\'); i-- ) {
	}
	CString filePath( fullFilePath.GetBuffer(), i );
	return filePath;
}

void SplitFilePath( const CString& fullFilePath, CString& path, CString& name, CString& ext )
{
	path = GetFilePath( fullFilePath );
	name = GetFileName( fullFilePath );
	ext = GetFileExt( fullFilePath );
}

CString CombineFilePath( const CString& path, const CString& name, const CString& ext )
{
	return path + T('\\') + name + T('.') + ext;
}

CString ChangeFileExtention( const CString& fullFilePath, const CString& newExt )
{
	CString path = GetFilePath( fullFilePath );
	CString name = GetFileName( fullFilePath );
	return CombineFilePath( path, name, newExt );
}

void CutIfRootDirStart( CString& fullPath, const CString& rootDir )
{
	CString curDir = rootDir;
    CString fName = fullPath;
	curDir.ToLower();
	fName.ToLower();
	if( fName.Find( curDir ) == 0 ) {
		fullPath.Replace( 0, curDir.Length() + 1, EmptyStr );
	}
}









































