---
Generator: Umba-Brief-Scanner
---

# umba-brief-scanner

- `[umba-brief-scanner/arg_parser.h:343]`
  (111) Хорошо бы переделать, чтобы минус мог быть элементом списка, очищая его.
  (222) заодно надо бы переделать и --exclude-files/--include-files.
  
  (333) Заодно проверяем, как работает заметка в несколько параграфов.

- `[umba-brief-scanner/notes.h:158]`
  Надо добавить опцию ком. строки для задания данного параметра
  --notes-source-info.

- `[umba-brief-scanner/notes.h:159]`
  --text-notes-full-path - используется при сохранении в простой текст.

- `[umba-brief-scanner/notes_impl.h:611]`
  Нужно выводить не тип заметки, а какой-то заголовок.

- `[umba-brief-scanner/test01.cpp:259]`
  Add HTML output here.

- `[umba-brief-scanner/test01.cpp:279]`
  Add HTML line break here.

- `[umba-brief-scanner/umba-brief-scanner.cpp:750]`
  Add HTML output here.

- `[umba-brief-scanner/umba-brief-scanner.cpp:789]`
  Не корректно определяется длина строк в многобайтной кодировке Мой поток вывода
  (umba::SimpleFormatter) - также некорректно работает с многобайтной кодировкой,
  но это обычно не проблема, если имена файлов  исходников используют только
  английский алфавит. При форматировании же текста кодировка зависит от кодировки
  исходника - может быть как однобайтной, так и многобайтной (UTF-8, например, и
  в линуксе это обычно уже стандарт) Поэтому, если где-то используются
  русскоязычные описания и используется форматирование, то лучше сделать его
  побольше - не 80, а 120 символов шириной, например.

- `[umba-brief-scanner/umba-brief-scanner.cpp:868]`
  Add HTML line break here.

