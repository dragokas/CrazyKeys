#pragma once
#include <windows.h>

const int QueueSize = 512;

class CTimerQueue {
public:
	CTimerQueue() : last( 1 ), first( 1 ) 
	{
		ZeroMemory( items, sizeof( CQueueItem ) * QueueSize );
	}
	~CTimerQueue() {};
	
	void Add( const DWORD& time, const wchar_t& sourceLetter, const bool& isUpper )//добавить в очередь
	{
		int index = first % QueueSize;//индекс, куда будем класть элемент
		items[index].time = time; 
		items[index].sourceLetter = sourceLetter; 
		items[index].isUpper = isUpper;
		first++;	
	}

	void Remove( DWORD& timeOdds, wchar_t& sourceLetter, bool& isUpper )//убрать из очереди
	{
		int index = last % QueueSize;//индекс последнего элемента
		int indexPrev = (last + QueueSize - 1) % QueueSize;//индекс предидущего
		timeOdds = items[index].time - items[indexPrev].time;
		sourceLetter = items[index].sourceLetter;
		isUpper = items[index].isUpper;
		last++;
	}

	bool HasNewChars() const// есть ли буквы в очереди
	{
		return ( last < first ); 
	}
	
private:
	struct CQueueItem {
		unsigned long time;//время прибытия буквы в хук (из структуры хука)
		wchar_t sourceLetter;//буква
		bool isUpper;//в каком регистре
	};

	CQueueItem items[QueueSize];//реализация очереди массивом, т.к. требуется быстродействие

	int last;//индекс последней буквы
	int first;//индекс буквыкуда можно положить первую букву

	DISALLOW_EVIL_CONSTRUCTORS( CTimerQueue );
};
