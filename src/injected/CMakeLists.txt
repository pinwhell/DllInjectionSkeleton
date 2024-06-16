add_library(injected SHARED injected.cc console.cc)
target_include_directories(injected PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)