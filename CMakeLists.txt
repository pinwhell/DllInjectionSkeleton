cmake_minimum_required(VERSION 3.0...3.15)

project(dll-injection-skeleton)

include(GenerateExportHeader)

if(WIN32)
	add_definitions(-D_WIN32_WINNT=0x0502)
endif()

# Common compiler flags
add_compile_options(
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Werror>
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
)

add_subdirectory(src)
