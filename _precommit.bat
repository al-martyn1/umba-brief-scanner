if exist "%~dp0\_found-*.md"  del /Q /F "%~dp0\_found-*.md"
if exist "%~dp0\_found-*.txt" del /Q /F "%~dp0\_found-*.txt"

@set BRIEF_SCAN_PATHS="--scan=%~dp0"
@rem "--scan=%~dp0\../src" "--scan=%~dp0\../_src" "--scan=%~dp0\../examples"
@rem set BRIEF_TXT_EXTRA_OPTS=--scan-notes "--notes-output-path=%~dp0\./doc"
@set BRIEF_MD_EXTRA_OPTS=--scan-notes "--notes-output-path=%~dp0\./doc"

@call "%~dp0\.bat\fix_tabs.bat"
@call "%~dp0\.bat\gen-doc.bat"
