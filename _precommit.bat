if exist "%~dp0\_found-*.md"  del /Q /F "%~dp0\_found-*.md"
if exist "%~dp0\_found-*.txt" del /Q /F "%~dp0\_found-*.txt"

@set BRIEF_SCAN_PATHS="--scan=%~dp0"
@rem "--scan=%~dp0\../src" "--scan=%~dp0\../_src" "--scan=%~dp0\../examples"
@rem set BRIEF_EXTRA_OPTS_TXT=--scan-notes "--notes-output-path=%~dp0\doc\_md"
@set BRIEF_EXTRA_OPTS_MD=--scan-notes "--notes-output-path=%~dp0\doc\_md"

@rem call "%~dp0\.bat\fix_tabs.bat"
@call "%~dp0\.bat\gen-doc.bat"
