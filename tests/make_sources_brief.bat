@call _setup.bat

@set SCANNER=%UBS%
%SCANNER% --where
%SCANNER% @../umba-brief-scanner.rsp --scan=../src _sources_brief.txt
