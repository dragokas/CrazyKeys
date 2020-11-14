#pragma once
#include "Matrix.h"
#include "Str.h"

class CKANextStep;
class CRules;

class CKonAutTranslit {
public:
	CKonAutTranslit() {};
	~CKonAutTranslit();

	void BuildTranslitKA( const CRules& rules );//���������� ������� �� �����, ��� 2 ����� � ������� ���������
	void ResetState() { state = 0; }
	int GetState() const { return state; }
	
	bool NeedTranslit( wchar_t sourceLetter ) const;
	void GetNext( wchar_t letter, int& nLettersToDelete, CString& strToWrite );

private:
	struct CKACell {//��������� ��� ��������� ��������
		CKACell( int _newState, int _nLettersToDelete, const CString& _strToWrite ) 
			: nextState( _newState ), nLettersToDelete( _nLettersToDelete ), strToWrite( _strToWrite ) {}
		int nextState;//����� ��������� ��� ��������
		int nLettersToDelete;//���������� ���� ������� ���� ��������
		CString strToWrite;//����-�� ���� ������� ���� ��������
		CKACell( const CKACell& other ) 
			: nextState( other.nextState ), nLettersToDelete( other.nLettersToDelete ), strToWrite( other.strToWrite ) {}
	};
	void deleteOldKARules();//������� ������ ���������� ��� ������� ����������
	CVector<unsigned char> wcharToState;//�������������� wchar_t � ����� ����� ��� ��������� ��������
	//��� �������� �������, ������� �� ���������� �� ���������� ����������� ���.
	CMatrix<CKACell*> transitionRule;//transitionRule[i][j] ������ ������, ���� ������������ cell( i, j )
	int state;//������� ��������� �������
	const CKACell* cell( int state, wchar_t letter ) const { return transitionRule[state][wcharToState[letter]]; }
	CKACell*& cell( int state, wchar_t letter ) { return transitionRule[state][wcharToState[letter]]; }

	bool canAcceptLetters( const CString& inputWord ) const;
	void addStates( const CString& sourceStr, const CString& targetStr, unsigned int& nStates );//��������� ������� ����� ���������
	void setDeleteLetters( const CString& inputWord );//����������� ������� ���� ���� �������

	void printToFileVertical() const;//������ ������� �����������
	void printToFileHorizontal() const;//������ ������� ������������

	DISALLOW_EVIL_CONSTRUCTORS( CKonAutTranslit );
};
