# umba-brief-scanner

umba-brief-scanner - одна из утилит пакета umba-tools. Сканирует каталоги проекта и извлекает описания из
файлов с исходными кодами.


## Все umba-утилиты

Проект для сборки всех umba-утилит можно найти [тут](https://github.com/al-martyn1/umba-tools).


## umba-brief-scanner

Производит сканирование заданных каталогов на предмет поиска файлов по маске (include/exclude files).
В каждом файле производит поиск комментария специального вида:

```
/*(*|!) (\|@)file
        (\|@)brief Brief desription of the file
*/
```

Производится поиск entry points типа main/DllMain/etc, опция --entry-name=... - задает имя и тип возвращаемых значений

В результирующий отчет (txt|html) выводится список файлов с их описанием, файлы с entry points идут первыми.

Есть возможность группировки по пути.

Основные опции:

--main - генерировать только entry points в отчёте.

--update[=FILE] - в выходном txt файле можно дописывать описания файлов, и если оно не появляется в 
сорцах - берётся из предыдущей версии brief-файла.

--split-group - группирует/делит на группы.

--help - справка по параметрам командной строки.


### Пример работы утилиты umba-brief-scanner

Пример работы утилиты umba-brief-scanner можно найти [тут](/doc/_sources_brief.txt).


## Сборка утилиты umba-brief-scanner

Делаем `git clone https://github.com/al-martyn1/umba-brief-scanner`.

Переходим в каталог с зависимостями:
```
cd _libs
```

Запускаем один из bat-файлов:
```
clone_libs_http.bat
clone_libs_https.bat
clone_libs_ssh.bat
```

Открываем solution - `umba-brief-scanner.sln` и собираем нужную конфигурацию.
Можно из командной строки MSVC собрать проект при помощи MSBUILD.

