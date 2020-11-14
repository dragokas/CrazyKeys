#pragma once
#include "Matrix.h"
#include "Str.h"

class CKANextStep;
class CRules;

class CKonAutTranslit {
public:
	CKonAutTranslit() {};
	~CKonAutTranslit();

	void BuildTranslitKA( const CRules& rules );//построение конавта по файлу, где 2 числа и таблица транслита
	void ResetState() { state = 0; }
	int GetState() const { return state; }
	
	bool NeedTranslit( wchar_t sourceLetter ) const;
	void GetNext( wchar_t letter, int& nLettersToDelete, CString& strToWrite );

private:
	struct CKACell {//описывает шаг конечного автомата
		CKACell( int _newState, int _nLettersToDelete, const CString& _strToWrite ) 
			: nextState( _newState ), nLettersToDelete( _nLettersToDelete ), strToWrite( _strToWrite ) {}
		int nextState;//новое состояние для перехода
		int nLettersToDelete;//количество букв которое надо затереть
		CString strToWrite;//посл-ть букв которую надо написать
		CKACell( const CKACell& other ) 
			: nextState( other.nextState ), nLettersToDelete( other.nLettersToDelete ), strToWrite( other.strToWrite ) {}
	};
	void deleteOldKARules();//удаляет память выделенную под матрицу указателей
	CVector<unsigned char> wcharToState;//преобразование wchar_t в номер буквы для конечного автомата
	//сам конечный автомат, состоит из указателей на структурки описывающие шаг.
	CMatrix<CKACell*> transitionRule;//transitionRule[i][j] писать нельзя, надо пользоваться cell( i, j )
	int state;//текущее состояние конавта
	const CKACell* cell( int state, wchar_t letter ) const { return transitionRule[state][wcharToState[letter]]; }
	CKACell*& cell( int state, wchar_t letter ) { return transitionRule[state][wcharToState[letter]]; }

	bool canAcceptLetters( const CString& inputWord ) const;
	void addStates( const CString& sourceStr, const CString& targetStr, unsigned int& nStates );//добавляет цепочку новых состояний
	void setDeleteLetters( const CString& inputWord );//высчитывает слолько букв надо удалять

	void printToFileVertical() const;//печать конавта вертикально
	void printToFileHorizontal() const;//печать конавта гризонтально

	DISALLOW_EVIL_CONSTRUCTORS( CKonAutTranslit );
};
