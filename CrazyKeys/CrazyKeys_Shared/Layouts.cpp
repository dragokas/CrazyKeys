#include <stdio.h>
#include "Layouts.h"
#include "TextFileFuncs.h"
#include "MUI.h"
#include "CrazyKeysConst.h"

void CLayouts::Update()
{
	CVector<HKL> hKLs( GetKeyboardLayoutList( 0, 0 ) + 1 );
	GetKeyboardLayoutList( hKLs.GetSize() - 1, hKLs.GetPtr() );	
	hKLs[hKLs.GetSize() - 1] = 0;
	CString sourceLayoutName = EmptyStr;
	CString targetLayoutName = EmptyStr;
	if( 0 < layouts.GetCount() ) {//запоминаем какие раскладки сейчас стоят
		sourceLayoutName = layouts[sourceLayoutIndex]->GetNameWithDigits();
		targetLayoutName = layouts[targetLayoutIndex]->GetNameWithDigits();
	}
	updateHKL( hKLs );
	if( sourceLayoutName != EmptyStr && targetLayoutName != EmptyStr ) {//воcтонавливаем
		GuessLayout( sourceLayoutName, T_Source );
		GuessLayout( targetLayoutName, T_Target );
	}
}

void CLayouts::updateHKL( const CVector<HKL>& hKLs )
{
	for( int i = layouts.GetCount() - 1; 0 <= i; i-- ) {
		if( hKLs.Find( layouts[i]->GetHKL() ) == -1 ) {
			layouts.PopByIndex( i );//убераем все устаревшие
		}
	}
	for( int i = 0; i < hKLs.GetSize(); i++ ) {
		if( layouts.Find( hKLs[i] ) == -1 ) {//добавляем в нужном порядке
			layouts.PushByIndex( new CLayout( hKLs[i] ), min( layouts.GetCount(), i ) );
		}
	}
	isTextServicesOn = isAdvancedTextServicesOn();
	loadChangeBetweenLanguagesSeqStr();
	loadChangeBetweenLayoutsSeqStr();
	CList<CRegHotKey> hotKeys;
	createHotKeysList( hotKeys );
	hasRussianLayout = false;
	for( int i = 0; i < layouts.GetCount(); i++ ) {
		layouts[i]->SetChangeLayoutSequence( hotKeys );
		if( layouts[i]->GetHotkey().Layout == 0x0419 ) {
			hasRussianLayout = true;
		}
	}
}

void CLayouts::createHotKeysList( CList<CRegHotKey>& hotKeys ) const
{
	for( int i = 0x0100; i < 0x0120; i++ ) {
		wchar_t regPathDir[300];
		swprintf( regPathDir, L"Control Panel\\Input Method\\Hot Keys\\00000%X\\", i );
		HKEY hkey = 0;
		if( RegOpenKey( HKEY_CURRENT_USER, regPathDir, &hkey ) == ERROR_SUCCESS ) {
			CRegHotKey hotKey( regPathDir );
			DWORD dw1 = sizeof( hotKey.km.bVal ); 
			DWORD dw2 = sizeof( hotKey.ti.bVal );
			DWORD dw3 = sizeof( hotKey.vk.bVal );
			if( RegQueryValueEx( hkey, L"Key Modifiers", 0, 0, hotKey.km.bVal, &dw1 ) == ERROR_SUCCESS &&
				RegQueryValueEx( hkey, L"Target IME", 0, 0, hotKey.ti.bVal, &dw2 ) == ERROR_SUCCESS &&
				RegQueryValueEx( hkey, L"Virtual Key", 0, 0, hotKey.vk.bVal, &dw3 ) == ERROR_SUCCESS )
			{
				hotKeys.PushToTail( hotKey );//сохраняем
			}
			RegCloseKey( hkey );
		}		
	}
}

CString CLayouts::getHotkeySwitchSeq( int hotkey ) const
{
	if( !isTextServicesOn ) {
		return L"   ---   ";//если выключен, то они не работают
	}
	switch( hotkey ) {
		case 1: return L"alt+shift"; break;
		case 2: return L"ctrl+shift"; break;
		default: return L"   ---   ";//3 - не установлено, иначе ошибка
	}
}

void CLayouts::loadChangeBetweenLanguagesSeqStr()
{//из реестра узнаем какой клавижей переключать, с SPI_SETLANGTOGGLE проблемы
	languageHotkey = 3;
	GetIntFromRegistry( HKEY_CURRENT_USER, L"Keyboard Layout\\Toggle", L"Language Hotkey", languageHotkey );
}

