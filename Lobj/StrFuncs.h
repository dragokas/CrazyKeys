#pragma once
#include "presume.h"

template < typename CharType > 
int StrLen(const CharType* wchar)
{
	if(wchar == 0) {
		return -1;
	}
	int len;
	for(len = 0; wchar[len] != 0; len++) {
	}
	return len;
}

//сравнение двух строк -> {-1,0 1}
template < typename CharType > 
int StrCmp(const CharType* wchar1, const CharType* wchar2)
{
	for( ; *wchar1 != 0 || *wchar2 != 0; wchar1++, wchar2++ ) {
		if( *wchar1 != *wchar2 ) {
			return ( ((int)*wchar1) - *wchar2 );			
		}
	}
	return 0;
}

//сравнение двух строк без учёта реестра
template < typename CharType > 
bool StrCmpLow(const CharType* wchar1, const CharType* wchar2)
{
	for( ; *wchar1 != 0 || *wchar2 != 0; wchar1++, wchar2++ ) {
#ifdef _UNICODE
		if( towlower( *wchar1 ) != towlower( *wchar2 ) ) 
			return false;
#else
		if( tolower( *wchar1 ) != tolower( *wchar2 ) ) 
			return false;
#endif
	}
	return true;
}

template < typename CharType > 
void checksetStrLength( const CharType* wchar, int& len )
{
	if( len < 0 ) {
		len = StrLen(wchar);
	} else {
		assert( len <= StrLen(wchar) );
	}
}
//поиск индекса первого вхождения
template <typename CharType>
int GetChrPos( const CharType* wchar1, const CharType wch )
{
	for( int i = 0; wchar1[i] != '\0'; i++ ) {
		if( wchar1[i] == wch ) {
			return i;
		}
	}
	return -1;
}
//поиск количества вхождений
template <typename CharType>
int GetChrPosCount( const CharType* wchar1, CharType wchar2 )
{
	int count = 0;
	for( int i = 0; wchar1[i] != 0; i++ ) {
		if( wchar1[i] == wchar2 ) {
			count++;
		}
	}
	return count;
}
//поиск подстроки выдаёт номер позиции начала строки, либо -1, если длина строки ==-1 то она перещитывается
template <typename CharType>
int GetStrPos(const CharType* wchar1, const CharType* wchar2, int len1 = -1, int len2 = -1)
{
	checksetStrLength(wchar1, len1);
	checksetStrLength(wchar2, len2);
	presume(0 <= len1 && 0 <= len2);
	if(len1 < len2 || len2 == 0) {
		return -1;
	}
	const CharType* p1 = wchar1;
	for(int i = 0; i <= len1 - len2; i++, p1++) {
		const CharType* p11 = p1;
		const CharType* p2 = wchar2;
		int j;
		for(j = 0; j < len2 && *p11 == *p2; j++, p11++, p2++) {		
		}
		if(j == len2) {
			return i;
		}
	}
	return -1;
}
//поиск количества вхождений
template <typename CharType>
int GetStrPosCount( const CharType* wchar1, const CharType* wchar2, int len1 = -1, int len2 = -1 )
{
	checksetStrLength( wchar1, len1 );
	checksetStrLength( wchar2, len2 );
	int count = 0;
	int startpos = 0;
	int pos = GetStrPos( wchar1 + startpos, wchar2, len1 - startpos, len2 );	
	while( startpos < len1 && pos != -1 ) {
		count++;
		startpos += ( pos + len2 );
		pos = GetStrPos( wchar1 + startpos, wchar2, len1 - startpos, len2 );
	}
	return count;
}
//по двум строкам находит номер буквы которая не одинаковая
template <typename CharType>
int StrDifPos( const CharType* wchar1, const CharType* wchar2 )
{
	int i;
	for( i = 0; wchar1[i] != 0 && wchar2[i] != 0; i++) {
		if( wchar1[i] != wchar2[i] ) {
			return i;
		}		
	}
	return i;
}


















