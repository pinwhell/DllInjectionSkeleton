#include <dll_injector_cli.h>
#include <filesystem>

int main(int argc, const char* realArgv[])
{
	std::filesystem::current_path(INJECTED_PATH);

	const char* argv[]{
		realArgv[0], "eject", "Test.exe", "injected.dll"
	};

	dll_injector_main(sizeof(argv) / sizeof(argv[0]), (const char**)argv);
}