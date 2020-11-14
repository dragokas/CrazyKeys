#pragma once

#include "StopDebugNew.h"

#include "Presume.h"
#include "StrBody.h"
#include "StrFuncs.h"
#include "Archive.h"

//#include "StartDebugNew.h"
//#include "StopDebugNew.h"//очень долго

template <typename CharType> class CStr;
typedef CStr<TCHAR> CString;
typedef CStr<wchar_t> CUnicodeString;
typedef CStr<char> CAsciiString;

#define EmptyStr CString()

template<typename CharType> void Serialize( CArchive& archive, CStr<CharType>& s );

template<typename CharType = TCHAR>
class CStr {
public:
	typedef CharType ElementType;
	//конструкторы
	CStr();//конструктор по умолчанию
	CStr( const CStr& str );//конструктор копирования
	explicit CStr( int initBufferSize );//конструктор по длине зарезервированного буфера
	//конструктор, по дине строки и по выделяемому буфферу
	CStr( const CharType* wchar, int strLenght = -1, int bufSize = -1/*, bool needAllocBuffer = true*/ );
	CStr( int bufSize, const CharType* wchar);//по размеру буфера и строке
	//деструктор
	~CStr() { presume( body != 0 && 0 < body->nRef ); body->DecRef(); }
	//операторы присваивания
	CStr& operator=( const CStr& str );//оператор присваивания
	CStr& operator=( const CharType* wchar );//оператор присваивания CharType*
	
	//сравнения
	bool operator==(const CStr& str) const { return (StrCmp(body->buffer, str.body->buffer) == 0); }
	bool operator!=(const CStr& str) const { return (StrCmp(body->buffer, str.body->buffer) != 0); }
	bool operator<=(const CStr& str) const { return (StrCmp(body->buffer, str.body->buffer) <= 0); }
	bool operator<(const CStr& str) const { return (StrCmp(body->buffer, str.body->buffer) < 0); }
	bool operator==(const CharType* wchar) const { return (StrCmp(body->buffer, wchar) == 0); }
	bool operator!=(const CharType* wchar) const { return (StrCmp(body->buffer, wchar) != 0); }
	bool operator<=(const CharType* wchar) const { return (StrCmp(body->buffer, wchar) <= 0); }
	bool operator<(const CharType* wchar) const { return (StrCmp(body->buffer, wchar) < 0); }

	CStr& operator+=(const CStr& str) { return Append( str.body->buffer, str.body->length ); }
	CStr& operator+=(const CharType* wchar) { return Append( wchar, StrLen( wchar ) ); }
	CStr& operator+=(const CharType wchar) { CharType wchar3[2] = { wchar, 0 }; return Append( wchar3 ); }
	
	CStr& Append( const CStr& str ) { return Append( str, str.Length() ); }
	CStr& Append(const CharType* wchar, int len = -1);
	CStr& Append( const CharType wchar );

	CStr<CharType>& ToLower();
	CStr<CharType>& ToUpper();

	operator const CharType* const() const { return body->buffer; };//оператор преобразования из класса в константный указатель на буфер
	const CharType* GetBuffer() const { return body->buffer; }//отдача константного указателя на буфер

	CharType* GetRowBuffer() { MakeUnique(); return body->buffer; }
	void ValidateStrBuffer() { body->length = StrLen( body->buffer ); presume( body->length < body->bufferSize ); }

	int GetLength() const { return body->length; }
	int Length() const { return body->length; }
	int GetBufSize() const { return body->bufferSize; }
	void SetBufferSize( int newSize ) { MakeUnique( true, newSize ); }

	bool IsEmpty() const { return (body->length == 0); }
	void SetEmpty() { MakeUnique( false ); body->buffer[0] = 0; body->length = 0; }	

