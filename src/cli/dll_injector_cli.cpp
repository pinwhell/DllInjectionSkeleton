#include <stdio.h>
#include <gurka/dll_injector_cli.h>
#include <gurka/dll_injector.h>
#include <filesystem>
#include <iostream>

using namespace gurka;

int cli_dll_load(const char* procName, const char* dllPath, bool bRawPath = false)
{
    return loadDLL(procName, bRawPath ? dllPath : std::filesystem::absolute(dllPath).string().c_str()) ? 0 : 1;
}

int cli_dll_unload(const char* procName, const char* dllName)
{
    return unloadDLL(procName, dllName) ? 0 : 1;
}

void print_usage()
{
    printf(
        "usage: dll-injector-cli inject [process name.exe] [optional raw] [dll path]\n"
        "usage: dll-injector-cli eject  [process name.exe] [dll name]\n");
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

            return cli_dll_load(argv[2], std::string(argv[3]) == "raw" ? argv[4] : argv[3], std::string(argv[3]) == "raw");
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

        print_usage();
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}