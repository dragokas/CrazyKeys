#pragma once
#include <shlobj.h>
#include "Str.h"

CString GetCurrentDirectory();
CString GetCurrentFileName();
CString GetFolderPath( int nFolder );
bool DeleteDirectory( const CString& dirName, bool deleteSubDirs = true );
void SetDirectoryCompress( const CString& dirName );

CString GetFolderName( HWND hParent, CString title, UINT flags = ( BIF_NEWDIALOGSTYLE | BIF_NONEWFOLDERBUTTON ));

CString MakePath( CString path1, CString path2 );
CString MakePath( const CString& path1, const CString& path2, const CString& path3 );
CString MakePath( const CString& path1, const CString& path2, const CString& path3, const CString& path4 );
CString MakeFullPath( const CString& path );
CString MakeFullPath( const CString& path1, const CString& path2 );

CString GetFileExt( const CString& fullFilePath );//расширение файла
CString GetFileName( const CString& fullFilePath );//БЕЗ расширения
CString GetFileNameExt( const CString& fullFilePath );//с расширением
CString GetFilePath( const CString& fullFilePath );//путь к файлу
void SplitFilePath( const CString& fullFilePath, CString& path, CString& name, CString& ext );
CString CombineFilePath( const CString& path, const CString& name, const CString& ext );
CString ChangeFileExtention( const CString& fullFilePath, const CString& newExt );

void CutIfRootDirStart( CString& fullPath, const CString& rootDir );