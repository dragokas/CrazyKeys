#pragma once
#include <windows.h>
#include "Str.h"
#include "Queue.h"
#include "ShellPathFuncs.h"

class CPath {
public:
	CPath( CString _path, CPath* _parent = 0 );
	virtual ~CPath() {};
	
	DWORD GetAttributes() const { return attributes; }
	CPath* GetParent() const { return parent; }
	__int64 GetSize() const { return size.QuadPart; } 
	unsigned long GetSizeLo() const { return size.LowPart; } 
	unsigned long GetSizeHi() const { return size.HighPart; } 
	bool IsDirectory() const { return ( ( attributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ); }
	CString GetPath() const { return path; }
	CString GetName() const;
	CString GetExtension() const { return EmptyStr; };
	static CString GetExtension( const CString& fileName );
	static CString GetName( const CString& fileName );

	FILETIME GetCreateTime() const { return creationTime; }
	FILETIME GetLastAccessTime() const { return lastAccessTime; }
	FILETIME GetLastWriteTime() const { return lastWriteTime; }

	virtual void BuildTree() {};
	virtual void SortTree( int (* /*cmpFunc*/ )( CPath * const &, CPath * const & ) ) {};
	void Dettach();

protected:
	CString path;
	LARGE_INTEGER size;
	DWORD attributes;
	FILETIME creationTime;
	FILETIME lastAccessTime;
	FILETIME lastWriteTime;
	CPath* parent;

private:
	CPath( const CPath& ) {};//нет копирования
	CPath& operator= ( const CPath& ) {};//нет присваивания
};

class CDirPath : public CPath, public CList<CPath*> {
public:
	CDirPath( CString _path, CPath* _parent = 0 ) : CPath( _path, _parent ) {}
	virtual ~CDirPath();

	virtual void BuildTree();

	virtual void SortTree( int (*cmpFunc)( CPath * const &, CPath * const & ) );
private:
};

class CFilePath : public CPath {
public:
	CFilePath( CString _path, CPath* _parent = 0 ) : CPath( _path, _parent ) {}
	CString GetExtension() const { return CPath::GetExtension( path ); };
	virtual ~CFilePath() {};
};

bool ExistPath( CString path );
void ReceivePathList( CString path, CStrList& list, DWORD attributes, bool needFullPath = true, int depht = -1 );
void ReceivePathList( const CPath* path, CList<const CPath*>& list );

template<typename TypeName>
bool TreeWalk( CString path, 
			  bool (*onDirIn)(CString, TypeName*)= 0, 
			  bool (*onFile)(CString, TypeName*)= 0,
			  bool (*onDirOut)(CString, TypeName*)= 0, 
			  TypeName* pData = 0 )
{
	WIN32_FIND_DATA findData;
	presume( path.Last() != T('\\') );
	//CString searchPath = path + T("*.*");
	CString searchPath = MakePath( path, T("*.*") );
	HANDLE handle = FindFirstFile( searchPath.GetBuffer(), &findData );
	bool mayContinue = true;
	if( handle != INVALID_HANDLE_VALUE ) {
		do {
			CString tmpStr( MAX_PATH, findData.cFileName );
			if( tmpStr != T(".") && tmpStr != T("..") ) {
				if( ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ) {
					//CString dirName( MAX_PATH, (path + tmpStr + T("\\")) );
					CString dirName = MakePath( path, tmpStr );
					if( onDirIn != 0 ) {
						if( !onDirIn( dirName, pData ) ) {
							return false;
						}
					}
					if( !TreeWalk( dirName, onDirIn, onFile, onDirOut, pData ) ) {
						mayContinue = false;
						break;
					}
					if( onDirOut != 0 ) {
						if( !onDirOut( dirName, pData ) ) {
							return false;
						}
					}
				} else {
					if( onFile != 0 ) {
						if( !onFile( MakePath( path, tmpStr ), pData ) ) {
							return false;
						}
					}
				}
			}
		} while( FindNextFile( handle, &findData ) );
	}
	FindClose(handle);
	return mayContinue;
}
















































