# Brief Description for Project Sources

- [_libs/umba (Entry points)](#user-content-_libsumba-entry-points)
- [_src/umba-brief-scanner (Entry points)](#user-content-_srcumba-brief-scanner-entry-points)
- [_libs/encoding/compact_enc_det](#user-content-_libsencodingcompact_enc_det)
- [_libs/encoding](#user-content-_libsencoding)
- [_libs/encoding/util](#user-content-_libsencodingutil)
- [_libs/encoding/util/encodings](#user-content-_libsencodingutilencodings)
- [_libs/encoding/util/languages](#user-content-_libsencodingutillanguages)
- [_libs/encoding/util](#user-content-_libsencodingutil-1)
- [_libs/marty_cpp](#user-content-_libsmarty_cpp)
- [_libs/marty_cpp/examples](#user-content-_libsmarty_cppexamples)
- [_libs/marty_cpp](#user-content-_libsmarty_cpp-1)
- [_libs/marty_cpp/warnings](#user-content-_libsmarty_cppwarnings)
- [_libs/marty_decimal](#user-content-_libsmarty_decimal)
- [_libs/marty_decimal/warnings](#user-content-_libsmarty_decimalwarnings)
- [_libs/marty_utf](#user-content-_libsmarty_utf)
- [_libs/sfmt](#user-content-_libssfmt)
- [_libs/umba](#user-content-_libsumba)
- [_libs/umba/internal](#user-content-_libsumbainternal)
- [_libs/umba](#user-content-_libsumba-1)
- [_libs/umba/warnings](#user-content-_libsumbawarnings)
- [_libs/umba](#user-content-_libsumba-2)
- [_libs/umba_tokenizer/doc.3dp](#user-content-_libsumba_tokenizerdoc3dp)
- [_src/umba-brief-scanner](#user-content-_srcumba-brief-scanner)


# _libs/umba (Entry points)

 - **app_main.h** - 

# _src/umba-brief-scanner (Entry points)

 - **main.cpp** - Для тестирования, файл с описанием и main'ом
 - **test0.cpp** - 
 - **test00.cpp** - 


# _libs/encoding/compact_enc_det

 - **compact_enc_det.cc** - 
 - **compact_enc_det.h** - 
 - **compact_enc_det_fuzz_test.cc** - 
 - **compact_enc_det_generated_tables.h** - 
 - **compact_enc_det_generated_tables2.h** - 
 - **compact_enc_det_hint_code.cc** - 
 - **compact_enc_det_hint_code.h** - 
 - **compact_enc_det_unittest.cc** - 
 - **detail_head_string.inc** - 

# _libs/encoding

 - **disable_warnings_begin.h** - 
 - **disable_warnings_end.h** - 
 - **encoding.cpp** - 
 - **encoding.h** - 

# _libs/encoding/util

 - **basictypes.h** - 
 - **case_insensitive_hash.h** - 
 - **commandlineflags.h** - 

# _libs/encoding/util/encodings

 - **encodings.cc** - 
 - **encodings.h** - 
 - **encodings.pb.h** - 
 - **encodings_unittest.cc** - 

# _libs/encoding/util/languages

 - **languages.cc** - 
 - **languages.h** - 
 - **languages.pb.h** - 

# _libs/encoding/util

 - **logging.h** - 
 - **port.h** - 
 - **string_util.h** - 
 - **varsetter.h** - 

# _libs/marty_cpp

 - **c_escape.h** - C/C++ string escapement, simplified version (\o,\x,\u,\N not supported)
 - **enums.h** - 

# _libs/marty_cpp/examples

 - **01_normalization_and_tabs.cpp** - Тестируем cEscapeString, нормализацию CR LF, нормализацию trailing spaces, condenseSpacesToTabs, normalizeIndents
 - **02_enum_generator.cpp** - Тестирование генератора enum'ов marty_cpp::enum_generate_
 - **03_sort_includes.cpp** - Тестируем sort_includes.
 - **enum_gen_cpp_templates_txt.c** - 

# _libs/marty_cpp

 - **marty_cpp.h** - C++ (also good for other langs) identifiers style autodetect and format
 - **marty_cpp_exceptions.h** - 
 - **marty_enum.h** - Helpers for enum
 - **marty_enum_impl_helpers.h** - 
 - **marty_flag_ops.h** - 
 - **marty_flags.h** - Helpers for flags
 - **marty_ns.h** - Helpers for namespace
 - **marty_ss.h** - Simple stream template
 - **sort_includes.h** - 
 - **src_normalization.h** - Simple strings normalization - LF normalization, Tab normalization, split to lines

# _libs/marty_cpp/warnings

 - **disable_C5045.h** - 
 - **disable_spectre_mitigation.h** - 
 - **pop.h** - 
 - **push.h** - 
 - **push_disable_spectre_mitigation.h** - 

# _libs/marty_decimal

 - **marty_bcd.h** - 
 - **marty_bcd_decimal.h** - 
 - **marty_bcd_decimal_impl.h** - 
 - **marty_decimal.h** - 
 - **marty_decimal_math.h** - 
 - **marty_int_decimal.h** - 
 - **marty_int_decimal_impl.h** - 
 - **marty_relops_impl.h** - 
 - **undef_min_max.h** - 

# _libs/marty_decimal/warnings

 - **disable_C5045.h** - 
 - **disable_spectre_mitigation.h** - 
 - **pop.h** - 
 - **push.h** - 
 - **push_disable_spectre_mitigation.h** - 

# _libs/marty_utf

 - **utf.h** - 
 - **utf_impl.h** - 

# _libs/sfmt

 - **simple_formatter.cpp** - 
 - **umba_lout.cpp** - Реализация глобального объекта потока LOG вывода umba::lout

# _libs/umba

 - **CMakeCCompilerId.c** - 
 - **CMakeCXXCompilerId.cpp** - 
 - **_brief.html** - 
 - **algoritms.h** - Различные колхозные алгоритмы
 - **alloca.h** - Совместимая версия alloca/_alloca
 - **arithmetic_strong_typedef.h** - Какая-то шляпа, уже и не помню. Вроде оставлена чисто для совместимости
 - **assert.h** - Универсальная реализация UMBA_ASSERT
 - **attribute_weak.h** - 
 - **autolock.h** - Автоматический захват и освобождение ресурса. Объект-ресурс должен предоставлять методы lock и unlock.
 - **basic_interfaces.h** - Базовые интерфейсы
 - **basic_interfaces_impl.h** - Реализация некоторых базовых интерфейсов
 - **bits.h** - Манипуляции с битами
 - **bits_fmt.h** - Вывод битсетов в поток
 - **char_writers.h** - Реализации интерфейса ICharWritter для различных целевых шляп типа COM-порта или файла
 - **cli_tool_helpers.h** - Хелперы для утилит командной строки (CLI)
 - **clipboard_win32.h** - Хелперы для виндового клипборда
 - **cmd_line.h** - Разная шляпа по разбору командной строки для консольных утилит
 - **compiler_id_cpp.h** - 
 - **compiler_info.cpp** - 
 - **compiler_info.h** - 
 - **const_string.h** - Константные строки - обёртка для строковых литералов (экспериментальная фуета)
 - **container.h** - Тут разные странные контейнеры, как самописные, так и стыренные
 - **container_utility.h** - 
 - **critical_section.cpp** - Реализация критической секции
 - **critical_section.h** - Реализация критической секции.
 - **debug_helpers.h** - Поддержка отладки
 - **dump.h** - Хелперы для дампа
 - **enum_helpers.h** - Хелперы для enum'ов
 - **env.h** - Работа с переменными окружения
 - **errors.h** - Ошибки, и связанные с ними определения
 - **escape_string.h** - 
 - **exception.h** - Umba-исключения
 - **file_helpers.h** - Файловые хелперы. Они разъехались по разным местам. А хидер остался для совместимости
 - **filecache.h** - Файловый кеш (эксперименты)
 - **filename.h** - Утилиты для работы с именами файлов
 - **filename_set.h** - Отображение имен файлов в id и обратно
 - **filesys.h** - Прослойка абстракций для работы с файловой системой, версия, работающая одинаково с любыми типами имён (char/wchar_t)
 - **filesys_scanners.h** - Разные сканеры имен. По маскам и не только.
 - **flag_helpers.h** - Какие-то хелперы для enum флагов. Выпочковалось в итоге в часть marty_cpp
 - **format_message.h** - Форматирование сообщений при помощи макросов $(Macro)
 - **format_utils.h** - Утилиты для форматирования
 - **gmesg.h** - 
 - **hr_counter.h** - Подсистема hr_counter - счетчики высокого разрешения
 - **html_base.h** - 
 - **i_char_writer.h** - Интерфейс для использования в форматном выводе
 - **id_gen.h** - 
 - **include_finder.h** - Поиск подключаемых файлов
 - **info_log.h** - Хелперы для лога
 - **ini.h** - Разбор INI-файла
 - **interface.h** - Базовые определения для интерфейсов, и связанные с ним определения

# _libs/umba/internal

 - **env.h** - Работа с переменными окружения
 - **filesys.h** - Прослойка абстракций для работы с файловой системой
 - **filesys_impl_helpers.h** - 
 - **winerror_org.h** - 
 - **winerror_prepared.h** - 
 - **winstrerrorname.cxx** - 
 - **winstrerrorname.h** - 

# _libs/umba

 - **iterator.h** - 
 - **legacy.h** - Подключение классических хидеров скопом
 - **linefeedtype.h** - Объявление типа перевода строки и разные хелперы
 - **lineinfo.h** - LineInfo и сопутствующее. Устаревшая (непосредственно содержащая строку) версия API
 - **lineposinfo.h** - Типы для указания положения в текстовом файле
 - **lineview.h** - Структура LineView и соответствующие функции
 - **log.h** - Вывод в лог (на консоль), хелперы
 - **macro_helpers.h** - Макросики-матросики. У макросов нет вопросов. Хелперы
 - **macros.h** - Макросики-матросики. У макросов нет вопросов. Адаптация старенького кода для подстановки макросов. Осторожно! Может вызывать кровотечение из глаз
 - **name_version.h** - Работа с именоваными номерами версий
 - **nul_ostream.h** - Пустопорожний Ostream, никуда ничего не выводит
 - **null_inserter.h** - Ничего не делающий insert_iterator
 - **numeric_version.h** - Работа с номерами версий
 - **optimize_pop.h** - Восстанавливает предыдущий уровень оптимизации
 - **optimize_speed.h** - Включает режим оптимизации по скорости
 - **packpop.h** - Восстановление значения по умолчанию для упаковки структур
 - **parse_utils.h** - Утилиты для парсинга
 - **perf_counters.h** - Подсистема perf_counters - счетчики производительности
 - **preprocessor.h** - Различные общие макросы препроцессора
 - **program_location.h** - Поиск месторасположения self EXE-шника
 - **properties.h** - Реализация properties на чистом C++
 - **pushpack1.h** - Сохранение текущих настроек и установка упаковки структур по 1
 - **pushpack2.h** - Сохранение текущих настроек и установка упаковки структур по 2
 - **pushpack4.h** - Сохранение текущих настроек и установка упаковки структур по 4
 - **pushpack8.h** - Сохранение текущих настроек и установка упаковки структур по 8
 - **qtui-show-simple-message.h** - Qt-хелпер для отображения MessageBox
 - **regex_helpers.h** - Помощяки для плюсовых регулярок. В тч (простая_маска_*_?) -> ECMA Regex
 - **relops.h** - 
 - **rgbquad.h** - Реализация RGB четвертяка. Хранит RGB и Alpha-канал компоненты цвета
 - **rule_of_five.h** - Макросы для правила пяти - надоело руками выписывать. Правило пяти - https://habr.com/ru/companies/otus/articles/800089/
 - **scanners.h** - 
 - **scope_exec.h** - Выполнение лямбд при объявлении переменной и при выходе из скоупа
 - **shellapi.h** - 
 - **stl.h** - Совместимое со всеми компиляторами подключение стандартных заголовков
 - **stl_keil.h** - Некоторые фичи C++11, которые не реализованы библиотеке компилятора Keil
 - **stl_keil_initializer_list.h** - 
 - **stl_keil_type_traits.h** - Перетащенное под старый Кейл type_traits из GCC
 - **string_char_writers.h** - 
 - **string_plus.h** - Разная всячина для строк в C++ стиле
 - **text_iterator_traits.h** - 
 - **text_position_info.h** - 
 - **text_utils.h** - Шляпы с текстом
 - **the.h** - 
 - **the_.h** - 
 - **time_service.cpp** - На базе кода под STM32, адаптация для кроссплатформы, реализация
 - **time_service.h** - Подсистема time_service
 - **timestamp.h** - Представление временного интервала в сутках, часах, минутах, секундах и милисекундах
 - **transliteration.h** - 
 - **umba.h** - Подключение базовых файлов UMBA
 - **umba_cpp.h** - Разная всячина в C++ стиле
 - **undef_FormatMessage.h** - Undef'ы тупого мусора
 - **undef_min_max.h** - Undef'ы тупого мусора
 - **units.h** - 
 - **user_literals.h** - Типа пользовательские литералы
 - **utf.h** - 
 - **utf8.h** - 
 - **utf_impl.h** - 
 - **utility.h** - Утилитки в C++ стиле
 - **variadic_helpers.h** - Хелперы для вариадик-шаблонов

# _libs/umba/warnings

 - **disable_C4061.h** - 
 - **disable_C4062.h** - 
 - **disable_C4127.h** - 
 - **disable_C4191.h** - 
 - **disable_C4265.h** - 
 - **disable_C4355.h** - 
 - **disable_C4365.h** - 
 - **disable_C4464.h** - 
 - **disable_C4555.h** - 
 - **disable_C4623.h** - 
 - **disable_C4820.h** - 
 - **disable_C4840.h** - 
 - **disable_C4996.h** - 
 - **disable_C5045.h** - 
 - **disable_C5204.h** - 
 - **disable_C5267.h** - 
 - **disable_common.h** - 
 - **disable_condition_is_const.h** - 
 - **disable_def_ctor_implicitly_deleted.h** - 
 - **disable_enumerator_not_handled.h** - 
 - **disable_fn_or_var_unsafe.h** - 
 - **disable_implicit_copy_ctor_deprecated.h** - 
 - **disable_non_portable_variadic.h** - 
 - **disable_non_trivial_destructor_not_virtual.h** - 
 - **disable_padding_added.h** - 
 - **disable_rel_inc_contains_dbldot.h** - 
 - **disable_result_not_used.h** - 
 - **disable_signed_unsigned_mismatch.h** - 
 - **disable_spectre_mitigation.h** - 
 - **disable_this_used_in_base_member_initializer_list.h** - 
 - **disable_trivial_destructor_not_virtual.h** - 
 - **disable_unsafe_conversion.h** - 
 - **pop.h** - 
 - **push.h** - 
 - **push_disable_C4061.h** - 
 - **push_disable_C4062.h** - 
 - **push_disable_C4127.h** - 
 - **push_disable_C4191.h** - 
 - **push_disable_C4265.h** - 
 - **push_disable_C4355.h** - 
 - **push_disable_C4365.h** - 
 - **push_disable_C4464.h** - 
 - **push_disable_C4555.h** - 
 - **push_disable_C4623.h** - 
 - **push_disable_C4820.h** - 
 - **push_disable_C4840.h** - 
 - **push_disable_C4996.h** - 
 - **push_disable_C5045.h** - 
 - **push_disable_C5204.h** - 
 - **push_disable_C5267.h** - 
 - **push_disable_common.h** - 
 - **push_disable_condition_is_const.h** - 
 - **push_disable_def_ctor_implicitly_deleted.h** - 
 - **push_disable_enumerator_not_handled.h** - 
 - **push_disable_fn_or_var_unsafe.h** - 
 - **push_disable_implicit_copy_ctor_deprecated.h** - 
 - **push_disable_non_portable_variadic.h** - 
 - **push_disable_non_trivial_destructor_not_virtual.h** - 
 - **push_disable_padding_added.h** - 
 - **push_disable_rel_inc_contains_dbldot.h** - 
 - **push_disable_result_not_used.h** - 
 - **push_disable_signed_unsigned_mismatch.h** - 
 - **push_disable_spectre_mitigation.h** - 
 - **push_disable_this_used_in_base_member_initializer_list.h** - 
 - **push_disable_trivial_destructor_not_virtual.h** - 
 - **push_disable_unsafe_conversion.h** - 

# _libs/umba

 - **win32_utils.h** - 
 - **winconhelpers.h** - Хелперы для работы с консолью под Windows
 - **zz_detect_environment.h** - ZZ Определение сборочного окружения и целевой платформы.
 - **zz_error_codes.h** - ZZ Коды ошибок
 - **zz_i_char_writer_term.h** - ZZ Утилиты для вывода в терминал
 - **zz_i_char_writer_term_colors.h** - ZZ Цветочки для терминала
 - **zz_i_char_writer_term_console_type.h** - ZZ Дефиниции про консоль
 - **zz_i_char_writer_term_utils.h** - ZZ Какие-то консольные хелперы
 - **zz_inc_windows.h** - ZZ Правильный порядок инклюдов для винды
 - **zz_mcu_low_level.h** - ZZ Microcontroller Unit Low Level definitions

# _libs/umba_tokenizer/doc.3dp

 - **Keith_Cooper_Linda_Torczon-Engineering_a_Compiler-EN.pdf** - 
 - **Redkaya_professiya.pdf** - 
 - **Креншоу_Iforth.pdf** - 

# _src/umba-brief-scanner

 - **app_config.h** - Класс, задающий параметры работы приложения umba-brief-scanner. Настраивается опциями командной строки.
 - **app_ver_config.h** - Информация о приложении umba-brief-scanner и его версии - название, описание, версия, время/дата, домашняя страница
 - **arg_parser.h** - Разбор аргументов командной строки
 - **brief_info.h** - Реализация извлечения brief info из файлов сорцов
 - **commentUtils.h** - Утилитки для разбора коментариев - разбор всякой шляпы типа тупых никому ненужных баннеров
 - **cpp-tokenizer.h** - Токенизер для плюсов
 - **enums.h** - 
 - **enums_manual.h** - "Ручные" перечисления - то, что набито ручками, а не сгенерированно при помощи umba-enum-gen
 - **log.h** - Макросы для лога
 - **mdlex.h** - 
 - **notes.h** - Заметки (TODO, NOTE, и тп)
 - **notes_impl.h** - Заметки (TODO, NOTE, и тп)
 - **qt_stub.cpp** - Стаб, для того, чтобы windeployqt подсосал нужный рантайм для MSVC/GCC - а то совсем без Qt он не работает
 - **signature.h** - Поиск сигнатур, в т.ч. многострочных. Вытащил из MD PP, там они маркерами сниппетов работают
 - **test01.cpp** - Перетащил всю требуху из другого проекта, лишнее отрежу потом
 - **test02.cpp** - Простой тест регулярок по поиску точки входа и brief'а
 - **test03.cpp** - Тест arithmetic_strong_typedef - просто присунул тут по случаю
 - **test04.cpp** - Тестируем notes
 - **test05.cpp** - Делаем для теста вывод токенов
 - **umba-brief-scanner.cpp** - Утилита umba-brief-scanner - сканирование исходных текстов на C/C++ и получение списка кратких описаний файлов исходных текстов
 - **utils.h** - Разные утилитки

