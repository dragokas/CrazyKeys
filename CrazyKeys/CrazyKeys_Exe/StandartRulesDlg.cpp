#include "StandartRulesDlg.h"
#include "resource.h"
#include "resource.hm"
#include "Str.h"
#include "CrazyKeysMUI.h"
#include "Layout.h"
#include <tchar.h>
#include "CrazyKeysConst.h"

void CStandartRulesDlg::ShowDlg(HWND hParentWnd )
{
	if( NeedChoiseDialog() ) {
		CreateModalDlg( IDD_STANDART_RULES_DLG, hParentWnd );
	}	
}

BOOL CStandartRulesDlg::OnInitDlg()
{
	TranslateCrazyKeysDlg( hDlg );
	currentTranslit = 0;
	fillDlg();
	return TRUE;
}

BOOL CStandartRulesDlg::OnHelpDlg(HELPINFO* helpInfo )
{
	ShowHelpTopic( hDlg, ( helpInfo->dwContextId == 0 ) ? HIDC_DEFAULT_RULES : helpInfo->dwContextId );
	return TRUE;
}

BOOL CStandartRulesDlg::OnCommandDlg( WPARAM wParam, LPARAM /*lParam*/ )
{
	switch( LOWORD( wParam ) ) {
		case IDC_TRANSLIT_PREV:
			currentTranslit = max( 0, currentTranslit - 1 );
			fillDlg();
			return TRUE;
		case IDC_TRANSLIT_NEXT:
			currentTranslit = min( translits.Size() - 1, currentTranslit + 1 );
			fillDlg();
			return TRUE;
		case IDCANCEL:
			currentTranslit = -1;
		case IDOK: 
			EndDialog( hDlg, LOWORD( wParam ) );
			return TRUE;				
	}
	return TRUE;
}

void CStandartRulesDlg::fillDlg() const
{
	DWORD ids[3] = { IDC_EDIT_TRANSLIT1, IDC_EDIT_TRANSLIT2, IDC_EDIT_TRANSLIT3 };
	for( int i = 0; i < 3; i++ ) {
		SetDlgItemText( hDlg, ids[i], translits[currentTranslit]->GetEditColumn( i ).GetBuffer() );
	}
	//выставляем названия транслитов
	SetDlgItemText( hDlg, IDC_TRANSLIT_DIGIT_NAME, translits[currentTranslit]->GetDigitName() );
	SetDlgItemText( hDlg, IDC_TRANSLIT_USER_NAME, translits[currentTranslit]->GetUserName() );
	//серим кнопочки которые нельзя нажать
	EnableWindow( GetDlgItem( hDlg, IDC_TRANSLIT_PREV ), ( currentTranslit != 0 ) ? TRUE : FALSE );
	EnableWindow( GetDlgItem( hDlg, IDC_TRANSLIT_NEXT ), ( currentTranslit != translits.Size() - 1 ) ? TRUE : FALSE );
	//сменяем фокус с disable кнопок
	if( currentTranslit == 0 ) {//если мы поставили disable на prev, то надо таб переставить
		SetFocus( GetDlgItem( hDlg, ( translits.Size() != 1 ) ? IDC_TRANSLIT_NEXT : IDOK ) );
	}
	if( currentTranslit == translits.Size() - 1 ) {//если мы поставили disable на next, то надо таб переставить
		SetFocus( GetDlgItem( hDlg, ( translits.Size() != 1 ) ? IDC_TRANSLIT_PREV : IDOK ) );
	}
	//выводим оставшиеся буквы алфавитов	
	SetDlgItemText( hDlg, IDC_EDIT_SOURCE_USELESS, translits[currentTranslit]->GetSourceUselessStr() );
	SetDlgItemText( hDlg, IDC_EDIT_TARGET_USELESS, translits[currentTranslit]->GetTargetUselessStr() );
}

////////////////////////заполнение внутренних структур для удобного показа//////////////////////////////////////////////////

void CStandartRulesDlg::fillAlpha( CStrVector& alpha, const CVector<WORD>& wcharToScanCode )
{//заполнение алфавитом по раскладке
	alpha.DeleteAll();
	alpha.SetBufferSize( wcharToScanCode.Size() );
	for( int i = 0; i < wcharToScanCode.GetSize(); i++ ) {//идём по всем возможным буквам
		if( wcharToScanCode[i] != 0 ) {//если эта буква имеет скан код, то её можно нажать
			wchar_t letter[3] = { (wchar_t)i, 0, 0 };
			if( _istalpha( letter[0] ) != 0 || IsCharAlpha( letter[0] ) == TRUE ) {//если она из алфавита (в правильности данной функции я не на 100% уверен)
				alpha.Append( letter );//заносим в список алфавита, причём отсортированный по wchar_t коду :)
			}
		}
	}
}