	CharType First() const { return body->buffer[0]; }
	CharType Last() const { if( body->length == 0 ) return 0; return body->buffer[body->length - 1]; }
	CharType operator[](int index) const { presume(0 <= index && index <= body->length); return body->buffer[index]; }
	void SetChar( CharType ch, int index ) { presume(0 <= index && index < body->length); MakeUnique(); body->buffer[index] = ch; }
	void SetAt( int index, CharType ch ) { presume(0 <= index && index < body->length); MakeUnique(); body->buffer[index] = ch; }

	void Trunc( int pos ) { MakeUnique( true ); presume( 0 <= pos && pos <= Length() ); body->length = pos; body->buffer[pos] = 0; }
	void CutEnd( int countFromEnd ) { Trunc( body->length - countFromEnd ); }

	int FindOneOf( const CharType* charSet, int pos = 0 ) const;

	int Find( CharType symbol, int pos = 0 ) const;
	int Find( const CStr<CharType>& subStr, int pos = 0 ) const; 
	int Find( const CharType* subStr, int pos = 0 ) const;
	
	int PosCount( CharType subStr ) const 
	{ return ::GetStrPosCount( body->buffer, subStr, body->length ); }
	int PosCount( const CStr<CharType>& subStr) const 
	{ return ::GetStrPosCount( body->buffer, subStr.body->buffer, body->length, subStr.body->length ); }
	int PosCount( const CharType* subStr, int subStrLen = -1 ) const 
	{ return ::GetStrPosCount( body->buffer, subStr, body->length, subStrLen ); }

	void ReplaceAll( const CharType* subStrOld, const CharType* subStrNew, int lenOld = -1, int lenNew = -1 );
	void Replace( int fromPos, int toPos, const CharType* wchar, int len = -1 );

	void RemoveAll( const CharType* subStr );
	void Remove(int position, int count = 1);
	void Remove(const CharType* subStr, int len = -1);
	void Remove( CharType wchar );

	void Insert( int position, const CharType wchar );
	void Insert( int position, const CharType* wchar, int len = -1 );

	void MakeUnique( bool copyBuffer = true, int newBufSize = -1 );

	void AttachStr( CharType* newBuffer, int strLength = -1, int bufferSize = -1 );

	CStr GetLeft( int index ) const;//строка слева от индекса
	CStr GetRight( int index ) const;//строка справа от индекса
	bool Split( CStr& leftStr, const CharType* subStr, CStr& rightStr ) const;//разбивает исходную на 2 строки

	void CopyTo( CharType* buffer, int nCharsToCopy ) const; 

private:
	void replaceAll( const CharType* subStrOld, const CharType* subStrNew, int len );

	void swap( CStr<CharType>& s ) {
		Swap( body, s.body );
	}

	CStrBody<CharType>* body;

	friend void Serialize<CharType>( CArchive& archive, CStr<CharType>& s );
};

//////////////////////////////////////////////////////////////////////////
//общий конструктор
template<typename CharType>
CStr<CharType>::CStr() 
{
	static CStr<CharType> emptyStr( 1 );//пустая строка одна на всех
	body = emptyStr.body;//
	body->IncRef();//удалиться она когда статик строка удалится
}

template<typename CharType>
CStr<CharType>::CStr( const CStr& str ) 
{
	body = str.body;
	body->IncRef();	
}

template<typename CharType>
CStr<CharType>::CStr( int initBufferSize ) 
{
	presume( 0 < initBufferSize );
	body = new CStrBody<CharType>( max( 1, initBufferSize ) );
}

template<typename CharType>
CStr<CharType>::CStr( const CharType* wchar, int strLenght /*= -1*/, int bufSize /*= -1*//*, bool needAllocBuffer /*= true*/ )
{
	body = new CStrBody<CharType>( wchar, strLenght, bufSize );
}

template<typename CharType>
CStr<CharType>::CStr( int bufSize, const CharType* wchar )
{
	body = new CStrBody<CharType>( wchar, -1, bufSize, true );
}

