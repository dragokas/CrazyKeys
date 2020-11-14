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
	
	void Add( const DWORD& time, const wchar_t& sourceLetter, const bool& isUpper )//�������� � �������
	{
		int index = first % QueueSize;//������, ���� ����� ������ �������
		items[index].time = time; 
		items[index].sourceLetter = sourceLetter; 
		items[index].isUpper = isUpper;
		first++;	
	}

	void Remove( DWORD& timeOdds, wchar_t& sourceLetter, bool& isUpper )//������ �� �������
	{
		int index = last % QueueSize;//������ ���������� ��������
		int indexPrev = (last + QueueSize - 1) % QueueSize;//������ �����������
		timeOdds = items[index].time - items[indexPrev].time;
		sourceLetter = items[index].sourceLetter;
		isUpper = items[index].isUpper;
		last++;
	}

	bool HasNewChars() const// ���� �� ����� � �������
	{
		return ( last < first ); 
	}
	
private:
	struct CQueueItem {
		unsigned long time;//����� �������� ����� � ��� (�� ��������� ����)
		wchar_t sourceLetter;//�����
		bool isUpper;//� ����� ��������
	};

	CQueueItem items[QueueSize];//���������� ������� ��������, �.�. ��������� ��������������

	int last;//������ ��������� �����
	int first;//������ ��������� ����� �������� ������ �����

	DISALLOW_EVIL_CONSTRUCTORS( CTimerQueue );
};
