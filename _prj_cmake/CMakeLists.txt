cmake_minimum_required(VERSION 3.27.0)
project(umba-brief-scanner VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Полезные ключики GCC - https://codeforces.com/blog/entry/15547?locale=ru

#add_compile_definitions("_WIN32" "WIN32" "_UNICODE" "UNICODE" "YAML_CPP_STATIC_DEFINE")
add_compile_definitions("_WIN32" "WIN32" "YAML_CPP_STATIC_DEFINE")

#add_link_options("-municode")
add_link_options("-mconsole")
add_link_options("-Wl,--subsystem,console")
#add_link_options("--subsystem,console")

#set(CMAKE_GENERATOR_PLATFORM "x64")

set(CMAKE_VERBOSE_MAKEFILE on )
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(X64 ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

#set(CMAKE_COLOR_DIAGNOSTICS ON)

#-O0 
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    #-fdiagnostics-color=always
	#set(CMAKE_CXX_FLAGS "-Wa,-mbig-obj -flto -flto-report  -fno-rtti -Wall -Wextra -Werror -pedantic-errors -Wctor-dtor-privacy -Wextra-semi -Wnon-virtual-dtor -Woverloaded-virtual -Wredundant-decls -Wsign-promo")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  #-fignore-unknown-pragmas
  set(CMAKE_CXX_FLAGS "-Woverloaded-virtual -Wno-ctor-dtor-privacy -Wsign-promo")
  #add_compile_options(-fdiagnostics-color=always)
  add_compile_options(-Wa,-mbig-obj)
  # -fno-rtti
  add_compile_options(-Wall -Wno-unknown-pragmas -Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wredundant-decls -Wno-unused-function)
  add_compile_options(-Wno-unknown-pragmas)
  #-Wno-unknown-pragmas
  #-Wnon-virtual-dtor -Wctor-dtor-privacy
  #add_compile_options(-w)
  #add_compile_options(-w -Wa,-mbig-obj)
  #-fdiagnostics-color=always 
	#set(CMAKE_CXX_FLAGS "-Wa,-mbig-obj -fno-rtti")
  #-Wall   
  #-flto -flto-report
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
endif()

set(PRJ_ROOT ${PROJECT_SOURCE_DIR}/..)
set(SRC_ROOT ${PRJ_ROOT}/src)
set(UBS_SRC_ROOT ${SRC_ROOT}/umba-brief-scanner)
set(LIBS_ROOT ${PRJ_ROOT}/_libs)


include_directories(
  "${LIBS_ROOT}"
  "${LIBS_ROOT}/encoding"
  "${LIBS_ROOT}/sfmt/inc"
  "${LIBS_ROOT}/nlohmann/include"
  "${LIBS_ROOT}/yaml-cpp/include"
)



set (UMBA_BRIEF_SCANNER_COMMON_SOURCES
    "${LIBS_ROOT}/sfmt/umba_lout.cpp"
    "${LIBS_ROOT}/umba/time_service.cpp"
    "${LIBS_ROOT}/encoding/encoding.cpp"
    "${LIBS_ROOT}/encoding/compact_enc_det/compact_enc_det.cc"
    "${LIBS_ROOT}/encoding/compact_enc_det/compact_enc_det_hint_code.cc"
    "${LIBS_ROOT}/encoding/util/encodings/encodings.cc"
    "${LIBS_ROOT}/encoding/util/languages/languages.cc"
    )

add_compile_definitions("USE_UMBA_LOUT_COUT" "CONSOLE" "_CONSOLE")

# add_compile_options(-fsplit-stack)


add_executable(umba-brief-scanner  ${UMBA_BRIEF_SCANNER_COMMON_SOURCES} "${UBS_SRC_ROOT}/umba-brief-scanner.cpp" )
target_link_libraries(umba-brief-scanner ws2_32 iphlpapi shell32)

add_executable(test01  ${UMBA_BRIEF_SCANNER_COMMON_SOURCES} "${UBS_SRC_ROOT}/test01.cpp" )
target_link_libraries(test01 ws2_32 iphlpapi shell32)

add_executable(test02  ${UMBA_BRIEF_SCANNER_COMMON_SOURCES} "${UBS_SRC_ROOT}/test02.cpp" )
target_link_libraries(test02 ws2_32 iphlpapi shell32)

add_executable(test03  ${UMBA_BRIEF_SCANNER_COMMON_SOURCES} "${UBS_SRC_ROOT}/test03.cpp" )
target_link_libraries(test03 ws2_32 iphlpapi shell32)


