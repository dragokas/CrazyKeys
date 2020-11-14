#pragma once
static const int MaxStrLength = 128;//макс. длина строчки в диалоге

static const TCHAR Translation[][2][MaxStrLength] = {
	{ T("Cancel"), T("Отмена") },
	{ T("Help"), T("Помощь") },
//////////////////////////////////////////////////////////////////////////
	//ДИАЛОГ "УСТАНОВКА"
	{ T("Setup CrazyKeys translit"), T("Запуск CrazyKeys translit") },
	{ T("Welcome, CrazyKeys, it is"), T("Добро пожаловать, CrazyKeys это") },
	{ T("any letters\r\nin your\r\nFAVORITE layout"), T("Любая буква\r\nв вашей\r\nлюбимой раскладке") }, 
	{ T("Location"), T("Место расположения") },
	{ T("copy to "), T("скопировать в ") },
	{ T("leave program in current directory"), T("оставить программу в текущей папке") },
	{ T("Shortcuts"), T("Ярлыки") },
	{ T("create shortcut on desktop"), T("создать ярлык на рабочем столе") },
	{ T("create shortcut in windows startup"), T("создать ярлык в автозагрузке") },
//////////////////////////////////////////////////////////////////////////
	///ДИАЛОГ "ОПЦИИ"
//	{ T("Options CrazyKeys translit"), T("Настройки CrazyKeys translit") },
	//раскладка исходная
	{ T("Your keyboard layouts"), T("Раскладки вашей клавиатуры") },
	{ T("Source keyboard"), T("Исходная раскладка") },
	{ T("Target keyboard"), T("Итоговая раскладка") },
	//раскладка итоговая
	{ T("Change key sequence"), T("Смена сочетания клавиш") },
	{ T("input language"), T("языки ввода") },
	{ T("keyboard layout"), T("раскладки") },
	{ T("None"), T("нет") },	
	//языки и раскладки
	{ T("Text Services and Input Languages"), T("Языки и службы текстевого ввода") },
	{ T("Dialog"), T("Диалог") },
	{ T("Refresh"), T("Обновить") },	
	{ T("Optimal"), T("Наилучшие") },
	//правила
	{ T("Rules for convertion"), T("Правила преобразования") },
	{ T("source"), T("исход.") },
	{ T("target"), T("итог.") },
	{ T("Add"), T("Добавить") },
	{ T("Edit"), T("Изменить") },
	{ T("Delete selected rule"), T("Удалить правило") },
	{ T("Remove bad rules"), T("Удалить плохие") },
	{ T("Select typical rules"), T("Выбор типичных правил") },
	//сохранение опций
	{ T("Options file"), T("Файл настроек") },
	{ T("File Name"), T("Имя файла") },
	{ T("Load"), T("Загрузить") },
	{ T("Save"), T("Сохранить") },
	{ T("Save As"), T("Сохранить как") },
	//переключатель
	{ T("Switch key"), T("Переключатель") },
	{ T("Set focus and enter key"), T("В фокусе нажмите клавишу") },
	{ T("Block for system"), T("Блокировать") },
	{ T("It's better to use another"), T("Выберите другую") },
	//дополнительные пар-ры
	{ T("Advanced delays"), T("Продвинутые задержки") },
	{ T("Bonding delay"), T("Время склеивания") },
	{ T("Gap delay"), T("Время промежутков") },
	{ T("Timer delay"), T("Время таймера") },
	//язык диалога и интерфейса
	{ T("Interface language"), T("Язык интерфейса") },

	{ T("Version date"), T("Дата версии") },

//////////////////////////////////////////////////////////////////////////
	//ДИАЛОГ "ТРАНСЛИТЫ"
	{ T("Choosing a basic set of rules"), T("Выбор базового набора правил") },
	{T("Choice transliteration variant"), T("Выбирете вариант транслитерации") },
	{ T("Unused letters in layouts"), T("Неиспользуемые буквы в раскладках") },

	{T("< Prev"), T("< Назад") },
	{T("Next >"), T("Далее >") },
//////////////////////////////////////////////////////////////////////////
	//ДИАЛОГ "ДОБАВИТЬ"
	{T("Rule for transformation"), T("Правило для преобразования") },
	{ T("Source"), T("Исходная") },
	{ T("Target"), T("Итоговая") },
	//изменяемые строки в диалоге
	{ T("version "), T("версия ") },
	{ T(", from "), T(", от ") },
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	//остальные строки в программе
	//меню
	{ T("Options"), T("Настройки") },
	{ T("RUN"), T("ЗАПУСК") },
	{ T("PAUSE"), T("ПАУЗА") },
	{ T("STOP"), T("СТОП") },
	{ T("Exit"), T("Выход") },
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	//ошибки при запуске
	{ T("This program can be run only on windows NT/2000/XP/2003\n\
			but not on windows 3.1/95/98/Millennium"), 
		T("Эта программа может только работать под windows NT/2000/XP/2003\n\
			но не под windows 3.1/95/98/Millennium") },
	{ T("Error"), T("Ошибка") },
	{ T("The program is already running, another copy can not be run"), 
		T("Программа уже запущена, ещё одну копию запустить нельзя") },
	{ T("Internal error"), T("Внутренняя ошибка") },
	{ T("Problems with resource files"), T("Проблемы с файлами ресурсов") },
	//ошибки при установке
	{ T("Can't write to:\n%s"), T("Не возможно записать в:\n%s") },
	//ошибки с опциями
	{ T("Problem with layout sequence"), T("Проблемы с комбинацией переключения языков") },
	{ T("Some change layout sequences may be incorrect\
	\nto source sequence( %s )\nto target sequence( %s )"), 
	T("Возможно комбинация переключения не корректна\
	  \nк исходной( %s )\nк итоговой( %s )") },
	{ T("Empty"), T("Пусто") },
	//ошибки с правилами
	{ T("You have two similar rules"), T("2 одинаковых правила невозможны") },
	{ T("Error: bad rule"), T("") },
	{ T("Not all letters from source sequence you can type on source keyboard"), 
		T("Не все буквы из исходной последовательности можно напечатать в исходной раскладке") },
	{ T("Not all letters from target sequence you can type on target keyboard"), 
		T("Не все буквы из последовательности могут быть напечатаны в итоговой раскладке") },
	{ T("Not all letters from source sequence can be translated"), 
		T("Не все буквы из последовательности имеют правила перевода в итоговую раскладку") },
	{ T("No space is allowed in source or target part of sequence"), 
		T("Пробел не разрешён в исходной или итоговой последовательности") },
	{ T("Not defined reason"), T("") },
	{ T("There is an invalid rule:\n'%s'->'%s'\nbecause:\n%s\n\nRemove this rule?"), 
		T("Есть недопустимое правило:\n'%s'->'%s'\nпричина:\n%s\n\nУбрать это правило?") },
	
	{ T(""), T("") },
	{ T(""), T("") },
	{ T(""), T("") },
	
	{ T(""), T("") }//конец перевода
};
