@call _setup.bat

@set SCANNER=%UBS%
%SCANNER% --where
%SCANNER% @../umba-brief-scanner.rsp --scan=../src _sources_brief.txt
@rem --scan=src

@rem umba-brief-scanner --update --verbose=detailed --main --remove-path --filename-width=24 --text-width=140 doc/_sources_brief.txt
@rem > make_sources_brief.log
@rem doc/_sources_brief.txt
@rem --scan=..\..
