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
{//����� ���, ��� ����� ������������
	CString fName;//��� ����� �����, <user>.sav, ������� �� � ����� getUserCfgFileName()
	if( ExistFile( getUserCfgFileName() ) && ReadFileBuffer( getUserCfgFileName(), fName ) ) {
		if( ExistFile( fName ) ) {
			return fName;
		}
	}
	fName = GetUserName() + L".sav";//������ �� ��������� ��� �����
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
	//rules ���� � ����� �����
	int iSourceLayoutStr = findLineStart( lines, SourceLayoutStr );
	int iTargetLayoutStr = findLineStart( lines, TargetLayoutStr );
	if( iSourceLayoutStr == -1 || iTargetLayoutStr == -1 ) {//���������� � �������
		layouts.GuessDefaultLayouts();//���������, ������ � ���� ���� �������������
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
	if( iDelayTimeStr == -1 || iSleepTimeStr == -1 || iTimerTimeStr == -1 ) {//�������� � ����������
		SetTimes( DefaultDelayTime, DefaultSleepTime, DefaultTimerTime );//��������
		needToSaveFile = true;
	} else {
		delayTime = (short)Str2Int( lines[iDelayTimeStr] + DelayTimeStr.Length() );
		sleepTime = (short)Str2Int( lines[iSleepTimeStr] + SleepTimeStr.Length() );
		timerTime = (short)Str2Int( lines[iTimerTimeStr] + TimerTimeStr.Length() );
	}
	int iRulesStr = findLineStart( lines, RulesStr );
	if( iRulesStr == -1 ) {
		readDefaultRules();//���������� ������������� �������
		needToSaveFile = true;
	} else {
		for( int i = 0; i < iRulesStr; i++ ) {
			lines.PopFromHead();
		}
		rules.Fill( lines );//���������� �������
	}
	if( needToSaveFile ) {
		Save();//� ��������� �� ��� ���������
	}
}

void COptions::readDefaultRules()
{
	CStrList lines;
	ReadFileLines( CfgDefaultRulesFileName, lines, false, true );
	
	CString startLabel( layouts.GetLayout( T_Source )->GetLayoutDigitName() + 
		spaceBetweenLanguages + layouts.GetLayout( T_Target )->GetLayoutDigitName() );
	int startIndex = findLineStart( lines, startLabel );
	if( startIndex != -1 ) {//���� ����� ����������� ����� ������� �����������, �� ��������� ������ �������
		for( int i = 0; i <= startIndex; i++ ) {//������� �� ����� ������������
			lines.PopFromHead();
		}
		int endIndex = 0;//�������� ������
		for( endIndex = 0; endIndex < lines.GetCount() && lines[endIndex].Find( spaceBetweenLanguages ) == -1; endIndex++ ) {
		}
		for( ; endIndex < lines.GetCount(); ) {//������� �� ����� ��������� ������
			lines.PopFromTail();
		}
	}
	rules.Fill( lines );
	rules.RemoveBad( GetLayout( T_Source ), GetLayout( T_Target ) );
}

void COptions::loadDefault()
{
	layouts.GuessDefaultLayouts();//���������, ������ � ���� ���� �������������
	switchKey = VK_SCROLL;
	SetTimes( DefaultDelayTime, DefaultSleepTime, DefaultTimerTime );//��������
	readDefaultRules();//���������� ������������� �������
	Save();//� ��������� �� ��� ���������
}

bool COptions::Load()
{
	layouts.Update();//���������� �������� ������� � ������ ������ � �������
	optionsFileName = getOptionsFileName();
	if( ExistFile( optionsFileName ) ) {//��� ������� ��������
		loadFromFile();
	} else {//���� ����� � ������� ���, �� �� �� ���������
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
	lines.Append( L"������� �������� ���: (Write your problem here:)" );
	lines.Append( L"(���� � ��� �������� �� �������� ������� �����������," );
	lines.Append( L"�� ���������� ��� �������� ��������� ��������� [Unicode - anyone])" );
	lines.Append( L"�� �������� ������� � ����� ��������� �� ����� ��������������." );
	lines.Append( L"" );
	lines.Append( L"" );
	lines.Append( L"" );
	lines.Append( L"� �������� ��� (and send to me) broberg@yandex.ru" );

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
	if( optionsFileName != getOptionsFileName() ) {//���� ���� �������� �� ��������� � ��� ��� ��������
		ReadFileLines( getOptionsFileName(), lines, true );//�� ��� ���� � ����� ����
	}
	lines.Append( L"����� (the end)" );
	WriteFileLines( reportFileName, lines );
	return GetCurrentDirectory() + L"\\" + reportFileName;
}

