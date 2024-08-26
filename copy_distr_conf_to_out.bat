@call "%~dp0\.bat\setup_out_root_paths.bat"

@FOR %%i IN (%OUTROOTPATHS%) DO @(
        @IF EXIST "%%~i\" @(
            xcopy /Y /S /E /I /F /R "%~dp0\_distr_conf\conf\*"  "%%~i\\conf"
)
)