void CLayouts::loadChangeBetweenLayoutsSeqStr()
{//из реестра узнаем какой клавижей переключать, с SPI_SETLANGTOGGLE проблемы
	layoutHotkey = 3;
	GetIntFromRegistry( HKEY_CURRENT_USER, L"Keyboard Layout\\Toggle", L"Layout Hotkey", layoutHotkey );
}

void CLayouts::GuessLayout( CString layoutName, TLayouts which )
{
	int maxCommon = ( GetLongestCommonSubsequence( layoutName, layouts[0]->GetNameWithDigits() ) ).Length();
	getIndexRef( which ) = 0;
	for( int i = 1; i < layouts.GetCount(); i++ ) {
		int tmpVal = ( GetLongestCommonSubsequence( layoutName, layouts[i]->GetNameWithDigits() ) ).Length();
		if( maxCommon < tmpVal && ( which == T_Target || !layouts[i]->IsUnicode() ) ) {
			maxCommon = tmpVal;
			getIndexRef( which ) = i;
		}
	}
}

void CLayouts::GuessDefaultLayouts()
{
	HKL defHKL = 0;
	SystemParametersInfo( SPI_GETDEFAULTINPUTLANG, 0, &defHKL, 0 );
	int defIndex = -1, engIndex = -1, rusIndex = -1;
	for( int i = 0; i < layouts.GetCount(); i++ ) {
		if( (layouts[i])->GetHKL() == defHKL ) {
			defIndex = i;
		}
		if( layouts[i]->GetHotkey().LL == 0x04190419 ) {
			rusIndex = i;
		}
		if( layouts[i]->GetHotkey().LL == 0x04090409 ) {
			engIndex = i;
		}		
	}
	presume( defIndex != -1 );//мы должны найти язык по умолчанию
	sourceLayoutIndex = ( defIndex == -1 ) ? 0 : defIndex;
	if( rusIndex != -1 && rusIndex != defIndex ) {
		targetLayoutIndex = rusIndex;
	} else if( engIndex != -1 && engIndex != defIndex ) {
		targetLayoutIndex = engIndex;
	} else {
		targetLayoutIndex = layouts.GetCount() - 1;
	}
}

bool CLayouts::needChangeLayout() const
{
	return ( sourceLayoutIndex != targetLayoutIndex && 2 < layouts.GetCount() && !IsUnicode() );
}

bool CLayouts::needHotkeys( bool isOptimalQuery ) const
{//нужно ли проставлять последовательности "на язык"
	return ( !isTextServicesOn || 3 < layouts.GetCount() || (
		!isOptimalQuery && //вопрос не об оптимальности
        !layouts[sourceLayoutIndex]->GetHotkey().IsEmptyHotkey() &&
		!layouts[targetLayoutIndex]->GetHotkey().IsEmptyHotkey() &&
        languageHotkey == 1 //если альт между языками и обе последовательности уже есть
        ) );
}

bool CLayouts::recieveChangeLayoutStrings( CString& toSource, CString& toTarget ) const
{
	toSource = toTarget = EmptyStr;
	if( !needChangeLayout() ) {
		return true;//не надо менять раскладки
	}
	if( needHotkeys( false ) ) {
		toSource = layouts[sourceLayoutIndex]->GetChangeLayoutSequenceStr();
		toTarget = layouts[targetLayoutIndex]->GetChangeLayoutSequenceStr();
	} else {
		if( languageHotkey != 3 ) {//если можно менять между языками
			toSource = toTarget = getHotkeySwitchSeq( languageHotkey );		
		} else {//если нельзя поставить смену между, то ставим на каждый язык
			toSource = layouts[sourceLayoutIndex]->GetChangeLayoutSequenceStr();
			toTarget = layouts[targetLayoutIndex]->GetChangeLayoutSequenceStr();
		}
	}
	return ( !toSource.IsEmpty() && !toTarget.IsEmpty() );
}

bool CLayouts::SaveSwitchSequences( bool mayShowError ) const
{
	CString toSource;
	CString toTarget;
	bool isGoodHotkeys = recieveChangeLayoutStrings( toSource, toTarget );
	WriteFileBuffer( CfgCangeLangFileName, toSource + T("\n") + toTarget, true );
	if( mayShowError && !isGoodHotkeys ) {
		MsgBox( TranslateStr(T("Problem with layout sequence")), 
			TranslateStr(T("Some change layout sequences may be incorrect\
							 \nto source sequence( %s )\nto target sequence( %s )")),
			toSource.IsEmpty() ? TranslateStr(T("Empty")) : toSource.GetBuffer(), 
			toTarget.IsEmpty() ? TranslateStr(T("Empty")) : toTarget.GetBuffer() );
	}
	return isGoodHotkeys;
}

////////////////////////////////////////выставление оптимальных настроек//////////////////////////////////

