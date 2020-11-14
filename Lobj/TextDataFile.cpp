#include "TextDataFile.h"

/*int CTextDataFile::GetValue( CString key, int dafaultValue, CString comment )
{
	CString tmpDafaultStrValue = Int2Str( dafaultValue, 10 );
	CString tmpStrValue = GetValue( key, tmpDafaultStrValue, comment );
	return Str2Int( tmpStrValue );
}

CString CTextDataFile::GetValue( CString key, CString dafaultValue, CString comment )
{
	key += T( "=" );
	for( int i = 0; i < GetCount(); i++ ) {
		CString tmpStr = (*this)[i];
		int difPos = StrDifPos( key.GetBuffer(), tmpStr.GetBuffer() );
		if( difPos == key.GetLength() ) {
			return CString( tmpStr.GetBuffer() + difPos, tmpStr.GetLength() - difPos, tmpStr.GetLength() - difPos + 1 );
		}
	}
	if( !comment.IsEmpty() ) {
		PushToTail( CString( T( "//" ) + comment ) );
	}
	CString defaultStr( key + dafaultValue );
	PushToTail( defaultStr );
	return dafaultValue;
}

void CTextDataFile::SetValue( CString key, CString value, CString comment )
{
	CString tmp_key = key + T ("=" );
	for( int i = 0; i < GetCount(); i++ ) {
		CString tmp_str_i = (*this)[i];
		int difPos = StrDifPos( key.GetBuffer(), tmp_str_i.GetBuffer() );
		if( difPos == key.GetLength() ) {
			PopByIndex(i);
			i--;
		}
	}
	CString tmp_value = GetValue( key, value, comment );
	presume( tmp_value == value );
}

void CTextDataFile::SetValue( CString key, int value, CString comment )
{
	CString tmpStrValue = Int2Str( value, 10 );
	SetValue( key, tmpStrValue, comment );
}
*/
