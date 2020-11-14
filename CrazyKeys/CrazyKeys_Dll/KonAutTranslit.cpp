#include <stdio.h>
#include "KonAutTranslit.h"
#include "TextFileFuncs.h"
#include "Rules.h"
#include "MsgBox.h"

static const int BadState = 256 - 1;

CKonAutTranslit::~CKonAutTranslit()
{
	deleteOldKARules();
}

void CKonAutTranslit::deleteOldKARules()
{
	for( int i = 0; i < transitionRule.GetRowCount(); i++ ) {
		for( int j = 0; j < transitionRule.GetColumnCount(); j++ ) {
			if( transitionRule[i][j] != 0 ) {//матрица из указателей
				delete transitionRule[i][j];
				transitionRule[i][j] = 0;
			}
		}
	}
}

bool CKonAutTranslit::NeedTranslit( wchar_t sourceLetter ) const
{
	return ( sourceLetter != 0 && wcharToState[sourceLetter] != BadState && cell( 0, sourceLetter ) != 0 );
}

void CKonAutTranslit::GetNext( wchar_t letter, int& nLettersToDelete, CString& strToWrite )
{
	CKACell* pNode = cell( state, letter );
	if( pNode == 0 ) {
		pNode = cell( 0, letter );
	}
	if( pNode != 0 ) {
		nLettersToDelete = pNode->nLettersToDelete;
		strToWrite = pNode->strToWrite;
		state = pNode->nextState;
	}
}

bool CKonAutTranslit::canAcceptLetters( const CString& inputWord ) const
{
	for( int i = 0; i < inputWord.Length(); i++ ) {
		if( cell( 0, inputWord[i] ) == 0 ) {
			MsgBox( T("Error in KonAutTranslit canAcceptLetters"), L"%s, in letter %c", 
				inputWord.GetBuffer(), inputWord[i] );
			return false;
		}
	}
	return true;
}

void CKonAutTranslit::addStates( const CString& sourceStr, const CString& targetStr, unsigned int& nStates )
{//лепит новую цепочку состояний
	unsigned int stateCopy = 0;
	int tmpState = 0;
	for( int i = 0; i < sourceStr.Length(); i++ ) {
		CKACell*& pNode = cell( tmpState, sourceStr[i] );
		if( pNode == 0 || pNode->nextState == 0 ) {
			if( pNode == 0 ) {//наткнулись на пустую ячейку
				if( cell( stateCopy, sourceStr[i] ) == 0 ) {//чтобы по стопам не идти по стоящейся цепочке
					stateCopy = 0;//збрасываем начальное
				}
				pNode = new CKACell( *(cell( stateCopy, sourceStr[i] ) ) );
			}
			stateCopy = cell( stateCopy, sourceStr[i] )->nextState;
			assert( 0 <= nStates && stateCopy <= nStates )
			if( sourceStr[i + 1] == 0 ) {//если конец слова
				pNode->nextState = tmpState = 0;
				pNode->strToWrite = targetStr;
			} else {
				pNode->nextState = tmpState = ++nStates;
			}			
		} else {
			tmpState = cell( tmpState, sourceStr[i] )->nextState;
		}
	}
}

void CKonAutTranslit::setDeleteLetters( const CString& inputWord )
{//проставляет значения для удаления, проходом по всей цепочке
	int toDeleteCount = 0;
	int tmpState = 0;
	for( int i = 0; i < inputWord.Length() - 1; i++ ) {
		CKACell* pNode = cell( tmpState, inputWord[i] );
		toDeleteCount -= pNode->nLettersToDelete;
		toDeleteCount += pNode->strToWrite.Length();
		tmpState = pNode->nextState;
	}
	cell( tmpState, inputWord.Last() )->nLettersToDelete = toDeleteCount;
}

