# Утилита Umba Brief Scanner

    - [Все umba-утилиты](#user-content-все-umba-утилиты)
    - [umba-brief-scanner](#user-content-umba-brief-scanner)
      - [Пример работы утилиты umba-brief-scanner](#user-content-пример-работы-утилиты-umba-brief-scanner)
    - [Сборка утилиты umba-brief-scanner](#user-content-сборка-утилиты-umba-brief-scanner)
  - [Сборка и отладка проекта](#user-content-сборка-и-отладка-проекта)
    - [Извлечение библиотек](#user-content-извлечение-библиотек)
    - [Генерация сборочных скриптов и сборка](#user-content-генерация-сборочных-скриптов-и-сборка)
    - [Открытие проекта в IDE](#user-content-открытие-проекта-в-ide)
      - [Запуск VSCode](#user-content-запуск-vscode)
      - [Запуск MSVC](#user-content-запуск-msvc)

 

umba-brief-scanner - одна из утилит пакета umba-tools. Сканирует каталоги проекта и извлекает описания из
файлов с исходными кодами.


## Все umba-утилиты

Проект для сборки всех umba-утилит можно найти [тут](https://github.com/al-martyn1/umba-tools).


## umba-brief-scanner

Производит сканирование заданных каталогов на предмет поиска файлов по маске (include/exclude files).
В каждом файле производит поиск комментария специального вида (в формате Doxygen):

```
/*(*|!) (\|@)file
        (\|@)brief Brief desription of the file
*/
```

Производится поиск entry points типа main/DllMain/etc, опция --entry-name=... - задает имя и тип возвращаемых значений

В результирующий отчет (`TXT`|`HTML`|`MD`) выводится список файлов с их описанием, файлы с entry points идут первыми.

Есть возможность группировки по пути.

Основные опции:

--main - генерировать только entry points в отчёте.

--update[=FILE] - в выходном txt файле можно дописывать описания файлов, и если оно не появляется в 
сорцах - берётся из предыдущей версии brief-файла.

--split-group - группирует/делит на группы.

--help - справка по параметрам командной строки.


### Пример работы утилиты umba-brief-scanner

Пример работы утилиты umba-brief-scanner можно найти [тут](/doc/_sources_brief.txt) или [тут](/doc/_sources_brief.md).


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


# Сборка и отладка проекта


## Извлечение библиотек

Для сборки проекта требуются внешние библиотеки. Внешние библиотеки не подгружаются автоматически.
Для их подгрузки требуется выполнить скрипт `_libs/_clone_libs.bat`.


## Генерация сборочных скриптов и сборка

Для генерации сборочных скриптов и их сборки при помощи `MSBuild` предназначены файлы:

 - `.bat/build_all_msvc2019.bat`
 - `.bat/build_all_msvc2022.bat`
 - `.bat/cmake_generate_msvc2019.bat`
 - `.bat/cmake_generate_msvc2022.bat`


## Открытие проекта в IDE

### Запуск VSCode

Для запуска VSCode предназначен файл `_start_code.bat`. 
VSCode с плагином "CMake Tools" сам обнаружит `CMakeLists.txt`. Следует выбрать подходящий тулчейн
и можно начинать работу.


### Запуск MSVC

Для запуска MSVC предназначен файл `_start_msvc.bat`.
По умолчанию будет сгенерированна и открыта в 2019ой студии конфигурация `x86`.
Для того, чтобы открывалась другая студия с другой конфигурацией, следует
переименовать файл `setup_msvc.bat.example` в `setup_msvc.bat` и настроить нужную конфигурацию там.




