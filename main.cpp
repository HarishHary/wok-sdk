#include "common_includes.h"
#include "sdk/sdk.h"
#include "hooks/hooks.h"

BOOL WINAPI detach_wok() {
	wok::detach();
	return TRUE;
}

DWORD WINAPI init_wok(LPVOID base) {
	wok::init();

	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	FreeLibraryAndExitThread(static_cast<HMODULE>(base), EXIT_FAILURE);

	return 1;
}

BOOL APIENTRY DllMain(const HMODULE hinst_dll, const unsigned long reason_for_call, const void* reserved) {
	if (reason_for_call == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hinst_dll);
		if (auto thread = CreateThread(0, 0, init_wok, hinst_dll, 0, 0))
			CloseHandle(thread);
	}
	else if (reason_for_call == DLL_PROCESS_DETACH && !reserved) {
		return detach_wok();
	}

	return TRUE;
}
