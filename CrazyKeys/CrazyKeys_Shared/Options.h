#pragma once
#include "Layouts.h"
#include "Rules.h"
#include "Queue.h"

class COptions {
public:
	COptions() {}
	//методы о раскладках переадресуются на класс раскладок 
	int GetLayoutIndex( TLayouts which ) const { return layouts.GetLayoutIndex( which ); }
	const CPtrList<CLayout>& GetLayouts() const { return layouts.GetLayouts(); }
	const CLayout* GetLayout( TLayouts which ) const { return layouts.GetLayout( which ); }
	void SetLayout( int index, TLayouts which ) { layouts.SetLayout( index, which ); }
	CString GetLayoutSwitchSeqStr() const { return layouts.GetLayoutSwitchSeqStr(); }
	CString GetLanguageSwitchSeqStr() const { return layouts.GetLanguageSwitchSeqStr(); }
	bool HasRussianLayout() const { return layouts.HasRussianLayout(); }
	void UpdateLayouts() { layouts.Update(); }
	void SetOptimalLayoutSequences() { layouts.SetOptimalSequences(); }
	//методы дополнительных опций
	void SetSwitchKey( BYTE _switchKey ) { switchKey = _switchKey; }
	BYTE GetSwitchKey() const { return switchKey; }
	void SetBlockSwitchKey( bool _blockSwitchKey ) { blockSwitchKey = _blockSwitchKey; }
	bool GetBlockSwitchKey() const { return blockSwitchKey; }
	void SetTimes( short _delayTime, short _sleepTime, short _timerTime );
	short GetTimerTime() const { return timerTime; }
	short GetDelayTime() const { return delayTime; }
	short GetSleepTime() const { return sleepTime; }
	bool IsUnicode() const { return layouts.IsUnicode(); }
	//методы сохранения и загрузки
	CString GetFileName() const { return optionsFileName; }
	bool Load();
	bool LoadAs( const CString& newOptionsFileName );
	void Save( const CString& saveAsFileName = EmptyStr );
	bool AreOptionsGood( bool mayShowError = false );
	//методы для изменений через диалог правил
	CRules& GetRules() { return rules; }
	const CRules& GetRules() const { return rules; }
	void RemoveBadRules() { rules.RemoveBad( GetLayout( T_Source ), GetLayout( T_Target ) ); };//отфильтровать правила от плохих

	CString CreateReport() const;

private:
	//для файла опций
	CString getUserCfgFileName() const;
	CString getOptionsFileName() const;
	void setOptionsFileName( const CString& fName ) const;
	int findLineStart( const CStrList& lines, const CString& line ) const;
	void loadFromFile();
	void loadDefault();
	void readDefaultRules();

	CLayouts layouts;//раскладки
	
	CString optionsFileName;//имя файла опций

	BYTE switchKey;//клавиша переключатель
	bool blockSwitchKey;//блокировать переключатель т.е. не отдавать системе

	short timerTime;//время таймера, как часто проверяется очередь
	short delayTime;//время ожидания одной последовательности для склеивания
	short sleepTime;//время сна при отсылании клавиш
	
	CRules rules;

	DISALLOW_EVIL_CONSTRUCTORS( COptions );
};
