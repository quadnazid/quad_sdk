#include "includes.hpp"

unsigned long __stdcall initialize(void* instance) {
	while (!GetModuleHandleA("serverbrowser.dll"))
		Sleep(200);

	try {
		interfaces::initialize();
		hooks::initialize();
	}

	catch (const std::runtime_error& error) {
		MessageBoxA(nullptr, error.what(), "[!] ERROR: UNKNOWN_!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<module>(instance), 0);
	}
}

unsigned long __stdcall deinitialize() {
	hooks::shutdown();

	return TRUE;
}

std::_int32 __stdcall main(const module instance, const unsigned long reason, const void* reserved) {
	disable_calls(instance);

	switch (reason) {
	case DLL_PROCESS_ATTACH: {
		if (auto handle = CreateThread(nullptr, NULL, initialize, instance, NULL, nullptr))
			CloseHandle(handle);

		break;
	}
	case DLL_PROCESS_DETACH: {
		deinitialize();
		break;
	}
	}
	return true;
}