void CLayouts::SetOptimalSequences()
{
	Update();
	if( !needChangeLayout() ) {
		return;//если язык не меняется, то хоткеи безразличны
	}
    writeToRegOptimalSwitchHotkeySeq( needHotkeys( true ) );
	writeToRegHotkeysSeq( needHotkeys( true ) );
}

bool CLayouts::isAdvancedTextServicesOn() const
{
	bool isOn = false;
	HKEY hkey = 0;
	if( RegOpenKey( HKEY_CURRENT_USER, L"Software\\Microsoft\\CTF", &hkey ) == ERROR_SUCCESS ) {
		UBinRegValue value;
		DWORD dw = sizeof( value.bVal ) ;
		if( RegQueryValueEx( hkey, L"Disable Thread Input Manager", 0, 0, value.bVal, &dw ) == ERROR_SUCCESS ) {
			isOn = ( value.bVal[0] == 0 );
		}
		RegCloseKey( hkey );
	}
	return isOn;
}

void CLayouts::writeToRegOptimalSwitchHotkeySeq( bool needHotkeys ) const
{//с альтом переключения плохие, поэтому нам нужен контрол
	if( ( !needHotkeys && languageHotkey == 2 ) ||//не нужны переключения на язык
		( needHotkeys && languageHotkey == 1 && layoutHotkey == 3 ) )//или нужны переключения на язык и смена альтом
	{
		return;
	}
	int tmpLanguageHotkey = ( needHotkeys ) ? 1 : 2;//если нужны хоткеи то alt иначе ctrl
	int tmpLayoutHotkey = 3;//а смену лайаутов отключаем, т.к. она может начать конфликтовать
	SetStrToRegistry( HKEY_CURRENT_USER, L"Keyboard Layout\\Toggle", L"Hotkey", Int2Str( tmpLanguageHotkey ) );
	SetStrToRegistry( HKEY_CURRENT_USER, L"Keyboard Layout\\Toggle", L"Language Hotkey", Int2Str( tmpLanguageHotkey ) );
	SetStrToRegistry( HKEY_CURRENT_USER, L"Keyboard Layout\\Toggle", L"Layout Hotkey", Int2Str( tmpLayoutHotkey ) );
}

void CLayouts::deleteBadHotkeysFromRegistry( CList<CRegHotKey>& hotKeys, bool needHotkeys ) const
{
	for( int i = hotKeys.GetCount() - 1; 0 <= i; i-- ) {//удаляем из реестра и из hotKeys те, которых уже нет
		if( layouts.Find( hotKeys[i].LL ) == -1 || //если языка уже нет
			( !needHotkeys && hotKeys[i].CtrlOrAlt == 6 ) )//хоткей не надо ставить и ctrl будет мешать
		{             
			RegDeleteKey( HKEY_CURRENT_USER, hotKeys[i].regLocation.GetBuffer() );
			hotKeys.PopByIndex( i );//чтобы hotKeys совпадал с реестром
		}
	}
}

void CLayouts::deleteDoubleHotkeysFromRegistry( CList<CRegHotKey>& hotKeys ) const
{
	for( int i = 0; i < hotKeys.GetCount() - 1; i++ ) {
		for( int j = i + 1; j < hotKeys.GetCount(); j++ ) {
			if( hotKeys[i].CtrlOrAlt == hotKeys[j].CtrlOrAlt && hotKeys[i].ExKey == hotKeys[j].ExKey ) {
				RegDeleteKey( HKEY_CURRENT_USER, hotKeys[j].regLocation.GetBuffer() );
				hotKeys.PopByIndex( j );
				RegDeleteKey( HKEY_CURRENT_USER, hotKeys[i].regLocation.GetBuffer() );				
				hotKeys.PopByIndex( i );
				deleteDoubleHotkeysFromRegistry( hotKeys );
			}
		}
	}
}

void CLayouts::deleteFromRegIfAlt( int layoutIndex, CList<CRegHotKey>& hotKeys )
{
	if( layouts[layoutIndex]->GetHotkey().CtrlOrAlt == 5 ) {//alt надо удалять
		int hotkeyIndex = hotKeys.Find( layouts[layoutIndex]->GetHotkey().regLocation );
		presume( hotkeyIndex != -1 );
		if( hotkeyIndex != -1 ) {
			RegDeleteKey( HKEY_CURRENT_USER, hotKeys[hotkeyIndex].regLocation.GetBuffer() );
			hotKeys.PopByIndex( hotkeyIndex );//убиваем из списка чтобы синхронизировать
			layouts[layoutIndex]->SetChangeLayoutSequence( hotKeys );//не забыть обновить в лайауте
		}
	}
}

