add_executable(cli_test cli_test.cpp ../src/cli/dll_injector_cli.cpp)
target_link_libraries(cli_test gurka::injector injected_macro_path)