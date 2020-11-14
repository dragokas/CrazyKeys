#include "str.h"

#include "Matrix.h"

//#include "StrFuncs.h"
//#include "CharDef.h"

CString GetLineBreak()
{
#ifdef _UNICODE
	wchar_t buf[] = { 13, 10, 0 };
#else
	char buf[] = { 10, 0 };
#endif
	CString lineBreak( buf );
	return lineBreak;
}

CString Int2Str( int number, int base )
{
	return Int64ToStr( (__int64)number, base );		
}

int Str2Int( const CString& str, int base )
{
	return (int)StrToInt64( str, base );
}

CString Int64ToStr( __int64 number, int base )
{
	assert( 2 <= base && base <= 16 ); 
	TCHAR numberSymbols[] = T("0123456789ABCDEF-");
	if( number == 0 ) {
		return T("0");
	}
	int sign = ( number > 0 ) ? 1 : -1;
	if( number < 0 ) {
		number *= -1;
	}
	CString str( 33 );
	for( number; 0 < number; number /= base ) {
		str.Insert( 0, numberSymbols[number % base] );		
	}
	if( sign == -1 ) {
		str.Insert( 0, T('-') );
	}
	return str;	
}

__int64 StrToInt64( const CString& str, int base )
{
	assert( 2 <= base && base <= 16 ); 
	CString tmpStr = str;
	tmpStr.ToLower();
	int sign = ( tmpStr[0] == T('-') ) ? -1: 1;
	int i = ( sign == -1 ) ? 1: 0;
	__int64 res = 0;
	for( ; i < tmpStr.Length(); i++ ) {
		if( T('0') <= tmpStr[i] && tmpStr[i] <= T('9') ) {
			res = res*base + (tmpStr[i] - T('0'));
		} else {
			res = res*base + (tmpStr[i]) - T('a') + 10;
		}
	}
	return res * sign;
}

void RecieveCharBuffer( char* buffer, int bufferSize, CString str )
{
	assert( str.Length() < bufferSize );
	for( int i = 0; i <= str.Length(); i++ ) {
		buffer[i] = (char)str[i];
	}
}

void RecieveWCharBuffer( wchar_t* buffer, int bufferSize, CString str )
{
	assert( str.Length() < bufferSize );
	for( int i = 0; i <= str.Length(); i++ ) {
		buffer[i] = (wchar_t)str[i];
	}
}

CStr<char> GetStrA( const wchar_t* buffer, int len )
{
	checksetStrLength( buffer, len );
	CStr<char> str( len + 1 );
	for( int i = 0; buffer[i] != 0; i++ ) {
		str.Append( (const char) buffer[i] );
	}
	return str;
}

CStr<wchar_t> GetStrW( const char* buffer, int len )
{
	checksetStrLength( buffer, len );
	CStr<wchar_t> str( len + 1 );
	for( int i = 0; buffer[i] != 0; i++ ) {
		str.Append( (const wchar_t) buffer[i] );
	}
	return str;
}

CStr<char> GetStrA( const char* buffer, int )
{
	return buffer;
}

CStr<wchar_t> GetStrW( const wchar_t* buffer, int )
{
	return buffer;
}

CString GetLongestCommonSubsequence( const CString a, const CString b )
{
	CMatrix<int> max_len( a.Length() + 1, b.Length() + 1 );
	max_len.SetValue( 0 );
	for( int i = a.Length() - 1; 0 <= i; i-- ) {
		for(int j = b.Length() - 1; 0 <= j; j-- ) {
			if( a[i] == b[j] ) {
				max_len[i][j] = 1 + max_len[i+1][j+1];
			} else {
				max_len[i][j] = max( max_len[i+1][j], max_len[i][j+1] );
			}
		}
	}
	//PrintMatrix( max_len );
	CString res( max( a.Length(), b.Length() ) );
	for( int i = 0, j = 0; max_len[i][j] != 0; ) {
		if(a[i] == b[j]) {
			res.Append( a[i] );
			i++;
			j++;
		} else {
			( max_len[i][j] == max_len[i+1][j] ) ? i++ : j++;
		}
	}
	return res;
}





