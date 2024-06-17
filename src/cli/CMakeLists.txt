set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../bin)

add_executable(dll-injector-cli dll_injector_cli.cpp cli.cpp)
target_link_libraries(dll-injector-cli dll-injector)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set_target_properties(dll-injector-cli PROPERTIES OUTPUT_NAME dll-injector-cli-64)
else()
    set_target_properties(dll-injector-cli PROPERTIES OUTPUT_NAME dll-injector-cli)
endif()