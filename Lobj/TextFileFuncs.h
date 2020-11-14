#pragma once
#include "TmpFileBody.h"
#include "Vector.h"
#include "Queue.h"
#include "Exception.h"

void RecieveStandartLineBreaks( CVector<CStr<char> >& lineBreaks );
void RecieveStandartLineBreaks( CVector<CStr<wchar_t> >& lineBreaks );

template<typename CharType>
bool ReadFileBuffer( const CString& fileName, CStr<CharType>& buffer, bool showExeption = false )
{
	try {
		CTmpFileBody file( fileName );
		file.TryToOpen();
		file.TryToRead();
		file.ReceiveBuffer( buffer );
	} catch( const CException& e ) {
		if( showExeption ) {
			e.ShowMessage();
		}
		return false;
	}
	return true;
}

template<typename CharType>
int FindOneOf( const CStr<CharType>& text, int startPos, const CVector<CStr<CharType> >& str, int& iStr ) 
{
	for( int i = startPos; i < text.Length(); i++ ) {
		for( int j = 0; j < str.GetSize(); j++ ) {
			if( StrDifPos<CharType>( text + i, str[j] ) == str[j].Length() ) {
				iStr = j;
				return i;
			}
		}
	}
	return -1;
}

template<typename CharType>
void AddItem( CVector<CStr<CharType> >& lines, const CStr<CharType>& line ) { lines.Append( line ); }

template<typename CharType>
void AddItem( CList<CStr<CharType> >& lines, const CStr<CharType>& line ) { lines.PushToTail( line ); }

template<typename CollectionType, typename CharType>
void AddItem( const CStr<CharType>& text, int from, int to, CollectionType& lines, bool allowEmptyLines )
{
	static CStr<CharType> emptyLine;
	if( from < to ) {
		AddItem( lines, CStr<CharType>( text + from, to - from ) );
	} else if( from == to && allowEmptyLines ) {
		AddItem( lines, emptyLine );
	}	
}

template<typename CollectionType, typename CharType>
void SplitText( const CStr<CharType>& text, const CVector<CStr<CharType> >& separators,
				CollectionType& lines, bool allowEmptyLines )
{
	if( text.Length() == 0 ) {
		return;
	}
	int curPos = 0;
	int iSeparator = 0;
	int nextSepStart = FindOneOf( text, curPos, separators, iSeparator );
	while( nextSepStart != -1 ) {
		AddItem( text, curPos, nextSepStart, lines, allowEmptyLines );
		curPos = nextSepStart + separators[iSeparator].Length();
		nextSepStart = FindOneOf( text, curPos, separators, iSeparator );
	}
	AddItem( text, curPos, text.Length(), lines, allowEmptyLines );
}


template<typename CollectionType>
bool ReadFileLines( const CString& fileName, CollectionType& lines, bool allowEmptyLines = true, bool showExeption = false )
{
	typedef CollectionType::ElementType StringType;//вытаскиваем из коллекции тип строка
	typedef StringType::ElementType CharType;//из строки вытаскиваем шаблон тип строки

	CStr<CharType> text;
	if( ReadFileBuffer( fileName, text, showExeption ) ) {
		CVector<CStr<CharType> > lineBreaks;
		RecieveStandartLineBreaks( lineBreaks );
		SplitText( text, lineBreaks, lines, allowEmptyLines );
		return true;
	}
	return false;
}

/*
template<typename CollectionType>
bool ReadFileLines( const CString& fileName, CollectionType& lines, bool allowEmptyLines = true, bool showExeption = false )
{
	return ReadFileLines<CollectionType, TCHAR>( fileName, lines, allowEmptyLines, showExeption );
}*/

//////////////////////////////////////////////////////////////////////////
template<typename CharType>
int GetCollectionSize( const CVector<CStr<CharType> >& lines ) { return lines.GetSize(); }
template<typename CharType>
int GetCollectionSize( const CList<CStr<CharType> >& lines ) { return lines.GetCount(); }

template<typename CollectionType, typename CharType>
void CombineText( const CollectionType& lines, const CStr<CharType>&  separator, CStr<CharType>& text )
{
	int bufferSize = ( separator.Length() * ( GetCollectionSize( lines ) - 1 ) ) + 1;
	for( int i = 0; i < GetCollectionSize( lines ); i++ ) {
		bufferSize += lines[i].Length();
	}
	text.Trunc( 0 );
	text.SetBufferSize( bufferSize );
	for( int i = 0; i < GetCollectionSize( lines ) - 1; i++ ) {
		presume( text.Length() + lines[i].Length() + separator.Length() < text.GetBufSize() )
		( text += lines[i] ) += separator;
	}
	if( 0 < GetCollectionSize( lines ) ) {
		text += lines.Last();
	}
}

template<typename CharType>
bool WriteFileBuffer( const CString& fileName, const CStr<CharType>& buffer, bool showExeption = false )
{
	try {
		CTmpFileBody body( fileName );
		body.TryToCreate();
		body.TryToWriteBufferStr( buffer );
	} catch( const CException& e ) {
		if( showExeption ) {
			e.ShowMessage();
		}
		return false;
	}
	return true;
}

template<typename CollectionType>
bool WriteFileLines( const CString& fileName, const CollectionType& lines, bool showExeption = false )
{
	typedef CollectionType::ElementType StringType;//вытаскиваем из коллекции тип строка
	typedef StringType::ElementType CharType;//из строки вытаскиваем шаблон тип строки
	CStr<CharType> text;
	CharType separator[3] = { 13, 10, 0 };
	CombineText<CollectionType, CharType>( lines, separator, text );
	return WriteFileBuffer( fileName, text, showExeption);
}

/*
template<typename CollectionType>
bool WriteFileLines( const CString& fileName, const CollectionType& lines, bool showExeption = false )
{
	return WriteFileLines<CollectionType, TCHAR>( fileName, lines, showExeption );
}*/





