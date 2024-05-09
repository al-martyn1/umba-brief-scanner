@call _setup.bat

@set SCANNER=%UBS%
%SCANNER% --where
%SCANNER% @../umba-brief-scanner.rsp --quet --doxyfication=always --scan=../src --scan=../_libs doxy
