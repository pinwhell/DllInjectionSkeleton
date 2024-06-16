add_library(dll-injector STATIC dll_injector.cc)
target_include_directories(dll-injector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(dll-injector psapi)