template<typename CharType>
CStr<CharType>& CStr<CharType>::operator= ( const CStr& str ) 
{
	if( this != &str ) {
		body->DecRef();
		body = str.body;
		body->IncRef();	
	}
	return *this;
}

template<typename CharType>
CStr<CharType>& CStr<CharType>::operator=( const CharType* wchar ) 
{
	CStrBody<CharType>* pBody = body;
	body = new CStrBody<CharType>(wchar);
	pBody->DecRef();
	return *this;
}

/////////////////////////////////// склеивание строк ///////////////////////////////////////

template <typename CharType>
CStr<CharType> Concatenate( const CharType* str1, const CharType* str2, int len1 = -1, int len2 = -1 )
{
	checksetStrLength( str1, len1 );
	checksetStrLength( str2, len2 );
	CStr<CharType> res( str1, len1, len1 + len2 + 1 );
	return res.Append( str2, len2 );
}

template <typename CharType >
CStr<CharType> operator+( const CharType* wchar, const CStr<CharType>& str )
{
	return Concatenate<CharType>( wchar, str, -1, str.Length() );
}

template <typename CharType>
CStr<CharType> operator+( const CStr<CharType>& str, const CharType* wchar )
{
	return Concatenate<CharType>( str, wchar, str.Length(), -1 );
}

template <typename CharType>
CStr<CharType> operator+( CharType wchar, const CStr<CharType>& str )
{
	CharType wch[2] = { wchar, 0 };
	return Concatenate<CharType>( wch, str, -1, str.Length() );
}

template <typename CharType>
CStr<CharType> operator+( const CStr<CharType>& str, CharType wchar )
{
	CharType wch[2] = { wchar, 0 };
	return Concatenate<CharType>( str, wch, str.Length(), -1 );
}

template <typename CharType>
CStr<CharType> operator+( const CStr<CharType>& str1, const CStr<CharType>& str2 )
{
	return Concatenate<CharType>( str1, str2, str1.Length(), str2.Length() );
}

template <typename CharType>
CStr<CharType>& CStr<CharType>::Append( const CharType* wchar, int len = -1 )
{
	checksetStrLength( wchar, len );
	MakeUnique( true, body->length + len + 1 );
	MemCpy( body->buffer + body->length, wchar, sizeof(CharType) * len );
	body->length += len;
	body->buffer[body->length] = 0;
	return *this;
}

template <typename CharType>
CStr<CharType>& CStr<CharType>::Append( const CharType wchar )
{
	CharType wstr[] = { wchar, 0 };
	Append( wstr, 1 );
	return *this;
}

//////////////////////////////////////////////////////////////////////////
////////////////////////поиски подстрок///////////////////////////////////
//////////////////////////////////////////////////////////////////////////
template <typename CharType>
int CStr<CharType>::FindOneOf( const CharType* charSet, int pos = 0 ) const
{
	presume( 0 <= pos && pos <= Length() );
	for( int i = pos; i < Length(); i++ ) {
		if( ::GetChrPos( charSet, body->buffer[i] ) != -1 ) {
			return i;
		}
	}
	return -1;
}

template <typename CharType>
int CStr<CharType>::Find( CharType symbol, int pos = 0 ) const
{
	presume( 0 <= pos && pos <= Length() );
	int res = ::GetChrPos( body->buffer + pos, symbol );
	if( res == -1 ) {
		return -1;
	}
	return res + pos;
}

template <typename CharType>
int CStr<CharType>::Find( const CStr<CharType>& substring, int pos = 0 ) const
{
	presume( 0 <= pos && pos <= Length() );
	int res = ::GetStrPos( body->buffer + pos, substring.body->buffer, body->length - pos, substring.Length() );
	if( res == -1 ) {
		return -1;
	}
	return res + pos;
}

