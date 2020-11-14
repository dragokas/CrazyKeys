#include "TmpFileBody.h"

#include "Exception.h"

#include "StartDebugNew.h"

#define Cant_open_Str T("Can't open file")
#define Cant_read_Str T("Can't read file")
#define Cant_Create_Str T("Can't create or open file")
#define Cant_Write_Str T("Can't write to file")
#define Cant_Seek T("Can't seek in file")
#define No_File_Info_Str T("Can't get information about file")
#define ACII_2_Unicode_Failed_Str T("Can't convert ascii 2 unicode")
#define Unicode_2_ACII_Failed_Str T("Can't convert unicode 2 ascii")
#define Not_Text_File_Str T("This file is not text")

CTmpFileBody::CTmpFileBody( const CString& _fileName )
: hFile(INVALID_HANDLE_VALUE), pBuffer(0), fileSize(-1), fileName( _fileName )
{
}

CTmpFileBody::~CTmpFileBody()
{
	if( hFile != INVALID_HANDLE_VALUE ) {
		CheckZero( CloseHandle( hFile ) );
	}
	delete[] pBuffer;
}

void CTmpFileBody::TryToOpen() throw(CException)
{
	hFile = CreateFile( fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
	if( hFile == INVALID_HANDLE_VALUE ) {
		hFile = CreateFile( fileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
		if( hFile == INVALID_HANDLE_VALUE ) {
			throw CException( Cant_open_Str, fileName );
		}
	}
	TryToGetFileInfo();
}

void CTmpFileBody::TryToCreate() throw(CException)
{
	hFile = CreateFile( fileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
	if( hFile == INVALID_HANDLE_VALUE ) {
		hFile = CreateFile( fileName, GENERIC_WRITE, 0, 0, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
		if( hFile == INVALID_HANDLE_VALUE ) {
			throw CException( Cant_Create_Str, fileName );	
		}
	}
	TryToGetFileInfo();
}

void CTmpFileBody::TryToGetFileInfo() throw(CException)
{
	BY_HANDLE_FILE_INFORMATION fi;
	if( GetFileInformationByHandle( hFile, &fi ) == 0 ) {
		throw CException( No_File_Info_Str, fileName );
	}
	fileSize = fi.nFileSizeLow;
}

//////////////////////////////////////////////////////////////////////////

void CTmpFileBody::TryToRead( bool isTextFile /* = true */ ) throw(CException)
{
	assert( 0 <= fileSize );
	delete[] pBuffer;	
	pBuffer = new char[fileSize + 2];
	pBuffer[fileSize] = 0;
	DWORD bytesReaded = 0;
	if( fileSize < 4 || !isTextFile ) {//менее 4-х букв не может быть юникод, или просто сырым считать
		if( 0 < fileSize && ReadFile( hFile, pBuffer, fileSize, &bytesReaded, 0 ) == 0 ) {
			throw CException( Cant_read_Str, fileName );//не удалось считать
		}
		if( !isTextFile ) {//если ето не текст,
			return;//то ничего делать не надо
		} else {//иначе цепляем на строчку его
			setStr( charStr, pBuffer, fileSize );
		}
	} else {//в файле много букв
		if( ReadFile( hFile, pBuffer, 2, &bytesReaded, 0 ) == 0 ) {
			throw CException( Cant_read_Str, fileName );//не удалось считать первые 2 байта
		}
		if( (unsigned char)pBuffer[0] == 0xff && (unsigned char)pBuffer[1] == 0xfe ) {//юникодные первые 2 байтика
			if( ReadFile( hFile, pBuffer, fileSize - 2, &bytesReaded, 0 ) == 0 ) {
				throw CException( Cant_read_Str, fileName );//не удалось считать первые 2 байта
			}
			setStr( wcharStr, (wchar_t*)pBuffer, (fileSize - 2) / 2 );
		} else {//наверно не юникод
			if( ReadFile( hFile, pBuffer + 2, fileSize - 2, &bytesReaded, 0 ) == 0 ) {
				throw CException( Cant_read_Str, fileName );//не удалось считать за первыми 2-мя байтами
			}
			int uniTest = IS_TEXT_UNICODE_UNICODE_MASK;
			if( IsTextUnicode( pBuffer, fileSize, &uniTest ) != 0 ) {//100% юникод
				setStr( wcharStr, (wchar_t*)pBuffer, fileSize / 2 );
			} else {//не юникод
				setStr( charStr, (char*)pBuffer, fileSize );
			}			
		}
	}
	pBuffer = 0;
}

template<typename T>
void CTmpFileBody::setStr( CStr<T>& str, T* buffer, int strLength )
{
	buffer[strLength] = 0;
	str.AttachStr( buffer, strLength, strLength + 1 );
//	str = CStr<T>( strLength + 1, buffer );//размер буфера и буфер
//	delete buffer;
	buffer = 0;
}

void CTmpFileBody::ReceiveBuffer( CStr<char>& str ) throw(CException)
{
	if( charStr.IsEmpty() && !wcharStr.IsEmpty() ) {//аски строка пуста, а в юникод есть текст
		char* lpBuf = new char[wcharStr.GetLength() + 1];
		BOOL defChar = TRUE;
		if( WideCharToMultiByte( CP_ACP, WC_NO_BEST_FIT_CHARS, wcharStr.GetBuffer(), wcharStr.GetLength() + 1, 
            lpBuf, wcharStr.GetLength() + 1, 0, &defChar ) != wcharStr.GetLength() + 1 ) 
		{
			throw CException( Unicode_2_ACII_Failed_Str, fileName);
		}
		charStr.AttachStr( lpBuf, wcharStr.GetLength() );
	}
	str = charStr;//у нас есть готовый аски
}

void CTmpFileBody::ReceiveBuffer( CStr<wchar_t>& str ) throw(CException)
{
	if( wcharStr.IsEmpty() && !charStr.IsEmpty() ) {//юникод строка пуста, а в аски есть текст
		wchar_t* lpBuf = new wchar_t[fileSize + 2];
		if( MultiByteToWideChar( CP_THREAD_ACP, MB_ERR_INVALID_CHARS, charStr.GetBuffer(), charStr.GetLength() + 1, 
			lpBuf, charStr.GetLength() + 1 ) != charStr.GetLength() + 1 ) 
		{
			throw CException( ACII_2_Unicode_Failed_Str, fileName );
		}
		wcharStr.AttachStr( lpBuf, charStr.GetLength() );
	}
	str = wcharStr;//у нас есть готовый юникод
}

void CTmpFileBody::TryToWriteBufferStr( CStr<wchar_t> str, bool append /*= false*/ ) throw(CException)
{
	if( !append || fileSize < 2 ) {
		DWORD bytesWritten = 0;
		wchar_t unicodeMask[2]; unicodeMask[0] = 65279;
		if( WriteFile( hFile, unicodeMask, sizeof(wchar_t), &bytesWritten, 0) == 0 || bytesWritten != 2 ) {
			throw CException( Cant_Write_Str, fileName );
		}
	}
	TryToWriteBuffer( str.GetBuffer(), str.Length() * sizeof(wchar_t), append );
}

void CTmpFileBody::TryToWriteBufferStr( CStr<char> str, bool append /*= false*/ ) throw(CException)
{
	TryToWriteBuffer( str.GetBuffer(), str.Length() * sizeof(char), append );
}

void CTmpFileBody::TryToWriteBuffer( const void* buf, DWORD size, bool append /*= false*/ ) throw(CException)
{
	if( append && SetFilePointer( hFile, 0, 0, FILE_END ) == INVALID_SET_FILE_POINTER ) {
		throw CException( Cant_Seek, fileName );
	}
	DWORD bytesWritten = 0;
	if( WriteFile( hFile, buf, size, &bytesWritten, 0 ) == 0 ) {
		throw CException( Cant_Write_Str, fileName );
	}
	if( bytesWritten != size ) {
		throw CException( Cant_Write_Str, fileName );
	}
}

//////////////////////////////////////////////////////////////////////////

int GetFileLength( CString fileName )//-1 если файла нет
{
	CTmpFileBody fs( fileName );
	try {
		fs.TryToOpen();
		fs.TryToGetFileInfo();
	} catch( CException e ) {
		//e.ShowMessage();
		return -1;
	}
	return fs.GetFileSize();
}

bool ExistFile( CString fileName )
{
	return ( -1 < GetFileLength( fileName ) );
}



