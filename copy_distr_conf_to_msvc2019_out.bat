@call tests/_setup.bat
@xcopy /Y /S /E /I /F /R _distr_conf\conf\* %UBSPATH%\..\conf
@rem xcopy /Y /S /E /I /F /R _distr_conf\conf\* %UBSPATH%.out\msvc2019\x86\conf