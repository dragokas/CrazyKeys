#pragma once
#include <windows.h>
#include "Queue.h"
#include "Vector.h"
#include "Dialog.h"

class CLayout;

class CStandartRulesDlg : public CDialog {
public:	
	CStandartRulesDlg( HINSTANCE hInstance, const CStrList& rulesFromFile, const CLayout* sourceLayout, const CLayout* targetLayout )
		: CDialog( hInstance ), currentTranslit( -1 ) { fillTranslits( rulesFromFile, sourceLayout, targetLayout ); }
	//����� �� �������� ������ ��� ������ � ������
	bool NeedChoiseDialog() const { return ( 0 < translits.Size() ); }
	//�������� ������
	void ShowDlg( HWND hParentWnd );
	//�������� ����� ������� �������, false - � ������ ���� ��� ����� cancel
	bool RecieveChosenTranslit( CStrList& recieveRules ) const;

protected:
	virtual BOOL OnInitDlg();
	virtual BOOL OnHelpDlg( HELPINFO* helpInfo );
	virtual BOOL OnCommandDlg( WPARAM wParam, LPARAM lParam );

private:
	//���������� ����������� ������������� ����������
	class CTranslit {//��� ������ ���������
	public:
		CTranslit( const CString& header, const CStrVector& sourceAlpha, const CStrVector& targetAlpha );
		bool AddRule( const CString& rule, const CLayout* sourceLayout, const CLayout* targetLayout );
		void SortTranslit() { originalRules.QuickSort(); translitTable.QuickSort( cmpLastLetter ); }

		static bool IsOriginalRulesEqual( const CTranslit& a, const CTranslit& b ) 
		{ return ( a.originalRules == b.originalRules ); }
		
		CString GetSourceUselessStr() const { return getUselessStr( sourceUseless ); }
		CString GetTargetUselessStr() const { return getUselessStr( targetUseless ); }
		CString GetDigitName() const { return digitName; }
		CString GetUserName() const { return userName; }
		CString GetEditColumn( int index ) const;

		void RecieveRules( CStrList& recieveRules ) const { recieveRules.CopyFrom( originalRules ); }

	private:
		static bool isLastLetter( const CString& line, const CString& target ) 
		{ return ( target.Length() == 1 && line.Last() == target.First() ); }

		static int cmpLastLetter( const CString& a, const CString& b ) 
		{  return ( a.Last() < b.Last() ) ? -1 : ( b.Last() < a.Last() ) ? 1 : 0; }
		
		CString getUselessStr( const CStrVector& useless ) const;//�������� ������� ����������������� ����

		CString digitName;//��� ���������, �������� ������ ����
		CString userName;//�������� ���������
		CStrVector translitTable;//������ ������ ���� : "s1, s2 - t", ��� s1, s2 �������� ������������������, � t ��� �������� �����
		CStrVector sourceUseless;//������� ��������� ��������. �����, ��� ������� ��� ������ ��������������
		CStrVector targetUseless;//������� ��������� ��������. �����, ������� ������� ������
		CStrVector originalRules;//������������ �������
	};

	//��������� � ���������� ������� ����� �� ���������
	void fillAlpha( CStrVector& alpha, const CVector<WORD>& wcharToScanCode );
	//��������� ��� ���������
	void fillTranslits( const CStrList& rules, const CLayout* sourceLayout, const CLayout* targetLayout );
	//��������� ���� ������
	void fillDlg() const;
	
	CPtrList<CTranslit> translits;//���������
	int currentTranslit;//������ ���������/������������ ��������

	DISALLOW_EVIL_CONSTRUCTORS( CStandartRulesDlg );
};