template <typename CharType>
int CStr<CharType>::Find( const CharType* substring, int pos = 0 ) const
{
	presume( 0 <= pos && pos <= Length() );
	int res = ::GetStrPos( body->buffer + pos, substring, body->length - pos, -1 );
	if( res == -1 ) {
		return -1;
	}
	return res + pos;
}
//////////////////////////////////////////////////////////////////////////
template<typename CharType>
CStr<CharType>& CStr<CharType>::ToLower()
{
	MakeUnique();
	for( int i = 0; i < body->length; i++ ) {
#ifdef _UNICODE
		body->buffer[i] = towlower( body->buffer[i] );
#else
		body->buffer[i] = (TCHAR)tolower( body->buffer[i] );
#endif
	}
	return *this;
}

template<typename CharType>
CStr<CharType>& CStr<CharType>::ToUpper()
{
	MakeUnique();
	for( int i = 0; i < body->length; i++ ) {
#ifdef _UNICODE
		body->buffer[i] = towupper( body->buffer[i] );
#else
		body->buffer[i] = toupper( body->buffer[i] );
#endif
	}
	return *this;
}
//////////////////////////////////////////////////////////////////////////
template<typename CharType>
void CStr<CharType>::ReplaceAll( const CharType* subStrOld, const CharType* subStrNew, int lenOld = -1, int lenNew = -1 )
{
	checksetStrLength( subStrOld, lenOld);
	checksetStrLength( subStrNew, lenNew);
	assert( 0 <= lenOld && 0 <= lenNew );
	if( lenOld == lenNew ) {
		replaceAll( subStrOld, subStrNew, lenOld );
		return;
	}	
	int posCount = PosCount( subStrOld, lenOld );
	CStrBody<CharType>* oldBody = body;
	oldBody->IncRef();//чтобы не потерять
	MakeUnique( false, 1 + max( body->length,body->length + ( ( lenNew - lenOld ) * posCount ) ) );
	int startposOld = 0;
	int startposNew = 0;
	for( int i = 0; i < posCount; i++ ) {
		int pos = ::GetStrPos( oldBody->buffer + startposOld, subStrOld, oldBody->length - startposOld, lenOld );	
		MemCpy( body->buffer + startposNew, oldBody->buffer + startposOld, pos * sizeof(CharType) );		
		MemCpy( body->buffer + startposNew + pos, subStrNew, lenNew * sizeof(CharType) );
		startposOld += (pos + lenOld);
		startposNew += (pos + lenNew);
	}
	MemCpy( body->buffer + startposNew, oldBody->buffer + startposOld, ( oldBody->length - startposOld + 1 ) * sizeof(CharType) );
	body->length = oldBody->length + ((lenNew - lenOld) * posCount);
	oldBody->DecRef();
	presume( body->length == StrLen( body->buffer ) );
}

template<typename CharType>
void CStr<CharType>::replaceAll(const CharType* subStrOld, const CharType* subStrNew, int len)
{
	checksetStrLength( subStrOld, len );
	checksetStrLength( subStrNew, len );
	MakeUnique( true );
	int startpos = 0;
	for( ; ; ) {
		int pos = ::GetStrPos(body->buffer + startpos, subStrOld, body->length - startpos, len );	
		if( pos == -1 ) {
			break;
		}
		MemCpy( body->buffer + startpos + pos, subStrNew, sizeof(CharType) * len );
		startpos += (pos + len);
	}
	presume( body->length == StrLen( body->buffer ) );
}

//////////////////////////////////////////////////////////////////////////
template<typename CharType>
void CStr<CharType>::Remove( int position, int count = 1 )
{
	presume( 0 <= position && position < body->length && 0 <= count );
	if( body->IsUnique() ) {
		MemCpy( body->buffer + position, body->buffer + position + count, sizeof(CharType) * ( body->length - position - count + 1 ) );
		body->length -= count;
	} else {
		CStrBody<CharType>* oldBody = body;
		body = new CStrBody<CharType>( oldBody->buffer, position, oldBody->bufferSize );
		MemCpy( body->buffer + position, oldBody->buffer + position + count, sizeof(CharType) * ( oldBody->length - position - count + 1 ) );
		body->length = oldBody->length - count;
		oldBody->DecRef();
	}	
}

