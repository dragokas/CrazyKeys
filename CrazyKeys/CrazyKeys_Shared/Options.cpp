#include "Options.h"
#include "TextFileFuncs.h"
#include "CrazyKeysConst.h"
#include "ShellFuncs.h"
#include "CrazyKeysMUI.h"
#include "ShellPathFuncs.h"

static const CString SourceLayoutStr = L"Source Keyboard layout = ";
static const CString TargetLayoutStr = L"Target Keyboard layout = ";
static const CString SwitchKeyStr = L"Switch Key = ";
static const CString BlockSwitchKeyStr = L"Block Switch Key = ";
static const CString InterfaceLanguageStr = L"Interface Language = ";
static const CString DelayTimeStr = L"Delay Time = ";
static const CString SleepTimeStr = L"Sleep Time = ";
static const CString TimerTimeStr = L"Timer Time = ";
static const CString RulesStr = L"Rules = ";

static const CString UnknownUserNameSav = L"UnknownUserName.sav";

CString COptions::getUserCfgFileName() const
{
	return /*GetUserName() + L"'s_"*/ + L"CrazyKeysOptionsSavFile.cfg";
}

CString COptions::getOptionsFileName() const
{//выдаёт имя, где опции пользователя
	CString fName;//имя файла опций, <user>.sav, записан он в файле getUserCfgFileName()
	if( ExistFile( getUserCfgFileName() ) && ReadFileBuffer( getUserCfgFileName(), fName ) ) {
		if( ExistFile( fName ) ) {
			return fName;
		}
	}
	fName = GetUserName() + L".sav";//создаём по умолчанию имя файла
	WriteFileBuffer( getUserCfgFileName(), fName );
	return fName;
}

void COptions::setOptionsFileName( const CString& fName ) const
{
	WriteFileBuffer( getUserCfgFileName(), ( fName.IsEmpty() ) ? UnknownUserNameSav : fName );
}

void COptions::SetTimes( short _delayTime, short _sleepTime, short _timerTime )
{
	delayTime = _delayTime;
	sleepTime = _sleepTime;
	timerTime = _timerTime;
}

void COptions::Save( const CString& saveAsFileName /*= EmptyStr*/ )
{
	if( !saveAsFileName.IsEmpty() ) {
		setOptionsFileName( saveAsFileName );
		optionsFileName = saveAsFileName;
	}
	layouts.SaveSwitchSequences();

	CStrVector lines;
	lines.SetBufferSize( 8 + rules.GetCount() );

	lines.Append( SourceLayoutStr + layouts.GetLayout( T_Source )->GetNameWithDigits() );
	lines.Append( TargetLayoutStr + layouts.GetLayout( T_Target )->GetNameWithDigits() );

	lines.Append( SwitchKeyStr + Int2Str( switchKey ) );
	lines.Append( BlockSwitchKeyStr + Int2Str( (int)blockSwitchKey ) );

	lines.Append( DelayTimeStr + Int2Str( delayTime ) );
	lines.Append( SleepTimeStr + Int2Str( sleepTime ) );
	lines.Append( TimerTimeStr + Int2Str( timerTime ) );

	lines.Append( RulesStr );
	for( int i = 0; i < rules.GetCount(); i++ ) {
		lines.Append( rules.GetRule( i ) );
	}
	WriteFileLines( optionsFileName, lines, true );
}

int COptions::findLineStart( const CStrList& lines, const CString& line ) const
{
	for( int i = 0; i < lines.GetCount(); i++ ) {
		if( lines[i].Find( line ) == 0 ) {
			return i;
		}
	}
	return -1;
}

void COptions::loadFromFile() 
{
	CStrList lines;
	ReadFileLines( optionsFileName, lines, true );
	bool needToSaveFile = false;
	//rules идут в самом конце
	int iSourceLayoutStr = findLineStart( lines, SourceLayoutStr );
	int iTargetLayoutStr = findLineStart( lines, TargetLayoutStr );
	if( iSourceLayoutStr == -1 || iTargetLayoutStr == -1 ) {//неточность с языками
		layouts.GuessDefaultLayouts();//угадываем, откуда и куда надо транслировать
		needToSaveFile = true;
	} else {
		layouts.GuessLayout( lines[iSourceLayoutStr] + SourceLayoutStr.Length(), T_Source );
		layouts.GuessLayout( lines[iTargetLayoutStr] + TargetLayoutStr.Length(), T_Target );
	}
	int iSwitchKeyStr = findLineStart( lines, SwitchKeyStr );
	if( iSwitchKeyStr == -1 ) {
		switchKey = VK_SCROLL;
		needToSaveFile = true;
	} else {
		switchKey = (BYTE)Str2Int( lines[iSwitchKeyStr] + SwitchKeyStr.Length() );
	}
	int iBlockSwitchKeyStr = findLineStart( lines, BlockSwitchKeyStr );
	if( iBlockSwitchKeyStr == -1 ) {
		blockSwitchKey = ( VKeyGoodness[switchKey] != 0 );
		needToSaveFile = true;
	} else {
		blockSwitchKey = ( Str2Int( lines[iBlockSwitchKeyStr] + BlockSwitchKeyStr.Length() ) == 1 );
	}
	int iDelayTimeStr = findLineStart( lines, DelayTimeStr );
	int iSleepTimeStr = findLineStart( lines, SleepTimeStr );
	int iTimerTimeStr = findLineStart( lines, TimerTimeStr );
	if( iDelayTimeStr == -1 || iSleepTimeStr == -1 || iTimerTimeStr == -1 ) {//проблемы с задержками
		SetTimes( DefaultDelayTime, DefaultSleepTime, DefaultTimerTime );//задержки
		needToSaveFile = true;
	} else {
		delayTime = (short)Str2Int( lines[iDelayTimeStr] + DelayTimeStr.Length() );
		sleepTime = (short)Str2Int( lines[iSleepTimeStr] + SleepTimeStr.Length() );
		timerTime = (short)Str2Int( lines[iTimerTimeStr] + TimerTimeStr.Length() );
	}
	int iRulesStr = findLineStart( lines, RulesStr );
	if( iRulesStr == -1 ) {
		readDefaultRules();//запихиваем универсальные правила
		needToSaveFile = true;
	} else {
		for( int i = 0; i < iRulesStr; i++ ) {
			lines.PopFromHead();
		}
		rules.Fill( lines );//зачитываем правила
	}
	if( needToSaveFile ) {
		Save();//и сохраняем всё что загрузили
	}
}

