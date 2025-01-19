del /Q /S _src\?found-*.md
del /Q /S _src\?found-*.md_
del /Q /S _src\?found-*._md
del /Q /S _src\?found-*._md_
del /Q /S _src\?found-*.txt

del /Q /S doc\?found-*.md
del /Q /S doc\?found-*.md_
del /Q /S doc\?found-*._md
del /Q /S doc\?found-*._md_
del /Q /S doc\?found-*.txt

@call "%~dp0\.bat\fix_tabs.bat"
@call "%~dp0\.bat\gen-doc.bat"
