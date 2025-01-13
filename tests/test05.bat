@call %~dp0find_test_exe.bat test05
@rem call %~dp0setup_test_files.bat

@set TEST_PATH=%~dp0\..\_src\umba-brief-scanner
@set TEST_FILES=^
%TEST_PATH%\test01.cpp   ^
%TEST_PATH%\arg_parser.h ^
%TEST_PATH%\notes.h     

%TEST_EXE% %TEST_FILES%
