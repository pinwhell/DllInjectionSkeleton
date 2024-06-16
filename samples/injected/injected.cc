#include <thread>
#include <windows.h>

#include "console.h"

HINSTANCE ghInstance;

void injectedMain()
{
  static Console console;
  
  console.init("Injected DLL");
  console.write("hey its me ur brother");
}

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  ghInstance = hinstDLL;
  (void)lpvReserved;

  static std::thread injectedMainThread;

  switch (fdwReason)
  {
    case DLL_PROCESS_ATTACH:
    {
      injectedMainThread = std::thread(injectedMain);
      break;
    }

    case DLL_PROCESS_DETACH:
    {
      if (injectedMainThread.joinable())
      {
        injectedMainThread.join();
      }
      break;
    }
  }

  return TRUE;
}