void CKonAutTranslit::BuildTranslitKA( const CRules& rules )
{
	wcharToState.SetSize( 256 * 256 );
	wcharToState.SetValue( BadState );
	int maxStates = 0;
	unsigned char nLetters = 0;
	for( int i = 0; i < rules.GetCount(); i++ ) {
		for( int j = 0; j < rules.Source( i ).Length(); j++ ) {
			if( wcharToState[rules.Source( i )[j]] == BadState ) {
				wcharToState[rules.Source( i )[j]] = nLetters++;				
			}
		}
		maxStates += rules.Source( i ).Length();
	}
	deleteOldKARules();//удаляем память старых указательй
	transitionRule.SetNewSize( maxStates, nLetters );
	transitionRule.SetValue( 0 );

	unsigned int nStates = 0;
	for( int i = 0; i < rules.GetCount(); i++ ) {//по всем строкам
		//дело в том что русские буквы они в char, принимают отрицательные значения, чтобы они этого не далали:
		CString sourceStr = rules.Source( i );
		presume( sourceStr.Length() != 0 );
		if( sourceStr.Length() == 1 ) {//однобуквенная
			cell( 0, sourceStr[0] ) = new CKACell( 0, 0, rules.Target( i ) );
		} else {//иначе строим цепочку
			if( canAcceptLetters( sourceStr ) ) {//если все буквы имеют состояния
				addStates( sourceStr, rules.Target( i ), nStates );
				setDeleteLetters( sourceStr );
			}
		}
	}
	transitionRule.SetNewSize( nStates + 1, transitionRule.GetColumnCount() );
	//printToFileVertical();
	//printToFileHorizontal();
}

void CKonAutTranslit::printToFileVertical() const
{
	wchar_t fName[150];
	swprintf( fName, L"!debug__Vertical_KA_%dx%d.txt", transitionRule.GetColumnCount(), transitionRule.GetRowCount() );
	CStrList tf;
	tf.PushToTail( L"1 " );
	for( wchar_t i = 0; i < 256 * 256 - 1; i++ ) {
		if( wcharToState[i] != BadState ) {
			tf.Last() += ( CString( L" __ " ) + i + L" ___ " );
		}
	}	 
	for( int i = 0; i < transitionRule.GetRowCount(); i++ ) {
		tf.PushToTail( CString( Int2Str( i ) + L" " ) );
		for( wchar_t j = 0; j < 256 * 256 - 1; j++ ) {
			if( wcharToState[j] != BadState ) {
				const CKACell* pNode = cell( i, j );
				if( pNode != NULL ) {
					wchar_t tmpStr[100];
					swprintf( tmpStr, L"{%2d,%d,%2s} ", pNode->nextState, pNode->nLettersToDelete, pNode->strToWrite.GetBuffer() );
					tf.Last() += tmpStr;
				} else {
					tf.Last() += L" _ ]X[ __ ";
				}
			}
		}	
	}
	WriteFileLines( fName, tf );
}

void CKonAutTranslit::printToFileHorizontal() const
{
	wchar_t fName[150];
	swprintf( fName, L"!debug__Horizontal_KA_%dx%d.txt", transitionRule.GetRowCount(), transitionRule.GetColumnCount() );
	CStrList tf;
	tf.PushToTail( L"2" );
	for( int i = 0; i < transitionRule.GetRowCount(); i++ ) {
		wchar_t tmpStr[100];
		swprintf( tmpStr, L" __ %2d __ ", i );
		tf.Last() += tmpStr;
	}
	for( wchar_t i = 0; i < 256 * 256 - 1; i++ ) {
		if( wcharToState[i] != BadState ) {
			tf.PushToTail( i + CString( L" " ) );
			for( int j = 0; j < transitionRule.GetRowCount(); j++ ) {
				const CKACell* pNode = cell( j, i );
				if( pNode != NULL ) {
					wchar_t tmpStr[100];
					swprintf( tmpStr, L"{%2d,%d,%2s} ", pNode->nextState, pNode->nLettersToDelete, pNode->strToWrite.GetBuffer() );
					tf.Last() += tmpStr;
				} else {
					tf.Last() += L" _ ]X[ __ ";
				}
			}
		}
	}
	WriteFileLines( fName, tf );
}