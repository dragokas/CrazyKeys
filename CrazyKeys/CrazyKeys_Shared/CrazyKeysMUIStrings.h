#pragma once
static const int MaxStrLength = 128;//����. ����� ������� � �������

static const TCHAR Translation[][2][MaxStrLength] = {
	{ T("Cancel"), T("������") },
	{ T("Help"), T("������") },
//////////////////////////////////////////////////////////////////////////
	//������ "���������"
	{ T("Setup CrazyKeys translit"), T("������ CrazyKeys translit") },
	{ T("Welcome, CrazyKeys, it is"), T("����� ����������, CrazyKeys ���") },
	{ T("any letters\r\nin your\r\nFAVORITE layout"), T("����� �����\r\n� �����\r\n������� ���������") }, 
	{ T("Location"), T("����� ������������") },
	{ T("copy to "), T("����������� � ") },
	{ T("leave program in current directory"), T("�������� ��������� � ������� �����") },
	{ T("Shortcuts"), T("������") },
	{ T("create shortcut on desktop"), T("������� ����� �� ������� �����") },
	{ T("create shortcut in windows startup"), T("������� ����� � ������������") },
//////////////////////////////////////////////////////////////////////////
	///������ "�����"
//	{ T("Options CrazyKeys translit"), T("��������� CrazyKeys translit") },
	//��������� ��������
	{ T("Your keyboard layouts"), T("��������� ����� ����������") },
	{ T("Source keyboard"), T("�������� ���������") },
	{ T("Target keyboard"), T("�������� ���������") },
	//��������� ��������
	{ T("Change key sequence"), T("����� ��������� ������") },
	{ T("input language"), T("����� �����") },
	{ T("keyboard layout"), T("���������") },
	{ T("None"), T("���") },	
	//����� � ���������
	{ T("Text Services and Input Languages"), T("����� � ������ ���������� �����") },
	{ T("Dialog"), T("������") },
	{ T("Refresh"), T("��������") },	
	{ T("Optimal"), T("���������") },
	//�������
	{ T("Rules for convertion"), T("������� ��������������") },
	{ T("source"), T("�����.") },
	{ T("target"), T("����.") },
	{ T("Add"), T("��������") },
	{ T("Edit"), T("��������") },
	{ T("Delete selected rule"), T("������� �������") },
	{ T("Remove bad rules"), T("������� ������") },
	{ T("Select typical rules"), T("����� �������� ������") },
	//���������� �����
	{ T("Options file"), T("���� ��������") },
	{ T("File Name"), T("��� �����") },
	{ T("Load"), T("���������") },
	{ T("Save"), T("���������") },
	{ T("Save As"), T("��������� ���") },
	//�������������
	{ T("Switch key"), T("�������������") },
	{ T("Set focus and enter key"), T("� ������ ������� �������") },
	{ T("Block for system"), T("�����������") },
	{ T("It's better to use another"), T("�������� ������") },
	//�������������� ���-��
	{ T("Advanced delays"), T("����������� ��������") },
	{ T("Bonding delay"), T("����� ����������") },
	{ T("Gap delay"), T("����� �����������") },
	{ T("Timer delay"), T("����� �������") },
	//���� ������� � ����������
	{ T("Interface language"), T("���� ����������") },

	{ T("Version date"), T("���� ������") },

//////////////////////////////////////////////////////////////////////////
	//������ "���������"
	{ T("Choosing a basic set of rules"), T("����� �������� ������ ������") },
	{T("Choice transliteration variant"), T("�������� ������� ��������������") },
	{ T("Unused letters in layouts"), T("�������������� ����� � ����������") },

	{T("< Prev"), T("< �����") },
	{T("Next >"), T("����� >") },
//////////////////////////////////////////////////////////////////////////
	//������ "��������"
	{T("Rule for transformation"), T("������� ��� ��������������") },
	{ T("Source"), T("��������") },
	{ T("Target"), T("��������") },
	//���������� ������ � �������
	{ T("version "), T("������ ") },
	{ T(", from "), T(", �� ") },
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	//��������� ������ � ���������
	//����
	{ T("Options"), T("���������") },
	{ T("RUN"), T("������") },
	{ T("PAUSE"), T("�����") },
	{ T("STOP"), T("����") },
	{ T("Exit"), T("�����") },
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	//������ ��� �������
	{ T("This program can be run only on windows NT/2000/XP/2003\n\
			but not on windows 3.1/95/98/Millennium"), 
		T("��� ��������� ����� ������ �������� ��� windows NT/2000/XP/2003\n\
			�� �� ��� windows 3.1/95/98/Millennium") },
	{ T("Error"), T("������") },
	{ T("The program is already running, another copy can not be run"), 
		T("��������� ��� ��������, ��� ���� ����� ��������� ������") },
	{ T("Internal error"), T("���������� ������") },
	{ T("Problems with resource files"), T("�������� � ������� ��������") },
	//������ ��� ���������
	{ T("Can't write to:\n%s"), T("�� �������� �������� �:\n%s") },
	//������ � �������
	{ T("Problem with layout sequence"), T("�������� � ����������� ������������ ������") },
	{ T("Some change layout sequences may be incorrect\
	\nto source sequence( %s )\nto target sequence( %s )"), 
	T("�������� ���������� ������������ �� ���������\
	  \n� ��������( %s )\n� ��������( %s )") },
	{ T("Empty"), T("�����") },
	//������ � ���������
	{ T("You have two similar rules"), T("2 ���������� ������� ����������") },
	{ T("Error: bad rule"), T("") },
	{ T("Not all letters from source sequence you can type on source keyboard"), 
		T("�� ��� ����� �� �������� ������������������ ����� ���������� � �������� ���������") },
	{ T("Not all letters from target sequence you can type on target keyboard"), 
		T("�� ��� ����� �� ������������������ ����� ���� ���������� � �������� ���������") },
	{ T("Not all letters from source sequence can be translated"), 
		T("�� ��� ����� �� ������������������ ����� ������� �������� � �������� ���������") },
	{ T("No space is allowed in source or target part of sequence"), 
		T("������ �� �������� � �������� ��� �������� ������������������") },
	{ T("Not defined reason"), T("") },
	{ T("There is an invalid rule:\n'%s'->'%s'\nbecause:\n%s\n\nRemove this rule?"), 
		T("���� ������������ �������:\n'%s'->'%s'\n�������:\n%s\n\n������ ��� �������?") },
	
	{ T(""), T("") },
	{ T(""), T("") },
	{ T(""), T("") },
	
	{ T(""), T("") }//����� ��������
};
