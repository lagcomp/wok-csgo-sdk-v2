#include "common_includes.h"
#include "globals.h"
#include "init.h"

void __stdcall init_wok(HMODULE module) {
	wok::init();

	while (!input::get_key<ON_KEY>(VK_DELETE)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(200u));
	}

	wok::unload();

	FreeLibraryAndExitThread(module, 0);
}

int __stdcall DllMain(HMODULE module, unsigned long reason_for_call, void* reserved) {
	if (reason_for_call != DLL_PROCESS_ATTACH)
		return 0;

	if (g::module = module) {
		DisableThreadLibraryCalls(module);
	}

	if (const auto thread = CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(init_wok), module, 0, 0)) {
		CloseHandle(thread);
	}

	return 1;
}