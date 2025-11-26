#include <Windows.h>
#include <stdint.h>

namespace gurka {
	uint64_t getModuleBaseAddress(DWORD processID, const char* dllName);
	size_t findPids(const char* programName, size_t resPidsSz, DWORD* outPids);
	bool injectDLL(HANDLE hProc, const char* dllFullPath);
	bool injectDLL(DWORD procId, const char* dllFullPath);
	bool unloadDLL(HANDLE hProc, const char* dllName);
	bool unloadDLL(HANDLE hProc, const char* dllName);
	bool loadDLL(const char* procName, const char* dllPath);
	bool unloadDLL(DWORD procId, const char* dllName);
	bool unloadDLL(const char* procName, const char* dllPath);
	bool unloadDllByPid(DWORD pid, const char* dllName);
	bool loadDllByPid(DWORD pid, const char* dllPath);
	extern bool EnableDllLog;
}