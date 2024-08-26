# Brief Description for Project Sources

  - [umba-brief-scanner (Entry points)](#user-content-umba-brief-scanner-entry-points)
  - [umba-brief-scanner](#user-content-umba-brief-scanner)


# umba-brief-scanner (Entry points)

 - **main.cpp** - Для тестирования, файл с описанием и main'ом
 - **qt_stub.cpp** - Стаб, для того, чтобы windeployqt подсосал нужный рантайм для MSVC - а то совсем без Qt он не работает
 - **test0.cpp** - 
 - **test00.cpp** - 
 - **test01.cpp** - Перетащил всю требуху из другого проекта, лишнее отрежу потом
 - **test02.cpp** - Простой тест регулярок по поиску точки входа и brief'а
 - **test03.cpp** - Тест arithmetic_strong_typedef - просто присунул тут по случаю
 - **tmp.cpp** - Утилита umba-brief-scanner - сканирование исходных текстов на C++ и получение списка описаний
 - **umba-brief-scanner.cpp** - Утилита umba-brief-scanner - сканирование исходных текстов на C++ и получение списка описаний


# umba-brief-scanner

 - **app_config.h** - Класс конфигурации
 - **app_ver_config.h** - Информация о приложении umba-brief-scanner и его версии
 - **arg_parser.h** - Разбор аргументов командной строки
 - **brief_info.h** - Извлечение brief info
 - **commentUtils.h** - Утилитки для разбора коментариев
 - **enums_manual.h** - "Ручные" перечисления
 - **log.h** - Макросы для лога
 - **signature.h** - Поиск вхождения сигнатуры, в т.ч. многострочного. Вытащил из MD PP

