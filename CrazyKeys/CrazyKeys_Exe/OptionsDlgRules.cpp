#include "OptionsDlg.h"
#include "AddNewRuleDlg.h"
#include "resource.h"
#include "StandartRulesDlg.h"
#include "TextFileFuncs.h"
#include "CrazyKeysConst.h"
#include "ListViewFuncs.h"
#include "MUI.h"

static CString TmpSelectedSourceStr;

void COptionsDlg::initRuleList() const
{
	HWND hList = GetDlgItem( hDlg, IDC_LIST_RULES );
	ListView_SetExtendedListViewStyle( hList, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	CStrVector headerWords;
	headerWords.SetBufferSize( 2 );
	headerWords.Append( TranslateStr(L"source") );
	headerWords.Append( TranslateStr(L"target") );
	InitListViewHeader( hList, headerWords, true );
}

void COptionsDlg::fillRuleList() const
{
	FillListViewItems( GetDlgItem( hDlg, IDC_LIST_RULES ), options.GetRules().GetRulesTable() );
}

////////////////////////////////////////////////////////////////////////// кнопки у листа к правилам

void COptionsDlg::setVisibleModifyed() const
{
	HWND hList = GetDlgItem( hDlg, IDC_LIST_RULES );
	if( !TmpSelectedSourceStr.IsEmpty() ) {
		int changedIndex = options.GetRules().FindSourceIndex( TmpSelectedSourceStr );
		ListView_EnsureVisible( hList, changedIndex, TRUE );
	}
}

void COptionsDlg::onAddNewRule()
{
	TmpSelectedSourceStr = EmptyStr;
	CAddNewRuleDlg addDlg( hInst );
	addDlg.ShowDlg( hDlg, options.GetLayout( T_Source )->GetHKL(), options.GetLayout( T_Target )->GetHKL() );
	if( addDlg.IsRuleAdded() ) {
		options.GetRules().Add( addDlg.GetSource(), addDlg.GetTarget() );
		TmpSelectedSourceStr = addDlg.GetSource();
		fillRuleList();
		setVisibleModifyed();
	}
}

void COptionsDlg::onEditRule()
{
	HWND hList = GetDlgItem( hDlg, IDC_LIST_RULES );
	int selectedIndex = ListView_GetSelectionMark( hList );
	if( selectedIndex != -1 && selectedIndex < options.GetRules().GetCount() ) {
 		TmpSelectedSourceStr =  options.GetRules().Source( selectedIndex );
 		CAddNewRuleDlg addDlg( hInst );
 		addDlg.ShowDlg( hDlg, TmpSelectedSourceStr, options.GetLayout( T_Target )->GetHKL() );
 		if( addDlg.IsRuleAdded() ) {
 			options.GetRules().Add( addDlg.GetSource(), addDlg.GetTarget() );
 			TmpSelectedSourceStr = addDlg.GetSource();
 			fillRuleList();
 			setVisibleModifyed();
 		}
 	}
}

void COptionsDlg::onDeleteRule()
{
	HWND hList = GetDlgItem( hDlg, IDC_LIST_RULES );
	int selectedIndex = ListView_GetSelectionMark( hList );
	if( selectedIndex != -1 && selectedIndex < options.GetRules().GetCount() ) {
		options.GetRules().Delete( selectedIndex );
		fillRuleList();
	}
}

void COptionsDlg::onLoadDefaultRules()
{
	CStrList lines;
	ReadFileLines( CfgDefaultRulesFileName, lines, true );
	const CRules& rules = options.GetRules();
	for( int i = 0; i < rules.GetCount(); i++ ) {
		lines.PushByIndex( rules.GetRule( i ), i ); 
	}
	lines.PushToHead( options.GetLayout( T_Source )->GetLayoutDigitName() + spaceBetweenLanguages +
		options.GetLayout( T_Target )->GetLayoutDigitName() + L" current transliteration" );

	CStandartRulesDlg dlg( hInst, lines, options.GetLayout( T_Source ), options.GetLayout( T_Target ) );
	if( dlg.NeedChoiseDialog() ) {
		dlg.ShowDlg( hDlg );
		if( dlg.RecieveChosenTranslit( lines ) ) {
			options.GetRules().Fill( lines );
			onRemoveBadRules();
		}
	} else {
		MsgBox( L"Notify", L"No standard translit for layouts" );
	}
}

void COptionsDlg::onRemoveBadRules()
{
	options.RemoveBadRules();
	fillRuleList();
	setVisibleModifyed();
}



