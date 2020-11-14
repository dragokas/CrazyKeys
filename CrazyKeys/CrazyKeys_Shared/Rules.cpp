#include "Rules.h"
#include "Layout.h"
#include "MUI.h"
#include "MsgBox.h"

void CRules::Add( const CString& source, const CString& target )
{
	int oldIndex = rules.Find<CEqualSource>( source );// findSourceIndex( source );
	if( oldIndex != -1 ) {//такой source в правилах есть
		rules.Delete( oldIndex );//то удаляем чотбы заменить
	}
	if( 1 < source.Length() ) {//если длинны source, надо проверить все буквы 
		for( int i = 0; i < source.Length(); i++ ) {
			CString tmpStr( source.GetBuffer() + i, 1, 2 );
			if( rules.Find<CEqualSource>( tmpStr ) == -1 ) {//нет правила на букву
				rules.Append( CRule( tmpStr, tmpStr ) );//добавляем фиктивное правило
			}
		}
	}
	rules.Append( CRule( source, target ) );
	TSortedType how = sortedType;
	sortedType = ST_Unknown;
	Sort( how );
}

void CRules::Fill( const CStrList& lines )
{
	rules.DeleteAll();
	rules.SetBufferSize( lines.Size() );
	for( int i = 0; i < lines.Size(); i++ ) {
		CString source;
		CString target;
		if( lines[i].Split( source, L" ", target ) && target.Find( L' ' ) == -1 ) {
			rules.Append( CRule( source, target ) );
		}
	}
	sortedType = ST_Unknown;
	Sort( ST_Internal );
}

int CRules::getFirstWrongRuleIndex( TRuleErrorType& errorType, const CLayout* source, const CLayout* target, int fromIndex ) const
{

	for( int i = fromIndex; i < rules.Size(); i++ ) {
		const CRule& rule = rules[i];
		if( 0 < i && rules[i - 1].source == rule.source ) {
			errorType = RET_Same;
			return i;
		}
		if( !source->IsWordInLayout( rule.source ) ) {
            errorType = RET_NotInSource;
			return i;
		}
		if( !target->IsWordInLayout( rule.target ) ) {
            errorType = RET_NotInTarget;
			return i;
		}
		if( rule.source.Find( L" " ) != -1 || rule.target.Find( L" " ) != -1 ) {
			errorType = RET_SpacesInSequence;
			return i;
		}
		if( 1 < rule.source.Length() ) {
			for( int j = 0; j < rule.source.Length(); j++ ) {
				wchar_t wchar[] = { rule.source[j], 0 };
				if( rules.Find<CEqualSource>( wchar ) == -1 ) {
					errorType = RET_NoSubTrans;
					return i;//подпоследовательность не имеет переходов
				}
			}
		}
	}
	errorType = RET_Ok;
	return -1;
}

bool CRules::askUserToRemoveRule( int wrongRuleIndex, TRuleErrorType errorType ) const
{ 
	presume( errorType != RET_Ok );
	const TCHAR* pReason = 0;//L"not defined reason";
	switch( errorType ) {
		case RET_Same: 
			pReason = TranslateStr(T("You have two similar rules") );
			break;
		case RET_NotInSource: 
			pReason = TranslateStr(T("Not all letters from source sequence you can type on source keyboard")); 
			break;
		case RET_NotInTarget: 
			pReason = TranslateStr(T("Not all letters from target sequence you can type on target keyboard")); 
			break;
		case RET_NoSubTrans: 
			pReason = TranslateStr(T("Not all letters from source sequence can be translated")); 
			break;
		case RET_SpacesInSequence: 
			pReason = TranslateStr(T("No space is allowed in source or target part of sequence")); 
			break;
		default:
			pReason = TranslateStr(T("Not defined reason")); 

	}
	if( MsgBox( MB_YESNO, TranslateStr(T("Error: bad rule")), 
		TranslateStr(T("There is an invalid rule:\n'%s'->'%s'\nbecause:\n%s\n\nRemove this rule?")), 
		rules[wrongRuleIndex].source.GetBuffer(), rules[wrongRuleIndex].target.GetBuffer(), pReason ) == IDYES ) 
	{ 
		return true;
	}
	return false;
}

bool CRules::checkSortedRules( bool mayShowError, const CLayout* source, const CLayout* target )
{
	presume( sortedType == ST_Internal );
	TRuleErrorType errorType = RET_Ok;
	int wrongIndex = getFirstWrongRuleIndex( errorType, source, target );
	while( wrongIndex != -1 ) {
		if( mayShowError && askUserToRemoveRule( wrongIndex, errorType ) ) {
			rules.Delete( wrongIndex );
		} else {
			return false;
		}
		wrongIndex = getFirstWrongRuleIndex( errorType, source, target, wrongIndex );
	}
	return true;

}

bool CRules::CheckRules( bool mayShowError, const CLayout* source, const CLayout* target )
{
	TSortedType savedSortedType = sortedType;
	Sort( ST_Internal );
	bool checkResult = checkSortedRules( mayShowError, source, target );
	Sort( savedSortedType );
	return checkResult;
}

void CRules::RemoveBad( const CLayout* source, const CLayout* target )
{
	TSortedType savedSortedType = sortedType;
	Sort( ST_Internal );
	TRuleErrorType errorType = RET_Ok;
	int wrongIndex = getFirstWrongRuleIndex( errorType, source, target );
	while( wrongIndex != -1 ) {
		rules.Delete( wrongIndex );
		wrongIndex = getFirstWrongRuleIndex( errorType, source, target, wrongIndex );
	}
	Sort( savedSortedType );
}

void CRules::Sort( TSortedType how )
{
	if( how != sortedType ) {
		switch( how ) {
			case ST_Source: rules.QuickSort<CCmpBySource>(); break;
			case ST_Target: rules.QuickSort<CCmpByTarget>(); break;
			case ST_Internal: rules.QuickSort<CCmpByInternal>(); break;
			default: assertFalse;
		}
		sortedType = how;
	}
}

const CStrMatrix& CRules::GetRulesTable() const
{
	table.SetNewSize( rules.GetSize(), 2 );
	for( int i = 0; i < rules.GetSize(); i++ ) {
		table[i][0] = rules[i].source;	
		table[i][1] = rules[i].target;
	}
	return table;
}