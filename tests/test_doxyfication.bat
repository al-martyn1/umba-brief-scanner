@call _setup.bat

@set SCANNER=%UBS%
%SCANNER% --where
@rem --quet
%SCANNER% @../umba-brief-scanner.rsp --overwrite --doxyfication=always --scan=../src --scan=../_libs doxy
