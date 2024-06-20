#include <cstdio>
#include <string>
#include <windows.h>
#include <psapi.h>
#include <vector>
#include <TlHelp32.h>

// Function to get a pointer to the filename from a given path
const char* GetFilename(const char* path) {
    // Pointers to the last occurrences of '/' and '\'
    const char* unix_sep = strrchr(path, '/');
    const char* win_sep = strrchr(path, '\\');

    // Determine which is the last separator
    const char* last_sep = unix_sep > win_sep ? unix_sep : win_sep;

    // If there's no separator, the path itself is the filename
    return last_sep ? last_sep + 1 : path;
}

HMODULE GetMHandle(DWORD processID, const char* dllName) {
    HMODULE hMod = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);

    if (hSnapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 moduleEntry;
        moduleEntry.dwSize = sizeof(moduleEntry);

        if (Module32First(hSnapshot, &moduleEntry)) {
            do {
                if (strstr(moduleEntry.szModule, dllName)) {
                    hMod = moduleEntry.hModule;
                    break;
                }
            } while (Module32Next(hSnapshot, &moduleEntry));
        }
        CloseHandle(hSnapshot);
    }
    return hMod;
}

bool IsModuleLoaded(HANDLE hProc, const char* moduleName)
{
    return GetMHandle(GetProcessId(hProc), moduleName) != 0;
}

size_t findPids(const char* programName, size_t resPidsSz, DWORD* outPids)
{
  // Enumerate all processes
  size_t resIndex = 0;
  DWORD pidsArr[1024];
  DWORD temp;
  if (!EnumProcesses(pidsArr, sizeof(pidsArr), &temp))
  {
    return {};
  }

  // Find the first process with the given program name
  auto noPids = temp / sizeof(DWORD);
  for (auto i = 0u; i < noPids; i++)
  {
    if (pidsArr[i] == 0)
    {
      continue;
    }

    auto tempHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pidsArr[i]);
    if (tempHandle == NULL)
    {
      continue;
    }

    HMODULE tempModule;
    if (EnumProcessModules(tempHandle, &tempModule, sizeof(tempModule), &temp))
    {
      TCHAR szProcessName[MAX_PATH];
      GetModuleBaseName(tempHandle, tempModule, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
      if (strcmp(programName, szProcessName) == 0)
      {
        outPids[resIndex++] = pidsArr[i];
        if ((resIndex < resPidsSz) == false)
            break;
      }
    }
  }

  return resIndex;
}

bool injectDLL(HANDLE hProc, const char* dllFullPath)
{
    const char* dllName = GetFilename(dllFullPath);
    DWORD pid = GetProcessId(hProc);

    if (IsModuleLoaded(hProc, dllName))
        return true;

    // Get the address to the function LoadLibraryA in kernel32.dll
    auto LoadLibAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (LoadLibAddr == NULL)
    {
        return false;
    }

    // Allocate memory inside the opened process
    auto dereercomp = VirtualAllocEx(hProc, NULL, strlen(dllFullPath), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (dereercomp == NULL)
    {
        return false;
    }

    // Write the DLL name to the allocated memory
    if (!WriteProcessMemory(hProc, dereercomp, dllFullPath, strlen(dllFullPath), NULL))
    {
        return false;
    }

    // Create a thread in the opened process
    auto remoteThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibAddr, dereercomp, 0, NULL);
    if (remoteThread == NULL)
    {
        return false;
    }

    // Wait until thread have started (or stopped?)
    WaitForSingleObject(remoteThread, INFINITE);

    if (IsModuleLoaded(hProc, dllName))
        printf("PID-%d: Loaded %s\n", pid, dllName);

    // Free the allocated memory
    VirtualFreeEx(hProc, dereercomp, strlen(dllFullPath), MEM_RELEASE);

    // Close the handles
    CloseHandle(remoteThread);

    return true;
}

bool injectDLL(DWORD procId, const char* dllFullPath)
{
    HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, procId);

    if (hProcess == INVALID_HANDLE_VALUE)
        return false;

    bool bInjected = injectDLL(hProcess, dllFullPath);

    CloseHandle(hProcess);

    return bInjected;
}

bool unloadDLL(HANDLE hProc, const char* dllName)
{
    DWORD procId = GetProcessId(hProc);

    if (!IsModuleLoaded(hProc, dllName))
        return true;

    if (procId == 0)
        return false;

    HMODULE hMod = GetMHandle(procId, dllName);

    // Get the address to the function FreeLibrary in kernel32.dll
    auto FreeLibAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "FreeLibrary");
    if (FreeLibAddr == NULL)
    {
        return false;
    }

    // Create a thread in the opened process
    auto remoteThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)FreeLibAddr, (LPVOID)hMod, 0, NULL);
    if (remoteThread == NULL)
    {
        return false;
    }

    // Wait until thread have started (or stopped?)
    WaitForSingleObject(remoteThread, INFINITE);

    if (!IsModuleLoaded(hProc, dllName))
        printf("PID-%d: Unloaded %s\n", procId, dllName);

    // Close the handles
    CloseHandle(remoteThread);

    return true;
}

bool unloadDLL(DWORD procId, const char* dllName)
{
    HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, procId);

    if (hProcess == INVALID_HANDLE_VALUE)
        return false;

    bool bUnloaded = unloadDLL(hProcess, dllName);

    CloseHandle(hProcess);

    return bUnloaded;
}

bool loadDLL(const char* procName, const char* dllPath)
{
    DWORD pids[1024];
    size_t nPids = findPids(procName, sizeof(pids), pids);

    if (nPids == 0)
    {
        printf("Could not find process: %s\n", procName);
        return false;
    }

    for (size_t i = 0; i < nPids; i++)
    {
        if (!injectDLL(pids[i], dllPath))
            printf("%s:%d Could not inject DLL: %s\n", procName, pids[i], dllPath);
    }

    return true;
}

bool unloadDLL(const char* procName, const char* dllName)
{
    DWORD pids[1024];
    size_t nPids = findPids(procName, sizeof(pids), pids);

    if (nPids == 0)
    {
        fprintf(stderr, "Could not find process: %s\n", procName);
        return false;
    }

    for (size_t i = 0; i < nPids; i++)
    {
        if (!unloadDLL(pids[i], dllName))
            fprintf(stderr, "%s:%d Could not unload DLL: %s\n", procName, pids[i], dllName);
    }

    return true;
}