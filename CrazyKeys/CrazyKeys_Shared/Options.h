#pragma once
#include "Layouts.h"
#include "Rules.h"
#include "Queue.h"

class COptions {
public:
	COptions() {}
	//������ � ���������� �������������� �� ����� ��������� 
	int GetLayoutIndex( TLayouts which ) const { return layouts.GetLayoutIndex( which ); }
	const CPtrList<CLayout>& GetLayouts() const { return layouts.GetLayouts(); }
	const CLayout* GetLayout( TLayouts which ) const { return layouts.GetLayout( which ); }
	void SetLayout( int index, TLayouts which ) { layouts.SetLayout( index, which ); }
	CString GetLayoutSwitchSeqStr() const { return layouts.GetLayoutSwitchSeqStr(); }
	CString GetLanguageSwitchSeqStr() const { return layouts.GetLanguageSwitchSeqStr(); }
	bool HasRussianLayout() const { return layouts.HasRussianLayout(); }
	void UpdateLayouts() { layouts.Update(); }
	void SetOptimalLayoutSequences() { layouts.SetOptimalSequences(); }
	//������ �������������� �����
	void SetSwitchKey( BYTE _switchKey ) { switchKey = _switchKey; }
	BYTE GetSwitchKey() const { return switchKey; }
	void SetBlockSwitchKey( bool _blockSwitchKey ) { blockSwitchKey = _blockSwitchKey; }
	bool GetBlockSwitchKey() const { return blockSwitchKey; }
	void SetTimes( short _delayTime, short _sleepTime, short _timerTime );
	short GetTimerTime() const { return timerTime; }
	short GetDelayTime() const { return delayTime; }
	short GetSleepTime() const { return sleepTime; }
	bool IsUnicode() const { return layouts.IsUnicode(); }
	//������ ���������� � ��������
	CString GetFileName() const { return optionsFileName; }
	bool Load();
	bool LoadAs( const CString& newOptionsFileName );
	void Save( const CString& saveAsFileName = EmptyStr );
	bool AreOptionsGood( bool mayShowError = false );
	//������ ��� ��������� ����� ������ ������
	CRules& GetRules() { return rules; }
	const CRules& GetRules() const { return rules; }
	void RemoveBadRules() { rules.RemoveBad( GetLayout( T_Source ), GetLayout( T_Target ) ); };//������������� ������� �� ������

	CString CreateReport() const;

private:
	//��� ����� �����
	CString getUserCfgFileName() const;
	CString getOptionsFileName() const;
	void setOptionsFileName( const CString& fName ) const;
	int findLineStart( const CStrList& lines, const CString& line ) const;
	void loadFromFile();
	void loadDefault();
	void readDefaultRules();

	CLayouts layouts;//���������
	
	CString optionsFileName;//��� ����� �����

	BYTE switchKey;//������� �������������
	bool blockSwitchKey;//����������� ������������� �.�. �� �������� �������

	short timerTime;//����� �������, ��� ����� ����������� �������
	short delayTime;//����� �������� ����� ������������������ ��� ����������
	short sleepTime;//����� ��� ��� ��������� ������
	
	CRules rules;

	DISALLOW_EVIL_CONSTRUCTORS( COptions );
};