void CLayouts::writeToRegHotkeysSeq( bool needHotkeys )
{
	CList<CRegHotKey> hotKeys;
	createHotKeysList( hotKeys );
	deleteBadHotkeysFromRegistry( hotKeys, needHotkeys );
	deleteDoubleHotkeysFromRegistry( hotKeys );
	if( needHotkeys ) {
		writeToRegSourceAndTargetWithCtrl( hotKeys );
	}
}

void CLayouts::writeToRegSourceAndTargetWithCtrl( CList<CRegHotKey>& hotKeys, bool isSecondCall /*= false*/ )
{
	if( layouts[sourceLayoutIndex]->GetHotkey().CtrlOrAlt != 6 ) {
		if( !writeToRegHotkeyWithCtrl( sourceLayoutIndex, hotKeys ) && !isSecondCall ) {
			deleteAllHotkeysWithtAlt( hotKeys );
			writeToRegSourceAndTargetWithCtrl( hotKeys, true );
		}
	}
	if( layouts[targetLayoutIndex]->GetHotkey().CtrlOrAlt != 6 ) {
		if( !writeToRegHotkeyWithCtrl( targetLayoutIndex, hotKeys ) && !isSecondCall ) {
			deleteAllHotkeysWithtAlt( hotKeys );
			writeToRegSourceAndTargetWithCtrl( hotKeys, true );
		}
	}
	presume( layouts[sourceLayoutIndex]->GetHotkey().CtrlOrAlt == 6 &&
		layouts[targetLayoutIndex]->GetHotkey().CtrlOrAlt == 6 );
}

bool CLayouts::writeToRegHotkeyWithCtrl( int layoutIndex, CList<CRegHotKey>& hotKeys )
{
	deleteFromRegIfAlt( layoutIndex, hotKeys );
	//итак, данной в реестре нет, и надо вставить хоткей на ету клаву
	CRegHotKey hotKey;
	for( hotKey.ExKey = 0x30; hotKey.ExKey < 0x30 + 10; hotKey.ExKey++ ) {//от ['0'..'9']
		if( hotKeys.Find( hotKey.ExKey ) == -1 ) {
			break;//нашли свободную цифру
		}
	}
	if( hotKey.ExKey != 0x30 + 10 ) {//нашли свободный от хоткеяв цифру
		for( int i = 0x0100; i < 0x0120; i++ ) {
			wchar_t regPathDir[300];
			swprintf( regPathDir, L"Control Panel\\Input Method\\Hot Keys\\00000%X\\", i );
			if( hotKeys.Find( regPathDir ) == -1 ) {//нашли первую в реестре ячейку куда можно записать
				hotKey.regLocation = regPathDir;//путь в реестре
				hotKey.LL = layouts[layoutIndex]->GetHotkey().LL;//язык и раскладка
				hotKey.CtrlOrAlt = 6;//ctrl, как доп. клавиша				
				writeToReg( hotKey );
				hotKeys.PushToTail( hotKey );
				layouts[layoutIndex]->SetChangeLayoutSequence( hotKeys );
				return true;
			}
		}
	}
	return false;//ужас, нет ни одной свободной позиции с контролом!
}

void CLayouts::deleteAllHotkeysWithtAlt( CList<CRegHotKey>& hotKeys )
{
	for( int i = hotKeys.GetCount() - 1; 0 <= i; i-- ) {//удаляем ВСЕХ с ctrl
		if( hotKeys[i].CtrlOrAlt == 6 ) {
			RegDeleteKey( HKEY_CURRENT_USER, hotKeys[i].regLocation.GetBuffer() );
			hotKeys.PopByIndex( i );//чтобы hotKeys совпадал с реестром
		}
	}
	for( int i = 0; i < layouts.GetCount(); i++ ) {//вычистить из раскладок
		if( layouts[i]->GetHotkey().CtrlOrAlt == 6 ) {//все хоткеи которые с ctrl
			layouts[i]->SetChangeLayoutSequence( hotKeys );
		}
	}
}

void CLayouts::writeToReg( const CRegHotKey& hotkey ) const
{
	HKEY hKey = 0;
	if( RegCreateKeyEx( HKEY_CURRENT_USER, hotkey.regLocation, 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hKey, 0 ) == ERROR_SUCCESS ) {
		RegSetValueEx( hKey, L"Key Modifiers", 0, REG_BINARY, hotkey.km.bVal, 4 );
		RegSetValueEx( hKey, L"Target IME", 0, REG_BINARY, hotkey.ti.bVal, 4 );
		RegSetValueEx( hKey, L"Virtual Key", 0, REG_BINARY, hotkey.vk.bVal, 4 );
		RegCloseKey( hKey );
	}
}












































