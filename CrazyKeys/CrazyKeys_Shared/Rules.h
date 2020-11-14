#pragma once
#include "Str.h"
#include "Vector.h"
#include "Queue.h"
#include "Matrix.h"

class CLayout;

class CRules {
public:
	enum TSortedType{ ST_Source, ST_Target, ST_Internal, ST_Unknown };

	CRules() {}

	CString Source( int index ) const { return rules[index].source; }
	CString Target( int index ) const { return rules[index].target; }
	CString GetRule( int index ) const { return rules[index].source + L" " + rules[index].target; }

	void Delete( int index ) { rules.Delete( index ); }
	void Add( const CString& source, const CString& target );
	int GetCount() const { return rules.Size(); }

	void Fill( const CStrList& lines );
	void RemoveBad( const CLayout* source, const CLayout* target );
	bool CheckRules( bool mayShowError, const CLayout* source, const CLayout* target );
	void Sort( TSortedType how );
	int FindSourceIndex( const CString& source ) const { return rules.Find<CEqualSource>( source ); }

	const CStrMatrix& GetRulesTable() const;//выдать матрицу правил

private:
	struct CRule {//правило
		CRule( const CString& _source, const CString& _target ) :
			source( _source ), target( _target ) {}
		CString source;//исходная
		CString target;//итоговая
	};

	struct CEqualSource {//для поиска в правилах source
		static bool Eq( const CRule& rule, const CString& source )
			{ return ( rule.source == source ); }
	};
	struct CCmpBySource {//сравнание по source в сортировке
		static int Cmp( const CRule& a, const CRule& b )
			{ return (a.source < b.source) ? -1: ( b.source < a.source ) ? 1 : 0; }
	};
	struct CCmpByTarget {//сравнание по target в сортировке
		static int Cmp( const CRule& a, const CRule& b )
			{ return (a.target < b.target) ? -1: ( b.target < a.target ) ? 1 : 0; }
	};
	struct CCmpByInternal {//сравнание для сортировки при проверке правил и построения конавта
		static int Cmp( const CRule& a, const CRule& b )
			{ return (a.source.Length() < b.source.Length() ) ? -1 : 
		( b.source.Length() < a.source.Length() ) ? 1 : CCmpBySource::Cmp( a, b ); }
	};
	//тип ошибки в правилах
	enum TRuleErrorType{ RET_Ok, RET_Same, RET_NotInSource, RET_NotInTarget, RET_NoSubTrans, RET_SpacesInSequence };

	bool askUserToRemoveRule( int wrongRuleIndex, TRuleErrorType errorType ) const;
	int getFirstWrongRuleIndex( TRuleErrorType& errorType, const CLayout* source, const CLayout* target, int fromIndex = 0 ) const;
	bool checkSortedRules( bool mayShowError, const CLayout* source, const CLayout* target );

	CVector<CRule> rules;//правила
	TSortedType sortedType;//как правила сейчас отсортированны
	mutable CStrMatrix table;//закешированные правила в виде таблицы для удобства показа

	DISALLOW_EVIL_CONSTRUCTORS( CRules );
};