template<typename CharType>
void CStr<CharType>::Remove(const CharType* subStr, int len = -1)
{
	checksetStrLength( subStr, len );
	MakeUnique();
	int startposOld = 0;
	int startposNew = 0;
	int pos = ::StrPos( body->buffer + startposOld, subStr, body->length - startposOld, len );
	int posCount = 0;
	for( int i = 0; pos != -1; i++, posCount++ ) {
		
		MemCpy( body->buffer + startposNew, body->buffer + startposOld, pos * sizeof(CharType) );		
		startposOld += (pos + len);
		startposNew += pos;
		pos = ::StrPos( body->buffer + startposOld, subStr, body->length - startposOld, len );	
	}
	MemCpy( body->buffer + startposNew, body->buffer + startposOld, ( body->length - startposOld + 1 ) * sizeof(CharType) );
	body->length -= (len * posCount);
	presume( body->length == StrLen( body->buffer ) );
}

template<typename CharType>
void CStr<CharType>::Remove( const CharType wchar )
{
	CharType wchar3[2] = { wchar, 0 };
	Remove( wchar3, 1 );
}

template<typename CharType>
void CStr<CharType>::RemoveAll( const CharType* subStr )
{
	ReplaceAll( subStr, EmptyStr );
}

//////////////////////////////////////////////////////////////////////////
template<typename CharType>
void CStr<CharType>::Insert( int position, const CharType wchar )
{
	CharType wchar3[2] = { wchar, 0 };
	Insert(position, wchar3, 1);
}

template<typename CharType>
void CStr<CharType>::Insert( int position, const CharType* wchar, int len )
{
	assert( 0 <= position && position <= body->length );
	checksetStrLength( wchar, len );
	MakeUnique( true, body->length + len + 1 );
	MemCpy( body->buffer + position + len, body->buffer + position, sizeof(CharType) * ( body->length - position ));//конец поподвинуть далеше
	MemCpy( body->buffer + position, wchar, sizeof(CharType) * len);//вставляемый кусочек
	body->length += len;
	body->buffer[body->length] = 0;
	presume( body->length == StrLen( body->buffer ) );
}

template<typename CharType>
void CStr<CharType>::Replace( int fromPos, int toPos, const CharType* wchar, int len )
{
	assert( 0 <= fromPos && fromPos <= body->length );
	assert( 0 <= toPos && toPos <= body->length );
	assert( fromPos <= toPos );
	checksetStrLength( wchar, len );
	MakeUnique( true, body->length + 1 + max( 0, len - ( toPos - fromPos ) ) );

	int delta = len - ( toPos - fromPos );
	if( 0 < delta ) {//раздвигаем, (двигаемся с конца, чтобы не затереть)
		for( int i = body->length; i >= toPos; i-- ) {
			body->buffer[i + delta] = body->buffer[i]; 
		}
	} else if( delta < 0 ) {//сдвигаем (двигаемся с начала, чтобы не затереть)
		int i;
		for( i = toPos; body->buffer[i] != '\0'; i++ ) {
			body->buffer[i + delta] = body->buffer[i];
		}
		body->buffer[i + delta] = 0;
	}
	for( int i = 0; i < len; i++ ) {//просто копируем
		body->buffer[fromPos + i] = wchar[i];
	}
	body->length += delta;
	presume( body->buffer[body->length] == 0 );
	presume( body->length == StrLen( body->buffer ) );
}

template<typename CharType>
CStr<CharType> CStr<CharType>::GetLeft( int index ) const
{
	assert( 0 <= index && index < body->length );
	if( index == 0 ) {
		return EmptyStr;
	}
	return CStr( body->buffer, index, index + 1 );
}