void CStandartRulesDlg::fillTranslits( const CStrList& rules, const CLayout* sourceLayout, const CLayout* targetLayout )
{
	CStrVector sourceAlpha;//исходный алфавит по раскладке
	CStrVector targetAlpha;//итоговый алфавит по раскладке
	fillAlpha( sourceAlpha, sourceLayout->GetWcharToScanCodeMap() );
	if( !targetLayout->IsUnicode() ) {//не юникод, заполняем полным алфавитом
		fillAlpha( targetAlpha, targetLayout->GetWcharToScanCodeMap() );
	}
	for( int i = 0; i < rules.GetCount(); ) {//по всем правилам всех транслитов
		if( rules[i].Find( spaceBetweenLanguages ) != -1 ) {//начался язык
			CPtr<CTranslit> pTranslit = new CTranslit( rules[i], sourceAlpha, targetAlpha );//создаём транслит
			bool goodTranslit = true;//предположим что все правила хорошие
			for( i += 1; i < rules.GetCount() && rules[i].Find( spaceBetweenLanguages ) == -1; i++ ) {
				if( goodTranslit && !pTranslit->AddRule( rules[i], sourceLayout, targetLayout ) ) {//правило не добавилось
					goodTranslit = false;//транслит плохой
				}
			}
			if( goodTranslit ) {//все правила успешно добавились
				pTranslit->SortTranslit();//сортируем правила по target и оригинальные правила, чтобы транслиты можно было по ним сравнивать
				if( translits.Find( *pTranslit, CTranslit::IsOriginalRulesEqual, 0 ) == -1 ) {//уникальный транслит
					translits.PushToTail( pTranslit );//запоминаем транслит
				}
			}
		}
	}
}

bool CStandartRulesDlg::RecieveChosenTranslit( CStrList& recieveRules ) const
{
	if( currentTranslit == -1 ) {//вышли из диалога по cancel
		return false;//ничего не трогаем
	}
	recieveRules.DeleteAll();
	translits[currentTranslit]->RecieveRules( recieveRules );
	return true;
}

//////////////////////////////////////////////////////////////////////////

CStandartRulesDlg::CTranslit::CTranslit( const CString& header, const CStrVector& sourceAlpha, const CStrVector& targetAlpha )
{
	sourceUseless.CopyFrom( sourceAlpha );
	targetUseless.CopyFrom( targetAlpha );
	translitTable.CopyFrom( targetAlpha );
	originalRules.SetBufferSize( 64 );//резервируем место под орогинальные правила
	CString tmp = header;
	const CString tmpSpace = L"$->$";
	tmp.ReplaceAll( spaceBetweenLanguages, tmpSpace );
    tmp.Split( digitName, L" ", userName );
	digitName.ReplaceAll( tmpSpace, L" -> " );
	
}

bool CStandartRulesDlg::CTranslit::AddRule( const CString& rule, const CLayout* sourceLayout, const CLayout* targetLayout )
{
	CString sourceStr, targetStr;
	if( rule.PosCount( L" " ) == 1 && rule.Split( sourceStr, L" ", targetStr ) ) {
		if( !sourceLayout->IsWordInLayout( sourceStr ) || !targetLayout->IsWordInLayout( targetStr ) ) {
			return false;//правило не из раскладки
		}
		originalRules.Append( rule );
		if( targetStr.Length() == 1 ) {
			int targetIndex = translitTable.Find( targetStr, CTranslit::isLastLetter );//ищем в правых частях индекс итоговой буквы
			if( targetIndex != -1 ) {//если это буква из итогового алфавита
				CString& foundStr = translitTable[targetIndex];//добавляем справа правую часть правила
				foundStr.Replace( 0, 0, ( foundStr.Length() == 1 ) ? sourceStr + L"  -  " : sourceStr + L", " );
			} else if( targetLayout->IsUnicode() ) {
				translitTable.Append( sourceStr + L"  -  " + targetStr );
			}
		}
		int sourceIndex = sourceUseless.Find( sourceStr ); 
		if( sourceIndex != -1 ) {//если эта буква есть ещё в списке исходных ненужных у транслита
			sourceUseless.Delete( sourceIndex );//то она уже стала нужной
		}
		int targetIndex = targetUseless.Find( targetStr ); 
		if( targetIndex != -1 ) {//если эта буква есть ещё в списке итоговых ненужных у транслита
			targetUseless.Delete( targetIndex );//то она уже стала нужной
		}
	}
	return true;
}

CString CStandartRulesDlg::CTranslit::getUselessStr( const CStrVector& useless ) const
{
	CString buffer( max( 1, ( useless.Size() * 32 ) ) );
	if( 0 < useless.Size() ) {
		for( int i = 0; i < useless.Size() - 1; i++ ) {
			( buffer += useless[i] ) += L" ";
		}
		buffer += useless.Last();	
	}
	return buffer;

}

CString CStandartRulesDlg::CTranslit::GetEditColumn( int index ) const
{
	presume( 0 <= index && index < 3 );
	int n = ( translitTable.Size() + 2 ) / 3;
	CString buffer( 48 * 10 );
	for( int i = index * n; i < translitTable.Size() && i < ( index + 1 ) * n; i++ ) {
		( buffer += translitTable[i] ) += L"\r\n";	
	}
	return buffer;
}












