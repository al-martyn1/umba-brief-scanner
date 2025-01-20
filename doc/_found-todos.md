---
Generator: Umba Brief Scanner
---

# _libs/encoding


## attention

- `[_libs/encoding/encoding.h:315]`
  Надо бы тут разобраться с файлами, которые UTF-16


## todo

- [ ] `[_libs/encoding/encoding.cpp:370]`
  А если продетектированная по BOM кодировка не соответствует заданной
  параметром?

- [ ] `[_libs/encoding/encoding.cpp:386]`
  разобраться, не будет ли precomposed всякие ударения соединять с символом?



# _libs/encoding/compact_enc_det


## note

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:1933]`
  initial pointer is to FRONT of string, not back

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:2013]`
  Encoding probabilites are all zero at this point

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:2470]`
  this requires at least one lower AND one upper AND one digit to pass

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:5134]`
  destate and scoped_debug_data have exactly the same scope All other
  FLAGS_enc_detect_detail tests use destate.debug_data != NULL

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:5197]`
  Encoding probabilites are all zero at this point

- `[_libs/encoding/compact_enc_det/compact_enc_det.cc:5206]`
  probabilities up to this point are subject to derating for small numbers of
  bigrams. Probability changes after this point are not derated.

- `[_libs/encoding/compact_enc_det/compact_enc_det_unittest.cc:1986]`
  Unicode handled differently because of embedded NUL

- `[_libs/encoding/compact_enc_det/compact_enc_det_unittest.cc:4745]`
  strlen will not work for UTF-16 and UTF-32 strings with embedded NUL


## todo

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:46]`
  as of 2007.10.09:
  
  Consider font=TT-BHxxx as user-defined => binary Demote GB18030 if no 8x3x pair
  Map byte2 ascii punct to 0x60, digits to 0x7e, gets them into hires Consider
  removing/ignoring bytes 01-1F to avoid crap pollution Possibly boost declared
  encoding in robust scan googlebot tiny files look for ranges of encodings
  consider tags just as > < within aligned block of 32 flag too few characters in
  postproc (Latin 6 problem) Remove slow scan beyond 16KB Consider removing
  kMostLikelyEncoding or cut it in half

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:343]`
  only allocate this if being used

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:1150]`
  change this to be per encoding, not permuted

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:1671]`
  This should all be folded into postproc-enc-detect.cc

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:1808]`
  This should all be folded into postproc-enc-detect.cc

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:1896]`
  This should all be folded into postproc-enc-detect.cc

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:2131]`
  make this more graceful

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:3978]`
  boost subset/superset also Boost(destatep, kRelatedEncoding[best_enc],
  kBestEncBoost);

- [ ] `[_libs/encoding/compact_enc_det/compact_enc_det.cc:4696]`
  also want to boost Shift-JIS here if byte1 is Ax..Dx TEMP printf(" 
  Rescore[%02x] %s +%d\n", high_hash, MyRankedEncName(rankedencoding),
  kGentlePairBoost);



# _libs/encoding/util


## note

- `[_libs/encoding/util/basictypes.h:49]`
  unsigned types are DANGEROUS in loops and other arithmetical places.  Use the
  signed types unless your variable represents a bit pattern (eg a hash value) or
  you really need the extra bit.  Do NOT use 'unsigned' to express "this value
  should always be positive"; use assertions for this.

- `[_libs/encoding/util/basictypes.h:115]`
  that the function doesn't need an implementation, as we only use its type.


## todo

- [ ] `[_libs/encoding/util/basictypes.h:34]`
  (mbelshe) Remove these type guards.  These are temporary to avoid conflicts
  with npapi.h.

- [ ] `[_libs/encoding/util/basictypes.h:57]`
  (mbelshe) Remove these type guards.  These are temporary to avoid conflicts
  with npapi.h.


## warning

- `[_libs/encoding/util/basictypes.h:281]`
  if Dest or Source is a non-POD type, the result of the memcpy is likely to
  surprise you.



# _libs/encoding/util/encodings


## note

- `[_libs/encoding/util/encodings/encodings.cc:48]`
  that the preferred MIME name may differ slightly from the official IANA name:
  i.e. ISO-8859-1 vs. ISO_8859-1:1987

- `[_libs/encoding/util/encodings/encodings.cc:216]`
  that MimeEncodingName() returns "Shift_JIS", since KDDI uses "Shift_JIS" in
  HTTP headers and email messages.

- `[_libs/encoding/util/encodings/encodings.cc:459]`
  that despite an RFC to the contrary, ARABIC_ENCODING (ISO-8859-6) is NOT
  visual.

- `[_libs/encoding/util/encodings/encodings.cc:576]`
  that MimeEncodingName() can return the same mime encoding name for different
  encoding enums like JAPANESE_SHIFT_JIS and KDDI_SHIFT_JIS.  In that case, the
  encoding enum first seen will be the value for the encoding name in the map.

- `[_libs/encoding/util/encodings/encodings.h:39]`
  The Encoding enum must always start at 0. This assumption has been made and
  used.

- `[_libs/encoding/util/encodings/encodings.h:146]`
  that the name of this function is somewhat misleading. There is nothing "right
  to left" about these encodings. They merely contain code points for characters
  in RTL languages such as Hebrew and Arabic. But this is also true for UTF-8.


## todo

- [ ] `[_libs/encoding/util/encodings/encodings.cc:315]`
  This list could be expanded.  Many other encodings are supersets of 7-bit
  Ascii.  In fact, Japanese JIS and Unicode are the only two encodings that I
  know for a fact should *not* be in this list.

- [ ] `[_libs/encoding/util/encodings/encodings.cc:374]`
  This list could maybe be expanded.  Other encodings may be 8-bit.

- [ ] `[_libs/encoding/util/encodings/encodings.cc:520]`
  Unify MimeEncodingName and EncodingName, or determine why such a unification is
  not possible.

- [ ] `[_libs/encoding/util/encodings/encodings.cc:525]`
  Should this be invalid_encoding_name()?

- [ ] `[_libs/encoding/util/encodings/encodings.cc:860]`
  Check if it is possible to remove the non-standard, non-netscape-use names. It
  is because this routine is used for encoding detections from html meta info.
  Non-standard names may introduce noise on encoding detection.

- [ ] `[_libs/encoding/util/encodings/encodings.cc:865]`
  Unify EncodingNameAliasToEncoding and EncodingFromName, or determine why such a
  unification is not possible.
  ----------------------------------------------------------------------

- [ ] `[_libs/encoding/util/encodings/encodings.h:48]`
  Include a SWIG workaround header file.

- [ ] `[_libs/encoding/util/encodings/encodings.h:55]`
  Make these static const Encoding values instead of macros.

- [ ] `[_libs/encoding/util/encodings/encodings.h:106]`
  the current implementation is likely incomplete.  It would be good to consider
  the full matrix of all pairs of encodings and to fish out all compatible pairs.

- [ ] `[_libs/encoding/util/encodings/encodings.h:151]`
  Get rid of this function. The only special-case we should need to worry about
  are visual encodings. Anything we need to do for all 'RTL' encodings we need to
  do for UTF-8 as well.

- [ ] `[_libs/encoding/util/encodings/encodings.h:165]`
  Get rid of this function. The only special-case we should need to worry about
  are visual encodings.


## warning

- `[_libs/encoding/util/encodings/encodings.h:115]`
  This function does not currently return true for all encodings that are
  supersets of Ascii 7-bit.

- `[_libs/encoding/util/encodings/encodings.h:122]`
  This function does not currently return true for all encodings that are 8-bit
  encodings.



# _libs/encoding/util/languages


## note

- `[_libs/encoding/util/languages/languages.h:122]`
  Technically we're talking about scripts, not languages. There are languages
  that can be written in more than one script. Examples: - Kurdish and Azeri
  ('AZERBAIJANI') can be written left-to-right in Latin or Cyrillic script, and
  right-to-left in Arabic script. - Sindhi and Punjabi are written in different
  scripts, depending on region and dialect. - Turkmen used an Arabic script
  historically, but not any more. - Pashto and Uyghur can use Arabic script, but
  use a Roman script on the Internet. - Kashmiri and Urdu are written either with
  Arabic or Devanagari script.
  
  This function only returns true for languages that are always, unequivocally
  written in right-to-left script.

- `[_libs/encoding/util/languages/languages.h:150]`
  See important notes under IsRightToLeftLanguage(...).
  
  This function returns true for languages that *may* appear on the web in a
  right-to-left script, even if they may also appear in a left-to-right script.
  
  This function should typically be used in cases where doing some work on
  left-to-right text would be OK (usually a no-op), and this function is used
  just to cut down on unnecessary work on regular, LTR text.

- `[_libs/encoding/util/languages/languages.h:250]`
  See the note below about the codes for Chinese languages.

- `[_libs/encoding/util/languages/languages.h:265]`
  CHINESE LANGUAGE CODES
  
  There are three functions that return codes for Chinese languages.
  LanguageCode(lang) and LanguageCodeWithDialects(lang) are defined here.
  LanguageCode(lang, encoding) is defined in i18n/encodings.lang_enc.h. The
  following list shows the different results.
  
  LanguageCode(CHINESE) returns "zh" LanguageCode(CHINESE_T) returns "zh-TW".
  
  LanguageCodeWithDialects(CHINESE) returns "zh-CN".
  LanguageCodeWithDialects(CHINESE_T) returns "zh-TW".
  
  LanguageCode(CHINESE_T, <any encoding>) returns "zh-TW". LanguageCode(CHINESE,
  CHINESE_BIG5) returns "zh-TW". LanguageCode(CHINESE, <any other encoding>)
  returns "zh-CN".
  
  --------------------------------------------

- `[_libs/encoding/util/languages/languages.pb.h:187]`
  If you add a language, you will break a unittest. See the note at the top of
  this enum.


## todo

- [ ] `[_libs/encoding/util/languages/languages.cc:69]`
  Although Teragram has two output names "TG_UNKNOWN_LANGUAGE" and "Unknown",
  they are essentially the same. Need to unify them. "un" and "ut" are invented
  by us, not from ISO-639.

- [ ] `[_libs/encoding/util/languages/languages.h:33]`
  Include a header containing swig-compatible enum.

- [ ] `[_libs/encoding/util/languages/languages.h:137]`
  If we want to do anything special with multi-script languages we should create
  new 'languages' for each language+script, as we do for traditional vs.
  simplified Chinese. However most such languages are rare in use and even rarer
  on the web, so this is unlikely to be something we'll be concerned with for a
  while.


## warning

- `[_libs/encoding/util/languages/languages.h:106]`
  This function provides only a simple test on the values of the two Language
  arguments. It returns false if either language is invalid. It returns true if
  the language arguments are equal, or if they are both Chinese languages, both
  Norwegian languages, or both Portuguese languages, as defined by
  IsChineseLanguage, IsNorwegianLanguage, and IsPortugueseLanguage. Otherwise it
  returns false.



# _libs/marty_cpp


## attention

- `[_libs/marty_cpp/marty_cpp.h:3225]`
  название? UPD: чего я тут хотел, уже не помню, надо бы коменты более развёрнуто
  делать

- `[_libs/marty_cpp/marty_cpp.h:4800]`
  std::make_tuple - что тут сказать хотел, непонятно, забыл уже

- `[_libs/marty_cpp/marty_cpp.h:4981]`
  заменить std::stoll, чтоб умело парсить двоичные константы и разделители try


## todo

- [ ] `[_libs/marty_cpp/marty_cpp.h:685]`
  пока range_error не кидаем, но надо доделать

- [ ] `[_libs/marty_cpp/marty_cpp.h:2290]`
  Надо подумать, править баг и как его править

- [ ] `[_libs/marty_cpp/marty_cpp.h:3602]`
  Нужен каст к underlaying типу, если он задан. Сюда надо будет передавать
  шаблоны и строку underlaying типа

- [ ] `[_libs/marty_cpp/marty_cpp.h:4155]`
  Нужен каст к underlaying типу, если он задан. Сюда надо будет передавать
  шаблоны и строку underlaying типа



# _libs/marty_cpp/warnings


## see-also

- `[_libs/marty_cpp/warnings/push.h:1]`
  https://devblogs.microsoft.com/cppblog/broken-warnings-theory/
    https://learn.microsoft.com/en-us/cpp/preprocessor/warning?view=msvc-170



# _libs/marty_decimal


## attention

- `[_libs/marty_decimal/marty_bcd.h:377]`
  Оно не нужно - что не нужно? Надо будет глянуть сюда повнимательнее


## note

- `[_libs/marty_decimal/marty_bcd_decimal_impl.h:140]`
  Вообще не паримся по поводу несимметричности множества целых относительно нуля


## undone

- `[_libs/marty_decimal/marty_int_decimal_impl.h:449]`
  Need to make correct rounding

- `[_libs/marty_decimal/marty_int_decimal_impl.h:459]`
  Need to make correct rounding



# _libs/marty_decimal/tests/src


## note

- `[_libs/marty_decimal/tests/src/benchmark_tests.cpp:852]`
  cpp_dec_float: 1) гарантирует 50 значащих десятичных цифр всего, а не после
  десятичной точки 2) Немотря на название, не является десятичным числом -
  косвенным подтверждением этого является то, что на рекурентном тесте Мюллера
  его расколбашивает точно так же, как и встроенный в язык double. dec в названии
  говорит только о том, что данный тип хранит не менее, чем заданное число
  десятичных знаков.



# _libs/marty_decimal/warnings


## see-also

- `[_libs/marty_decimal/warnings/push.h:1]`
  https://devblogs.microsoft.com/cppblog/broken-warnings-theory/
    https://learn.microsoft.com/en-us/cpp/preprocessor/warning?view=msvc-170



# _libs/marty_utf


## todo

- [ ] `[_libs/marty_utf/utf_impl.h:723]`
  Надо бы сделать: UTF-32 из string. UTF-32 из wstring. string  из UTF-32.
  wstring из UTF-32.



# _libs/umba


## attention

- `[_libs/umba/assert.h:81]`
  Сделать как надо UMBA_ASSERT для GCC/Clang



- `[_libs/umba/cmd_line.h:1647]`
  Тут закоменчено что-то старое, хз зачем я тут внимание акцентировал.
    std::map<std::string, int>::const_iterator eit = optInfo.enumVals.find(optArgCopy);
    if (eit == optInfo.enumVals.end())
    {
    errMsg = std::string("Invalid option value taken. Option value can be one of: ") + optInfo.getAllEnumNames(", ", " or ") + std::string(" (") + optInfo.getAllOptionNames("/") + std::string(")");
    return false;
    }

- `[_libs/umba/cmd_line.h:3002]`
  Надо ProgramLocation проверить на юникод

- `[_libs/umba/macros.h:417]`
  Чего-то с прокси не срослось - компилятор помирает от вложенности шаблонов
  Порешал, сделав getter нешаблонным параметром с виртуальным оператором ()

- `[_libs/umba/macros.h:421]`
  Пока не будем ничего делать, потом разберёмся

- `[_libs/umba/shellapi.h:335]`
  тут нужен замут через dl*, но пока лень и не особо нужно

- `[_libs/umba/time_service.cpp:184]`
  Сделать как надо umba::time_service::init()

- `[_libs/umba/time_service.cpp:196]`
  Сделать как надо umba::time_service::start()

- `[_libs/umba/time_service.cpp:205]`
  Сделать как надо umba::time_service::stop()

- `[_libs/umba/time_service.cpp:232]`
  Сделать как надо

- `[_libs/umba/time_service.cpp:258]`
  Сделать как надо umba::time_service::getCurTimeHires()

- `[_libs/umba/zz_detect_environment.h:683]`
  ??? Чо сказать хотел, не понятно

- `[_libs/umba/zz_mcu_low_level.h:30]`
  Сделать как надо UMBA_INSTRUCTION_BARRIER

- `[_libs/umba/zz_mcu_low_level.h:54]`
  Сделать как надо UMBA_DATA_MEMORY_BARRIER

- `[_libs/umba/zz_mcu_low_level.h:72]`
  Сделать как надо UMBA_DATA_SYNCHRONIZATION_BARRIER

- `[_libs/umba/zz_mcu_low_level.h:123]`
  Сделать как надо UMBA_INTERRUPTS_DISABLED

- `[_libs/umba/zz_mcu_low_level.h:139]`
  Сделать как надо UMBA_DISABLE_IRQ

- `[_libs/umba/zz_mcu_low_level.h:148]`
  Сделать как надо UMBA_ENABLE_IRQ


## note

- `[_libs/umba/exception.h:255]`
  - required to be using namespace ::umba::omanip or using namespace std::iomanip
  declared before using this macro

- `[_libs/umba/filecache.h:16]`
  Not safe for threading


## see-also

- `[_libs/umba/debug_helpers.h:105]`
  https://github.com/scottt/debugbreak/blob/master/debugbreak.h


## todo

- [ ] `[_libs/umba/enum_helpers.h:475]`
  Сделать так же, как для QString

- [ ] `[_libs/umba/enum_helpers.h:483]`
  Сделать так же, как для QString

- [ ] `[_libs/umba/filesys_scanners.h:30]`
  Надо бы переименовать в umba::filesys::scanners

- [ ] `[_libs/umba/filesys_scanners.h:270]`
  Нужно что-то решать с отсутствующим расширением

- [ ] `[_libs/umba/filesys_scanners.h:487]`
  Нужно что-то решать с отсутствующим расширением

- [ ] `[_libs/umba/format_message.h:57]`
  Надо подумать на тему замены десятичного разделителя и разделителя разрядов

- [ ] `[_libs/umba/format_message.h:303]`
  Не реализовано ! Десятичный разделитель - между целой и дробной частью

- [ ] `[_libs/umba/format_message.h:307]`
  Не реализовано ! Разделитель груп разрядов и размер группы

- [ ] `[_libs/umba/format_message.h:311]`
  Не реализовано ! Установка нац особенностей форматирования десятичных чисел,
  делает decSep и decGroup

- [ ] `[_libs/umba/regex_helpers.h:197]`
  Если не обрезать, то регулярки в GCC 7.3 падают Чет не работает, а обрезать в
  прикладухе - работает

- [ ] `[_libs/umba/regex_helpers.h:232]`
  Если не обрезать, то регулярки в GCC 7.3 падают Чет не работает, а обрезать в
  прикладухе - работает

- [ ] `[_libs/umba/string_plus.h:2099]`
  Надо подумать, править баг и как его править

- [ ] `[_libs/umba/string_plus.h:2156]`
  Надо подумать, править баг и как его править

- [ ] `[_libs/umba/utf_impl.h:772]`
  Надо бы сделать: UTF-32 из string UTF-32 из wstring string  из UTF-32 wstring
  из UTF-32


## undone

- `[_libs/umba/bits.h:482]`
  need assert

- `[_libs/umba/bits.h:495]`
  need assert

















- `[_libs/umba/critical_section.h:207]`
  need to make normal implementation

- `[_libs/umba/filename.h:1407]`
  not implemented


## warning









# _libs/umba/internal


## note

- `[_libs/umba/internal/winerror_org.h:22986]`
  This assumes that WIN32 errors fall in the range -32k to 32k.
  
  Define bits here so macros are guaranteed to work

- `[_libs/umba/internal/winerror_org.h:31141]`
  that additional FACILITY_SSPI errors are in issperr.h
  
  ****************** FACILITY_CERT ******************
  
  MessageId: TRUST_E_PROVIDER_UNKNOWN
  
  MessageText:
  
  Unknown trust provider.


## todo

- [ ] `[_libs/umba/internal/filesys.h:587]`
  Посмотреть "Именование файлов, путей и пространств имен" -
  https://learn.microsoft.com/ru-ru/windows/win32/fileio/naming-a-file

- [ ] `[_libs/umba/internal/filesys.h:1487]`
  При возникновении исключения hFind утекает. Надо бы переделать, и такие моменты
  надо бы везде это проверить

- [ ] `[_libs/umba/internal/filesys.h:1546]`
  При возникновении исключения hFind утекает. Надо бы везде это проверить HANDLE
  hFind = ::FindFirstFileA( umba::filename::prepareForNativeUsage(path).c_str(),
  &fndData );

- [ ] `[_libs/umba/internal/filesys.h:1920]`
  Переделать !!!
  https://stackoverflow.com/questions/2910377/get-home-directory-in-linux

- [ ] `[_libs/umba/internal/winerror_org.h:34314]`
  Add DXG Win32 errors here
  
  Common errors {0x2000..0x20ff}
  
  MessageId: ERROR_GRAPHICS_NOT_EXCLUSIVE_MODE_OWNER
  
  MessageText:
  
  Exclusive mode ownership is needed to create unmanaged primary allocation.



# _libs/umba/warnings


## see-also

- `[_libs/umba/warnings/push.h:1]`
  https://devblogs.microsoft.com/cppblog/broken-warnings-theory/
    https://learn.microsoft.com/en-us/cpp/preprocessor/warning?view=msvc-170


## todo

- [ ] `[_libs/umba/warnings/disable_unsafe_conversion.h:9]`
  Проверить на 8/9/10/11/12 ой версии GCC - на 13ой точно есть


## warning

- `[_libs/umba/warnings/disable_unsafe_conversion.h:1]`
  C4191: 'type cast': unsafe conversion from 'FARPROC' to '...'



# _src/umba-brief-scanner


## attention

- `[_src/umba-brief-scanner/app_config.h:82]`
  Не забывать копировать и/или подготавливать поля класса в функции
  getAdjustedConfig

- `[_src/umba-brief-scanner/notes_impl.h:660]`
  С какого-то перепугу у нас иногда вылезают ацстые заметки, надо разобраться

- `[_src/umba-brief-scanner/test05.cpp:298]`
  Inform about exception

- `[_src/umba-brief-scanner/umba-brief-scanner.cpp:911]`
  конец обработки брифов


## todo

- [ ] `[_src/umba-brief-scanner/arg_parser.h:344]`
  (111) Хорошо бы переделать, чтобы минус мог быть элементом списка, очищая его.
  (222) заодно надо бы переделать и --exclude-files/--include-files
  
  (333) Заодно проверяем, как работает заметка в несколько параграфов

- [ ] `[_src/umba-brief-scanner/notes.h:159]`
  Надо добавить опцию ком. строки для задания данного параметра
  --notes-source-info

- [ ] `[_src/umba-brief-scanner/notes.h:160]`
  --text-notes-full-path - используется при сохранении в простой текст

- [ ] `[_src/umba-brief-scanner/notes_impl.h:615]`
  Нужно выводить не тип заметки, а какой-то заголовок

- [ ] `[_src/umba-brief-scanner/test01.cpp:260]`
  Add HTML output here

- [ ] `[_src/umba-brief-scanner/test01.cpp:280]`
  Add HTML line break here

- [ ] `[_src/umba-brief-scanner/umba-brief-scanner.cpp:756]`
  Add HTML output here

- [ ] `[_src/umba-brief-scanner/umba-brief-scanner.cpp:795]`
  Не корректно определяется длина строк в многобайтной кодировке Мой поток вывода
  (umba::SimpleFormatter) - также некорректно работает с многобайтной кодировкой,
  но это обычно не проблема, если имена файлов  исходников используют только
  английский алфавит. При форматировании же текста кодировка зависит от кодировки
  исходника - может быть как однобайтной, так и многобайтной (UTF-8, например, и
  в линуксе это обычно уже стандарт) Поэтому, если где-то используются
  русскоязычные описания и используется форматирование, то лучше сделать его
  побольше - не 80, а 120 символов шириной, например.

- [ ] `[_src/umba-brief-scanner/umba-brief-scanner.cpp:874]`
  Add HTML line break here

