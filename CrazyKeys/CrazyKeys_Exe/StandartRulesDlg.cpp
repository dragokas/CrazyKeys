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
	//���������� �������� ����������
	SetDlgItemText( hDlg, IDC_TRANSLIT_DIGIT_NAME, translits[currentTranslit]->GetDigitName() );
	SetDlgItemText( hDlg, IDC_TRANSLIT_USER_NAME, translits[currentTranslit]->GetUserName() );
	//����� �������� ������� ������ ������
	EnableWindow( GetDlgItem( hDlg, IDC_TRANSLIT_PREV ), ( currentTranslit != 0 ) ? TRUE : FALSE );
	EnableWindow( GetDlgItem( hDlg, IDC_TRANSLIT_NEXT ), ( currentTranslit != translits.Size() - 1 ) ? TRUE : FALSE );
	//������� ����� � disable ������
	if( currentTranslit == 0 ) {//���� �� ��������� disable �� prev, �� ���� ��� �����������
		SetFocus( GetDlgItem( hDlg, ( translits.Size() != 1 ) ? IDC_TRANSLIT_NEXT : IDOK ) );
	}
	if( currentTranslit == translits.Size() - 1 ) {//���� �� ��������� disable �� next, �� ���� ��� �����������
		SetFocus( GetDlgItem( hDlg, ( translits.Size() != 1 ) ? IDC_TRANSLIT_PREV : IDOK ) );
	}
	//������� ���������� ����� ���������	
	SetDlgItemText( hDlg, IDC_EDIT_SOURCE_USELESS, translits[currentTranslit]->GetSourceUselessStr() );
	SetDlgItemText( hDlg, IDC_EDIT_TARGET_USELESS, translits[currentTranslit]->GetTargetUselessStr() );
}

////////////////////////���������� ���������� �������� ��� �������� ������//////////////////////////////////////////////////

void CStandartRulesDlg::fillAlpha( CStrVector& alpha, const CVector<WORD>& wcharToScanCode )
{//���������� ��������� �� ���������
	alpha.DeleteAll();
	alpha.SetBufferSize( wcharToScanCode.Size() );
	for( int i = 0; i < wcharToScanCode.GetSize(); i++ ) {//��� �� ���� ��������� ������
		if( wcharToScanCode[i] != 0 ) {//���� ��� ����� ����� ���� ���, �� � ����� ������
			wchar_t letter[3] = { (wchar_t)i, 0, 0 };
			if( _istalpha( letter[0] ) != 0 || IsCharAlpha( letter[0] ) == TRUE ) {//���� ��� �� �������� (� ������������ ������ ������� � �� �� 100% ������)
				alpha.Append( letter );//������� � ������ ��������, ������ ��������������� �� wchar_t ���� :)
			}
		}
	}
}

void CStandartRulesDlg::fillTranslits( const CStrList& rules, const CLayout* sourceLayout, const CLayout* targetLayout )
{
	CStrVector sourceAlpha;//�������� ������� �� ���������
	CStrVector targetAlpha;//�������� ������� �� ���������
	fillAlpha( sourceAlpha, sourceLayout->GetWcharToScanCodeMap() );
	if( !targetLayout->IsUnicode() ) {//�� ������, ��������� ������ ���������
		fillAlpha( targetAlpha, targetLayout->GetWcharToScanCodeMap() );
	}
	for( int i = 0; i < rules.GetCount(); ) {//�� ���� �������� ���� ����������
		if( rules[i].Find( spaceBetweenLanguages ) != -1 ) {//������� ����
			CPtr<CTranslit> pTranslit = new CTranslit( rules[i], sourceAlpha, targetAlpha );//������ ��������
			bool goodTranslit = true;//����������� ��� ��� ������� �������
			for( i += 1; i < rules.GetCount() && rules[i].Find( spaceBetweenLanguages ) == -1; i++ ) {
				if( goodTranslit && !pTranslit->AddRule( rules[i], sourceLayout, targetLayout ) ) {//������� �� ����������
					goodTranslit = false;//�������� ������
				}
			}
			if( goodTranslit ) {//��� ������� ������� ����������
				pTranslit->SortTranslit();//��������� ������� �� target � ������������ �������, ����� ��������� ����� ���� �� ��� ����������
				if( translits.Find( *pTranslit, CTranslit::IsOriginalRulesEqual, 0 ) == -1 ) {//���������� ��������
					translits.PushToTail( pTranslit );//���������� ��������
				}
			}
		}
	}
}

bool CStandartRulesDlg::RecieveChosenTranslit( CStrList& recieveRules ) const
{
	if( currentTranslit == -1 ) {//����� �� ������� �� cancel
		return false;//������ �� �������
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
	originalRules.SetBufferSize( 64 );//����������� ����� ��� ������������ �������
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
			return false;//������� �� �� ���������
		}
		originalRules.Append( rule );
		if( targetStr.Length() == 1 ) {
			int targetIndex = translitTable.Find( targetStr, CTranslit::isLastLetter );//���� � ������ ������ ������ �������� �����
			if( targetIndex != -1 ) {//���� ��� ����� �� ��������� ��������
				CString& foundStr = translitTable[targetIndex];//��������� ������ ������ ����� �������
				foundStr.Replace( 0, 0, ( foundStr.Length() == 1 ) ? sourceStr + L"  -  " : sourceStr + L", " );
			} else if( targetLayout->IsUnicode() ) {
				translitTable.Append( sourceStr + L"  -  " + targetStr );
			}
		}
		int sourceIndex = sourceUseless.Find( sourceStr ); 
		if( sourceIndex != -1 ) {//���� ��� ����� ���� ��� � ������ �������� �������� � ���������
			sourceUseless.Delete( sourceIndex );//�� ��� ��� ����� ������
		}
		int targetIndex = targetUseless.Find( targetStr ); 
		if( targetIndex != -1 ) {//���� ��� ����� ���� ��� � ������ �������� �������� � ���������
			targetUseless.Delete( targetIndex );//�� ��� ��� ����� ������
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












