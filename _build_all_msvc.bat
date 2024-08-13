@rem Генерация сборочных скриптов и сборка x86/x64 Debug/Release для заданной версии MSVC
@rem call this: call _build_all_msvc.bat msvc2019|msvc2022
@call %~dp0\.bat\_call_cmake_msvc_impl.bat %1 x86 GENERATE && @call %~dp0\.bat\_call_cmake_msvc_impl.bat %1 x64 GENERATE && @call %~dp0\.bat\_call_cmake_msvc_impl.bat %1 x86 BUILD && @call %~dp0\.bat\_call_cmake_msvc_impl.bat %1 x64 BUILD