template<typename CharType>
CStr<CharType> CStr<CharType>::GetRight( int index ) const
{
	assert( 0 <= index && index <= body->length );
	if( index == body->length ) {
		return EmptyStr;
	}
	return CStr( body->buffer + index, body->length - index, body->length - index + 1 );
}

template<typename CharType>
bool CStr<CharType>::Split( CStr& leftStr, const CharType* subStr, CStr& rightStr ) const
{
	assert( &leftStr != this || &rightStr != this );
	int index = Find( subStr );
	if( index == -1 ) {
		return false;
	}
	leftStr = GetLeft( index );
	rightStr = GetRight( index + StrLen( subStr ) );		
	return true;
}

template<typename CharType>
void CStr<CharType>::CopyTo( CharType* buffer, int nCharsToCopy ) const
{
	MemCpy( buffer, body->buffer, nCharsToCopy * sizeof( CharType ) );
}

//////////////////////////////////////////////////////////////////////////

template<typename CharType>
void CStr<CharType>::MakeUnique( bool copyBuffer = true, int newBufSize = -1 )
{
	if( !body->IsUnique() || ( newBufSize != -1 && body->bufferSize < newBufSize ) ) {
		CStrBody<CharType>* oldBody = body;
		if( copyBuffer ) {
			body = new CStrBody<CharType>( oldBody->buffer, oldBody->length, (newBufSize == -1) ? oldBody->length + 1 : max(newBufSize, body->bufferSize) ); 
		} else {
			body = new CStrBody<CharType>( max(newBufSize, body->bufferSize) );
		}
		oldBody->DecRef();		
	}
}

template<typename CharType>
void CStr<CharType>::AttachStr( CharType* newBuffer, int strLength, int bufferSize /*= -1*/ )
{
	body->DecRef();
	body = new CStrBody<CharType>( newBuffer, strLength, bufferSize, false );
}

template<typename CharType>
void Serialize( CArchive& archive, CStr<CharType>& s )
{
	int length = s.Length();
	Serialize( archive, length );
	if( archive.IsReading() ) {
		CharType* pNewBuf = new CharType[length + 1];
		pNewBuf[length] = 0;		
		archive.Read( pNewBuf, sizeof( CharType ) * length, typeid( s ).name() );
		presume( pNewBuf[length] == 0 );
		CStr<CharType> copy;
		copy.AttachStr( pNewBuf, length, length + 1 );
		s.swap( copy );
	} else {
		archive.Write( s.GetBuffer(), sizeof( CharType ) * length, typeid( s ).name() );
	}
}

//////////////////////////////////////////////////////////////////////////
////////////////////////// функции имеющие отношение к строкам //////////
//////////////////////////////////////////////////////////////////////////

template <typename CharType>
int Char2Int( const CharType* wchar, int base = 10 )
{
	int sign = ( wchar[0] == ' ' ) ? -1: 1;
	if( sign == -1 ) {
		wchar++;
	}
	int res = 0;
	for( int i = 0; wchar[i] != '\0'; i++ ) {
		if( '0' <= wchar[i] && wchar[i] <= '9' ) {
			res = res*base + (wchar[i] - '0');
		} else {
			res = res*base + (_tolower(wchar[i]) - 'a' + 10);
		}
	}
	return res;
}

CString Int2Str( int number, int base = 10 );
int Str2Int( const CString& str, int base = 10 );

CString Int64ToStr( __int64 number, int base = 10 );
__int64 StrToInt64( const CString& str, int base = 10 );


CString GetLineBreak();

CStr<char> GetStrA( const wchar_t* buffer, int len = -1 );
CStr<char> GetStrA( const char* buffer, int len = -1 );
CStr<wchar_t> GetStrW( const char* buffer, int len = -1 );
CStr<wchar_t> GetStrW( const wchar_t* buffer, int len = -1 );

#ifdef UNICODE 
#define GetStr GetStrW
#else
#define GetStr GetStrA
#endif

CString GetLongestCommonSubsequence( const CString a, const CString b );