void COptions::readDefaultRules()
{
	CStrList lines;
	ReadFileLines( CfgDefaultRulesFileName, lines, false, true );
	
	CString startLabel( layouts.GetLayout( T_Source )->GetLayoutDigitName() + 
		spaceBetweenLanguages + layouts.GetLayout( T_Target )->GetLayoutDigitName() );
	int startIndex = findLineStart( lines, startLabel );
	if( startIndex != -1 ) {//если нашли разделитель между нужными раскладками, то поудаляем лишние правила
		for( int i = 0; i <= startIndex; i++ ) {//удаляем всё перед разделителем
			lines.PopFromHead();
		}
		int endIndex = 0;//конечная строка
		for( endIndex = 0; endIndex < lines.GetCount() && lines[endIndex].Find( spaceBetweenLanguages ) == -1; endIndex++ ) {
		}
		for( ; endIndex < lines.GetCount(); ) {//убираем всё после последней строки
			lines.PopFromTail();
		}
	}
	rules.Fill( lines );
	rules.RemoveBad( GetLayout( T_Source ), GetLayout( T_Target ) );
}

void COptions::loadDefault()
{
	layouts.GuessDefaultLayouts();//угадываем, откуда и куда надо транслировать
	switchKey = VK_SCROLL;
	SetTimes( DefaultDelayTime, DefaultSleepTime, DefaultTimerTime );//задержки
	readDefaultRules();//запихиваем универсальные правила
	Save();//и сохраняем всё что загрузили
}

bool COptions::Load()
{
	layouts.Update();//выставляем раскадки которые в данный момент в системе
	optionsFileName = getOptionsFileName();
	if( ExistFile( optionsFileName ) ) {//идёт обычная загрузка
		loadFromFile();
	} else {//если файла с опциями нет, то всё по умолчанию
		loadDefault();
	}
	return AreOptionsGood();
}

bool COptions::LoadAs( const CString& newOptionsFileName )
{
	if( !ExistFile( newOptionsFileName ) ) {
		return false;
	}
	setOptionsFileName( newOptionsFileName );
	return Load();
}

bool COptions::AreOptionsGood( bool mayShowError /*= false*/ )
{
	return ( layouts.SaveSwitchSequences( mayShowError ) && 
		rules.CheckRules( mayShowError, GetLayout( T_Source ), GetLayout( T_Target ) ) );
}

CString COptions::CreateReport() const
{
	const CString reportFileName = L"report.txt";
	CStrVector lines;
	lines.Append( L"Опишите проблему тут: (Write your problem here:)" );
	lines.Append( L"(если у Вас проблема со скобками которые вставляются," );
	lines.Append( L"то попробуйте как итоговую раскладку поставить [Unicode - anyone])" );
	lines.Append( L"не забудьте указать с какой раскладки на какую транслитерация." );
	lines.Append( L"" );
	lines.Append( L"" );
	lines.Append( L"" );
	lines.Append( L"и пришлите мне (and send to me) broberg@yandex.ru" );

	lines.Append( L"Extra info" );
	lines.Append( L"Windows " + Int2Str( (DWORD)(LOBYTE(LOWORD(GetVersion()))) ) + 
		L"." + Int2Str( (DWORD)(HIBYTE(LOWORD(GetVersion()))) ) );
	lines.Append( L"CrazyKeys : " + GetCrazyKeysVersionEx() );
    lines.Append( L"All languages:" );
	for( int i = 0; i < GetLayouts().GetCount(); i++ ) {
		lines.Append( GetLayouts()[i]->GetNameWithSeq() );
	}
	lines.Append( L"Change betveen languages sequence : " + layouts.GetLanguageSwitchSeqStr() );
	lines.Append( L"Change betveen layouts sequence : " + layouts.GetLayoutSwitchSeqStr() );
	lines.Append( L"IsTextServicesOn = " + Int2Str( layouts.IsTextServicesOn() ? 1 : 0 ) );

	ReadFileLines( optionsFileName, lines, true );
	if( optionsFileName != getOptionsFileName() ) {//если файл настроек не совпадает с тем что запомнен
		ReadFileLines( getOptionsFileName(), lines, true );//то его тоже в отчёт берём
	}
	lines.Append( L"конец (the end)" );
	WriteFileLines( reportFileName, lines );
	return GetCurrentDirectory() + L"\\" + reportFileName;
}

