add_library_ns(gurka injector STATIC dll_injector.cc)
target_include_dir_iface(gurka-injector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include include)
target_link_libraries(gurka-injector psapi)
install_target_and_headers (gurka injector)