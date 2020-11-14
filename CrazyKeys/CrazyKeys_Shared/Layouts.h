#pragma once
#include "Queue.h"
#include "Layout.h"

enum TLayouts{ T_Source, T_Target };

class CLayouts {
public:
	CLayouts() { sourceLayoutIndex = 13;}
	void Update();//выставить все языки которые сейчас в системе

	bool IsUnicode() const { return layouts[targetLayoutIndex]->IsUnicode(); }
	int GetLayoutIndex( TLayouts which ) const { return (which == T_Source) ? sourceLayoutIndex : targetLayoutIndex; }
	const CPtrList<CLayout>& GetLayouts() const { return layouts; }
	const CLayout* GetLayout( TLayouts which ) const { return layouts[GetLayoutIndex(which)]; }
	void SetLayout( int index, TLayouts which ) { getIndexRef( which ) = index; }

	CString GetLanguageSwitchSeqStr() const { return getHotkeySwitchSeq( languageHotkey ); }
	CString GetLayoutSwitchSeqStr() const { return getHotkeySwitchSeq( layoutHotkey ); }
	bool SaveSwitchSequences( bool mayShowError = false ) const;

	void GuessLayout( CString layoutName, TLayouts which );
	void GuessDefaultLayouts();
	bool HasRussianLayout() const { return hasRussianLayout; };

	void SetOptimalSequences();
	bool IsTextServicesOn() const { return isTextServicesOn; }

private:
	int& getIndexRef( TLayouts which ) { return (which == T_Source) ? sourceLayoutIndex : targetLayoutIndex; }

	void updateHKL( const CVector<HKL>& hKLs );
	void createHotKeysList( CList<CRegHotKey>& hotKeys ) const; 

	CString getHotkeySwitchSeq( int hotkey ) const;
	void loadChangeBetweenLanguagesSeqStr();
	void loadChangeBetweenLayoutsSeqStr();
	bool recieveChangeLayoutStrings( CString& toSource, CString& toTarget ) const;

	bool needChangeLayout() const;
	bool needHotkeys( bool isOptimalQuery ) const;

	bool isAdvancedTextServicesOn() const;
	void writeToRegOptimalSwitchHotkeySeq( bool needHotkeys ) const;
	void deleteBadHotkeysFromRegistry( CList<CRegHotKey>& hotKeys, bool needHotkeys ) const;
	void deleteDoubleHotkeysFromRegistry( CList<CRegHotKey>& hotKeys ) const;
	void deleteFromRegIfAlt( int layoutIndex, CList<CRegHotKey>& hotKeys );
	void writeToRegHotkeysSeq( bool needHotkeys );
	void writeToRegSourceAndTargetWithCtrl( CList<CRegHotKey>& hotKeys, bool isSecondCall = false ); 
	bool writeToRegHotkeyWithCtrl( int layoutIndex, CList<CRegHotKey>& hotKeys );
	void deleteAllHotkeysWithtAlt( CList<CRegHotKey>& hotKeys );
	void writeToReg( const CRegHotKey& hotkey ) const;

	CPtrList<CLayout> layouts;
	int sourceLayoutIndex;
	int targetLayoutIndex;

	int languageHotkey;
	int layoutHotkey;
	bool isTextServicesOn;
	bool hasRussianLayout;

	DISALLOW_EVIL_CONSTRUCTORS( CLayouts );
};