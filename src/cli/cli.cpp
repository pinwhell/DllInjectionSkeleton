#include <stdio.h>
#include <dll_injector.h>

int dll_load(const char* procName, const char* dllPath)
{
    return loadDLL(procName, dllPath) ? 0 : 1;
}

int dll_unload(const char* procName, const char* dllName)
{
    return unloadDLL(procName, dllName) ? 0 : 1;
}

void print_usage()
{
    printf(
        "usage: dll-injector-cli inject [process name.exe] [dll path]\n"
        "usage: dll-injector-cli eject  [process name.exe] [dll name]\n");
}

int main(int argc, char* argv[])
{
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

        return dll_load(argv[2], argv[3]);
    }

    if (!strcmp(option, "eject"))
    {
        if (argc < 4)
        {
            print_usage();
            return 0;
        }

        return dll_unload(argv[2], argv[3]);
    }

    print_usage();    
    return 0;
}