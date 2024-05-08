@if exist %~dp0\_set_pp.bat       @call %~dp0\_set_pp.bat
@if exist %~dp0\_set_cmn_opts.bat @call %~dp0\_set_cmn_opts.bat
@echo Setup results:
@echo   UBS: %UBS%
@echo   OPTS: %CMNOPTS%
