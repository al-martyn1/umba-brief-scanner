@rem %UMBA_TOOLS% - eg F:\umba-tools

@if "%UMBA_TOOLS%"=="" goto UMBA_TOOLS_VAR_NOT_SET
@goto UMBA_TOOLS_VAR_IS_SET

:UMBA_TOOLS_VAR_NOT_SET
@echo UMBA_TOOLS environmetnt variable is not set
@exit /B 1

:UMBA_TOOLS_VAR_IS_SET


@if not exist %UMBA_TOOLS%\bin    mkdir %UMBA_TOOLS%\bin
@if not exist %UMBA_TOOLS%\conf   mkdir %UMBA_TOOLS%\conf

@copy /Y .out\msvc2019\x64\Release\umba-brief-scanner.exe    %UMBA_TOOLS%\bin\

@xcopy /Y /S /E /I /F /R _distr_conf\conf\*                  %UMBA_TOOLS%\conf


umba-brief-scanner --help > help.txt