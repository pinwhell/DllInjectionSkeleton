#include <stdio.h>
#include <gurka/dll_injector_cli.h>
#include <gurka/dll_injector.h>
#include <filesystem>
#include <iostream>
#include <string>

using namespace gurka;

int cli_dll_load(const char* target, const char* dllPath, bool bRawPath = false)
{
    const char* actualPath = bRawPath ? dllPath : std::filesystem::absolute(dllPath).string().c_str();
    try {
        DWORD pid = static_cast<DWORD>(std::stoul(target));
        return loadDllByPid(pid, actualPath) ? 0 : 1;
    }
    catch (...) {
        return loadDLL(target, actualPath) ? 0 : 1;
    }
}

int cli_dll_unload(const char* target, const char* dllName)
{
    try {
        DWORD pid = static_cast<DWORD>(std::stoul(target));
        return unloadDllByPid(pid, dllName) ? 0 : 1;
    }
    catch (...) {
        return unloadDLL(target, dllName) ? 0 : 1;
    }
}

int cli_dll_reload(const char* target, const char* dllName)
{
    try {
        DWORD pid = static_cast<DWORD>(std::stoul(target));
        unloadDllByPid(pid, dllName);
        return loadDllByPid(pid, dllName) ? 0 : 1;
    }
    catch (...) {
        unloadDLL(target, dllName);
        return loadDLL(target, dllName) ? 0 : 1;
    }
}

void print_usage()
{
    printf(
        "usage: dll-injector-cli inject [process name.exe or PID] [optional raw] [dll path]\n"
        "usage: dll-injector-cli eject [process name.exe or PID] [dll name]\n"
        "usage: dll-injector-cli reload|reinject [process name.exe or PID] [dll name]\n");
}

int gurka::dll_injector_main(int argc, const char** argv)
{
    try {
        if (argc < 2)
        {
            print_usage();
            return 0;
        }
        const char* option = argv[1];
        if (!strcmp(option, "inject"))
        {
            if (argc < 4)
            {
                print_usage();
                return 0;
            }
            if (std::string(argv[3]) == "raw") {
                if (argc < 5) {
                    print_usage();
                    return 0;
                }
                return cli_dll_load(argv[2], argv[4], true);
            }
            else {
                return cli_dll_load(argv[2], argv[3], false);
            }
        }
        if (!strcmp(option, "eject"))
        {
            if (argc < 4)
            {
                print_usage();
                return 0;
            }
            return cli_dll_unload(argv[2], argv[3]);
        }
        if (!strcmp(option, "reload") || !strcmp(option, "reinject"))
        {
            if (argc < 4)
            {
                print_usage();
                return 0;
            }
            return cli_dll_reload(argv[2], argv[3]);
        }
        print_usage();
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}