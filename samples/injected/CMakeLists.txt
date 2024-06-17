add_library(injected SHARED injected.cc console.cc)
target_include_directories(injected PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

add_library(injected_macro_path INTERFACE)
target_compile_definitions(injected_macro_path INTERFACE INJECTED_PATH="${CMAKE_CURRENT_BINARY_DIR}")
