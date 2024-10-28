#include <gurka/dll_injector_cli.h>
#include <filesystem>

using namespace gurka;

int main(int argc, const char* realArgv[])
{
	std::filesystem::current_path(INJECTED_PATH);

	const char* argv[]{
		realArgv[0], "eject", "App.exe", "TestDll.dll"
	};

	dll_injector_main(sizeof(argv) / sizeof(argv[0]), (const char**)argv);
}