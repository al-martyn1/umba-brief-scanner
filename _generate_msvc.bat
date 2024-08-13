@rem Генерация сборочных скриптов x86/x64 Debug/Release для заданной версии MSVC
@rem call this: call _generate_msvc.bat msvc2019|msvc2022
@rem https://learn.microsoft.com/en-us/cpp/build/customize-cmake-settings?view=msvc-170
@call %~dp0\.bat\_generate_msvc.bat %1 && @call %~dp0\.bat\_generate_msvc.bat %1

